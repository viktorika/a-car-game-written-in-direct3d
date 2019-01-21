#pragma once

#include <DirectXMath.h>

using namespace DirectX;

class Camera {
public:
	Camera(){}
	virtual ~Camera(){}
	virtual XMMATRIX GetViewMatrix() = 0;
	virtual void ApplyRotation(float yawDelta, float pitchDelta) = 0;
	virtual void SetPositions(XMFLOAT3 pos, XMFLOAT3 target) = 0;
};
