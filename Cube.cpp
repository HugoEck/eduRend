#include "Cube.h"
#include "C:\Users\hugo\OneDrive\Desktop\Datorgrafik\eduRend\src\model.h"
#pragma once



    Cube::Cube(ID3D11Device* dxdevice, ID3D11DeviceContext* dxdevice_context)
        : Model(dxdevice, dxdevice_context)
    {

        // Vertex and index arrays
        std::vector<Vertex> vertices;
        std::vector<unsigned> indices;
        std::vector<Material> m_materials;

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

        // Initialize cube map filenames
        const char* cube_filenames[6] = {
            "cubemaps/brightday/posx.png",
            "cubemaps/brightday/negx.png",
            "cubemaps/brightday/posy.png",
            "cubemaps/brightday/negy.png",
            "cubemaps/brightday/posz.png",
            "cubemaps/brightday/negz.png"
        };

        // Load cube map texture
        HRESULT hr = LoadCubeTextureFromFile(m_dxdevice, cube_filenames, &cube_texture);
        if (SUCCEEDED(hr))
            std::cout << "Cubemap loaded successfully" << std::endl;
        else
            std::cout << "Cubemap failed to load" << std::endl;

        Vertex v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23;

        // Define the vertices for a cube
        // Front face
        //v0.Position = { -0.5f, -0.5f, 0.5f };
        //v0.Normal = { 0, 0, 1 };
        //v0.TexCoord = { 0, 0 };
        //
        //v1.Position = { 0.5f, -0.5f, 0.5f };
        //v1.Normal = { 0, 0, 1 };
        //v1.TexCoord = { 0, 1 };
        //
        //v2.Position = { 0.5f, 0.5f, 0.5f };
        //v2.Normal = { 0, 0, 1 };
        //v2.TexCoord = { 1, 1 };
        //
        //v3.Position = { -0.5f, 0.5f, 0.5f };
        //v3.Normal = { 0, 0, 1 };
        //v3.TexCoord = { 1, 0 };

        // Front face (inverted winding order)
        v0.Position = { -0.5f, -0.5f, 0.5f };
        v0.Normal = { 0, 0, -1 }; // Invert the normal
        v0.TexCoord = { 1, 1 }; // Flip the texture coordinates

        v1.Position = { 0.5f, -0.5f, 0.5f };
        v1.Normal = { 0, 0, -1 };
        v1.TexCoord = { 0, 1 };

        v2.Position = { 0.5f, 0.5f, 0.5f };
        v2.Normal = { 0, 0, -1 };
        v2.TexCoord = { 0, 0 };

        v3.Position = { -0.5f, 0.5f, 0.5f };
        v3.Normal = { 0, 0, -1 };
        v3.TexCoord = { 1, 0 };
        
        vertices.push_back(v0);
        vertices.push_back(v1);
        vertices.push_back(v2);
        vertices.push_back(v3);
        
        //// Back face
        //v4.Position = { 0.5f, -0.5f, -0.5f };
        //v4.Normal = { 0, 0, -1 };
        //v4.TexCoord = { 1, 0 };
        //
        //v5.Position = { -0.5f, -0.5f, -0.5f };
        //v5.Normal = { 0, 0, -1 };
        //v5.TexCoord = { 0, 0 };
        //
        //v6.Position = { -0.5f, 0.5f, -0.5f };
        //v6.Normal = { 0, 0, -1 };
        //v6.TexCoord = { 0, 1 };
        //
        //v7.Position = { 0.5f, 0.5f, -0.5f };
        //v7.Normal = { 0, 0, -1 };
        //v7.TexCoord = { 1, 1 };

        // Back face (inverted winding order)
        v4.Position = { 0.5f, -0.5f, -0.5f };
        v4.Normal = { 0, 0, 1 }; // Invert the normal
        v4.TexCoord = { 1, 0 }; // Flip the texture coordinates

        v5.Position = { -0.5f, -0.5f, -0.5f };
        v5.Normal = { 0, 0, 1 };
        v5.TexCoord = { 0, 0 };

        v6.Position = { -0.5f, 0.5f, -0.5f };
        v6.Normal = { 0, 0, 1 };
        v6.TexCoord = { 0, 1 };

        v7.Position = { 0.5f, 0.5f, -0.5f };
        v7.Normal = { 0, 0, 1 };
        v7.TexCoord = { 1, 1 };
        
        vertices.push_back(v4);
        vertices.push_back(v5);
        vertices.push_back(v6);
        vertices.push_back(v7);
        
        //// Right face
        //v8.Position = { 0.5f, -0.5f, 0.5f };
        //v8.Normal = { 1, 0, 0 };
        //v8.TexCoord = { 0, 0 };
        //
        //v9.Position = { 0.5f, -0.5f, -0.5f };
        //v9.Normal = { 1, 0, 0 };
        //v9.TexCoord = { 1, 0 };
        //
        //v10.Position = { 0.5f, 0.5f, -0.5f };
        //v10.Normal = { 1, 0, 0 };
        //v10.TexCoord = { 1, 1 };
        //
        //v11.Position = { 0.5f, 0.5f, 0.5f };
        //v11.Normal = { 1, 0, 0 };
        //v11.TexCoord = { 0, 1 };

        // Right face (inverted winding order)
        v8.Position = { 0.5f, -0.5f, 0.5f };
        v8.Normal = { -1, 0, 0 }; // Invert the normal
        v8.TexCoord = { 0, 1 }; // Flip the texture coordinates

        v9.Position = { 0.5f, -0.5f, -0.5f };
        v9.Normal = { -1, 0, 0 };
        v9.TexCoord = { 1, 1 };

        v10.Position = { 0.5f, 0.5f, -0.5f };
        v10.Normal = { -1, 0, 0 };
        v10.TexCoord = { 1, 0 };

        v11.Position = { 0.5f, 0.5f, 0.5f };
        v11.Normal = { -1, 0, 0 };
        v11.TexCoord = { 0, 0 };
        
        vertices.push_back(v8);
        vertices.push_back(v9);
        vertices.push_back(v10);
        vertices.push_back(v11);
        
        //// Left face
        //v12.Position = { -0.5f, -0.5f, -0.5f };
        //v12.Normal = { -1, 0, 0 };
        //v12.TexCoord = { 0, 1 };
        //
        //v13.Position = { -0.5f, -0.5f, 0.5f };
        //v13.Normal = { -1, 0, 0 };
        //v13.TexCoord = { 1, 1 };
        //
        //v14.Position = { -0.5f, 0.5f, 0.5f };
        //v14.Normal = { -1, 0, 0 };
        //v14.TexCoord = { 1, 0 };
        //
        //v15.Position = { -0.5f, 0.5f, -0.5f };
        //v15.Normal = { -1, 0, 0 };
        //v15.TexCoord = { 0, 0 };

        // Left face (inverted winding order)
        v12.Position = { -0.5f, -0.5f, -0.5f };
        v12.Normal = { 1, 0, 0 }; // Invert the normal
        v12.TexCoord = { 0, 0 }; // Flip the texture coordinates

        v13.Position = { -0.5f, -0.5f, 0.5f };
        v13.Normal = { 1, 0, 0 };
        v13.TexCoord = { 1, 0 };

        v14.Position = { -0.5f, 0.5f, 0.5f };
        v14.Normal = { 1, 0, 0 };
        v14.TexCoord = { 1, 1 };

        v15.Position = { -0.5f, 0.5f, -0.5f };
        v15.Normal = { 1, 0, 0 };
        v15.TexCoord = { 0, 1 };
        
        vertices.push_back(v12);
        vertices.push_back(v13);
        vertices.push_back(v14);
        vertices.push_back(v15);
        
        //// Top face
        //v16.Position = { -0.5f, 0.5f, 0.5f };
        //v16.Normal = { 0, 1, 0 };
        //v16.TexCoord = { 0, 0 };
        //
        //v17.Position = { 0.5f, 0.5f, 0.5f };
        //v17.Normal = { 0, 1, 0 };
        //v17.TexCoord = { 1, 0 };
        //
        //v18.Position = { 0.5f, 0.5f, -0.5f };
        //v18.Normal = { 0, 1, 0 };
        //v18.TexCoord = { 1, 1 };
        //
        //v19.Position = { -0.5f, 0.5f, -0.5f };
        //v19.Normal = { 0, 1, 0 };
        //v19.TexCoord = { 0, 1 };

        // Top face (inverted winding order)
        v16.Position = { -0.5f, 0.5f, 0.5f };
        v16.Normal = { 0, -1, 0 }; // Invert the normal
        v16.TexCoord = { 0, 0 }; // Flip the texture coordinates

        v17.Position = { 0.5f, 0.5f, 0.5f };
        v17.Normal = { 0, -1, 0 };
        v17.TexCoord = { 1, 0 };

        v18.Position = { 0.5f, 0.5f, -0.5f };
        v18.Normal = { 0, -1, 0 };
        v18.TexCoord = { 1, 1 };

        v19.Position = { -0.5f, 0.5f, -0.5f };
        v19.Normal = { 0, -1, 0 };
        v19.TexCoord = { 0, 1 };
        
        vertices.push_back(v16);
        vertices.push_back(v17);
        vertices.push_back(v18);
        vertices.push_back(v19);
        
        //// Bottom face
        //v20.Position = { -0.5f, -0.5f, -0.5f };
        //v20.Normal = { 0, -1, 0 };
        //v20.TexCoord = { 1, 0 };
        //
        //v21.Position = { 0.5f, -0.5f, -0.5f };
        //v21.Normal = { 0, -1, 0 };
        //v21.TexCoord = { 0, 0 };
        //
        //v22.Position = { 0.5f, -0.5f, 0.5f };
        //v22.Normal = { 0, -1, 0 };
        //v22.TexCoord = { 0, 1 };
        //
        //v23.Position = { -0.5f, -0.5f, 0.5f };
        //v23.Normal = { 0, -1, 0 };
        //v23.TexCoord = { 1, 1 };

        // Bottom face (inverted winding order)
        v20.Position = { -0.5f, -0.5f, -0.5f };
        v20.Normal = { 0, 1, 0 }; // Invert the normal
        v20.TexCoord = { 1, 0 }; // Flip the texture coordinates

        v21.Position = { 0.5f, -0.5f, -0.5f };
        v21.Normal = { 0, 1, 0 };
        v21.TexCoord = { 0, 0 };

        v22.Position = { 0.5f, -0.5f, 0.5f };
        v22.Normal = { 0, 1, 0 };
        v22.TexCoord = { 0, 1 };

        v23.Position = { -0.5f, -0.5f, 0.5f };
        v23.Normal = { 0, 1, 0 };
        v23.TexCoord = { 1, 1 };
        
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

        InitializeCubeMapSamplerState(
            D3D11_FILTER_MIN_MAG_MIP_LINEAR, // Filter type
            D3D11_TEXTURE_ADDRESS_CLAMP,     // Address mode for U coordinate
            D3D11_TEXTURE_ADDRESS_CLAMP,     // Address mode for V coordinate
            D3D11_TEXTURE_ADDRESS_CLAMP,      // Address mode for W coordinate
            16                                // Anisotropy level
        );
    }

    // Implement the Render method for rendering the cube
    void Cube::Render() const
    {
        // Bind our vertex buffer
        const UINT32 stride = sizeof(Vertex);
        const UINT32 offset = 0;
        m_dxdevice_context->IASetVertexBuffers(0, 1, &m_vertex_buffer, &stride, &offset);

        // Bind our index buffer
        m_dxdevice_context->IASetIndexBuffer(m_index_buffer, DXGI_FORMAT_R32_UINT, 0);

        // Make the draw call
        m_dxdevice_context->DrawIndexed(m_number_of_indices, 0, 0);

        m_dxdevice_context->PSSetSamplers(1, 1, &m_cubeMapSamplerState);

        // Set cube map texture in pixel shader
        const unsigned cube_slot = 2; // Choose a suitable slot for the cube map texture
        m_dxdevice_context->PSSetShaderResources(cube_slot, 1, &cube_texture.TextureView);
    }

    void Cube::InitializeCubeMapSamplerState(
        D3D11_FILTER filter,
        D3D11_TEXTURE_ADDRESS_MODE addressU,
        D3D11_TEXTURE_ADDRESS_MODE addressV,
        D3D11_TEXTURE_ADDRESS_MODE addressW,
        UINT maxAnisotropy)
    {
        HRESULT hr;

        // Sampler state description
        D3D11_SAMPLER_DESC samplerDesc = {};
        samplerDesc.Filter = filter;
        samplerDesc.AddressU = addressU;
        samplerDesc.AddressV = addressV;
        samplerDesc.AddressW = addressW;
        samplerDesc.MaxAnisotropy = maxAnisotropy;
        samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS; // Default comparison function
        samplerDesc.MinLOD = 0; // Default minimum level-of-detail
        samplerDesc.MaxLOD = D3D11_FLOAT32_MAX; // Default maximum level-of-detail

        // Create the sampler state
        ASSERT(hr = m_dxdevice->CreateSamplerState(&samplerDesc, &m_cubeMapSamplerState));
    }