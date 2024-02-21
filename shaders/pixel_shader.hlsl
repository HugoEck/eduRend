
Texture2D texDiffuse : register(t0);
SamplerState texDiffuseSampler : register(s0);

struct PSIn
{
	float4 Pos  : SV_Position;
	float3 Normal : NORMAL;
	float2 TexCoord : TEX;
    float3 WorldPos : WORLDPOS;
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
	//float Shininess = 50.0f;
};

//-----------------------------------------------------------------------------------------
// Pixel Shader
//-----------------------------------------------------------------------------------------

float4 PS_main(PSIn input) : SV_Target
{
    float Shininess = 50.0f;
// Calculate the direction from the surface point to the light source
    float3 lightDirection = normalize(lightPosition.xyz - input.WorldPos);

    // Calculate the diffuse lighting contribution using Lambert's cosine law
    float diffuseFactor = max(0.0f, dot(input.Normal, lightDirection));
    float3 diffuseLight = DiffuseColour * diffuseFactor;
    
    
    // Calculate the view direction
    float3 viewDirection = normalize(cameraPosition.xyz - input.WorldPos);

    // Calculate the reflection direction
    float3 reflectionDirection = reflect(-lightDirection, input.Normal);

    // Calculate the specular lighting contribution using the Phong reflection model
    float specularFactor = pow(max(0.0f, dot(viewDirection, reflectionDirection)), Shininess);
    float3 specularLight = SpecularColour * specularFactor;

    // Combine ambient, diffuse, and specular lighting
    float3 finalColor = AmbientColour + diffuseLight + specularLight;

    // Return the final color as a float4
    return float4(finalColor, Shininess);
};