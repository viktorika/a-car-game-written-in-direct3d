#pragma once

#include <DirectXmath.h>
#include <array>
#include <d3d11.h>

using std::array;
using DirectX::XMFLOAT3;
//定义每个面的下标
enum { TOP, BOTTOM, LEFT, RIGHT, FRONT, BACK };
class SkyBoxModel {
public:
	SkyBoxModel();
	virtual ~SkyBoxModel();
	//通过照相机的坐标设置天空盒的顶点
	void setPosition(XMFLOAT3 camerapos);
	//通过下标访问顶点对应顶点坐标
	XMFLOAT3 getPosition(int index);
	array<int, 4> getvertexs(int index);

private:
	//单个面包含4个点
	array<XMFLOAT3, 8> postion_;
	array<array<int, 4>, 6> facevertexs_;
};
