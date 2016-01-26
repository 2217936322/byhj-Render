#include "D3DApp.h"

namespace byhj
{

static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
static D3DApp *D3DAppHandle = 0;

void D3DApp::v_run(std::shared_ptr<App> the_app)
{	
	bool ret = init_window();

	m_pRender->v_init();

	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));
	while (ret)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) )
		{
			if(msg.message == WM_QUIT)
				break;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else 
		{
			m_pGui->v_update();
			m_pGui->v_render();

            m_pRender->v_update();
			m_pRender->v_render();

		}

	}

	m_pGui->v_render();
	m_pRender->v_shutdown();

}


bool D3DApp::init_window()
{
	//Set the window in the middle of screen
	m_ScreenWidth = GetSystemMetrics(SM_CXSCREEN) * 0.75;
	m_ScreenHeight = GetSystemMetrics(SM_CYSCREEN) * 0.75;
	m_PosX = (GetSystemMetrics(SM_CXSCREEN) - m_ScreenWidth)  / 2;
	m_PosY = (GetSystemMetrics(SM_CYSCREEN) - m_ScreenHeight) / 2;
	m_ScreenNear = 0.1f;
	m_ScreenFar  = 1000.0f;

	D3DAppHandle = this;
	m_hInstance = GetModuleHandle(NULL);

	WNDCLASSEX wc;	
	wc.cbSize = sizeof(WNDCLASSEX);	
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc;	
	wc.cbClsExtra = NULL;	
	wc.cbWndExtra = NULL;	
	wc.hInstance = m_hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);	
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);	
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 3);
	wc.lpszMenuName = NULL;	
	wc.lpszClassName = m_WndClassName;	
	wc.hIconSm = LoadIcon(NULL, IDI_WINLOGO); 

	if (!RegisterClassEx(&wc))
	{
		MessageBox(NULL, L"Registering Class Failded",	L"Error", MB_OK | MB_ICONERROR);
		return 1;
	}

	//Create the window and show

	m_hWnd = CreateWindowEx(	
		NULL,	           
		m_WndClassName,
		m_AppName,
		WS_OVERLAPPEDWINDOW,	
		m_PosX, m_PosY,
		m_ScreenWidth,	
		m_ScreenHeight,	
		NULL,
		NULL,
		m_hInstance,	
		NULL
		);

	if (!m_hWnd )	
	{
		MessageBox(NULL, L"Creating Window Failed", L"Error", MB_OK | MB_ICONERROR);
		return 1;
	}

	ShowWindow(m_hWnd, SW_SHOW);

	return true;
}


LRESULT CALLBACK D3DApp::MessageHandler(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_KEYDOWN:
		{
			if(wParam == VK_ESCAPE)
				PostMessage(GetHwnd(), WM_DESTROY, 0, 0);
		}
	case WM_LBUTTONDOWN:
	case WM_MBUTTONDOWN:
	case WM_RBUTTONDOWN:
		v_OnMouseDown(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		return 0;
	case WM_LBUTTONUP:
	case WM_MBUTTONUP:
	case WM_RBUTTONUP:
		v_OnMouseUp(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		return 0;
	case WM_MOUSEMOVE:
		v_OnMouseMove(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		return 0;
	case WM_MOUSEWHEEL:
		v_OnMouseWheel(wParam, GET_WHEEL_DELTA_WPARAM(wParam), GET_Y_LPARAM(lParam));
	default:
		{
			return DefWindowProc(hWnd, uMsg, wParam, lParam);
		}
	} 

}

LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	switch(umessage)
	{
	case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}
	case WM_CLOSE:
		{
			PostQuitMessage(0);		
			return 0;
		}
		// All other messages pass to the message handler in the system class.
	default:
		{
			return D3DAppHandle->MessageHandler(hwnd, umessage, wparam, lparam);
		}
	}
}


}