#include "Brick.h"
#include "DX11DemoBase.h"
#include  <WICTextureLoader.h>
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
	XMVECTOR vertices[] = {
		XMVectorSet(0.0f, 0.0f, 1.0f, 1.0f),
		XMVectorSet(1.0f, 0.0f, 1.0f, 1.0f),
		XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f),
		XMVectorSet(1.0f, 0.0f, 0.0f, 1.0f),
	};
	VertexPos *map = new VertexPos[1000000 << 2];
	XMFLOAT2 texture[] = {
		XMFLOAT2(0.0f,0.0f),
		XMFLOAT2(1.0f,0.0f),
		XMFLOAT2(0.0f,1.0f),
		XMFLOAT2(1.0f,1.0f)
	};
	for (int x = -500; x < 500; ++x) {
		for (int z = -500; z < 500; ++z) {
			XMMATRIX translationMat = XMMatrixTranslation(x, 0.0f, z);
			//worldMat = XMMatrixTranspose(worldMat);
			for (int i = 0; i < 4; ++i) {
				XMVECTOR now = XMVector4Transform(vertices[i], translationMat);
				map[(x + 500) * 4000 + (z + 500) * 4 + i] = { XMFLOAT3(XMVectorGetX(now),XMVectorGetY(now),XMVectorGetZ(now)),texture[i] };
			}
		}
	}
	D3D11_BUFFER_DESC vertexDesc;
	ZeroMemory(&vertexDesc, sizeof(vertexDesc));
	vertexDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexDesc.ByteWidth = sizeof(VertexPos) * 4000000;
	D3D11_SUBRESOURCE_DATA resourceData;
	ZeroMemory(&resourceData, sizeof(resourceData));
	resourceData.pSysMem = map;
	d3dResult = d3dDevice_->CreateBuffer(&vertexDesc, &resourceData, &vertexBuffer_);
	delete[]map;
	if (FAILED(d3dResult))
		return false;
	//创建索引缓存
	int *indices = new int[1000000 * 6];
	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;
	indices[3] = 1;
	indices[4] = 3;
	indices[5] = 2;
	for (int i = 6; i < 6000000; ++i) 
		indices[i] = indices[i - 6] + 4;
	D3D11_BUFFER_DESC indexDesc;
	ZeroMemory(&indexDesc, sizeof(indexDesc));
	indexDesc.Usage = D3D11_USAGE_DEFAULT;
	indexDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexDesc.ByteWidth = sizeof(int) * 6000000;
	indexDesc.CPUAccessFlags = 0;
	resourceData.pSysMem = indices;
	d3dResult = d3dDevice_->CreateBuffer(&indexDesc, &resourceData, &indexBuffer_);
	if (FAILED(d3dResult))
		return false;
	delete[]indices;
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
	d3dContext_->IASetIndexBuffer(indexBuffer_, DXGI_FORMAT_R32_UINT, 0);
	d3dContext_->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	XMMATRIX rotationMat = XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f);
	XMMATRIX scaling = XMMatrixScaling(1.0f, 1.0f, 1.0f);
	XMMATRIX rsMat = rotationMat * scaling;
	d3dContext_->PSSetShaderResources(0, 1, &colorMap_);
	XMMATRIX translationMat = XMMatrixTranslation(0.0f, 0.0f, 0.0f);
	XMMATRIX worldMat = rsMat * translationMat;
	worldMat = XMMatrixTranspose(worldMat);
	d3dContext_->UpdateSubresource(worldCB_, 0, 0, &worldMat, 0, 0);
	d3dContext_->DrawIndexed(1000000*6, 0, 0);
	/*for (float x = -1000; x < 1000; x += 10) {
		for (float z = -1000; z < 1000; z += 10) {
			XMMATRIX translationMat = XMMatrixTranslation(x, 0.0f, z);
			XMMATRIX worldMat = rsMat * translationMat;
			worldMat = XMMatrixTranspose(worldMat);
			d3dContext_->UpdateSubresource(worldCB_, 0, 0, &worldMat, 0, 0);
			d3dContext_->DrawIndexed(6, 0, 0);
		}
	}*/
}