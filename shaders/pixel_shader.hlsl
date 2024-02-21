
Texture2D texDiffuse : register(t0);
SamplerState texDiffuseSampler : register(s0);

struct PSIn
{
	float4 Pos  : SV_Position;
	float3 Normal : NORMAL;
	float2 TexCoord : TEX;
};

cbuffer LightCameraBuffer : register(b0)
{
    float4 cameraPosition;
    float4 lightPosition;
}

cbuffer MaterialBuffer : register(b1)
{
	float3 AmbientColour;
    float3 DiffuseColour;
	float3 SpecularColour;
	float Shininess;
};

//-----------------------------------------------------------------------------------------
// Pixel Shader
//-----------------------------------------------------------------------------------------

float4 PS_main(PSIn input) : SV_Target
{
    
// Calculate the direction from the surface point to the light source
    float3 lightDirection = normalize(lightPosition.xyz - input.Pos.xyz);

    // Calculate the diffuse lighting contribution using Lambert's cosine law
    float diffuseFactor = max(0.0f, dot(input.Normal, lightDirection));
    float3 diffuseLight = DiffuseColour * diffuseFactor;
    
    
    //// Calculate the view direction
    //float3 viewDirection = normalize(cameraPosition.xyz - input.Pos.xyz);

    //// Calculate the reflection direction
    //float3 reflectionDirection = reflect(-lightDirection, input.Normal);

    //// Calculate the specular lighting contribution using the Phong reflection model
    //float specularFactor = pow(max(0.0f, dot(viewDirection, reflectionDirection)), Shininess);
    //float3 specularLight = SpecularColour * specularFactor;

    // Combine ambient, diffuse, and specular lighting
    float3 finalColor = AmbientColour + diffuseLight + SpecularColour;

    // Return the final color as a float4
    return float4(finalColor, Shininess);
};