#pragma once

#include <array>
#include <DirectXmath.h>

using std::array;
using namespace DirectX;

class Cube {
public:
	Cube();
	virtual ~Cube();

protected:
	//8个顶点
	array<XMFLOAT3, 8> postion_;
	//每个面对应的顶点
	array<array<int, 4>, 6> facevertexs_;
};
