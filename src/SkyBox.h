#pragma once

#include <array>
#include "Model.h"

using std::array;
using DirectX::XMFLOAT3;
using DirectX::XMFLOAT2;

class SkyBox :public Model{
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
	//array<array<int, 4>, 6> vertexNumber_;

	//8个顶点
	array<XMFLOAT3, 8> postion_;
	//单个面包含4个点
	array<array<int, 4>, 6> facevertexs_;
};
