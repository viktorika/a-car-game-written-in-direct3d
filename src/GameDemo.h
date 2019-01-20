#pragma once

#include <DirectXmath.h>
#include <memory>
#include "Dx11DemoBase.h"
#include "LookAtCamera .h"
#include "ArcCamera.h"
#include "SkyBox.h"
#include "Brick.h"
#include "Car.h"
#include <vector>

using DirectX::XMMATRIX;
using std::shared_ptr;
using std::vector;

class GameDemo :public Dx11DemoBase{
public:
	GameDemo();
	virtual ~GameDemo();
	bool LoadContent();
	void UnloadContent();
	void Update();
	void Render();
	void Switch();

private:
	vector<shared_ptr<Camera>> camera_;
	int status_;

	SkyBox skybox_;
	Brick brick_;
	Car car_;

	ID3D11SamplerState* colorMapSampler_;

	ID3D11Buffer* viewCB_;
	ID3D11Buffer* worldCB_;
	ID3D11Buffer* projCB_;
	XMMATRIX projMatrix_;

	ID3D11VertexShader* solidColorVS_;
	ID3D11PixelShader* solidColorPS_;

	ID3D11InputLayout* inputLayout_;
	
};
