#pragma once
#include "C:\Users\hugo\OneDrive\Desktop\Datorgrafik\eduRend\src\model.h"

class Cube : public Model
{
    ID3D11SamplerState* m_cubeMapSamplerState = nullptr;
    std::vector<Material> m_cubeTextures;

    unsigned m_number_of_indices = 0;
public:

    Cube(ID3D11Device* dxdevice, ID3D11DeviceContext* dxdevice_context);

    virtual void Render() const override;

    virtual ~Cube() {};

};

