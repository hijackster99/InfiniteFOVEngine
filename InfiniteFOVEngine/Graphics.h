#pragma once
#include "IFEWin.h"
#include "IFException.h"
#include "DxgiInfoManager.h"
#include <vector>
#include <d3d11.h>
#include <wrl.h>
#include <DirectXMath.h>
#ifdef _DEBUG
#define IS_DEBUG TRUE
#else
#define IS_DEBUG FALSE
#endif

class __declspec(dllexport) Graphics
{
	friend class __declspec(dllexport) Bindable;
public:
	class Exception : public IFException
	{
		using IFException::IFException;
	};
	class HrException : public Exception
	{
	public:
		HrException(int line, const TCHAR* file, HRESULT hr, std::vector<tstring> infoMsgs = {}) noexcept;
		const char* what() const noexcept override;
		const WCHAR* wwhat() const noexcept override;
		const TCHAR* GetType() const noexcept override;
		HRESULT GetErrorCode() const noexcept;
		tstring GetErrorString() const noexcept;
		tstring GetErrorDescription() const noexcept;
		tstring GetErrorInfo() const noexcept;
	private:
		HRESULT hr;
		tstring info;
	};
	class InfoException : public Exception
	{
	public:
		InfoException(int line, const TCHAR* file, std::vector<tstring> infoMsgs = {}) noexcept;
		const char* what() const noexcept override;
		const WCHAR* wwhat() const noexcept override;
		const TCHAR* GetType() const noexcept override;
		tstring GetErrorInfo() const noexcept;
	private:
		tstring info;
	};
	class DeviceRemovedException : public HrException
	{
		using HrException::HrException;
	public:
		const TCHAR* GetType() const noexcept override;
	private:
		tstring reason;
	};
	Graphics(HWND hwnd);
	Graphics(const Graphics&) = delete;
	Graphics& operator=(const Graphics&) = delete;
	~Graphics() = default;
	void EndFrame();
	void ClearBuffer(float red, float green, float blue) noexcept;
	void DrawIndexed(UINT count) noexcept(!IS_DEBUG);
	void SetProjection(DirectX::FXMMATRIX proj) noexcept;
	DirectX::XMMATRIX GetProjection() const noexcept;
	void DrawTestTriangle(float angle, float x, float z);
private:
	DirectX::XMMATRIX projection;
#ifndef NDEBUG
	DxgiInfoManager infoManager;
#endif
	Microsoft::WRL::ComPtr<ID3D11Device> pDevice;
	Microsoft::WRL::ComPtr<IDXGISwapChain> pChain;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pTarget;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> pDSV;
};