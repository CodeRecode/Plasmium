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
    float3 N = normalize(input.normal);
    float3 V = normalize(input.viewDirection);
    float3 L = normalize(lightDirection);
    float3 H = normalize(V + L);
    float HdotL = dot(H, L);
    float PI = 3.141592;
    float alpha = 10;
    float3 specular = float3(.1, .1, .1);
	
    float3 F = specular + (1 - specular) * pow(1 - HdotL, 5);
    float D = ((alpha + 2) / (2 * PI)) * pow(max(0.0f, dot(H, N)), alpha);
    float G = 1 / (HdotL * HdotL);
	
    float3 ambient = diffuseColor * .15;
    float3 brdf = max(0.0, dot(N, L)) * (diffuseColor + F * D * G);
	
    return float4(ambient + brdf, 1);

}