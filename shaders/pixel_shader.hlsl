
Texture2D texDiffuse : register(t0);
SamplerState texDiffuseSampler : register(s0);

Texture2D texNormal : register(t1);
SamplerState texNormalSampler : register(s1);

struct PSIn
{
	float4 Pos  : SV_Position;
	float3 Normal : NORMAL;
	float2 TexCoord : TEX;
    float3 WorldPos : WORLDPOS;
    float3 Tangent : TANGENT; // Added tangent
    float3 Binormal : BINORMAL; // Added binormal
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
};

//-----------------------------------------------------------------------------------------
// Pixel Shader
//-----------------------------------------------------------------------------------------

float4 PS_main(PSIn input) : SV_Target
{
    //float Shininess = 50.0f;
    //
    //float3 diffuseTextureColor = texDiffuse.Sample(texDiffuseSampler, input.TexCoord).rgb;
    //
    //// Calculate the direction from the surface point to the light source
    //float3 lightDirection = normalize(lightPosition.xyz - input.WorldPos);
    //
    //// Calculate the diffuse lighting contribution using Lambert's cosine law
    //float3 diffuseLight = diffuseTextureColor.rgb * DiffuseColour.rgb * max(0.0f, dot(input.Normal, lightDirection));
    ////float3 diffuseLight = DiffuseColour * diffuseFactor;
    //
    //// Calculate the view direction
    //float3 viewDirection = normalize(cameraPosition.xyz - input.WorldPos);
    //
    //// Calculate the reflection direction
    //float3 reflectionDirection = reflect(-lightDirection, input.Normal);
    //
    //// Calculate the specular lighting contribution using the Phong reflection model
    //float3 specularLight = SpecularColour * pow(max(0.0f, dot(reflectionDirection, viewDirection)), Shininess);
    //
    //// Combine ambient, diffuse, and specular lighting
    //float3 finalColor = AmbientColour + diffuseLight + specularLight;
    //
    //// Return the final color as a float4
    //return float4(finalColor, Shininess);
    
    
    float Shininess = 50.0f;
    
    // Sample diffuse texture
    float3 diffuseTextureColor = texDiffuse.Sample(texDiffuseSampler, input.TexCoord).rgb;
    
    // Sample normal map texture and transform the normal from tangent space to world space
    float3 normalSample = texNormal.Sample(texNormalSampler, input.TexCoord).rgb * 2.0f - 1.0f; // Map from [0, 1] to [-1, 1]
    float3 normal = normalize(input.Tangent * normalSample.x + input.Binormal * normalSample.y + input.Normal * normalSample.z);
    
    // Calculate the direction from the surface point to the light source
    float3 lightDirection = normalize(lightPosition.xyz - input.WorldPos);
    
    // Calculate the diffuse lighting contribution using Lambert's cosine law
    float3 diffuseLight = diffuseTextureColor.rgb * DiffuseColour.rgb * max(0.0f, dot(normal, lightDirection));
    
    // Calculate the view direction
    float3 viewDirection = normalize(cameraPosition.xyz - input.WorldPos);
    
    // Calculate the reflection direction
    float3 reflectionDirection = reflect(-lightDirection, normal);
    
    // Calculate the specular lighting contribution using the Phong reflection model
    float3 specularLight = SpecularColour * pow(max(0.0f, dot(reflectionDirection, viewDirection)), Shininess);
    
    // Combine ambient, diffuse, and specular lighting
    float3 finalColor = AmbientColour + diffuseLight + specularLight;
    
    // Return the final color as a float4
    return float4(finalColor, Shininess);
    

};