#include"Dx11DemoBase.h"

Dx11DemoBase::Dx11DemoBase()
	: driverType_(D3D_DRIVER_TYPE_NULL), 
	featureLevel_(D3D_FEATURE_LEVEL_11_0),
	d3dDevice_(nullptr), 
	d3dContext_(nullptr), 
	swapChain_(nullptr), 
	backBufferTarget_(nullptr),
	depthStencilView_(nullptr),
	depthTexture_(nullptr)
{}

Dx11DemoBase::~Dx11DemoBase()
{
	Shutdown();
}

bool Dx11DemoBase::LoadContent()
{
	// Override with demo specifics, if any...
	return true;
}

void Dx11DemoBase::UnloadContent()
{
	// Override with demo specifics, if any...
}

void Dx11DemoBase::Shutdown()
{
	UnloadContent();
	if (backBufferTarget_) backBufferTarget_->Release();
	if (swapChain_) swapChain_->Release();
	if (d3dContext_) d3dContext_->Release();
	if (d3dDevice_) d3dDevice_->Release();
	if (depthStencilView_) depthStencilView_->Release();
	if (depthTexture_) depthTexture_->Release();
	backBufferTarget_ = nullptr;
	swapChain_ = nullptr;
	d3dContext_ = nullptr;
	d3dDevice_ = nullptr;
	depthStencilView_ = nullptr;
	depthTexture_ = nullptr;
}

bool Dx11DemoBase::Initialize(HINSTANCE hInstance, HWND hwnd)
{
	//初始化设备上下文和交换链
	hInstance_ = hInstance;
	hwnd_ = hwnd;
	RECT dimensions;
	GetClientRect(hwnd, &dimensions);
	unsigned int width = dimensions.right - dimensions.left;
	unsigned int height = dimensions.bottom - dimensions.top;
	D3D_DRIVER_TYPE driverTypes[] =
	{
	D3D_DRIVER_TYPE_HARDWARE, D3D_DRIVER_TYPE_WARP,
	D3D_DRIVER_TYPE_REFERENCE, D3D_DRIVER_TYPE_SOFTWARE
	};
	unsigned int totalDriverTypes = ARRAYSIZE(driverTypes);
	D3D_FEATURE_LEVEL featureLevels[] =
	{
	D3D_FEATURE_LEVEL_11_0,
	D3D_FEATURE_LEVEL_10_1,
	D3D_FEATURE_LEVEL_10_0
	};
	unsigned int totalFeatureLevels = ARRAYSIZE(featureLevels);
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Width = width;
	swapChainDesc.BufferDesc.Height = height;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = hwnd;
	swapChainDesc.Windowed = true;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	unsigned int creationFlags = 0;
#ifdef _DEBUG
	creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
	HRESULT result;
	unsigned int driver = 0;
	for (driver = 0; driver < totalDriverTypes; ++driver)
	{
		result = D3D11CreateDeviceAndSwapChain(0, driverTypes[driver], 0, creationFlags,
			featureLevels, totalFeatureLevels,
			D3D11_SDK_VERSION, &swapChainDesc, &swapChain_,
			&d3dDevice_, &featureLevel_, &d3dContext_);
		if (SUCCEEDED(result))
		{
			driverType_ = driverTypes[driver];
			break;
		}
	}
	if (FAILED(result))
		return false;

	//创建目标渲染视图
	ID3D11Texture2D* backBufferTexture;
	result = swapChain_->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferTexture);
	if (FAILED(result))
		return false;
	result = d3dDevice_->CreateRenderTargetView(backBufferTexture, 0, &backBufferTarget_);
	if (backBufferTexture)
		backBufferTexture->Release();
	if (FAILED(result))
		return false;

	//创建并且设置视口
	D3D11_VIEWPORT viewport;
	viewport.Width = static_cast<float>(width);
	viewport.Height = static_cast<float>(height);
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;
	d3dContext_->RSSetViewports(1, &viewport);

	//创建深度缓存目标视图
	D3D11_TEXTURE2D_DESC depthTexDesc;
	ZeroMemory(&depthTexDesc, sizeof(depthTexDesc));
	depthTexDesc.Width = width;
	depthTexDesc.Height = height;
	depthTexDesc.MipLevels = 1;
	depthTexDesc.ArraySize = 1;
	depthTexDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthTexDesc.SampleDesc.Count = 1;
	depthTexDesc.SampleDesc.Quality = 0;
	depthTexDesc.Usage = D3D11_USAGE_DEFAULT;
	depthTexDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthTexDesc.CPUAccessFlags = 0;
	depthTexDesc.MiscFlags = 0;
	result = d3dDevice_->CreateTexture2D(&depthTexDesc, NULL, &depthTexture_);
	if (FAILED(result))
		return false;
	// Create the depth stencil view
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	ZeroMemory(&descDSV, sizeof(descDSV));
	descDSV.Format = depthTexDesc.Format;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;
	result = d3dDevice_->CreateDepthStencilView(depthTexture_, &descDSV, &depthStencilView_);
	if (FAILED(result))
		return false;

	//设置视图
	d3dContext_->OMSetRenderTargets(1, &backBufferTarget_, depthStencilView_);
	return LoadContent();
}

bool CompileD3DShader(LPCTSTR filePath, const char* entry, const char* shaderModel, ID3DBlob** buffer)
{
	DWORD shaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
	shaderFlags |= D3DCOMPILE_DEBUG;
#endif
	ID3DBlob* errorBuffer = 0;
	HRESULT result;
	result = D3DCompileFromFile(filePath, 0, 0, entry, shaderModel,shaderFlags, 0, buffer, &errorBuffer);
	if (FAILED(result))
	{
		if (errorBuffer != 0)
		{
			OutputDebugStringA((char*)errorBuffer->GetBufferPointer());
			errorBuffer->Release();
		}
		return false;
	}
	if (errorBuffer != 0)
		errorBuffer->Release();
	return true;
}