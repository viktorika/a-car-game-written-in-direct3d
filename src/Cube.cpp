#include "Cube.h"

Cube::Cube() {
	facevertexs_ = {
		array<int,4>{4,5,6,7},
		{2,3,0,1},
		{4,6,0,2},
		{7,5,3,1},
		{6,7,2,3},
		{5,4,1,0}
	};
	postion_ = {
		XMFLOAT3(-1,-1,-1),
		XMFLOAT3(1,-1,-1),
		XMFLOAT3(-1,-1,1),
		XMFLOAT3(1,-1,1),
		XMFLOAT3(-1,1,-1),
		XMFLOAT3(1,1,-1),
		XMFLOAT3(-1,1,1),
		XMFLOAT3(1,1,1)
	};
}

Cube::~Cube() {}