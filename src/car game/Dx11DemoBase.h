#pragma once

#include <d3d11.h>
#include <d3dcompiler.h>
#include <dinput.h>

#define KEYDOWN(name, key) ( name[key] & 0x80 )
#define BUTTONDOWN(name, key) ( name.rgbButtons[key] & 0x80 )

class Dx11DemoBase{
public:
	Dx11DemoBase();
	virtual ~Dx11DemoBase();

	bool Initialize(HINSTANCE hInstance, HWND hwnd);
	void Shutdown();

	virtual bool LoadContent();
	virtual void UnloadContent();
	virtual void Update() = 0;
	virtual void Render() = 0;

protected:
	HINSTANCE hInstance_;
	HWND hwnd_;

	D3D_DRIVER_TYPE driverType_;
	ID3D11Device* d3dDevice_;

	D3D_FEATURE_LEVEL featureLevel_;
	ID3D11DeviceContext* d3dContext_;
	IDXGISwapChain* swapChain_;
	ID3D11RenderTargetView* backBufferTarget_;

	ID3D11Texture2D* depthTexture_;
	ID3D11DepthStencilView* depthStencilView_;

	LPDIRECTINPUT8 directInput_;
	LPDIRECTINPUTDEVICE8 keyboardDevice_;
	LPDIRECTINPUTDEVICE8 mouseDevice_;
	char keyboardKeys_[256];
	char prevKeyboardKeys_[256];
	DIMOUSESTATE mouseState_;

	bool CompileD3DShader(LPCTSTR filePath, const char* entry, const char* shaderModel, ID3DBlob** buffer);
};


