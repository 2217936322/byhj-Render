#include "RenderSystem.h"

namespace byhj
{

RenderSystem::RenderSystem() 
{
	m_AppName = L"DirectX11-init D3D";
}

RenderSystem::~RenderSystem()
{

}

void RenderSystem::v_init()
{
	init_device();
	init_camera();
	init_object();
	init_fbo();
}

void RenderSystem::v_update()
{

}

void RenderSystem::v_render()
{
	BeginScene(); 
	static float rot = 0.0f;
	rot +=  m_Timer.GetDeltaTime();
	UpdateScene();
	m_pD3D11DeviceContext->RSSetState(m_pRasterState.Get());
	m_pD3D11DeviceContext->OMSetDepthStencilState(m_pDepthStencilState.Get(), 1);
	//////////////////////Render Refraction To Texture/////////////////////////////
	float blackColor[] ={ 0.0f, 0.0f, 0.0f, 0.0f};
	// Setup a clipping plane based on the height of the water to clip everything above it.
	XMFLOAT4 clipPlane = XMFLOAT4(0.0f, -1.0f, 0.0f, 2.75f + 0.1f);
	m_pD3D11DeviceContext->OMSetRenderTargets(1, m_pRefractRTV.GetAddressOf(), m_pDepthStencilView.Get());
	m_pD3D11DeviceContext->ClearDepthStencilView(m_pDepthStencilView.Get(), D3D11_CLEAR_DEPTH|D3D11_CLEAR_STENCIL, 1.0f, 0);
	m_pD3D11DeviceContext->ClearRenderTargetView(m_pRenderTargetView.Get(), blackColor);

	XMMATRIX World = XMMatrixTranslation(0.0f, 2.0f, 0.0f);
	XMStoreFloat4x4(&m_Matrix.model, XMMatrixTranspose(World));
    m_RefracShader.use(m_pD3D11DeviceContext.Get(), m_Matrix);
	m_pD3D11DeviceContext->PSSetShaderResources(0, 1, m_TextureMgr.GetTexture(L"marble01.dds").GetAddressOf());
	bathModel.render(m_pD3D11DeviceContext.Get());


	//////////////////////Render Reflection To Texture////////////////////////////////
	m_pD3D11DeviceContext->RSSetState(m_pRasterState.Get());
	m_pD3D11DeviceContext->OMSetDepthStencilState(m_pDepthStencilState.Get(), 1);
	m_pD3D11DeviceContext->OMSetRenderTargets(1, m_pReflectRTV.GetAddressOf(), m_pDepthStencilView.Get());
	m_pD3D11DeviceContext->ClearDepthStencilView(m_pDepthStencilView.Get(), D3D11_CLEAR_DEPTH|D3D11_CLEAR_STENCIL, 1.0f, 0);
	m_pD3D11DeviceContext->ClearRenderTargetView(m_pRenderTargetView.Get(), blackColor);

	// Translate to where the wall model will be rendered.
	World = XMMatrixTranslation(0.0f, 6.0f, 8.0f);
	XMStoreFloat4x4(&m_Matrix.model, XMMatrixTranspose(World));

	XMFLOAT4X4 View  = m_Camera.GetViewMatrix();

	XMFLOAT3 camPos = m_Camera.GetPos();
	XMFLOAT3 camTarget = m_Camera.GetTarget();
	XMFLOAT3 camUp = m_Camera.GetUp();
	camPos.y =  -camPos.y;
	XMVECTOR pos    =  XMLoadFloat3(&camPos);
	XMVECTOR target =  XMLoadFloat3(&camTarget);
	XMVECTOR up     =  XMLoadFloat3(&camUp);
	XMMATRIX reflectMat = XMMatrixLookAtLH(pos, target, up);
	XMFLOAT4X4 Reflect;
	XMStoreFloat4x4(&Reflect, XMMatrixTranspose(reflectMat));
	m_Matrix.view = Reflect;

	m_LightShader.use(m_pD3D11DeviceContext.Get(), m_Matrix);
	m_pD3D11DeviceContext->PSSetShaderResources(0, 1, m_TextureMgr.GetTexture(L"wall01.dds").GetAddressOf());
	wallModel.render(m_pD3D11DeviceContext.Get());

	///////////////////////////////Render Scene///////////////////////////////////////

	BeginScene();

	m_Matrix.view = m_Camera.GetViewMatrix();
		
	/////////////////////Render the scene Model//////////////////////////////
	XMMATRIX Model = XMMatrixTranslation(0.0f, 1.0f, 0.0f);
	XMStoreFloat4x4(&m_Matrix.model, XMMatrixTranspose(Model));
	m_LightShader.use(m_pD3D11DeviceContext.Get(), m_Matrix);
	m_pD3D11DeviceContext->PSSetShaderResources(0, 1, m_TextureMgr.GetTexture(L"ground01.dds").GetAddressOf());
	groundModel.render(m_pD3D11DeviceContext.Get());
	
	Model = XMMatrixTranslation(0.0f, 6.0f, 8.0f);
	XMStoreFloat4x4(&m_Matrix.model, XMMatrixTranspose(Model));
	m_LightShader.use(m_pD3D11DeviceContext.Get(), m_Matrix);
	m_pD3D11DeviceContext->PSSetShaderResources(0, 1, m_TextureMgr.GetTexture(L"wall01.dds").GetAddressOf());
	wallModel.render(m_pD3D11DeviceContext.Get());

	Model = XMMatrixTranslation(0.0f, 2.0f, 0.0f);
	XMStoreFloat4x4(&m_Matrix.model, XMMatrixTranspose(Model));
	m_LightShader.use(m_pD3D11DeviceContext.Get(), m_Matrix);
	m_pD3D11DeviceContext->PSSetShaderResources(0, 1, m_TextureMgr.GetTexture(L"marble01.dds").GetAddressOf());
	bathModel.render(m_pD3D11DeviceContext.Get());

	/////////////////////////////////////////////////////////////////////////////


	Model = XMMatrixTranslation(0.0f, 2.75f, 0.0f);
	XMStoreFloat4x4(&m_Matrix.model, XMMatrixTranspose(Model));
	m_WaterShader.use(m_pD3D11DeviceContext.Get(), m_Matrix, Reflect);
	m_pD3D11DeviceContext->PSSetShaderResources(0, 1, m_TextureMgr.GetTexture(L"water01.dds").GetAddressOf());
	m_pD3D11DeviceContext->PSSetShaderResources(1, 1, m_pReflectSRV.GetAddressOf());
	m_pD3D11DeviceContext->PSSetShaderResources(2, 1, m_pRefractSRV.GetAddressOf());
	waterModel.render(m_pD3D11DeviceContext.Get());

	DrawInfo();

	EndScene();

}

void RenderSystem::v_shutdown()
{

}


void RenderSystem::UpdateScene()
{
	m_Camera.update();
}
void  RenderSystem::v_OnMouseDown(WPARAM btnState, int x, int y)
{
	m_Camera.OnMouseDown(btnState, x, y, GetHwnd());
}

void  RenderSystem::v_OnMouseMove(WPARAM btnState, int x, int y)
{
	m_Camera.OnMouseMove(btnState, x, y);
}

void  RenderSystem::v_OnMouseUp(WPARAM btnState, int x, int y)
{
	m_Camera.OnMouseUp(btnState, x, y);
}
void  RenderSystem::v_OnMouseWheel(WPARAM btnState, int x, int y)
{
	m_Camera.OnMouseWheel(btnState, x, y, getAspect());
}

void RenderSystem::init_device()
{

	////////////////////////Create buffer desc////////////////////////////
	DXGI_MODE_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(DXGI_MODE_DESC));
	bufferDesc.Width                   = m_ScreenWidth;
	bufferDesc.Height                  = m_ScreenHeight;
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
	swapChainDesc.OutputWindow       = GetHwnd();
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
	
