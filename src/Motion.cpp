#include "Motion.h"

Motion::Motion() 
	:direction_(0),
	turn_(0),
	distance_(0),
	rotation_(XM_PI/90.0f)
{
	pos_ = XMVectorSet(0.0f, 2.0f, 0.0f, 1.0f);
	facing_ = XMVectorSet(0.0f, 0.0f, 1.0f / 20, 1.0f);
}

Motion::~Motion() {}

void Motion::setDirection(int direction) {
	direction_ = direction;
}

void Motion::setTurn(int turn){
	turn_ = turn;
}

void Motion::Move() {
	XMMATRIX rotation = XMMatrixRotationRollPitchYaw(0.0f, rotation_*turn_*direction_, 0.0f);
	XMMATRIX scaling = XMMatrixScaling(1.0f*direction_, 0.0f, 1.0f*direction_);
	XMVECTOR v = XMVector4Transform(facing_, rotation*scaling);
	facing_= XMVector4Transform(facing_, rotation);
	pos_ += v;
	distance_ = direction_ * 1.0f / 20;
}

XMFLOAT3 Motion::getFocus() {
	XMFLOAT3 focus_;
	XMVECTOR focus = facing_ * 200.0f;
	focus = pos_ + focus;
	XMStoreFloat3(&focus_, focus);
	return focus_;
}

XMFLOAT3 Motion::getPos() {
	XMFLOAT3 pos;
	XMStoreFloat3(&pos, pos_);
	return pos;
}

float Motion::getWholerotation() {
	XMVECTOR v1 = XMVectorSet(0.0f, 0.0f, 1.0f, 1.0f);
	XMVECTOR v2 = XMVector3Normalize(facing_);
	XMVECTOR v3 = XMVector3Cross(v1, v2);
	float wholerotation = XMVectorGetY(XMVector3AngleBetweenNormals(v1, v2));
	return XMVectorGetY(v3) > 0 ? wholerotation : -wholerotation;
}

int Motion::getTurn() {
	return turn_;
}

float Motion::getDistance() {
	return distance_;
}
int Motion::getDirection() {
	return direction_;
}