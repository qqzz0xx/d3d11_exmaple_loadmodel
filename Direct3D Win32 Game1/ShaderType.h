#pragma once
#include "SimpleMath.h"
using namespace std;

struct MVPConstantBuffer
{
	DirectX::XMFLOAT4X4 modelMatrix;
	DirectX::XMFLOAT4X4 viewMatrix;
	DirectX::XMFLOAT4X4 projectionMatrix;
};

struct LightConstantBuffer
{
	DirectX::XMFLOAT4 diffuseColor;
	DirectX::XMFLOAT3 lightDirection;
	float padding;
};