#pragma once

#include "Camera.h"

class ArcCamera:public Camera{
public:
	ArcCamera();
	virtual ~ArcCamera();
	void SetRotation(float x, float y);
	void SetTarget(XMFLOAT3& target);
	void ApplyRotation(float yawDelta, float pitchDelta);
	XMMATRIX GetViewMatrix();

private:
	XMFLOAT3 target_;
	float distance_;
	float xRotation_, yRotation_, xlimit, ylimit;
};