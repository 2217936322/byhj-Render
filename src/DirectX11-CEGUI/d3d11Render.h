#ifndef D3D11Render_H
#define D3D11Render_H

#include "d3d/d3dApp.h"
#include "d3d/d3dFWFont.h"


#include <d3d11.h>
#include <wrl.h>

#include "Timer.h"

#include <CEGUI/CEGUI.h>
#include <CEGUI/RendererModules/Direct3D11/Renderer.h>

#pragma  comment(lib, "CEGUIBase-0_d.lib")
#pragma  comment(lib, "CEGUICoreWindowRendererSet_d.lib")
#pragma  comment(lib, "CEGUIDirect3D11Renderer-0_d.lib")


using namespace CEGUI;

namespace byhj
{
	class D3D11Render : public D3DApp
	{
	public:
		D3D11Render();
		~D3D11Render();

		void v_init() ;
		void v_update();
		void v_render();
		void v_shutdown();

	private:

		void init_device();
		void init_camera();
		void init_object();


		void drawfps();
		void drawInfo();
		D3DFWFont m_Font;
		Timer m_Timer;

		float fps = 0.0f;
		int m_videoCardMemory;
		std::wstring m_videoCardInfo;

		IDXGISwapChain         * m_pSwapChain;
		ID3D11Device           * m_pD3D11Device;
		ID3D11DeviceContext    * m_pD3D11DeviceContext;
		ID3D11RenderTargetView * m_pRenderTargetView;
		ID3D11DepthStencilView * m_pDepthStencilView;
		ID3D11Texture2D        * m_pDepthStencilBuffer;
		ID3D11RasterizerState  * m_pRasterState;
		Direct3D11Renderer* renderer ;
	};
}
#endif