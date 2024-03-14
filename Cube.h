#pragma once
#include "C:\Users\hugo\OneDrive\Desktop\Datorgrafik\eduRend\src\model.h"

class Cube : public Model
{
    ID3D11SamplerState* m_cubeMapSamplerState = nullptr;
    Texture cube_texture;
    unsigned m_number_of_indices = 0;
public:

    Cube(ID3D11Device* dxdevice, ID3D11DeviceContext* dxdevice_context);

    virtual void Render() const override;

    void InitializeCubeMapSamplerState(
        D3D11_FILTER filter,
        D3D11_TEXTURE_ADDRESS_MODE addressU,
        D3D11_TEXTURE_ADDRESS_MODE addressV,
        D3D11_TEXTURE_ADDRESS_MODE addressW,
        UINT maxAnisotropy);

    virtual ~Cube() {};

};

