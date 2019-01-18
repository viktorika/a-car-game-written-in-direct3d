#pragma once

#include <DirectXmath.h>
#include "Dx11DemoBase.h"
#include "LookAtCamera .h"
#include "SkyBox.h"
#include "Brick.h"

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
	Brick brick_;

	ID3D11SamplerState* colorMapSampler_;

	ID3D11Buffer* viewCB_;
	ID3D11Buffer* worldCB_;
	ID3D11Buffer* projCB_;
	XMMATRIX projMatrix_;

	ID3D11VertexShader* solidColorVS_;
	ID3D11PixelShader* solidColorPS_;

	ID3D11InputLayout* inputLayout_;
};
