#include "D3D11Render.h"
#include "windowInfo.h"

namespace byhj
{

	D3D11Render::D3D11Render()
	{

	}
	D3D11Render::~D3D11Render()
	{

	}
	void D3D11Render::v_init()
	{
		init_device();
		init_camera();
		init_object();
	}

	void D3D11Render::v_update()
	{

	}

	void D3D11Render::v_render()
	{
		float bgColor[4] ={ 0.2f, 0.3f, 0.4f, 1.0f };

		m_pD3D11DeviceContext->RSSetState(m_pRasterState);
		m_pD3D11DeviceContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);
		m_pD3D11DeviceContext->ClearRenderTargetView(m_pRenderTargetView, bgColor);
		m_pD3D11DeviceContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	    m_triangle.render(m_pD3D11DeviceContext, m_matrix);

		m_pSwapChain->Present(0, 0);
	}

    void D3D11Render::v_shutdown()
    {
    
    }


    void D3D11Render::init_device()
    {
    
		////////////////////////Create buffer desc////////////////////////////
		DXGI_MODE_DESC bufferDesc;
		ZeroMemory(&bufferDesc, sizeof(DXGI_MODE_DESC));
		bufferDesc.Width                   = getClientWidth();
		bufferDesc.Height                  = getClientHeight();
		bufferDesc.RefreshRate.Numerator   = 60;
		bufferDesc.RefreshRate.Denominator = 1;
		bufferDesc.Format                  = DXGI_FORMAT_R8G8B8A8_UNORM;
		bufferDesc.ScanlineOrdering        = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		bufferDesc.Scaling                 = DXGI_MODE_SCALING_UNSPECIFIED;

		//Create swapChain Desc
		DXGI_SWAP_CHAIN_DESC swapChainDesc;
		ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));
		swapChainDesc.BufferDesc         = bufferDesc;
		swapChainDesc.SampleDesc.Count   = 1;
		swapChainDesc.SampleDesc.Quality = 0;
		swapChainDesc.BufferUsage        = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.BufferCount        = 1;
		swapChainDesc.OutputWindow       = getHwnd();
		swapChainDesc.Windowed           = TRUE;
		swapChainDesc.SwapEffect         = DXGI_SWAP_EFFECT_DISCARD;


		///////////////////////////////////////////////////////////////////////////
		HRESULT hr;
		//Create the double buffer chain
		hr = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE,
			NULL, NULL, NULL, NULL, D3D11_SDK_VERSION,
			&swapChainDesc, &m_pSwapChain, &m_pD3D11Device,
			NULL, &m_pD3D11DeviceContext);
		//DebugHR(hr);

		//Create back buffer, buffer also is a texture
		ID3D11Texture2D *pBackBuffer;
		hr = m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBackBuffer);
		hr = m_pD3D11Device->CreateRenderTargetView(pBackBuffer, NULL, &m_pRenderTargetView);
		pBackBuffer->Release();
		//DebugHR(hr);


		//Describe our Depth/Stencil Buffer
		D3D11_TEXTURE2D_DESC depthStencilDesc;

		depthStencilDesc.Width              = getClientWidth();
		depthStencilDesc.Height             = getClientWidth();
		depthStencilDesc.MipLevels          = 1;
		depthStencilDesc.ArraySize          = 1;
		depthStencilDesc.Format             = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilDesc.SampleDesc.Count   = 1;
		depthStencilDesc.SampleDesc.Quality = 0;
		depthStencilDesc.Usage              = D3D11_USAGE_DEFAULT;
		depthStencilDesc.BindFlags          = D3D11_BIND_DEPTH_STENCIL;
		depthStencilDesc.CPUAccessFlags     = 0;
		depthStencilDesc.MiscFlags          = 0;

		m_pD3D11Device->CreateTexture2D(&depthStencilDesc, NULL, &m_pDepthStencilBuffer);
		m_pD3D11Device->CreateDepthStencilView(m_pDepthStencilBuffer, NULL, &m_pDepthStencilView);
		m_pD3D11DeviceContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);


		// Setup the raster description which will determine how and what polygons will be drawn.
		D3D11_RASTERIZER_DESC rasterDesc;
		rasterDesc.AntialiasedLineEnable = false;
		rasterDesc.CullMode = D3D11_CULL_BACK;
		rasterDesc.DepthBias = 0;
		rasterDesc.DepthBiasClamp = 0.0f;
		rasterDesc.DepthClipEnable = true;
		rasterDesc.FillMode = D3D11_FILL_WIREFRAME;
		rasterDesc.FrontCounterClockwise = false;
		rasterDesc.MultisampleEnable = false;
		rasterDesc.ScissorEnable = false;
		rasterDesc.SlopeScaledDepthBias = 0.0f;

		// Create the rasterizer state from the description we just filled out.
		hr = m_pD3D11Device->CreateRasterizerState(&rasterDesc, &m_pRasterState);
		//DebugHR(hr);

		m_pD3D11DeviceContext->RSSetState(m_pRasterState);
    
    }
    
    void D3D11Render::init_camera()
    {
    	//view port Information
    	D3D11_VIEWPORT vp;
    	ZeroMemory(&vp, sizeof(D3D11_VIEWPORT));
    	vp.TopLeftX = 0;
    	vp.TopLeftY = 0;
		vp.Width    = static_cast<FLOAT>(getClientWidth() );
		vp.Height   = static_cast<FLOAT>(getClientHeight() );
    	m_pD3D11DeviceContext->RSSetViewports(1, &vp);

		//MVP Matrix
		static auto aspect = WindowInfo::getInstance()->getAspect();
		XMVECTOR camPos    = XMVectorSet(0.0f, 0.0f, -3.0f, 0.0f);
		XMVECTOR camTarget = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
		XMVECTOR camUp     = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
		XMMATRIX View      = XMMatrixLookAtLH(camPos, camTarget, camUp);
		XMMATRIX Proj      = XMMatrixPerspectiveFovLH(0.4f*3.14f, aspect, 0.1f, 1000.0f);
		XMMATRIX Model     = XMMatrixIdentity();

		XMStoreFloat4x4(&m_matrix.model, XMMatrixTranspose(Model));
		XMStoreFloat4x4(&m_matrix.view, XMMatrixTranspose(View));
		XMStoreFloat4x4(&m_matrix.proj, XMMatrixTranspose(Proj));
    }
    
    void D3D11Render::init_object()
    {
    	m_Timer.reset();
		m_triangle.init(m_pD3D11Device, m_pD3D11DeviceContext, getHwnd());
    }
    
	

}