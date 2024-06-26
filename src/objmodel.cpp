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

		// Compute Tangent and Binormal
		ComputeTangentBinormal(mesh->Vertices, indices);
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
		if (material.DiffuseTextureFilename.size()) {

			hr = LoadTextureFromFile(
				dxdevice,
				material.DiffuseTextureFilename.c_str(),
				&material.DiffuseTexture);
			std::cout << "\t" << material.DiffuseTextureFilename
				<< (SUCCEEDED(hr) ? " - OK" : "- FAILED") << std::endl;
		}
		if (material.NormalTextureFilename.size()) {

			hr = LoadTextureFromFile(
				dxdevice,
				material.NormalTextureFilename.c_str(),
				&material.NormalTexture);
			std::cout << "\t" << material.NormalTextureFilename
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
	m_dxdevice_context->PSSetConstantBuffers(1, 1, &m_material_buffer);

	vec4f ambientColor = { 0.2f, 0.2f, 0.2f, 1.0f };
	vec4f diffuseColor = { 1.0f, 1.0f, 1.0f, 1.0f };
	vec4f specularColor = { 1.0f, 1.0f, 1.0f, 1.0f };

	UpdateMaterialBuffer(ambientColor, diffuseColor, specularColor);

	// Iterate Drawcalls
	for (auto& indexRange : m_index_ranges)
	{
		// Fetch material
		const Material& material = m_materials[indexRange.MaterialIndex];

		// Bind diffuse texture to slot t0 of the PS
		m_dxdevice_context->PSSetShaderResources(0, 1, &material.DiffuseTexture.TextureView);
		m_dxdevice_context->PSSetShaderResources(1, 1, &material.NormalTexture.TextureView);

		// Make the drawcall
		m_dxdevice_context->DrawIndexed(indexRange.Size, indexRange.Start, 0);
	}
}

void OBJModel::CreateMaterialBuffer()
{
	HRESULT hr;
	D3D11_BUFFER_DESC materialBufferDesc = {};
	materialBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	materialBufferDesc.ByteWidth = sizeof(MaterialBuffer);
	materialBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	materialBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	materialBufferDesc.MiscFlags = 0;
	materialBufferDesc.StructureByteStride = 0;

	ASSERT(hr = m_dxdevice->CreateBuffer(&materialBufferDesc, nullptr, &m_material_buffer));
	std::cout << "HR RESULT MATERIALBUFFER " << hr << std::endl;
}

void OBJModel::UpdateMaterialBuffer(const vec4f ambientColor, const vec4f diffuseColor, const vec4f specularColor) const
{
	// Map the resource buffer, obtain a pointer, and then write the material properties to it
	D3D11_MAPPED_SUBRESOURCE resource;
	m_dxdevice_context->Map(m_material_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
	MaterialBuffer* buffer_data = static_cast<MaterialBuffer*>(resource.pData);

	// Copy material properties to the buffer
	buffer_data->AmbientColor = ambientColor;
	buffer_data->DiffuseColor = diffuseColor;
	buffer_data->SpecularColor = specularColor;

	// Unmap the buffer
	m_dxdevice_context->Unmap(m_material_buffer, 0);
}

void OBJModel::ComputeTangentBinormal(std::vector<Vertex>& vertices, std::vector<unsigned>& indices) {
	for (size_t i = 0; i < indices.size(); i += 3) {
		const Vertex& v0 = vertices[indices[i]];
		const Vertex& v1 = vertices[indices[i + 1]];
		const Vertex& v2 = vertices[indices[i + 2]];

		// Calculate edge vectors
		linalg::vec3f edge1 = v1.Position - v0.Position;
		linalg::vec3f edge2 = v2.Position - v0.Position;

		// Calculate texture coordinate differences
		linalg::vec2f deltaUV1 = v1.TexCoord - v0.TexCoord;
		linalg::vec2f deltaUV2 = v2.TexCoord - v0.TexCoord;

		// Calculate tangent
		float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);
		linalg::vec3f tangent;
		tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
		tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
		tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
		tangent = normalize(tangent);

		// Calculate binormal
		linalg::vec3f binormal;
		binormal.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
		binormal.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
		binormal.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
		binormal = normalize(binormal);

		// Assign tangent and binormal to vertices
		vertices[indices[i]].Tangent = tangent;
		vertices[indices[i + 1]].Tangent = tangent;
		vertices[indices[i + 2]].Tangent = tangent;

		vertices[indices[i]].Binormal = binormal;
		vertices[indices[i + 1]].Binormal = binormal;
		vertices[indices[i + 2]].Binormal = binormal;
	}
}




OBJModel::~OBJModel()
{
	for (auto& material : m_materials)
	{
		SAFE_RELEASE(material.DiffuseTexture.TextureView);
		SAFE_RELEASE(material.NormalTexture.TextureView);

		// Release other used textures ...
	}
	SAFE_RELEASE(m_material_buffer);
}