#pragma once

#include <DirectXMath.h>

using DirectX::XMFLOAT3;
using DirectX::XMMATRIX;

class LookAtCamera {
public:
	LookAtCamera();
	LookAtCamera(XMFLOAT3 pos, XMFLOAT3 target);

	void SetPositions(XMFLOAT3 pos, XMFLOAT3 target);
	XMFLOAT3 getPosition();
	XMMATRIX GetViewMatrix();
private:
	XMFLOAT3 position_;
	XMFLOAT3 target_;
	XMFLOAT3 up_;
};
