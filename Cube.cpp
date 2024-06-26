#include "Cube.h"
#include "C:\Users\hugo\OneDrive\Desktop\Datorgrafik\eduRend\src\model.h"
#pragma once



Cube::Cube(ID3D11Device* dxdevice, ID3D11DeviceContext* dxdevice_context)
	: Model(dxdevice, dxdevice_context)
{

	// Vertex and index arrays
	std::vector<Vertex> vertices;
	std::vector<unsigned> indices;

	m_cubeTextures.resize(6);

	//HRESULT hr = LoadTextureFromFile(dxdevice, "C:/Users/victo/OneDrive/Desktop/eduRend-main/assets/textures/crate.png", &m_material.DiffuseTexture);
	HRESULT hr = LoadTextureFromFile(dxdevice, "cubemaps/brightday/posx.png", &m_cubeTextures[0].DiffuseTexture);
	hr = LoadTextureFromFile(dxdevice, "cubemaps/brightday/negx.png", &m_cubeTextures[1].DiffuseTexture);
	hr = LoadTextureFromFile(dxdevice, "cubemaps/brightday/posz.png", &m_cubeTextures[2].DiffuseTexture);
	hr = LoadTextureFromFile(dxdevice, "cubemaps/brightday/negz.png", &m_cubeTextures[3].DiffuseTexture);
	hr = LoadTextureFromFile(dxdevice, "cubemaps/brightday/posy.png", &m_cubeTextures[4].DiffuseTexture);
	hr = LoadTextureFromFile(dxdevice, "cubemaps/brightday/negy.png", &m_cubeTextures[5].DiffuseTexture);


	Vertex v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23;

	// Define the vertices for a cube
	// Front face
	v0.Position = { -0.5f, -0.5f, 0.5f };
	v0.Normal = { 0, 0, 0 };
	v0.TexCoord = { 0, 0 };
	
	v1.Position = { 0.5f, -0.5f, 0.5f };
	v1.Normal = { 0, 0, 0 };
	v1.TexCoord = { 0, 1 };
	
	v2.Position = { 0.5f, 0.5f, 0.5f };
	v2.Normal = { 0, 0, 0 };
	v2.TexCoord = { 1, 1 };
	
	v3.Position = { -0.5f, 0.5f, 0.5f };
	v3.Normal = { 0, 0, 0 };
	v3.TexCoord = { 1, 0 };

	//// Front face (inverted winding order)
	//v0.Position = { -0.5f, -0.5f, 0.5f };
	//v0.Normal = { 0, 0, -1 }; // Invert the normal
	//v0.TexCoord = { 1, 1 }; // Flip the texture coordinates

	//v1.Position = { 0.5f, -0.5f, 0.5f };
	//v1.Normal = { 0, 0, -1 };
	//v1.TexCoord = { 0, 1 };

	//v2.Position = { 0.5f, 0.5f, 0.5f };
	//v2.Normal = { 0, 0, -1 };
	//v2.TexCoord = { 0, 0 };

	//v3.Position = { -0.5f, 0.5f, 0.5f };
	//v3.Normal = { 0, 0, -1 };
	//v3.TexCoord = { 1, 0 };

	vertices.push_back(v0);
	vertices.push_back(v1);
	vertices.push_back(v2);
	vertices.push_back(v3);

	//// Back face
	v4.Position = { 0.5f, -0.5f, -0.5f };
	v4.Normal = { 0, 0, 0 };
	v4.TexCoord = { 1, 0 };
	
	v5.Position = { -0.5f, -0.5f, -0.5f };
	v5.Normal = { 0, 0, 0 };
	v5.TexCoord = { 0, 0 };
	
	v6.Position = { -0.5f, 0.5f, -0.5f };
	v6.Normal = { 0, 0, 0 };
	v6.TexCoord = { 0, 1 };
	
	v7.Position = { 0.5f, 0.5f, -0.5f };
	v7.Normal = { 0, 0, 0 };
	v7.TexCoord = { 1, 1 };
	
	//// Back face (inverted winding order)
	//v4.Position = { 0.5f, -0.5f, -0.5f };
	//v4.Normal = { 0, 0, 1 }; // Invert the normal
	//v4.TexCoord = { 1, 0 }; // Flip the texture coordinates
	
	//v5.Position = { -0.5f, -0.5f, -0.5f };
	//v5.Normal = { 0, 0, 1 };
	//v5.TexCoord = { 0, 0 };
	
	//v6.Position = { -0.5f, 0.5f, -0.5f };
	//v6.Normal = { 0, 0, 1 };
	//v6.TexCoord = { 0, 1 };
	
	//v7.Position = { 0.5f, 0.5f, -0.5f };
	//v7.Normal = { 0, 0, 1 };
	//v7.TexCoord = { 1, 1 };
	
	vertices.push_back(v4);
	vertices.push_back(v5);
	vertices.push_back(v6);
	vertices.push_back(v7);
	
	//// Right face
	//v8.Position = { 0.5f, -0.5f, 0.5f };
	//v8.Normal = { 0, 0, 0 };
	//v8.TexCoord = { 0, 0 };
	//
	//v9.Position = { 0.5f, -0.5f, -0.5f };
	//v9.Normal = { 0, 0, 0 };
	//v9.TexCoord = { 1, 0 };
	//
	//v10.Position = { 0.5f, 0.5f, -0.5f };
	//v10.Normal = { 0, 0, 0 };
	//v10.TexCoord = { 1, 1 };
	//
	//v11.Position = { 0.5f, 0.5f, 0.5f };
	//v11.Normal = { 0, 0, 0 };
	//v11.TexCoord = { 0, 1 };
	
	// Right face (inverted winding order)
	v8.Position = { 0.5f, -0.5f, 0.5f };
	v8.Normal = { 0, 0, 0 }; // Invert the normal
	v8.TexCoord = { 0, 1 }; // Flip the texture coordinates
	
	v9.Position = { 0.5f, -0.5f, -0.5f };
	v9.Normal = { 0, 0, 0 };
	v9.TexCoord = { 1, 1 };
	
	v10.Position = { 0.5f, 0.5f, -0.5f };
	v10.Normal = { 0, 0, 0 };
	v10.TexCoord = { 1, 0 };
	
	v11.Position = { 0.5f, 0.5f, 0.5f };
	v11.Normal = { 0, 0, 0 };
	v11.TexCoord = { 0, 0 };
	
	vertices.push_back(v8);
	vertices.push_back(v9);
	vertices.push_back(v10);
	vertices.push_back(v11);
	
	//// Left face
	v12.Position = { -0.5f, -0.5f, -0.5f };
	v12.Normal = { 0, 0, 0 };
	v12.TexCoord = { 0, 1 };
	
	v13.Position = { -0.5f, -0.5f, 0.5f };
	v13.Normal = { 0, 0, 0 };
	v13.TexCoord = { 1, 1 };
	
	v14.Position = { -0.5f, 0.5f, 0.5f };
	v14.Normal = { 0, 0, 0 };
	v14.TexCoord = { 1, 0 };
	
	v15.Position = { -0.5f, 0.5f, -0.5f };
	v15.Normal = { 0, 0, 0 };
	v15.TexCoord = { 0, 0 };
	
	// Left face (inverted winding order)
	//v12.Position = { -0.5f, -0.5f, -0.5f };
	//v12.Normal = { 1, 0, 0 }; // Invert the normal
	//v12.TexCoord = { 0, 0 }; // Flip the texture coordinates
	
	//v13.Position = { -0.5f, -0.5f, 0.5f };
	//v13.Normal = { 1, 0, 0 };
	//v13.TexCoord = { 1, 0 };
	
	//v14.Position = { -0.5f, 0.5f, 0.5f };
	//v14.Normal = { 1, 0, 0 };
	//v14.TexCoord = { 1, 1 };
	
	//v15.Position = { -0.5f, 0.5f, -0.5f };
	//v15.Normal = { 1, 0, 0 };
	//v15.TexCoord = { 0, 1 };
	
	vertices.push_back(v12);
	vertices.push_back(v13);
	vertices.push_back(v14);
	vertices.push_back(v15);
	
	//// Top face
	v16.Position = { -0.5f, 0.5f, 0.5f };
	v16.Normal = { 0, 0, 0 };
	v16.TexCoord = { 1, 0 }; // Flip the v-coordinate

	v17.Position = { 0.5f, 0.5f, 0.5f };
	v17.Normal = { 0, 0, 0 };
	v17.TexCoord = { 0, 0 }; // Flip the v-coordinate

	v18.Position = { 0.5f, 0.5f, -0.5f };
	v18.Normal = { 0, 0, 0 };
	v18.TexCoord = { 0, 1 }; // Flip the v-coordinate

	v19.Position = { -0.5f, 0.5f, -0.5f };
	v19.Normal = { 0, 0, 0 };
	v19.TexCoord = { 1, 1 }; // Flip the v-coordinate
	
	//// Top face (inverted winding order)
	//v16.Position = { -0.5f, 0.5f, 0.5f };
	//v16.Normal = { 0, -1, 0 }; // Invert the normal
	//v16.TexCoord = { 0, 0 }; // Flip the texture coordinates
	
	//v17.Position = { 0.5f, 0.5f, 0.5f };
	//v17.Normal = { 0, -1, 0 };
	//v17.TexCoord = { 1, 0 };
	
	//v18.Position = { 0.5f, 0.5f, -0.5f };
	//v18.Normal = { 0, -1, 0 };
	//v18.TexCoord = { 1, 1 };
	
	//v19.Position = { -0.5f, 0.5f, -0.5f };
	//v19.Normal = { 0, -1, 0 };
	//v19.TexCoord = { 0, 1 };
	
	vertices.push_back(v16);
	vertices.push_back(v17);
	vertices.push_back(v18);
	vertices.push_back(v19);
	
	// Bottom face
	v20.Position = { -0.5f, -0.5f, -0.5f };
	v20.Normal = { 0, 0, 0 };
	v20.TexCoord = { 1, 0 };
	
	v21.Position = { 0.5f, -0.5f, -0.5f };
	v21.Normal = { 0, 0, 0 };
	v21.TexCoord = { 0, 0 };
	
	v22.Position = { 0.5f, -0.5f, 0.5f };
	v22.Normal = { 0, 0, 0 };
	v22.TexCoord = { 0, 1 };
	
	v23.Position = { -0.5f, -0.5f, 0.5f };
	v23.Normal = { 0, 0, 0 };
	v23.TexCoord = { 1, 1 };
	
	//// Bottom face (inverted winding order)
	//v20.Position = { -0.5f, -0.5f, -0.5f };
	//v20.Normal = { 0, 1, 0 }; // Invert the normal
	//v20.TexCoord = { 1, 0 }; // Flip the texture coordinates
	
	//v21.Position = { 0.5f, -0.5f, -0.5f };
	//v21.Normal = { 0, 1, 0 };
	//v21.TexCoord = { 0, 0 };
	
	//v22.Position = { 0.5f, -0.5f, 0.5f };
	//v22.Normal = { 0, 1, 0 };
	//v22.TexCoord = { 0, 1 };
	
	//v23.Position = { -0.5f, -0.5f, 0.5f };
	//v23.Normal = { 0, 1, 0 };
	//v23.TexCoord = { 1, 1 };
	
	vertices.push_back(v20);
	vertices.push_back(v21);
	vertices.push_back(v22);
	vertices.push_back(v23);

	// Define the indices for the cube
	// Front face
	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(3);
	indices.push_back(1);
	indices.push_back(2);
	indices.push_back(3);

	// Back face
	indices.push_back(4);
	indices.push_back(5);
	indices.push_back(7);
	indices.push_back(5);
	indices.push_back(6);
	indices.push_back(7);

	// Right face
	indices.push_back(8);
	indices.push_back(9);
	indices.push_back(11);
	indices.push_back(9);
	indices.push_back(10);
	indices.push_back(11);

	// Left face
	indices.push_back(12);
	indices.push_back(13);
	indices.push_back(15);
	indices.push_back(13);
	indices.push_back(14);
	indices.push_back(15);

	// Top face
	indices.push_back(16);
	indices.push_back(17);
	indices.push_back(19);
	indices.push_back(17);
	indices.push_back(18);
	indices.push_back(19);

	// Bottom face
	indices.push_back(20);
	indices.push_back(21);
	indices.push_back(23);
	indices.push_back(21);
	indices.push_back(22);
	indices.push_back(23);

	// Vertex buffer descriptor
	D3D11_BUFFER_DESC vertexbufferDesc = {};
	vertexbufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexbufferDesc.CPUAccessFlags = 0;
	vertexbufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexbufferDesc.MiscFlags = 0;
	vertexbufferDesc.ByteWidth = static_cast<UINT>(vertices.size() * sizeof(Vertex));

	// Data resource
	D3D11_SUBRESOURCE_DATA vertexData = {};
	vertexData.pSysMem = vertices.data();

	// Create vertex buffer on device using descriptor & data
	m_dxdevice->CreateBuffer(&vertexbufferDesc, &vertexData, &m_vertex_buffer);

	// Index buffer descriptor
	D3D11_BUFFER_DESC indexbufferDesc = {};
	indexbufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexbufferDesc.CPUAccessFlags = 0;
	indexbufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexbufferDesc.MiscFlags = 0;
	indexbufferDesc.ByteWidth = static_cast<UINT>(indices.size() * sizeof(unsigned));

	// Data resource
	D3D11_SUBRESOURCE_DATA indexData = {};
	indexData.pSysMem = indices.data();

	// Create index buffer on device using descriptor & data
	m_dxdevice->CreateBuffer(&indexbufferDesc, &indexData, &m_index_buffer);

	// Set the number of indices
	m_number_of_indices = (unsigned int)indices.size();
}

// Implement the Render method for rendering the cube
void Cube::Render() const
{
	// Get current rasterizer state
	ID3D11RasterizerState* prevRasterizerState = nullptr;
	m_dxdevice_context->RSGetState(&prevRasterizerState);

	// Disable back-face culling
	m_dxdevice_context->RSSetState(nullptr);

	// Bind vertex and index buffers
	const UINT32 stride = sizeof(Vertex);
	const UINT32 offset = 0;
	m_dxdevice_context->IASetVertexBuffers(0, 1, &m_vertex_buffer, &stride, &offset);
	m_dxdevice_context->IASetIndexBuffer(m_index_buffer, DXGI_FORMAT_R32_UINT, 0);

	// Bind textures and draw each quad separately
	for (int i = 0; i < 6; ++i) {
		m_dxdevice_context->PSSetShaderResources(0, 1, &m_cubeTextures[i].DiffuseTexture.TextureView);
		m_dxdevice_context->DrawIndexed(6, i * 6, 0); // Assuming each quad has 6 vertices
	}
	// Set back the original rasterizer state
	m_dxdevice_context->RSSetState(prevRasterizerState);

	// Release the reference to the previous rasterizer state
	if (prevRasterizerState)
		prevRasterizerState->Release();
}