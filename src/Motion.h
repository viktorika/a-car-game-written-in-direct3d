#pragma once

#include <DirectXMath.h>
#include <array>

using namespace DirectX;
using std::array;

class Motion {
public:
	Motion();
	virtual ~Motion();
	void setDirection(int direction_);
	void setTurn(int turn);
	void Move();
	XMFLOAT3 getPos();
	XMFLOAT3 getFocus();
	float getWholerotation();
	int getTurn();
	int getDirection();
	float getDistance();

private:
	XMVECTOR pos_;
	float rotation_, distance_;
	XMVECTOR facing_;
	int direction_, turn_;
};
