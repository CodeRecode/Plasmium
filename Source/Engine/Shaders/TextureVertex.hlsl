#pragma pack_matrix( row_major )

cbuffer Matrices
{
	matrix projectionMatrix;
	matrix viewMatrix;
	matrix worldMatrix;
};

cbuffer Camera
{
	float3 cameraPosition;
	float pad;
}

struct VertexInputType
{
	float4 position : POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
};

struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float3 viewDirection : TEXCOORD1;
};

PixelInputType main(VertexInputType input )
{
	PixelInputType output;
	input.position.w = 1.0f;

	float4 worldPosition = mul(input.position, worldMatrix);
	output.position = mul(worldPosition, viewMatrix);
	output.position = mul(output.position, projectionMatrix);

	output.tex = input.tex;

    output.normal = mul(input.normal, (float3x3)worldMatrix);
	output.normal = normalize(output.normal);

	output.viewDirection = normalize(cameraPosition.xyz - worldPosition.xyz);

	return output;
}