	///////////////////// Set up the description of the depth buffer.////////////////////////
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));
	depthBufferDesc.Width              = m_ScreenWidth;
	depthBufferDesc.Height             = m_ScreenHeight;
	depthBufferDesc.MipLevels          = 1;
	depthBufferDesc.ArraySize          = 1;
	depthBufferDesc.Format             = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count   = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage              = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags          = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags     = 0;
	depthBufferDesc.MiscFlags          = 0;
	// Create the texture for the depth buffer using the filled out description.
	hr = m_pD3D11Device->CreateTexture2D(&depthBufferDesc, NULL, &m_pDepthStencilBuffer);
	//DebugHR(hr);

	//////////////////////////// initialize the description of the stencil state.///////////////////////////////////////////////
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	depthStencilDesc.DepthEnable      = true;
	depthStencilDesc.DepthWriteMask   = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc        = D3D11_COMPARISON_LESS_EQUAL;
	depthStencilDesc.StencilEnable    = true;
	depthStencilDesc.StencilReadMask  = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	// Stencil operations if pixel is front-facing.
	depthStencilDesc.FrontFace.StencilFailOp      = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp      = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc        = D3D11_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back-facing.
	depthStencilDesc.BackFace.StencilFailOp      = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp      = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc        = D3D11_COMPARISON_ALWAYS;

	// Create the depth stencil state.
	hr = m_pD3D11Device->CreateDepthStencilState(&depthStencilDesc, &m_pDepthStencilState);
	//DebugHR(hr);

	// initialize the depth stencil view.
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));
	depthStencilViewDesc.Format             =  DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension      = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	hr = m_pD3D11Device->CreateDepthStencilView(m_pDepthStencilBuffer.Get(), &depthStencilViewDesc, &m_pDepthStencilView);
	//DebugHR(hr);


	///////////////////////////////////////////////////////////////////////////////////////////
	D3D11_RASTERIZER_DESC rasterDesc;
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_BACK;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;
	// Create the rasterizer state from the description we just filled out.
	hr = m_pD3D11Device->CreateRasterizerState(&rasterDesc, &m_pRasterState);
	DebugHR(hr);

	///////////////////////////////////////////////////////////////////////////////

	unsigned int numModes, i, numerator, denominator, stringLength;
	IDXGIFactory* factory;
	IDXGIAdapter* adapter;
	IDXGISurface *surface;
	DXGI_ADAPTER_DESC adapterDesc;

	// Create a DirectX graphics interface factory.
	CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
	// Use the factory to create an adapter for the primary graphics interface (video card).
	factory->EnumAdapters(0, &adapter);
	adapter->GetDesc(&adapterDesc);
	m_videoCardMemory = (int)(adapterDesc.DedicatedVideoMemory / 1024 / 1024);

	// Convert the name of the video card to a character array and store it.
	m_videoCardInfo = std::wstring(L"Video Card  :") + adapterDesc.Description;

}

