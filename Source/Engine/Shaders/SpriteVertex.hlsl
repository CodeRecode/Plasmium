cbuffer Matrices
{
	matrix orthographicMatrix;
	matrix viewMatrix;
	matrix worldMatrix;
};

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
};

PixelInputType main(VertexInputType input)
{
	PixelInputType output;
	input.position.w = 1.0f;
	
	// Ignore View & World
	output.position = mul(input.position, orthographicMatrix);
	output.position.z = 1.0f;
	
	output.tex = input.tex;

	return output;
}