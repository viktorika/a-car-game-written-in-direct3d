#include "GameDemo.h"
#include <string>

using std::string;
using namespace DirectX;

const string texture = "\\Texture\\";
enum CAMERA{FIRST,THIRD};
const float radian = XM_PI / 180;

GameDemo::GameDemo()
	:colorMapSampler_(nullptr),
	status_(FIRST)
{
	camera_.emplace_back(new LookAtCamera());
	camera_.emplace_back(new ArcCamera());
}

GameDemo::~GameDemo() 
{}

bool GameDemo::LoadContent()
{
	HRESULT d3dResult;
	//编译并创建顶点着色器
	ID3DBlob* vsBuffer = 0;
	bool compileResult = CompileD3DShader(L"Fx\\solid.fx", "VS_Main", "vs_5_0", &vsBuffer);
	if (compileResult == false)
		return false;
	d3dResult = d3dDevice_->CreateVertexShader(vsBuffer->GetBufferPointer(), vsBuffer->GetBufferSize(), 0, &solidColorVS_);
	if (FAILED(d3dResult))
	{
		if (vsBuffer)
			vsBuffer->Release();
		return false;
	}
	compileResult = CompileD3DShader(L"Fx\\tiresolid.fx", "VS_Main", "vs_5_0", &vsBuffer);
	if (compileResult == false)
		return false;
	d3dResult = d3dDevice_->CreateVertexShader(vsBuffer->GetBufferPointer(), vsBuffer->GetBufferSize(), 0, &tiresolidColorVS_);
	if (FAILED(d3dResult))
	{
		if (vsBuffer)
			vsBuffer->Release();
		return false;
	}
	//创建顶点输入布局
	D3D11_INPUT_ELEMENT_DESC solidColorLayout[] =
	{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	unsigned int totalLayoutElements = ARRAYSIZE(solidColorLayout);
	d3dResult = d3dDevice_->CreateInputLayout(solidColorLayout, totalLayoutElements, vsBuffer->GetBufferPointer(), vsBuffer->GetBufferSize(), &inputLayout_);
	vsBuffer->Release();
	if (FAILED(d3dResult))
		return false;
	//编译并创建像素着色器
	ID3DBlob* psBuffer = 0;
	compileResult = CompileD3DShader(L"Fx\\solid.fx", "PS_Main", "ps_5_0", &psBuffer);
	if (compileResult == false)
		return false;
	d3dResult = d3dDevice_->CreatePixelShader(psBuffer->GetBufferPointer(), psBuffer->GetBufferSize(), 0, &solidColorPS_);
	psBuffer->Release();
	if (FAILED(d3dResult))
		return false;
	//创建采样状态对象
	D3D11_SAMPLER_DESC colorMapDesc;
	ZeroMemory(&colorMapDesc, sizeof(colorMapDesc));
	colorMapDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	colorMapDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	colorMapDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	colorMapDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	colorMapDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	colorMapDesc.MaxLOD = D3D11_FLOAT32_MAX;
	d3dResult = d3dDevice_->CreateSamplerState(&colorMapDesc, &colorMapSampler_);
	if (FAILED(d3dResult))
		return false;
	//创建变换矩阵和常量缓存
	D3D11_BUFFER_DESC constDesc;
	ZeroMemory(&constDesc, sizeof(constDesc));
	constDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constDesc.ByteWidth = sizeof(XMMATRIX);
	constDesc.Usage = D3D11_USAGE_DEFAULT;
	projMatrix_ = XMMatrixPerspectiveFovLH(XM_PIDIV2, 16.0f / 9.0f, 0.01f, 40000.0f);
	projMatrix_ = XMMatrixTranspose(projMatrix_);
	d3dResult = d3dDevice_->CreateBuffer(&constDesc, 0, &worldCB_);
	if (FAILED(d3dResult))
		return false;
	d3dResult = d3dDevice_->CreateBuffer(&constDesc, 0, &viewCB_);
	if (FAILED(d3dResult))
		return false;
	d3dResult = d3dDevice_->CreateBuffer(&constDesc, 0, &projCB_);
	if (FAILED(d3dResult))
		return false;
	d3dResult = d3dDevice_->CreateBuffer(&constDesc, 0, &textureCB_);
	if (FAILED(d3dResult))
		return false;
	//初始化skybox
	if (!skybox_.Init_Resource(d3dDevice_)) return false;
	if (!brick_.Init_Resource(d3dDevice_)) return false;
	if (!car_.Init_Resource(d3dDevice_)) return false;
	return true;
}

void GameDemo::UnloadContent()
{
	if (colorMapSampler_) colorMapSampler_->Release();
	colorMapSampler_ = nullptr;
}

void GameDemo::Update()
{
	keyboardDevice_->Acquire();
	mouseDevice_->Acquire();
	keyboardDevice_->GetDeviceState(sizeof(keyboardKeys_), (LPVOID)&keyboardKeys_);
	mouseDevice_->GetDeviceState(sizeof(mouseState_), (LPVOID)&mouseState_);
	if (KEYDOWN(prevKeyboardKeys_, DIK_ESCAPE) && !KEYDOWN(keyboardKeys_, DIK_ESCAPE))
		PostQuitMessage(0);
	if (KEYDOWN(prevKeyboardKeys_, DIK_V) && !KEYDOWN(keyboardKeys_, DIK_V))
		Switch();
	if (!KEYDOWN(prevKeyboardKeys_, DIK_W) && KEYDOWN(keyboardKeys_, DIK_W))
		motion_.setDirection(1);
	else if (KEYDOWN(prevKeyboardKeys_, DIK_W) && !KEYDOWN(keyboardKeys_, DIK_W))
		motion_.setDirection(0);
	if (!KEYDOWN(prevKeyboardKeys_, DIK_S) && KEYDOWN(keyboardKeys_, DIK_S))
		motion_.setDirection(-1);
	else if (KEYDOWN(prevKeyboardKeys_, DIK_S) && !KEYDOWN(keyboardKeys_, DIK_S))
		motion_.setDirection(0);
	if (!KEYDOWN(prevKeyboardKeys_, DIK_A) && KEYDOWN(keyboardKeys_, DIK_A))
		motion_.setTurn(-1);
	else if (KEYDOWN(prevKeyboardKeys_, DIK_A) && !KEYDOWN(keyboardKeys_, DIK_A))
		motion_.setTurn(0);
	if (!KEYDOWN(prevKeyboardKeys_, DIK_D) && KEYDOWN(keyboardKeys_, DIK_D))
		motion_.setTurn(1);
	else if (KEYDOWN(prevKeyboardKeys_, DIK_D) && !KEYDOWN(keyboardKeys_, DIK_D))
		motion_.setTurn(0);
	motion_.Move();
	memcpy(prevKeyboardKeys_, keyboardKeys_, sizeof(keyboardKeys_));
	camera_[FIRST]->SetPositions(motion_.getPos(),motion_.getFocus());
	camera_[THIRD]->SetPositions(motion_.getPos(), motion_.getPos());
	car_.setCenter(motion_.getPos());
	car_.setWholerotation(motion_.getWholerotation());
	car_.settiretexturerotaion(motion_.getDistance());
	car_.setwheelTurn(motion_.getDirection(), motion_.getTurn());
	if (status_)
		camera_[THIRD]->ApplyRotation(mouseState_.lY*radian, mouseState_.lX*radian);
}

void GameDemo::Render()
{
	if (d3dContext_ == 0)
		return;
	float clearColor[4] = { 0.0f, 0.0f, 0.25f, 1.0f };
	d3dContext_->ClearRenderTargetView(backBufferTarget_, clearColor);
	d3dContext_->ClearDepthStencilView(depthStencilView_, D3D11_CLEAR_DEPTH, 1.0f, 0);
	d3dContext_->IASetInputLayout(inputLayout_);
	d3dContext_->VSSetShader(solidColorVS_, 0, 0);
	d3dContext_->PSSetShader(solidColorPS_, 0, 0);
	d3dContext_->PSSetSamplers(0, 1, &colorMapSampler_);
	XMMATRIX viewMatrix_ = camera_[status_]->GetViewMatrix();
	viewMatrix_ = XMMatrixTranspose(viewMatrix_);
	d3dContext_->UpdateSubresource(viewCB_, 0, 0, &viewMatrix_, 0, 0);
	d3dContext_->UpdateSubresource(projCB_, 0, 0, &projMatrix_, 0, 0);
	d3dContext_->VSSetConstantBuffers(0, 1, &worldCB_);
	d3dContext_->VSSetConstantBuffers(1, 1, &viewCB_);
	d3dContext_->VSSetConstantBuffers(2, 1, &projCB_);
	d3dContext_->VSSetConstantBuffers(3, 1, &textureCB_);
	skybox_.Render(d3dContext_, worldCB_,viewCB_);
	brick_.Render(d3dContext_, worldCB_,viewCB_);
	car_.Render(d3dContext_, worldCB_, viewCB_, tiresolidColorVS_, textureCB_);
	swapChain_->Present(0, 0);
}

void GameDemo::Switch() {
	status_ ^= 1;
}

