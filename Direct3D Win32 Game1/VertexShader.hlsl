
cbuffer MVPConstantBuffer : register(b0)
{
	float4x4 modelMatrix;
	float4x4 viewMatrix;
	float4x4 projectionMatrix;
};

struct VertexInput
{
	float3 pos     : POSITION;
	float3 normal  : NORMAL;
};

struct PixelInput
{
	float4 position : SV_POSITION;
	float3 normal : NORMAL;
};

PixelInput main(VertexInput input)
{
	PixelInput output;
    float4x4 mvp = modelMatrix * viewMatrix * projectionMatrix;
    output.position = mul(float4(input.pos, 1), mvp);

    output.normal = mul(input.normal, (float3x3) modelMatrix);

	return output;
}