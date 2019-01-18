#pragma once

#include <DirectXmath.h>
#include "Dx11DemoBase.h"
#include "LookAtCamera .h"
#include "SkyBox.h"

using DirectX::XMMATRIX;

class GameDemo :public Dx11DemoBase{
public:
	GameDemo();
	virtual ~GameDemo();
	bool LoadContent();
	void UnloadContent();
	void Update(float dt);
	void Render();

private:
	LookAtCamera camera_;
	SkyBox skybox_;
};
