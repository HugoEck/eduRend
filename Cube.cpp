#include "Cube.h"
#pragma once

#include "C:\Users\hugo\Unity_Projects\eduRend\src\model.h"


    Cube::Cube(ID3D11Device* dxdevice, ID3D11DeviceContext* dxdevice_context)
        : Model(dxdevice, dxdevice_context)
    {
        // Vertex and index arrays
        std::vector<Vertex> vertices;
        std::vector<unsigned> indices;

        // Populate the vertex array with 24 unique Vertices for a cube
        for (int i = 0; i < 8; ++i)
        {
            Vertex vertex;

            float x = (i & 1) ? 0.5f : -0.5f;
            float y = (i & 2) ? 0.5f : -0.5f;
            float z = (i & 4) ? 0.5f : -0.5f;

            vertex.Position = { x, y, z };
            vertex.Normal = { x, y, z };

            vertices.push_back(vertex);
        }

        // Define the indices for the cube
        indices = { 0, 1, 2, 0, 2, 3, 4, 5, 6, 4, 6, 7, 8, 9, 10, 8, 10, 11, 12, 13, 14, 12, 14, 15, 16, 17, 18, 16, 18, 19, 20, 21, 22, 20, 22, 23 };

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
        // Bind our vertex buffer
        const UINT32 stride = sizeof(Vertex);
        const UINT32 offset = 0;
        m_dxdevice_context->IASetVertexBuffers(0, 1, &m_vertex_buffer, &stride, &offset);

        // Bind our index buffer
        m_dxdevice_context->IASetIndexBuffer(m_index_buffer, DXGI_FORMAT_R32_UINT, 0);

        // Make the draw call
        m_dxdevice_context->DrawIndexed(m_number_of_indices, 0, 0);
    }