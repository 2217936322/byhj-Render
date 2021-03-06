#ifndef DXUTHelper_H
#define DXUTHelper_H

#include "DXUT/DXUT.h"
#include "DXUT/SDKmisc.h"
#include "DXUT/DXUTcamera.h"
#include "DXUT/DXUTgui.h"
#include "DXUT/DXUTsettingsdlg.h"

namespace byhj
{
	class DXUTHelper {
	public:
		DXUTHelper();
		~DXUTHelper();

		static std::shared_ptr<DXUTHelper> getInsance();

		void init();
		void render();
		void shutdown();

	private:

#pragma region CALLFUNC


		static LRESULT CALLBACK MsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam,
			bool* pbNoFurtherProcessing, void* pUserContext);

		static void CALLBACK OnFrameMove(double fTime, float fElapsedTime, void* pUserContext);
		static void CALLBACK OnKeyboard(UINT nChar, bool bKeyDown, bool bAltDown, void* pUserContext);
		static void CALLBACK OnMouse(bool bLeftButtonDown, bool bRightButtonDown, bool bMiddleButtonDown,
			bool bSideButton1Down, bool bSideButton2Down, int nMouseWheelDelta,
			int xPos, int yPos, void* pUserContext);

		static bool CALLBACK ModifyDeviceSettings(DXUTDeviceSettings* pDeviceSettings, void* pUserContext);
		static bool CALLBACK OnDeviceRemoved(void* pUserContext);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		static bool CALLBACK IsD3D11DeviceAcceptable(const CD3D11EnumAdapterInfo *AdapterInfo, UINT Output, const CD3D11EnumDeviceInfo *DeviceInfo,
			DXGI_FORMAT BackBufferFormat, bool bWindowed, void* pUserContext);

		static HRESULT CALLBACK OnD3D11CreateDevice(ID3D11Device* pd3dDevice, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc,
			void* pUserContext);

		static HRESULT CALLBACK OnD3D11ResizedSwapChain(ID3D11Device* pd3dDevice, IDXGISwapChain* pSwapChain,
			const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext);

		static void CALLBACK OnD3D11FrameRender(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dImmediateContext,
			double fTime, float fElapsedTime, void* pUserContext);

		static void CALLBACK OnD3D11ReleasingSwapChain(void* pUserContext);
		static void CALLBACK OnD3D11DestroyDevice(void* pUserContext);

		///////////////////////////////////////////////////////////////////////////////////////////////
		LRESULT CALLBACK AppMsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam,
			bool* pbNoFurtherProcessing, void* pUserContext);

		void CALLBACK AppFrameMove(double fTime, float fElapsedTime, void* pUserContext);
		void CALLBACK AppKeyboard(UINT nChar, bool bKeyDown, bool bAltDown, void* pUserContext);
		void CALLBACK AppMouse(bool bLeftButtonDown, bool bRightButtonDown, bool bMiddleButtonDown,
			bool bSideButton1Down, bool bSideButton2Down, int nMouseWheelDelta,
			int xPos, int yPos, void* pUserContext);

		bool CALLBACK AppModifyDeviceSettings(DXUTDeviceSettings* pDeviceSettings, void* pUserContext);
		bool CALLBACK AppDeviceRemoved(void* pUserContext);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		bool CALLBACK AppD3D11DeviceAcceptable(const CD3D11EnumAdapterInfo *AdapterInfo, UINT Output, const CD3D11EnumDeviceInfo *DeviceInfo,
			DXGI_FORMAT BackBufferFormat, bool bWindowed, void* pUserContext);

		HRESULT CALLBACK AppD3D11CreateDevice(ID3D11Device* pd3dDevice, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc,
			void* pUserContext);

		HRESULT CALLBACK AppD3D11ResizedSwapChain(ID3D11Device* pd3dDevice, IDXGISwapChain* pSwapChain,
			const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext);

		void CALLBACK AppD3D11FrameRender(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dImmediateContext,
			double fTime, float fElapsedTime, void* pUserContext);

		void CALLBACK AppD3D11ReleasingSwapChain(void* pUserContext);
		void CALLBACK AppD3D11DestroyDevice(void* pUserContext);
#pragma endregion

		int m_ScreenWidth = 0;
		int m_ScreenHeight = 0;

	};
}

#endif