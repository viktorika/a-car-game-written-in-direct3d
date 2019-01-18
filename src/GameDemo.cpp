#include "GameDemo.h"

GameDemo::GameDemo()
{}

GameDemo::~GameDemo() 
{}

bool GameDemo::LoadContent()
{
	//设置照相机
	camera_.SetPositions(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 10.0f));
	//初始化skybox
	if (!skybox_.Init_Resource(d3dDevice_)) return false;
	skybox_.setPosition(camera_.getPosition());
	return true;
}

void GameDemo::UnloadContent()
{}

void GameDemo::Update(float dt)
{}

void GameDemo::Render()
{
	if (d3dContext_ == 0)
		return;
	float clearColor[4] = { 0.0f, 0.0f, 0.25f, 1.0f };
	d3dContext_->ClearRenderTargetView(backBufferTarget_, clearColor);
	d3dContext_->ClearDepthStencilView(depthStencilView_, D3D11_CLEAR_DEPTH, 1.0f, 0);
	skybox_.Render(d3dContext_,camera_,swapChain_);
}


