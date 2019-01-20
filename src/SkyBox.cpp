#include "SkyBox.h"
#include  <WICTextureLoader.h>
#include "DX11DemoBase.h"
#include "VertexPos.h"

using namespace DirectX;

SkyBox::SkyBox() 
	:vertexBuffer_(nullptr),
	indexBuffer_(nullptr)
{
	for (auto &it : colorMap_) 
		it = nullptr;
}

SkyBox::~SkyBox() {
	if (vertexBuffer_) vertexBuffer_->Release();
	if (indexBuffer_) indexBuffer_->Release();
	vertexBuffer_ = nullptr;
	indexBuffer_ = nullptr;
	for (auto &it : colorMap_) {
		if (it) it->Release();
		it = nullptr;
	}
}

bool SkyBox::Init_Resource(ID3D11Device* d3dDevice_) {
	HRESULT d3dResult;
	//创建顶点缓冲区
	D3D11_BUFFER_DESC vertexDesc;
	ZeroMemory(&vertexDesc, sizeof(vertexDesc));
	vertexDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexDesc.ByteWidth = sizeof(VertexPos) * 4;
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
	//加载贴图获得资源视图对象
	array<const wchar_t*, 6> path = { L"top.BMP",L"bottom.BMP",L"left.BMP",L"right.BMP",L"front.BMP",L"back.BMP" };
	wchar_t texture[15] = L"Texture\\";
	for (int i = 0; i < path.size(); ++i) {
		wchar_t target[30];
		wcscpy_s(target, texture);
		wcscat_s(target, path[i]);
		d3dResult = CreateWICTextureFromFile(d3dDevice_, target, nullptr, &colorMap_[i]);
		if (FAILED(d3dResult))
			return false;
	}
	return true;
}

void SkyBox::Render(ID3D11DeviceContext* d3dContext_, ID3D11Buffer* worldCB_, ID3D11Buffer* viewCB_) {
	unsigned int stride = sizeof(VertexPos);
	unsigned int offset = 0;
	d3dContext_->IASetVertexBuffers(0, 1, &vertexBuffer_, &stride, &offset);
	d3dContext_->IASetIndexBuffer(indexBuffer_, DXGI_FORMAT_R16_UINT, 0);
	d3dContext_->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	XMMATRIX rotationMat = XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f);
	XMMATRIX translationMat = XMMatrixTranslation(0.0f, 0.0f, 0.0f);
	XMMATRIX scaling = XMMatrixScaling(10000.0f, 10000.0f, 10000.0f);
	XMMATRIX worldMat = rotationMat * scaling * translationMat;
	worldMat = XMMatrixTranspose(worldMat);
	d3dContext_->UpdateSubresource(worldCB_, 0, 0, &worldMat, 0, 0);
	d3dContext_->VSSetConstantBuffers(0, 1, &worldCB_);
	d3dContext_->VSSetConstantBuffers(1, 1, &viewCB_);
	for (int i = 0; i < colorMap_.size(); ++i) {
		array<int, 4> index = facevertexs_[i];
		VertexPos vertex[]={
			{postion_[index[0]],XMFLOAT2(0,0)},
			{postion_[index[1]],XMFLOAT2(1,0)},
			{postion_[index[2]],XMFLOAT2(0,1)},
			{postion_[index[3]],XMFLOAT2(1,1)}
		};
		d3dContext_->UpdateSubresource(vertexBuffer_, 0, 0, &vertex, 0, 0);
		d3dContext_->PSSetShaderResources(0, 1, &colorMap_[i]);
		d3dContext_->DrawIndexed(6, 0, 0);
	}
}