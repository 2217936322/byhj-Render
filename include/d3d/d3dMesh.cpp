#include "d3dMesh.h"
#include "textureMgr.h"

#include <sstream>

namespace byhj {

	void D3DMesh::init(ID3D11Device *pD3D11Device, ID3D11DeviceContext *pD3D11DeviceContext) 
	{
	    init_buffer(pD3D11Device);
		init_texture(pD3D11Device);
		init_state(pD3D11Device, pD3D11DeviceContext);
	}

	void D3DMesh::update()
	{
	}

	void D3DMesh::render(ID3D11DeviceContext *pD3D11DeviceContext)
	{
		// Bind appropriate textures
		int diffuseNr = 1;
		int specularNr = 1;
		int normalNr = 1;

		for (int i = 0; i < m_Textures.size(); i++)
		{
			// Retrieve texture number (the N in diffuse_textureN)
			std::stringstream ss;
			std::string number;
			std::string name = m_Textures[i].type;
			if (name == "texture_diffuse")
				ss << diffuseNr++; // Transfer GLuint to stream
			else if (name == "texture_specular")
				ss << specularNr++; // Transfer GLuint to stream
			else if (name == "texture_normal")
				ss << normalNr++;

			number = ss.str();

			pD3D11DeviceContext->PSSetShaderResources(i, 1, &m_Textures[i].pTextureSRV);
			// Now set the sampler to the correct texture unit
		}

		// Set vertex buffer stride and offset.=
		unsigned int stride;
		unsigned int offset;
		stride = sizeof(Vertex);
		offset = 0;
		pD3D11DeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
		pD3D11DeviceContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

		pD3D11DeviceContext->UpdateSubresource(m_pMaterialBuffer, 0, NULL, &m_material, 0, 0);  
		pD3D11DeviceContext->PSSetConstantBuffers(0, 1, &m_pMaterialBuffer);

		pD3D11DeviceContext->DrawIndexed(m_IndexCount, 0, 0);
	}

	void D3DMesh::shutdown()
	{
	}


	void D3DMesh::init_buffer(ID3D11Device *pD3D11Device)
	{
		HRESULT hr;
		///////////////////////////Vertex Buffer ////////////////////////////////
		m_VertexCount =m_VertexData.size();

		// Set up the description of the static vertex buffer.
		D3D11_BUFFER_DESC VertexBufferDesc;
		VertexBufferDesc.Usage               = D3D11_USAGE_DEFAULT;
		VertexBufferDesc.ByteWidth           = sizeof(Vertex) * m_VertexCount;
		VertexBufferDesc.BindFlags           = D3D11_BIND_VERTEX_BUFFER;
		VertexBufferDesc.CPUAccessFlags      = 0;
		VertexBufferDesc.MiscFlags           = 0;
		VertexBufferDesc.StructureByteStride = 0;

		// Give the subresource structure a pointer to the vertex data.
		D3D11_SUBRESOURCE_DATA VBO;
		VBO.pSysMem          = &m_VertexData[0];
		VBO.SysMemPitch      = 0;
		VBO.SysMemSlicePitch = 0;

		// Now create the vertex buffer.
		hr = pD3D11Device->CreateBuffer(&VertexBufferDesc, &VBO, &m_pVertexBuffer);
		DebugHR(hr);

		/////////////////////////////////Index Buffer ///////////////////////////////////////

		m_IndexCount = m_IndexData.size();

		// Set up the description of the static index buffer.
		D3D11_BUFFER_DESC IndexBufferDesc;
		IndexBufferDesc.Usage               = D3D11_USAGE_DEFAULT;
		IndexBufferDesc.ByteWidth           = sizeof(UINT) * m_IndexCount;
		IndexBufferDesc.BindFlags           = D3D11_BIND_INDEX_BUFFER;
		IndexBufferDesc.CPUAccessFlags      = 0;
		IndexBufferDesc.MiscFlags           = 0;
		IndexBufferDesc.StructureByteStride = 0;

		// Give the subresource structure a pointer to the index data.
		D3D11_SUBRESOURCE_DATA IBO;
		IBO.pSysMem          = &m_IndexData[0];
		IBO.SysMemPitch      = 0;
		IBO.SysMemSlicePitch = 0;

		hr = pD3D11Device->CreateBuffer(&IndexBufferDesc, &IBO, &m_pIndexBuffer);
		DebugHR(hr);

		D3D11_BUFFER_DESC matDesc;
		ZeroMemory(&matDesc, sizeof(D3D11_BUFFER_DESC));
		matDesc.Usage = D3D11_USAGE_DEFAULT;
		matDesc.ByteWidth = sizeof(Material);
		matDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		matDesc.CPUAccessFlags = 0;
		matDesc.MiscFlags = 0;
		hr = pD3D11Device->CreateBuffer(&matDesc, NULL, &m_pMaterialBuffer);

	}
	void D3DMesh::init_state(ID3D11Device *pD3D11Device, ID3D11DeviceContext *pD3D11DeviceContext)
	{

	}

	void D3DMesh::init_texture(ID3D11Device *pD3D11Device)
	{
		for (int i = 0; i != m_Textures.size(); ++i) {
			m_Textures[i].pTextureSRV = TextureMgr::getInstance()->loadD3DTexture(pD3D11Device, m_Textures[i].name);
		}
	}
}