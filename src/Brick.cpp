#include "Brick.h"
#include  <WICTextureLoader.h>
#include "DX11DemoBase.h"
#include "VerTexPos.h"

using namespace DirectX;

Brick::Brick()
	:colorMap_(nullptr),
	vertexBuffer_(nullptr),
	indexBuffer_(nullptr)
{}

Brick::~Brick(){
	if (colorMap_) colorMap_->Release();
	if (vertexBuffer_) vertexBuffer_->Release();
	if (indexBuffer_) indexBuffer_->Release();
	colorMap_ = nullptr;
	vertexBuffer_ = nullptr;
	indexBuffer_ = nullptr;
}

bool Brick::Init_Resource(ID3D11Device* d3dDevice_) {
	HRESULT d3dResult;
	//创建顶点缓冲区
	VertexPos vertices[] =
	{
		{XMFLOAT3(0.0f,0.0f,10.0f),XMFLOAT2(0.0f,0.0f)},
		{XMFLOAT3(10.0f,0.0f,10.0f),XMFLOAT2(1.0f,0.0f)},
		{XMFLOAT3(0.0f,0.0f,0.0f),XMFLOAT2(0.0f,1.0f)},
		{XMFLOAT3(10.0f,0.0f,0.0f),XMFLOAT2(1.0f,1.0f)}
	};
	D3D11_BUFFER_DESC vertexDesc;
	ZeroMemory(&vertexDesc, sizeof(vertexDesc));
	vertexDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexDesc.ByteWidth = sizeof(VertexPos) * 4;
	D3D11_SUBRESOURCE_DATA resourceData;
	ZeroMemory(&resourceData, sizeof(resourceData));
	resourceData.pSysMem = vertices;
	d3dResult = d3dDevice_->CreateBuffer(&vertexDesc, &resourceData, &vertexBuffer_);
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
	resourceData.pSysMem = indices;
	d3dResult = d3dDevice_->CreateBuffer(&indexDesc, &resourceData, &indexBuffer_);
	if (FAILED(d3dResult))
		return false;
	//加载贴图获得资源视图对象
	d3dResult = CreateWICTextureFromFile(d3dDevice_,L"Texture\\brick.jpeg", nullptr, &colorMap_);
	if (FAILED(d3dResult))
		return false;
	return true;
}

void Brick::Render(ID3D11DeviceContext* d3dContext_, ID3D11Buffer* worldCB_,ID3D11Buffer* viewCB_) {
	unsigned int stride = sizeof(VertexPos);
	unsigned int offset = 0;
	d3dContext_->IASetVertexBuffers(0, 1, &vertexBuffer_, &stride, &offset);
	d3dContext_->IASetIndexBuffer(indexBuffer_, DXGI_FORMAT_R16_UINT, 0);
	d3dContext_->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	XMMATRIX rotationMat = XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f);
	XMMATRIX scaling = XMMatrixScaling(1.0f, 1.0f, 1.0f);
	XMMATRIX rsMat = rotationMat * scaling;
	d3dContext_->PSSetShaderResources(0, 1, &colorMap_);
	for (float x = -1000; x < 100; x += 10) {
		for (float z = -1000; z < 100; z += 10) {
			XMMATRIX translationMat = XMMatrixTranslation(x, 0.0f, z);
			XMMATRIX worldMat = rsMat * translationMat;
			worldMat = XMMatrixTranspose(worldMat);
			d3dContext_->UpdateSubresource(worldCB_, 0, 0, &worldMat, 0, 0);
			d3dContext_->DrawIndexed(6, 0, 0);
		}
	}
}