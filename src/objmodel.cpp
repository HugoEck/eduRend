#include "OBJModel.h"


OBJModel::OBJModel(
	const std::string& objfile,
	ID3D11Device* dxdevice,
	ID3D11DeviceContext* dxdevice_context)
	: Model(dxdevice, dxdevice_context)
{
	// Load the OBJ
	OBJLoader* mesh = new OBJLoader();
	mesh->Load(objfile);

	// Load and organize indices in ranges per drawcall (material)

	std::vector<unsigned> indices;
	unsigned int indexOffset = 0;

	for (auto& dc : mesh->Drawcalls)
	{
		// Append the drawcall indices
		for (auto& tri : dc.Triangles)
			indices.insert(indices.end(), tri.VertexIndices, tri.VertexIndices + 3);

		// Create a range
		unsigned int indexSize = (unsigned int)dc.Triangles.size() * 3;
		int materialIndex = dc.MaterialIndex > -1 ? dc.MaterialIndex : -1;
		m_index_ranges.push_back({ indexOffset, indexSize, 0, materialIndex });

		indexOffset = (unsigned int)indices.size();
	}

	// Vertex array descriptor
	D3D11_BUFFER_DESC vertexbufferDesc = { 0 };
	vertexbufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexbufferDesc.CPUAccessFlags = 0;
	vertexbufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexbufferDesc.MiscFlags = 0;
	vertexbufferDesc.ByteWidth = (UINT)(mesh->Vertices.size() * sizeof(Vertex));
	// Data resource
	D3D11_SUBRESOURCE_DATA vertexData = { 0 };
	vertexData.pSysMem = &(mesh->Vertices)[0];
	// Create vertex buffer on device using descriptor & data
	dxdevice->CreateBuffer(&vertexbufferDesc, &vertexData, &m_vertex_buffer);
	SETNAME(m_vertex_buffer, "VertexBuffer");

	// Index array descriptor
	D3D11_BUFFER_DESC indexbufferDesc = { 0 };
	indexbufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexbufferDesc.CPUAccessFlags = 0;
	indexbufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexbufferDesc.MiscFlags = 0;
	indexbufferDesc.ByteWidth = (UINT)(indices.size() * sizeof(unsigned));
	// Data resource
	D3D11_SUBRESOURCE_DATA indexData = { 0 };
	indexData.pSysMem = &indices[0];
	// Create index buffer on device using descriptor & data
	dxdevice->CreateBuffer(&indexbufferDesc, &indexData, &m_index_buffer);
	SETNAME(m_index_buffer, "IndexBuffer");

	// Copy materials from mesh
	append_materials(mesh->Materials);

	// Create the material buffer
	CreateMaterialBuffer();

	// Go through materials and load textures (if any) to device
	std::cout << "Loading textures..." << std::endl;
	for (auto& material : m_materials)
	{
		HRESULT hr;

		// Load Diffuse texture
		//
		if (material.DiffuseTextureFilename.size()) {

			hr = LoadTextureFromFile(
				dxdevice,
				material.DiffuseTextureFilename.c_str(),
				&material.DiffuseTexture);
			std::cout << "\t" << material.DiffuseTextureFilename
				<< (SUCCEEDED(hr) ? " - OK" : "- FAILED") << std::endl;
		}

		// + other texture types here - see Material class
		// ...
	}
	std::cout << "Done." << std::endl;

	SAFE_DELETE(mesh);
}

void OBJModel::Render() const
{
	// Bind vertex buffer
	const UINT32 stride = sizeof(Vertex);
	const UINT32 offset = 0;
	m_dxdevice_context->IASetVertexBuffers(0, 1, &m_vertex_buffer, &stride, &offset);

	// Bind index buffer
	m_dxdevice_context->IASetIndexBuffer(m_index_buffer, DXGI_FORMAT_R32_UINT, 0);

	// Iterate Drawcalls
	for (auto& indexRange : m_index_ranges)
	{
		// Fetch material
		const Material& material = m_materials[indexRange.MaterialIndex];

		// Update material buffer
		UpdateMaterialBuffer(material);

		// Bind diffuse texture to slot t0 of the PS
		m_dxdevice_context->PSSetShaderResources(0, 1, &material.DiffuseTexture.TextureView);
		// + bind other textures here, e.g. a normal map, to appropriate slots
		m_dxdevice_context->PSSetConstantBuffers(1, 1, &m_material_buffer);

		// Make the drawcall
		m_dxdevice_context->DrawIndexed(indexRange.Size, indexRange.Start, 0);
	}
}

void OBJModel::CreateMaterialBuffer()
{
	HRESULT hr;
	D3D11_BUFFER_DESC buffer_desc = {};
	buffer_desc.Usage = D3D11_USAGE_DYNAMIC;
	buffer_desc.ByteWidth = sizeof(MaterialBuffer);
	buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	hr = m_dxdevice->CreateBuffer(&buffer_desc, nullptr, &m_material_buffer);
	ASSERT(SUCCEEDED(hr)); // Check if buffer creation was successful
	if (FAILED(hr))
	{
		// Handle error (e.g., log error message, throw exception, etc.)
	}
}

void OBJModel::UpdateMaterialBuffer(const Material& material) const
{
	// Map the resource buffer, obtain a pointer, and then write the material properties to it
	D3D11_MAPPED_SUBRESOURCE resource;
	m_dxdevice_context->Map(m_material_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
	MaterialBuffer* buffer_data = static_cast<MaterialBuffer*>(resource.pData);

	// Copy material properties to the buffer
	buffer_data->AmbientColor = vec4f(material.AmbientColour, 1.0f);
	buffer_data->DiffuseColor = vec4f(material.DiffuseColour, 1.0f);
	buffer_data->SpecularColor = vec4f(material.SpecularColour, 1.0f);

	// Unmap the buffer
	m_dxdevice_context->Unmap(m_material_buffer, 0);
}




OBJModel::~OBJModel()
{
	for (auto& material : m_materials)
	{
		SAFE_RELEASE(material.DiffuseTexture.TextureView);

		// Release other used textures ...
	}
	SAFE_RELEASE(m_material_buffer);
}