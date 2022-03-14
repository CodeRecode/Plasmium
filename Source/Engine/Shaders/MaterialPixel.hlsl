cbuffer LightBuffer
{
	float4 ambientColor;
	float4 lightDiffuseColor;
	float3 lightDirection;
	float specularPower;
	float4 specularColor;
};

cbuffer MaterialBuffer
{
	float3 diffuseColor;
	float shininess;
};

struct PixelInputType
{
	float4 position : SV_POSITION;
	float3 normal : NORMAL;
	float3 viewDirection : TEXCOORD1;
};

float4 main(PixelInputType input) : SV_TARGET
{

	float lambert = saturate(dot(input.normal, lightDirection));
	float4 lightColor = ambientColor;
	float4 specular = float4(0.0f, 0.0f, 0.0f, 0.0f);

	if (lambert > 0.0f) {
		lightColor += lightDiffuseColor * lambert;
		lightColor = saturate(lightColor);

		float3 reflection = normalize(2 * lambert * input.normal - lightDirection);

		specular = pow(saturate(dot(reflection, input.viewDirection)), shininess / 4.0f);
	}
	
	float4 objectColor = float4(diffuseColor.xyz, 1.0f);
	return saturate(lightColor * objectColor + specular);
}