Texture2D shaderTexture;
SamplerState sampleType;

cbuffer LightBuffer
{
	float4 ambientColor;
	float4 diffuseColor;
	float3 lightDirection;
	float specularPower;
	float4 specularColor;
};

struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float3 viewDirection : TEXCOORD1;
};

float4 main(PixelInputType input) : SV_TARGET
{

	float lambert = saturate(dot(input.normal, lightDirection));
	float4 lightColor = ambientColor;
	float4 specular = float4(0.0f, 0.0f, 0.0f, 0.0f);

	if (lambert > 0.0f) {
		lightColor += diffuseColor * lambert;
		lightColor = saturate(lightColor);

		float3 reflection = normalize(2 * lambert * input.normal - lightDirection);

		specular = pow(saturate(dot(reflection, input.viewDirection)), specularPower / 4.0f);
	}
	
	float4 objectColor = shaderTexture.Sample(sampleType, input.tex);
	return saturate(lightColor * objectColor + specular);
}