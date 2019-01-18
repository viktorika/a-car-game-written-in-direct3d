#pragma once

#include <array>
#include "Model.h"

using std::array;
using namespace::DirectX;

class Brick :public Model{
public:
	Brick();
	virtual ~Brick();

	bool Init_Resource(ID3D11Device* d3dDevice_);
	void Render(ID3D11DeviceContext* d3dContext_, ID3D11Buffer* worldCB_, ID3D11Buffer* viewCB_);

private:
	ID3D11ShaderResourceView* colorMap_;
};
