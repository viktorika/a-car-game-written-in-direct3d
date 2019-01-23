#pragma once

#include <DirectXMath.h>
#include "Camera.h"

using DirectX::XMFLOAT3;
using DirectX::XMMATRIX;

class LookAtCamera :public Camera{
public:
	LookAtCamera();

	void SetPositions(XMFLOAT3 pos, XMFLOAT3 target);
	XMFLOAT3 getPosition();
	XMMATRIX GetViewMatrix();
	void ApplyRotation(float yawDelta, float pitchDelta){}

private:
	XMFLOAT3 position_;
	XMFLOAT3 target_;
	XMFLOAT3 up_;
};
