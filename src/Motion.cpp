#include "Motion.h"

Motion::Motion() 
	:direction_(0),
	turn_(0),
	distance_(0)
{
	pos_ = XMVectorSet(0.0f, 2.0f, 0.0f, 1.0f);
	rotation_ = XM_PI / 90.0f;
	facing_ = XMVectorSet(0.0f, 0.0f, 1.0f / 4, 1.0f);
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
	XMMATRIX scaling = XMMatrixScaling(1.0f*direction_, 1.0f, 1.0f*direction_);
	XMVECTOR v = XMVector4Transform(facing_, rotation*scaling);
	facing_= XMVector4Transform(facing_, rotation);
	pos_ += v;
	distance_ = (direction_ ? 1.0f / 4 : 0);
}

XMFLOAT3 Motion::getFacing() {
	XMFLOAT3 facing;
	XMVECTOR focus = facing_ * 40.0f;
	focus = pos_ + focus;
	XMStoreFloat3(&facing, focus);
	return facing;
}

XMFLOAT3 Motion::getPos() {
	XMFLOAT3 pos;
	XMStoreFloat3(&pos, pos_);
	return pos;
}