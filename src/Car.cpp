#include "Car.h"
#include <WICTextureLoader.h>
#include "VerTexPos.h"

Car::Car() 
	:bodycolorMap_(nullptr),
	tirefrontcolorMap_(nullptr),
	tiresidecolorMap_(nullptr),
	bodyvertexBuffer_(nullptr),
	tirevertexBuffer_(nullptr),
	bodyindexBuffer_(nullptr),
	tirefrontindexBuffer_(nullptr),
	tiresideindexBuffer_(nullptr)
{}

Car::~Car() {
	if (bodycolorMap_) bodycolorMap_->Release();
	if (tirefrontcolorMap_) tirefrontcolorMap_->Release();
	if (tiresidecolorMap_) tiresidecolorMap_->Release();
	if (bodyvertexBuffer_) bodyvertexBuffer_->Release();
	if (tirevertexBuffer_) tirevertexBuffer_->Release();
	if (bodyindexBuffer_) bodyindexBuffer_->Release();
	if (tirefrontindexBuffer_) tirefrontindexBuffer_->Release();
	if (tiresideindexBuffer_) tiresideindexBuffer_->Release();
	bodycolorMap_ = nullptr;
	tirefrontcolorMap_ = nullptr;
	tiresidecolorMap_ = nullptr;
	bodyvertexBuffer_ = nullptr;
	tirevertexBuffer_ = nullptr;
	bodyindexBuffer_ = nullptr;
	tirefrontindexBuffer_ = nullptr;
	tiresideindexBuffer_ = nullptr;
}

bool Car::Init_Resource(ID3D11Device* d3dDevice_) {
	HRESULT d3dResult;
	//创建顶点缓冲区
	D3D11_BUFFER_DESC vertexDesc;
	ZeroMemory(&vertexDesc, sizeof(vertexDesc));
	vertexDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexDesc.ByteWidth = sizeof(VertexPos) * 4;
	d3dResult = d3dDevice_->CreateBuffer(&vertexDesc, nullptr, &bodyvertexBuffer_);
	if (FAILED(d3dResult))
		return false;
	VertexPos tirevertices[1442] =
	{
		{XMFLOAT3(-0.5f,0.0f,0.0f),XMFLOAT2(0.0f,0.0f)},
		{XMFLOAT3(0.5f,0.0f,0.0f),XMFLOAT2(0.0f,0.0f)}
	};
	tirevertices[2] = { XMFLOAT3(-0.5,0.5,0), XMFLOAT2(0,0.5) };
	tirevertices[3] = { XMFLOAT3(-0.5,0.5,0),XMFLOAT2(0,0) };
	tirevertices[4] = { XMFLOAT3(0.5,0.5,0),XMFLOAT2(0,0.5) };
	tirevertices[5] = { XMFLOAT3(0.5,0.5,0),XMFLOAT2(1,0) };
	XMMATRIX rotationMat1 = XMMatrixRotationRollPitchYaw(-XM_PI/90.0f, 0.0f, 0.0f);
	XMMATRIX rotationMat2 = XMMatrixRotationRollPitchYaw(0.0f, 0.0f, XM_PI / 90.0f);
	XMVECTOR vec1 = XMVectorSet(-0.5f, 0.5f, 0.0f, 1.0f);
	XMVECTOR vec2 = XMVectorSet(0.5f, 0.5f, 0.0f, 1.0f);
	XMVECTOR vec3 = XMVectorSet(0.0f, 0.5f, 0.0f, 1.0f);
	vec1 = XMVector4Transform(vec1, rotationMat1);
	vec2 = XMVector4Transform(vec2, rotationMat1);
	vec3 = XMVector4Transform(vec3, rotationMat2);
	XMFLOAT3 next1, next2;
	XMFLOAT2 next3;
	XMStoreFloat3(&next1, vec1);
	XMStoreFloat3(&next2, vec2);
	XMStoreFloat2(&next3, vec3);
	tirevertices[6] = { next1,next3 };
	tirevertices[7] = { next1,XMFLOAT2(0,1) };
	tirevertices[8] = { next2,next3 };
	tirevertices[9] = { next2,XMFLOAT2(1,1) };
	D3D11_SUBRESOURCE_DATA resourceData;
	ZeroMemory(&resourceData, sizeof(resourceData));
	resourceData.pSysMem = tirevertices;
	vertexDesc.ByteWidth = sizeof(VertexPos) * 10;
	d3dResult = d3dDevice_->CreateBuffer(&vertexDesc, &resourceData, &tirevertexBuffer_);
	if (FAILED(d3dResult))
		return false;
	//创建索引缓存
	WORD bodyindices[] =
	{
		2,1,0,2,3,1
	};
	WORD tirefrontindices[] = {
		3,5,7,5,9,7
	};
	WORD tiresideindices[] = {
		0,2,6,1,8,4
	};
	D3D11_BUFFER_DESC indexDesc;
	ZeroMemory(&indexDesc, sizeof(indexDesc));
	indexDesc.Usage = D3D11_USAGE_DEFAULT;
	indexDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexDesc.ByteWidth = sizeof(WORD) * 6;
	indexDesc.CPUAccessFlags = 0;
	resourceData.pSysMem = bodyindices;
	d3dResult = d3dDevice_->CreateBuffer(&indexDesc, &resourceData, &bodyindexBuffer_);
	if (FAILED(d3dResult))
		return false;
	indexDesc.ByteWidth = sizeof(WORD) * 6;
	resourceData.pSysMem = tirefrontindices;
	d3dResult = d3dDevice_->CreateBuffer(&indexDesc, &resourceData, &tirefrontindexBuffer_);
	if (FAILED(d3dResult))
		return false;
	resourceData.pSysMem = tiresideindices;
	d3dResult = d3dDevice_->CreateBuffer(&indexDesc, &resourceData, &tiresideindexBuffer_);
	if (FAILED(d3dResult))
		return false;
	//加载贴图获得资源视图对象
	d3dResult = CreateWICTextureFromFile(d3dDevice_, L"Texture\\body.jpg", nullptr, &bodycolorMap_);
	if (FAILED(d3dResult))
		return false;
	d3dResult = CreateWICTextureFromFile(d3dDevice_, L"Texture\\tirefront.jpg", nullptr, &tirefrontcolorMap_);
	if (FAILED(d3dResult))
		return false;
	d3dResult = CreateWICTextureFromFile(d3dDevice_, L"Texture\\tireside.jpg", nullptr, &tiresidecolorMap_);
	if (FAILED(d3dResult))
		return false;
	return true;
}

