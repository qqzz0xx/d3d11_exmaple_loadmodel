cbuffer LightConstantBuffer : register(b1)
{
    float4 diffuseColor;
    float3 lightDirection;
    float padding;
};

struct PixelInput
{
    float4 position : SV_POSITION;
    float3 normal : NORMAL;
};

float4 main(PixelInput input) : SV_TARGET
{
    float3 lightDir;
    float diff;
    float4 color;
	
    color = float4(1.0f, 1.0f, 1.0f, 1.0f);
    lightDir = -lightDirection;
    diff = saturate(dot(input.normal, lightDir));

    color = color * saturate(diffuseColor * diff);

    return (1.0f, 1.0f, 1.0f, 1.0f);
}