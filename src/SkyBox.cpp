#include "SkyBox.h"
#include  <WICTextureLoader.h>
#include "DX11DemoBase.h"

using DirectX::CreateWICTextureFromFile;
using DirectX::XMMATRIX;
using DirectX::XMMatrixRotationRollPitchYaw;
using DirectX::XMMatrixTranslation;
using DirectX::XMMatrixPerspectiveFovLH;
using DirectX::XM_PIDIV2;

struct VertexPos {
	XMFLOAT3 pos;
	XMFLOAT2 tex0;
};

SkyBox::SkyBox() 
	:colorMapSampler_(nullptr),
	solidColorVS_(nullptr),
	solidColorPS_(nullptr),
	inputLayout_(nullptr),
	vertexBuffer_(nullptr),
	indexBuffer_(nullptr),
	viewCB_(nullptr),
	projCB_(nullptr)
{
	for (auto &it : colorMap_) 
		it = nullptr;
}

SkyBox::~SkyBox() {
	if (colorMapSampler_) colorMapSampler_->Release();
	if (solidColorVS_) solidColorVS_->Release();
	if (solidColorPS_) solidColorPS_->Release();
	if (inputLayout_) inputLayout_->Release();
	if (vertexBuffer_) vertexBuffer_->Release();
	if (indexBuffer_) indexBuffer_->Release();
	if (viewCB_) viewCB_->Release();
	if (projCB_) projCB_->Release();
	colorMapSampler_ = nullptr;
	solidColorVS_ = nullptr;
	solidColorPS_ = nullptr;
	inputLayout_ = nullptr;
	viewCB_ = nullptr;
	projCB_ = nullptr;
	for (auto &it : colorMap_) {
		it->Release();
		it = nullptr;
	}
}

bool SkyBox::Init_Resource(ID3D11Device* d3dDevice_) {
	HRESULT d3dResult;
	//编译并创建顶点着色器
	ID3DBlob* vsBuffer = 0;
	bool compileResult = CompileD3DShader(L"SkyBox.fx", "VS_Main", "vs_5_0", &vsBuffer);
	if (compileResult == false)
		return false;
	d3dResult = d3dDevice_->CreateVertexShader(vsBuffer->GetBufferPointer(), vsBuffer->GetBufferSize(), 0, &solidColorVS_);
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
	//创建顶点缓冲区
	D3D11_BUFFER_DESC vertexDesc;
	ZeroMemory(&vertexDesc, sizeof(vertexDesc));
	vertexDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexDesc.ByteWidth = sizeof(VertexPos) * 24;
	d3dResult = d3dDevice_->CreateBuffer(&vertexDesc, nullptr, &vertexBuffer_);
	if (FAILED(d3dResult))
		return false;
	//创建索引缓存
	WORD indices[] =
	{
		0,1,2,1,3,2
	};
	D3D11_BUFFER_DESC indexDesc;
	ZeroMemory(&indexDesc, sizeof(indexDesc));
	indexDesc.Usage = D3D11_USAGE_DEFAULT;
	indexDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexDesc.ByteWidth = sizeof(WORD) * 6;
	indexDesc.CPUAccessFlags = 0;
	D3D11_SUBRESOURCE_DATA resourceData;
	ZeroMemory(&resourceData, sizeof(resourceData));
	resourceData.pSysMem = indices;
	d3dResult = d3dDevice_->CreateBuffer(&indexDesc, &resourceData, &indexBuffer_);
	if (FAILED(d3dResult))
		return false;
	//编译并创建像素着色器
	ID3DBlob* psBuffer = 0;
	compileResult = CompileD3DShader(L"SkyBox.fx", "PS_Main", "ps_5_0", &psBuffer);
	if (compileResult == false)
		return false;
	d3dResult = d3dDevice_->CreatePixelShader(psBuffer->GetBufferPointer(), psBuffer->GetBufferSize(), 0, &solidColorPS_);
	psBuffer->Release();
	if (FAILED(d3dResult))
		return false;
	//加载贴图获得资源视图对象
	array<const wchar_t*, 6> path = { L"top.BMP",L"bottom.BMP",L"left.BMP",L"right.BMP",L"front.BMP",L"back.BMP" };
	for (int i = 0; i < path.size(); ++i) {
		d3dResult = CreateWICTextureFromFile(d3dDevice_, path[i], nullptr, &colorMap_[i]);
		if (FAILED(d3dResult))
			return false;
	}
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
	//创建存储变换矩阵的常量缓存
	D3D11_BUFFER_DESC constDesc;
	ZeroMemory(&constDesc, sizeof(constDesc));
	constDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constDesc.ByteWidth = sizeof(XMMATRIX);
	constDesc.Usage = D3D11_USAGE_DEFAULT;
	d3dResult = d3dDevice_->CreateBuffer(&constDesc, 0, &viewCB_);
	if (FAILED(d3dResult))
		return false;
	d3dResult = d3dDevice_->CreateBuffer(&constDesc, 0, &projCB_);
	if (FAILED(d3dResult))
		return false;
	projMatrix_ = XMMatrixPerspectiveFovLH(XM_PIDIV2, 640.0f / 480.0f, 0.01f, 15000.0f);
	projMatrix_ = XMMatrixTranspose(projMatrix_);
	return true;
}

void SkyBox::setPosition(XMFLOAT3 camerapos) {
	model_.setPosition(camerapos);
}

XMFLOAT3 SkyBox::getPosition(int index) {
	return model_.getPosition(index);
}

void SkyBox::Render(ID3D11DeviceContext* d3dContext_,LookAtCamera &camera_, IDXGISwapChain* swapChain_) {
	unsigned int stride = sizeof(VertexPos);
	unsigned int offset = 0;
	d3dContext_->IASetInputLayout(inputLayout_);
	d3dContext_->IASetVertexBuffers(0, 1, &vertexBuffer_, &stride, &offset);
	d3dContext_->IASetIndexBuffer(indexBuffer_, DXGI_FORMAT_R16_UINT, 0);
	d3dContext_->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	d3dContext_->VSSetShader(solidColorVS_, 0, 0);
	d3dContext_->PSSetShader(solidColorPS_, 0, 0);
	//d3dContext_->PSSetShaderResources(0, 1, &colorMap_);
	d3dContext_->PSSetSamplers(0, 1, &colorMapSampler_);
	viewMatrix_ = camera_.GetViewMatrix();
	viewMatrix_ = XMMatrixTranspose(viewMatrix_);
	d3dContext_->UpdateSubresource(viewCB_, 0, 0, &viewMatrix_, 0, 0);
	d3dContext_->UpdateSubresource(projCB_, 0, 0, &projMatrix_, 0, 0);
	d3dContext_->VSSetConstantBuffers(0, 1, &viewCB_);
	d3dContext_->VSSetConstantBuffers(1, 1, &projCB_);
	for (int i = 0; i < colorMap_.size(); ++i) {
		array<int, 4> index = model_.getvertexs(i);
		VertexPos vertex[]={
			{model_.getPosition(index[0]),XMFLOAT2(0,0)},
			{model_.getPosition(index[1]),XMFLOAT2(1,0)},
			{model_.getPosition(index[2]),XMFLOAT2(0,1)},
			{model_.getPosition(index[3]),XMFLOAT2(1,1)}
		};
		d3dContext_->UpdateSubresource(vertexBuffer_, 0, 0, &vertex, 0, 0);
		d3dContext_->PSSetShaderResources(0, 1, &colorMap_[i]);
		d3dContext_->DrawIndexed(6, 0, 0);
	}
	swapChain_->Present(0, 0);
}