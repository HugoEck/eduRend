
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

cbuffer MaterialBuffer
{
	float3 AmbientColor;
    float3 DiffuseColor;
	float3 SpecularColor;
	float Shininess = 1.0f;
};

//-----------------------------------------------------------------------------------------
// Pixel Shader
//-----------------------------------------------------------------------------------------

float4 PS_main(PSIn input) : SV_Target
{
    
    // Inside the pixel shader function
    //float4 diffuseColor = texDiffuse.Sample(texDiffuseSampler, input.TexCoord);
	// Debug shading #1: map and return normal as a color, i.e. from [-1,1]->[0,1] per component
	// The 4:th component is opacity and should be = 1

   // return /*float4(input.Normal * 0.5 + 0.5, 1) + */float4(DiffuseColor, Shininess);
    /*+float4(AmbientColor, Shininess) + float4(SpecularColor, Shininess);*/
	
	// Debug shading #2: map and return texture coordinates as a color (blue = 0)
//	return float4(input.TexCoord, 0, 1);
    
   // float4 ambient = float4(AmbientColor, 1.0f);
   // float4 diffuse = float4(DiffuseColor, 1.0f);
   // float4 specular = float4(SpecularColor, 1.0f);
   // float4 cameraPos = float4(cameraPosition);
   // float4 lightPos = float4(lightPosition);
   // 
   // // Debug shading #1: map and return normal as a color, i.e. from [-1,1]->[0,1] per component
   // // The 4:th component is opacity and should be = 1
   // return float4(input.Normal * 0.5 + 0.5, 1) + diffuse + ambient + specular + cameraPos + lightPos;
	
    // Debug shading #2: map and return texture coordinates as a color (blue = 0)
    // return float4(input.TexCoord, 0, 1);
    
    //    // Calculate the direction from the surface point to the light source
    //float3 lightDirection = normalize(lightPosition.xyz - input.Pos.xyz);
    //
    //// Calculate the diffuse lighting contribution using Lambert's cosine law
    //float diffuseFactor = max(0.0f, dot(input.Normal, lightDirection));
    //float3 diffuseLight = DiffuseColor * diffuseFactor;
    //
    //// Calculate the specular lighting contribution using the Phong reflection model
    //float3 viewDirection = normalize(cameraPosition.xyz - input.Pos.xyz);
    //float3 reflectionDirection = reflect(-lightDirection, input.Normal);
    //float specularFactor = pow(max(0.0f, dot(viewDirection, reflectionDirection)), Shininess);
    //float3 specularLight = SpecularColor * specularFactor;
    //
    //// Combine ambient, diffuse, and specular lighting
    //float3 finalColor = AmbientColor + diffuseLight + specularLight;
    //
    //// Apply the final color with the diffuse texture
    //return texDiffuse.Sample(texDiffuseSampler, input.TexCoord) * float4(finalColor, 1.0f);
    
        // Sample the diffuse texture
    float4 diffuseColor = texDiffuse.Sample(texDiffuseSampler, input.TexCoord);

    // Calculate the direction from the surface point to the light source
    float3 lightDirection = normalize(lightPosition.xyz - input.Pos.xyz);

    // Calculate the cosine of the angle between the normal and light direction
    float NdotL = max(dot(input.Normal, lightDirection), 0.0f);

    // Calculate the view direction
    float3 viewDirection = normalize(cameraPosition.xyz - input.Pos.xyz);

    // Calculate the reflection direction
    float3 reflectionDirection = reflect(-lightDirection, input.Normal);

    // Calculate the specular intensity using the reflection direction and view direction
    float specularIntensity = pow(max(dot(reflectionDirection, viewDirection), 0.0f), Shininess);

    // Combine ambient, diffuse, and specular components
    float3 finalColor = AmbientColor + diffuseColor.rgb * NdotL + SpecularColor * specularIntensity;

    // Return the final color as a float4
    return float4(finalColor, 0.0f);
    
}