void Car::Renderbody(ID3D11DeviceContext* d3dContext_, ID3D11Buffer* worldCB_, ID3D11Buffer* viewCB_) {
	unsigned int stride = sizeof(VertexPos);
	unsigned int offset = 0;
	d3dContext_->IASetVertexBuffers(0, 1, &bodyvertexBuffer_, &stride, &offset);
	d3dContext_->IASetIndexBuffer(bodyindexBuffer_, DXGI_FORMAT_R16_UINT, 0);
	d3dContext_->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	XMMATRIX rotationMat = XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f);
	XMMATRIX translationMat = XMMatrixTranslation(0.0f, 2.0f, 0.0f);
	XMMATRIX scaling = XMMatrixScaling(1.5f, 1.0f, 1.5f);
	XMMATRIX worldMat = rotationMat * scaling * translationMat;
	worldMat = XMMatrixTranspose(worldMat);
	d3dContext_->UpdateSubresource(worldCB_, 0, 0, &worldMat, 0, 0);
	d3dContext_->PSSetShaderResources(0, 1, &bodycolorMap_);
	for (int i = 0; i < 6; ++i) {
		array<int, 4> index = facevertexs_[i];
		VertexPos vertex[] = {
			{postion_[index[0]],XMFLOAT2(0,0)},
			{postion_[index[1]],XMFLOAT2(1,0)},
			{postion_[index[2]],XMFLOAT2(0,1)},
			{postion_[index[3]],XMFLOAT2(1,1)}
		};
		d3dContext_->UpdateSubresource(bodyvertexBuffer_, 0, 0, &vertex, 0, 0);
		d3dContext_->DrawIndexed(6, 0, 0);
	}
}
void Car::Rendertire(ID3D11DeviceContext* d3dContext_, ID3D11Buffer* worldCB_, ID3D11Buffer* viewCB_, ID3D11VertexShader* tiresolidColorVS_, ID3D11Buffer* textureCB_) {
	unsigned int stride = sizeof(VertexPos);
	unsigned int offset = 0;
	d3dContext_->IASetVertexBuffers(0, 1, &tirevertexBuffer_, &stride, &offset);
	d3dContext_->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	const float radian = -XM_PI / 90.0f;
	XMMATRIX translationMat[4] = {
		XMMatrixTranslation(-1.0f,0.5f,-1.0f),
		XMMatrixTranslation(1.0f,0.5f,-1.0f),
		XMMatrixTranslation(-1.0f,0.5f,1.0f),
		XMMatrixTranslation(1.0f,0.5f,1.0f),
	};
	for (int j = 0; j < 4;++j) {
		d3dContext_->IASetIndexBuffer(tirefrontindexBuffer_, DXGI_FORMAT_R16_UINT, 0);
		d3dContext_->PSSetShaderResources(0, 1, &tirefrontcolorMap_);
		for (int i = 0; i < 180; ++i) {
			XMMATRIX rotationMat = XMMatrixRotationRollPitchYaw(radian*i, 0.0f, 0.0f);
			XMMATRIX worldMat = rotationMat * translationMat[j];
			worldMat = XMMatrixTranspose(worldMat);
			d3dContext_->UpdateSubresource(worldCB_, 0, 0, &worldMat, 0, 0);
			d3dContext_->DrawIndexed(6, 0, 0);
		}
	}
	for (int j = 0; j < 4; ++j) {
		d3dContext_->VSSetShader(tiresolidColorVS_, 0, 0);
		d3dContext_->IASetIndexBuffer(tiresideindexBuffer_, DXGI_FORMAT_R16_UINT, 0);
		d3dContext_->PSSetShaderResources(0, 1, &tiresidecolorMap_);
		for (int i = 0; i < 180; ++i) {
			XMMATRIX rotationMat = XMMatrixRotationRollPitchYaw(radian*i, 0.0f, 0.0f);
			XMMATRIX worldMat = rotationMat * translationMat[j];
			worldMat = XMMatrixTranspose(worldMat);
			d3dContext_->UpdateSubresource(worldCB_, 0, 0, &worldMat, 0, 0);
			XMMATRIX textureMat = XMMatrixRotationRollPitchYaw(0.0f, 0.0f, radian*i);
			textureMat *= XMMatrixTranslation(0.5f, 0.5f, 0.0f);
			d3dContext_->UpdateSubresource(textureCB_, 0, 0, &textureMat, 0, 0);
			d3dContext_->DrawIndexed(6, 0, 0);
		}
	}
}

void Car::Render(ID3D11DeviceContext* d3dContext_, ID3D11Buffer* worldCB_, ID3D11Buffer* viewCB_, ID3D11VertexShader* tiresolidColorVS_, ID3D11Buffer* textureCB_) {
	Renderbody(d3dContext_, worldCB_, viewCB_);
	Rendertire(d3dContext_, worldCB_, viewCB_, tiresolidColorVS_, textureCB_);
}