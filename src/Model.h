#pragma once

#include <d3d11.h>
#include <DirectXmath.h>
#include "LookAtCamera .h"

using namespace DirectX;

struct VertexPos {
	XMFLOAT3 pos;
	XMFLOAT2 tex0;
};

class Model {
public:
	Model();
	virtual ~Model();
	virtual bool Init_Resource(ID3D11Device* d3dDevice_) = 0;
	virtual void Render(ID3D11DeviceContext* d3dContext_, ID3D11Buffer* worldCB_, ID3D11Buffer* viewCB_) = 0;

protected:
	ID3D11Buffer* vertexBuffer_;
	ID3D11Buffer* indexBuffer_;
};
