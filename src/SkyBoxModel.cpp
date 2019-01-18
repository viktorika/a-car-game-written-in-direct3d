#include "SkyBoxModel.h"

SkyBoxModel::SkyBoxModel() {
	facevertexs_ = {
		array<int,4>{4,5,6,7},
		{2,3,0,1},
		{4,6,0,2},
		{7,5,3,1},
		{6,7,2,3},
		{5,4,1,0}
	};
}

SkyBoxModel::~SkyBoxModel() {}

void SkyBoxModel::setPosition(XMFLOAT3 camerapos) {
	const float gigantic = 10000.0f;
	array < array<int, 3>, 8 > steparr = {
		array<int,3>{-1, -1, -1 },
		{1, -1, -1},
		{ -1,-1,1 },
		{ 1,-1,1 },
		{ -1,1,-1 },
		{ 1,1,-1 },
		{ -1,1,1 },
		{ 1,1,1 }
	};
	for (int i = 0; i < steparr.size(); ++i) {
		float pos[3];
		for (int j = 0; j < steparr[i].size(); ++j)
			pos[j] = steparr[i][j] * gigantic;
		postion_[i] = XMFLOAT3(pos[0], pos[1], pos[2]);
	}
}

XMFLOAT3 SkyBoxModel::getPosition(int index) {
	return postion_[index];
}

array<int, 4> SkyBoxModel::getvertexs(int index) {
	return facevertexs_[index];
}
