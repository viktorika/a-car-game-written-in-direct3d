#pragma once

#include "SkyBoxModel.h"
#include "LookAtCamera .h"

using std::array;
using DirectX::XMFLOAT3;
using DirectX::XMFLOAT2;

class SkyBox {
public:
	SkyBox();
	virtual ~SkyBox();

	//初始化资源
	bool Init_Resource(ID3D11Device* d3dDevice_);
	void setPosition(XMFLOAT3 camerapos);
	XMFLOAT3 getPosition(int index);
	void Render(ID3D11DeviceContext* d3dContext_, LookAtCamera &camera_, IDXGISwapChain* swapChain_);

private:
	ID3D11Buffer* vertexBuffer_;
	ID3D11Buffer* indexBuffer_;

	array<ID3D11ShaderResourceView*, 6> colorMap_;
	array<array<int, 4>, 6> vertexNumber_;
	ID3D11SamplerState* colorMapSampler_;

	ID3D11VertexShader* solidColorVS_;
	ID3D11PixelShader* solidColorPS_;
	
	ID3D11InputLayout* inputLayout_;

	SkyBoxModel model_;

	ID3D11Buffer* viewCB_;
	ID3D11Buffer* projCB_;
	XMMATRIX viewMatrix_;
	XMMATRIX projMatrix_;
};
