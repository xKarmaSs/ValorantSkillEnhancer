#pragma comment(lib,"d3d11.lib")
#pragma once
#include <iostream>
#include "windows.h"
#include <vector>
#include <dxgi.h>
#include <dxgi1_2.h>
#include <d3d11.h>
#include <stdlib.h>
#include <wrl/client.h>
#include <algorithm>

using Microsoft::WRL::ComPtr;
class ScreenCaptureBase {
public:
	BYTE* ScreenData;
	ScreenCaptureBase() : ScreenData(NULL) {};
	virtual void screenshot() = 0;
	virtual int getWidth() const = 0;

	virtual int getHeight() const = 0;
	virtual std::vector<int> getRGB(int x, int y) const = 0;


};
class ScreenCapture : public ScreenCaptureBase
{
private:
	HWND _hwnd;
	HDC _hdc;
	HDC _hdcTemp;
	HBITMAP _bmTemp;
	int _width;
	int _height;
	long _lastSSTime;
	int _delay;
	int _dimension;

public:
	ScreenCapture(HWND hwnd, int delay, int FOV);
	void screenshot();
	virtual int getWidth() const {
		return _width;
	}

	virtual int getHeight() const {
		return _height;
	}
	~ScreenCapture() {
		ReleaseDC(NULL, _hdc);
		DeleteDC(_hdcTemp);
		DeleteObject(_bmTemp);
	}

	std::vector<int> getRGB(int x, int y) const;

};
const D3D_FEATURE_LEVEL gFeatureLevels[] = {
	D3D_FEATURE_LEVEL_11_0,
	D3D_FEATURE_LEVEL_10_1,
	D3D_FEATURE_LEVEL_10_0,
	D3D_FEATURE_LEVEL_9_1
};
const D3D_DRIVER_TYPE gDriverTypes[] = {
	D3D_DRIVER_TYPE_HARDWARE
};

