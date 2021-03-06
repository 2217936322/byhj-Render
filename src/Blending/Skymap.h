#ifndef CUBEMAP_H
#define CUBEMAP_H

#include "d3d/d3dShader.h"
#include "d3d/D3DUtility.h"

#include "Common.h"

#include <vector>
#include <dxgi.h>
#include <d3dcommon.h>
#include <d3d11.h>

#include <DirectXMath.h> 
using namespace DirectX;

namespace byhj
{


class Skymap
{
public:

	void init(ID3D11Device *pD3D11Device, ID3D11DeviceContext *pD3D11DeviceContext,  HWND hWnd);
	void update();
	void render(ID3D11DeviceContext *pD3D11DeviceContext, const D3DMVPMatrix &mvpMatrix);
	void shutdown();

private:
	void createSphere(ID3D11Device *pD3D11Device, int LatLines, int LongLines);

	void init_buffer(ID3D11Device *pD3D11Device);
	void init_texture(ID3D11Device *pD3D11Device);	
	void init_shader(ID3D11Device *pD3D11Device, HWND hWnd);


	byhj::MatrixBuffer cbMatrix;

	int m_VertexCount = 0;
	int m_IndexCount = 0;
	int NumSphereVertices;
	int NumSphereFaces;

	ComPtr<ID3D11Buffer> m_pIndexBuffer;
	ComPtr<ID3D11Buffer> m_pVertexBuffer;
	ComPtr<ID3D11Buffer> m_pMVPBuffer;
	ComPtr<ID3D11SamplerState>       m_pTexSamplerState;
	ComPtr<ID3D11ShaderResourceView> m_pSkymapTexSRV;

	D3DShader SkymapShader;
};



}

#endif