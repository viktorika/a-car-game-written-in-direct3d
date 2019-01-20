#include "ArcCamera.h"
#include <algorithm>

using std::max;
using std::min;

ArcCamera::ArcCamera()
	:target_(0.0f,1.0f,0.0f),
	distance_(5.0f),
	xRotation_(0.0f),
	yRotation_(0.0f),
	xlimit(XM_PIDIV2),
	ylimit(XM_PI)
{}

ArcCamera::~ArcCamera(){}

void ArcCamera::SetRotation(float x, float y)
{
	xRotation_ = x;
	yRotation_ = y;
	xRotation_ = max(xRotation_, -xlimit);
	xRotation_ = min(xRotation_, xlimit);
	yRotation_ = max(yRotation_, -ylimit);
	yRotation_ = min(yRotation_, ylimit);
}void ArcCamera::SetTarget(XMFLOAT3& target)
{
	target_ = target;
}

void ArcCamera::ApplyRotation(float yawDelta, float pitchDelta)
{
	xRotation_ += yawDelta;
	yRotation_ += pitchDelta;
	xRotation_ = max(xRotation_, -xlimit);
	xRotation_ = min(xRotation_, xlimit);
	yRotation_ = max(yRotation_, -ylimit);
	yRotation_ = min(yRotation_, ylimit);
}

XMMATRIX ArcCamera::GetViewMatrix()
{
	XMVECTOR zoom = XMVectorSet(0.0f, 0.0f, -distance_, 1.0f);
	XMMATRIX rotation = XMMatrixRotationRollPitchYaw(xRotation_, yRotation_, 0.0f);
	zoom = XMVector3Transform(zoom, rotation);
	XMVECTOR lookAt = XMLoadFloat3(&target_);
	XMVECTOR pos = lookAt + zoom;
	XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f);
	up = XMVector3Transform(up, rotation);
	XMMATRIX viewMat = XMMatrixLookAtLH(pos, lookAt, up);
	return viewMat;
}