class ScreenCaptureDirectX : public ScreenCaptureBase
{
private:
	ComPtr<ID3D11Device> lDevice;
	ComPtr<ID3D11DeviceContext> lImmediateContext;
	ComPtr<IDXGIOutputDuplication> lDeskDupl;
	ComPtr<ID3D11Texture2D> lAcquiredDesktopImage;
	D3D11_TEXTURE2D_DESC desc;
	D3D11_MAPPED_SUBRESOURCE pRes;
	int FOV;
public:
	ScreenCaptureDirectX(int FOV) 
		: lDevice(nullptr), 
		  lImmediateContext(nullptr), 
		  lDeskDupl(nullptr), 
		  lAcquiredDesktopImage(nullptr),
		  FOV(FOV),
		  ScreenCaptureBase(){
		setup();
	}
	void setup() {
		lDevice.Reset();
		lImmediateContext.Reset();
		lDeskDupl.Reset();
		lAcquiredDesktopImage.Reset();
		HRESULT hr(E_FAIL);
		UINT gNumDriverTypes = ARRAYSIZE(gDriverTypes);
		UINT gNumFeatureLevels = ARRAYSIZE(gFeatureLevels);
		DXGI_OUTPUT_DESC lOutputDesc;
		DXGI_OUTDUPL_DESC lOutputDuplDesc;
		D3D_FEATURE_LEVEL lFeatureLevel;
		// Create device
		for (UINT DriverTypeIndex = 0; DriverTypeIndex < gNumDriverTypes; ++DriverTypeIndex)
		{
			hr = D3D11CreateDevice(
				nullptr,
				gDriverTypes[DriverTypeIndex],
				nullptr,
				0,
				gFeatureLevels,
				gNumFeatureLevels,
				D3D11_SDK_VERSION,
				&lDevice,
				&lFeatureLevel,
				&lImmediateContext);

			if (SUCCEEDED(hr))
			{
				// Device creation success, no need to loop anymore
				break;
			}

			lDevice.Reset();

			lImmediateContext.Reset();
		}

		if (FAILED(hr) || lDevice == nullptr)
			throw "Failed to Create Device";

		// Get DXGI device
		ComPtr<IDXGIDevice> lDxgiDevice;
		hr = lDevice.As(&lDxgiDevice);

		if (FAILED(hr))
			throw "Failed to get IDXGIDevice";

		// Get DXGI adapter
		ComPtr<IDXGIAdapter> lDxgiAdapter;
		hr = lDxgiDevice->GetParent(
			__uuidof(IDXGIAdapter), &lDxgiAdapter);

		if (FAILED(hr))
			throw "Failed to get DXGI adapter";

		lDxgiDevice.Reset();

		UINT Output = 0;

		// Get output
		ComPtr<IDXGIOutput> lDxgiOutput;
		hr = lDxgiAdapter->EnumOutputs(
			Output,
			&lDxgiOutput);

		if (FAILED(hr))
			throw "Failed to get DXGI adapter output";

		lDxgiAdapter.Reset();

		hr = lDxgiOutput->GetDesc(
			&lOutputDesc);

		if (FAILED(hr))
			throw "Failed to get dxgioutput desc";

		// QI for Output 1
		ComPtr<IDXGIOutput1> lDxgiOutput1;
		hr = lDxgiOutput.As(&lDxgiOutput1);

		if (FAILED(hr))
			throw "Failed to get IDXGIOutput1";

		lDxgiOutput.Reset();

		// Create desktop duplication
		hr = lDxgiOutput1->DuplicateOutput(
			lDevice.Get(), //TODO what im i doing here
			&lDeskDupl);

		if (FAILED(hr))
			throw "Failed to duplicate output";

		lDxgiOutput1.Reset();

		lDeskDupl->GetDesc(&lOutputDuplDesc);
		desc.Width = lOutputDuplDesc.ModeDesc.Width;
		desc.Height = lOutputDuplDesc.ModeDesc.Height;
		desc.Format = lOutputDuplDesc.ModeDesc.Format;
		std::cout << "Starting at " << desc.Width << "x" << desc.Height << std::endl;
		desc.ArraySize = 1;
		desc.BindFlags = 0;
		desc.MiscFlags = 0;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.MipLevels = 1;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE;
		desc.Usage = D3D11_USAGE_STAGING;
	}
	void screenshot() {
		HRESULT hr(E_FAIL);
		ComPtr<IDXGIResource> lDesktopResource;
		ID3D11Texture2D* currTexture;
		DXGI_OUTDUPL_FRAME_INFO lFrameInfo;

		hr = lDeskDupl->AcquireNextFrame(
			999999,
			&lFrameInfo,
			&lDesktopResource);

		if (FAILED(hr)) {
			setup();
			return;
		}

		hr = lDesktopResource.As(&lAcquiredDesktopImage);

		hr = lDevice->CreateTexture2D(&desc, NULL, &currTexture);
		if (FAILED(hr) || currTexture == nullptr) {
			std::cout << "Failed to CreateTexture2D" << std::endl;
			return;
		}


		lImmediateContext->CopyResource(currTexture, lAcquiredDesktopImage.Get());
	
		UINT subresource = D3D11CalcSubresource(0, 0, 0);
		lImmediateContext->Map(currTexture, subresource, D3D11_MAP_READ_WRITE, 0, &pRes);
		BYTE* data = reinterpret_cast<BYTE*>(pRes.pData);
		ScreenData = (BYTE*) pRes.pData;
		
		currTexture->Release();
		hr = lDeskDupl->ReleaseFrame();
	}

	int getWidth() const {
		return desc.Width;
	}

	int getHeight() const {
		return  desc.Height;
	}
	std::vector<int> getRGB(int x, int y) const
	{
		if (!ScreenData)
			return { 0, 0, 0 };

		return { ScreenData[4 * ((y * getWidth()) + x)],
			ScreenData[4 * ((y * getHeight()) + x) + 1],
			ScreenData[4 * ((y * getWidth()) + x) + 2] };
	}
};

