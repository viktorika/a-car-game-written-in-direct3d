#include "LookAtCamera .h"
using DirectX::XMMatrixLookAtLH;

LookAtCamera::LookAtCamera() 
	: position_(XMFLOAT3(0.0f, 1.0f, 0.0f)),
	target_(XMFLOAT3(0.0f, 1.0f, 10.0f)), 
	up_(XMFLOAT3(0.0f, 1.0f, 0.0f))
{}

void LookAtCamera::SetPositions(XMFLOAT3 pos, XMFLOAT3 target)
{
	position_ = pos;
	target_ = target;
}
XMMATRIX LookAtCamera::GetViewMatrix()
{
	XMMATRIX viewMat = XMMatrixLookAtLH(XMLoadFloat3(&position_), XMLoadFloat3(&target_), XMLoadFloat3(&up_));
	return viewMat;
}

XMFLOAT3 LookAtCamera::getPosition() {
	return position_;
}