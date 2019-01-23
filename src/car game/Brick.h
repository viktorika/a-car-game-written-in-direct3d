#pragma once

#include <array>
#include <d3d11.h>

using std::array;

class Brick {
public:
	Brick();
	virtual ~Brick();

	bool Init_Resource(ID3D11Device* d3dDevice_);
	void Render(ID3D11DeviceContext* d3dContext_, ID3D11Buffer* worldCB_, ID3D11Buffer* viewCB_);

private:
	ID3D11Buffer* vertexBuffer_;
	ID3D11Buffer* indexBuffer_;

	ID3D11ShaderResourceView* colorMap_;
};
