#pragma once

#include <array>
#include "Cube.h"
#include <d3d11.h>

using std::array;
using DirectX::XMFLOAT3;
using DirectX::XMFLOAT2;

class SkyBox :public Cube{
public:
	SkyBox();
	virtual ~SkyBox();

	//初始化资源
	bool Init_Resource(ID3D11Device* d3dDevice_);
	void Render(ID3D11DeviceContext* d3dContext_, ID3D11Buffer* worldCB_, ID3D11Buffer* viewCB_);

private:
	ID3D11Buffer* vertexBuffer_;
	ID3D11Buffer* indexBuffer_;

	array<ID3D11ShaderResourceView*, 6> colorMap_;

};
