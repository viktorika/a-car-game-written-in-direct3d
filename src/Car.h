#pragma once

#include <array>
#include <d3d11.h>
#include "Cube.h"

using std::array;

class Car :public Cube{
public:
	Car();
	virtual ~Car();
	bool Init_Resource(ID3D11Device* d3dDevice_);
	void Render(ID3D11DeviceContext* d3dContext_, ID3D11Buffer* worldCB_, ID3D11Buffer* viewCB_, ID3D11VertexShader* tiresolidColorVS_, ID3D11Buffer* textureCB_);
	void setCenter(XMFLOAT3 center);
	void setWholerotation(float wholerotation);
	void settiretexturerotaion(float distance);
	void setwheelTurn(int direction, int turn);

private:
	ID3D11ShaderResourceView* bodycolorMap_;
	ID3D11ShaderResourceView* tirefrontcolorMap_;
	ID3D11ShaderResourceView* tiresidecolorMap_;

	ID3D11Buffer* bodyvertexBuffer_;
	ID3D11Buffer* tirevertexBuffer_;
	ID3D11Buffer* bodyindexBuffer_;
	ID3D11Buffer* tirefrontindexBuffer_;
	ID3D11Buffer* tiresideindexBuffer_;

	//世界空间汽车的中心坐标
	XMFLOAT3 center_;
	//汽车整体相对世界空间的旋转角和轮胎相对汽车整体的旋转角
	float wholerotation_, tirerotation_;
	//轮胎纹理的旋转角
	float tiretexturerotation_;
	//前轮和后轮的转向
	array<int, 2> wheelturn_;

	void Renderbody(ID3D11DeviceContext* d3dContext_, ID3D11Buffer* worldCB_, ID3D11Buffer* viewCB_);
	void Rendertire(ID3D11DeviceContext* d3dContext_, ID3D11Buffer* worldCB_, ID3D11Buffer* viewCB_, ID3D11VertexShader* tiresolidColorVS_, ID3D11Buffer* textureCB_);
};