void RenderSystem::BeginScene()
{
	//Render 
	float bgColor[4] ={ 0.2f, 0.3f, 0.4f, 1.0f };

	m_pD3D11DeviceContext->RSSetState(m_pRasterState.Get());
	m_pD3D11DeviceContext->OMSetDepthStencilState(m_pDepthStencilState.Get(), 1);
	m_pD3D11DeviceContext->OMSetRenderTargets(1, m_pRenderTargetView.GetAddressOf(), m_pDepthStencilView.Get());
	m_pD3D11DeviceContext->ClearRenderTargetView(m_pRenderTargetView.Get(), bgColor);
	m_pD3D11DeviceContext->ClearDepthStencilView(m_pDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

}

void RenderSystem::EndScene()
{
	m_pSwapChain->Present(0, 0);
}

void RenderSystem::init_camera()
{
	//Viewport Infomation
	D3D11_VIEWPORT vp;
	ZeroMemory(&vp, sizeof(D3D11_VIEWPORT));
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.Width    = static_cast<FLOAT>(m_ScreenWidth);
	vp.Height   = static_cast<FLOAT>(m_ScreenHeight);
	m_pD3D11DeviceContext->RSSetViewports(1, &vp);

	//MVP Matrix
	XMVECTOR camPos    = XMVectorSet(0.0f, 0.0f, -5.0f, 0.0f);
	XMVECTOR camTarget = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	XMVECTOR camUp     = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	XMMATRIX View      = XMMatrixLookAtLH(camPos, camTarget, camUp);
	XMMATRIX Proj      = XMMatrixPerspectiveFovLH(0.4f*3.14f, getAspect(), 1.0f, 1000.0f);
	XMMATRIX Model     = XMMatrixRotationY(60.0f);


	XMStoreFloat4x4(&m_Matrix.model, XMMatrixTranspose(Model) );
	XMStoreFloat4x4(&m_Matrix.view,  XMMatrixTranspose(View) );
	XMStoreFloat4x4(&m_Matrix.proj,  XMMatrixTranspose(Proj) );

}

void RenderSystem::init_object()
{
	bathModel.init(m_pD3D11Device.Get(), m_pD3D11DeviceContext.Get());
	groundModel.init( m_pD3D11Device.Get(), m_pD3D11DeviceContext.Get());
	wallModel.init( m_pD3D11Device.Get(), m_pD3D11DeviceContext.Get());
	waterModel.init(m_pD3D11Device.Get(), m_pD3D11DeviceContext.Get());

	
	m_LightShader.init(m_pD3D11Device.Get(), m_pD3D11DeviceContext.Get(), GetHwnd());

	m_TextureMgr.LoadTexture(m_pD3D11Device.Get(), L"marble01.dds");
	m_TextureMgr.LoadTexture(m_pD3D11Device.Get(), L"ground01.dds");
	m_TextureMgr.LoadTexture(m_pD3D11Device.Get(), L"wall01.dds");
	m_TextureMgr.LoadTexture(m_pD3D11Device.Get(), L"water01.dds");
	m_WaterShader.init(m_pD3D11Device.Get(), m_pD3D11DeviceContext.Get(), GetHwnd());
	m_RefracShader.init(m_pD3D11Device.Get(), m_pD3D11DeviceContext.Get(), GetHwnd());

	m_Timer.Reset();
	m_Font.init(m_pD3D11Device.Get());
	m_Camera.SetRadius(15.0f);
}

void RenderSystem::init_fbo()
{

	D3D11_TEXTURE2D_DESC textureDesc;
	HRESULT result;
	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;

	ZeroMemory(&textureDesc, sizeof(textureDesc));
	textureDesc.Width = m_ScreenWidth;
	textureDesc.Height = m_ScreenHeight;
	textureDesc.MipLevels  = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	//Create the render target texture
	result = m_pD3D11Device->CreateTexture2D(&textureDesc, NULL, &m_pReflectRTT);
	result = m_pD3D11Device->CreateTexture2D(&textureDesc, NULL, &m_pRefractRTT);

	//Setup the description of the render target view
	renderTargetViewDesc.Format = textureDesc.Format;
	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	renderTargetViewDesc.Texture2D.MipSlice = 0;

	result = m_pD3D11Device->CreateRenderTargetView(m_pReflectRTT.Get(), &renderTargetViewDesc, &m_pReflectRTV);
	result = m_pD3D11Device->CreateRenderTargetView(m_pRefractRTT.Get(), &renderTargetViewDesc, &m_pRefractRTV);

	shaderResourceViewDesc.Format = textureDesc.Format;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;
	result = m_pD3D11Device->CreateShaderResourceView(m_pReflectRTT.Get(), &shaderResourceViewDesc, &m_pReflectSRV);
	result = m_pD3D11Device->CreateShaderResourceView(m_pRefractRTT.Get(), &shaderResourceViewDesc, &m_pRefractSRV);
}

void RenderSystem::TurnZBufferOn()
{
	m_pD3D11DeviceContext->OMSetDepthStencilState(m_pDepthStencilState.Get(), 1);
	return;
}


void RenderSystem::TurnZBufferOff()
{
	m_pD3D11DeviceContext->OMSetDepthStencilState(m_pDepthDisabledStencilState.Get(), 1);
	return;
}
void RenderSystem::DrawFps()
{
	static bool flag = true;
	if (flag)
	{
		m_Timer.Start();
		flag = false;
	}

	m_Timer.Count();
	static int frameCnt = 0;
	static float timeElapsed = 0.0f;
	frameCnt++;
	if (m_Timer.GetTotalTime() - timeElapsed >= 1.0f)
	{
		fps = frameCnt;
		frameCnt = 0;
		timeElapsed += 1.0f;
	}

	m_Font.drawFps(m_pD3D11DeviceContext.Get(), (UINT)fps);
}

void RenderSystem::DrawInfo()
{
	WCHAR WinInfo[255];
	swprintf(WinInfo, L"Window Size: %d x %d", m_ScreenWidth, m_ScreenHeight);
	DrawFps();
	m_Font.drawText(m_pD3D11DeviceContext.Get(), WinInfo, 22.0f, 10.0f, 40.0f);
	m_Font.drawText(m_pD3D11DeviceContext.Get(), m_videoCardInfo.c_str(), 22.0f, 10.0f, 70.0f);
}

}