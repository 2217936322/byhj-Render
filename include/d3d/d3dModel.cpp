#include "d3dModel.h"
#include "modelMgr.h"
#include "textureMgr.h"

namespace byhj
{
	void setColor(aiColor3D &c, XMFLOAT4 &m)
	{
		m.x = c[0];
		m.y = c[1];
		m.z = c[2];
	}

	void setBlend(float blend, D3DMesh::Material &mat)
	{
		mat.ambient.w = blend;
		mat.specular.w = blend;
		mat.diffuse.w = blend;
		mat.emissive.w = blend;
	}
	void D3DModel::init(ID3D11Device *pD3D11Device, ID3D11DeviceContext *pD3D11DeviceContext)
	{
		D3D11_BLEND_DESC blendDesc;
		ZeroMemory(&blendDesc, sizeof(blendDesc));
		D3D11_RENDER_TARGET_BLEND_DESC rtbd;
		ZeroMemory(&rtbd, sizeof(rtbd));
		rtbd.BlendEnable			 = true;
		rtbd.SrcBlend				 = D3D11_BLEND_SRC_ALPHA;
		rtbd.DestBlend				 = D3D11_BLEND_INV_SRC_ALPHA;
		rtbd.BlendOp				 = D3D11_BLEND_OP_ADD;
		rtbd.SrcBlendAlpha			 = D3D11_BLEND_ONE;
		rtbd.DestBlendAlpha			 = D3D11_BLEND_ZERO;
		rtbd.BlendOpAlpha			 = D3D11_BLEND_OP_ADD;
		rtbd.RenderTargetWriteMask	 = D3D10_COLOR_WRITE_ENABLE_ALL;
		blendDesc.AlphaToCoverageEnable = false;
		blendDesc.RenderTarget[0] = rtbd;
		pD3D11Device->CreateBlendState(&blendDesc, &Transparency);


		for (int i = 0; i != m_D3DTransparentMeshes.size(); ++i) {
			m_D3DTransparentMeshes[i].init(pD3D11Device, pD3D11DeviceContext);
		}
		for (int i = 0; i != m_D3DOpaqueMeshes.size(); ++i) {
			m_D3DOpaqueMeshes[i].init(pD3D11Device, pD3D11DeviceContext);
		}
	}
	// Draws the model, and thus all its meshes
	void D3DModel::render(ID3D11DeviceContext *pD3D11DeviceContext)
	{


		for (int i = 0; i < this->m_D3DOpaqueMeshes.size(); i++) {
			pD3D11DeviceContext->OMSetBlendState(0, 0, 0xffffffff);
			   this->m_D3DOpaqueMeshes[i].render(pD3D11DeviceContext);
		}

		for (int i = 0; i < this->m_D3DTransparentMeshes.size(); i++) {
			float blendFactor[] ={ 0.0f, 0.0f, 0.0f, 1.0f };
			pD3D11DeviceContext->OMSetBlendState(Transparency, blendFactor, 0xffffffff);
			this->m_D3DTransparentMeshes[i].render(pD3D11DeviceContext);
		}
	}



	void D3DModel::loadModel(std::string modelFile)
	{
		auto dir = ModelMgr::getInstance()->getDir();
		auto fileDir = dir + modelFile;


		Assimp::Importer importer;
		const aiScene *pScene = importer.ReadFile(dir + modelFile, aiProcess_Triangulate | aiProcess_FlipUVs);

		if (!pScene || pScene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !pScene->mRootNode) {
			std::cerr << "Fail to load the model ( " << modelFile << ") : "
				<< importer.GetErrorString() << std::endl;
		}
		else {
			std::cout << "Load Model: " << fileDir << std::endl;
		}

		m_dir = fileDir.substr(0, fileDir.find_last_of('/') + 1);
				TextureMgr::getInstance()->setDir(m_dir);
		processNode(pScene->mRootNode, pScene);
	}

	void D3DModel::processNode(aiNode *pNode, const aiScene *pScene)
	{
		for (GLuint i = 0; i < pNode->mNumMeshes; ++i) {
			aiMesh *pMesh = pScene->mMeshes[pNode->mMeshes[i]];

			D3DMesh meshData;
			processMesh(pMesh, pScene, meshData);
			if (meshData.isBlend()) {
				m_D3DTransparentMeshes.push_back(meshData);
			}
			else {
				m_D3DOpaqueMeshes.push_back(meshData);
			}
		}

		for (GLuint i = 0; i < pNode->mNumChildren; ++i) {
			processNode(pNode->mChildren[i], pScene);
		}
	}


	void D3DModel::processMesh(aiMesh *mesh, const aiScene *scene, D3DMesh &d3dMesh)
	{
		// Data to fill
		std::vector<D3DMesh::Vertex> vertices;
		std::vector<GLuint> indices;
		std::vector<D3DMesh::Texture> textures;
		D3DMesh::Material mat;
		XMVECTOR v[3], uv[3];

		// Walk through each of the mesh's vertices
		for (UINT i = 0, j = 0; i < mesh->mNumVertices; i++) {
			D3DMesh::Vertex vertex;
			XMFLOAT3 vector; // We declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
			// Positions
			vector.x = mesh->mVertices[i].x;
			vector.y = mesh->mVertices[i].y;
			vector.z = mesh->mVertices[i].z;
			vertex.position = vector;

			// Normals
			vector.x = mesh->mNormals[i].x;
			vector.y = mesh->mNormals[i].y;
			vector.z = mesh->mNormals[i].z;
			vertex.normal = vector;

			// Texture Coordinates // Does the mesh contain texture coordinates
			if (mesh->mTextureCoords[0]) {
				XMFLOAT2 vec;
				// A vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
				// use models where a vertex can have multiple texture coordinates so we always take the first set (0).
				vec.x = mesh->mTextureCoords[0][i].x;
				vec.y = mesh->mTextureCoords[0][i].y;
				vertex.texcoord = vec;
			}
			else {
				vertex.texcoord = XMFLOAT2(0.0f, 0.0f);
			}

			vertices.push_back(vertex);
		
		    v[j]  = XMLoadFloat3(&vertex.position);
		    uv[j] = XMLoadFloat2(&vertex.texcoord);
		    
		    if ( (j+1) % 3 == 0)
		    {
		    	// Edges of the triangle : postion delta
		    	XMVECTOR deltaPos1 = v[1]  - v[0];
		    	XMVECTOR deltaPos2 = v[2]  - v[0];
		    	XMVECTOR deltaUV1  = uv[1] - uv[0];
		    	XMVECTOR deltaUV2  = uv[2] - uv[0];
		    
		    	float uv1x = XMVectorGetX(deltaUV1);
		    	float uv1y = XMVectorGetY(deltaUV1);
		    	float uv2x = XMVectorGetX(deltaUV2);
		    	float uv2y = XMVectorGetY(deltaUV2);
		    
		    	float r = 1.0f / (uv1x * uv2y - uv1y * uv2x);
		    	XMVECTOR tangent = (deltaPos1 * uv2y   - deltaPos2 * uv1y) * r;
		    	XMVECTOR bitangent = (deltaPos2 * uv1x   - deltaPos1 * uv2x) * r;
		    
		    	XMStoreFloat3(&vertices[i].tangent, tangent);
		    	XMStoreFloat3(&vertices[i-1].tangent, tangent);
		    	XMStoreFloat3(&vertices[i-2].tangent, tangent);
		    
		    	XMStoreFloat3(&  vertices[i].bitangent, bitangent);
		    	XMStoreFloat3(&vertices[i-1].bitangent, bitangent);
		    	XMStoreFloat3(&vertices[i-2].bitangent, bitangent);
		    	j = 0;
		    } else {
		    	++j;
		    }

		}


		// Now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
		for (GLuint i = 0; i < mesh->mNumFaces; i++) {
			aiFace face = mesh->mFaces[i];
			// Retrieve all indices of the face and store them in the indices vector
			for (GLuint j = 0; j < face.mNumIndices; j++) {
				indices.push_back(face.mIndices[j]);
			}
		}

		// Process materials
		if (mesh->mMaterialIndex >= 0) {
			int tt = mesh->mMaterialIndex;
			aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
			// We assume a convention for sampler names in the shaders. Each diffuse texture should be named
			// as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER. 
			// Same applies to other texture as the following list summarizes:
			// Diffuse: texture_diffuseN
			// Specular: texture_specularN
			// Normal: texture_normalN

			aiColor3D ambient;
			if (AI_SUCCESS == material->Get(AI_MATKEY_COLOR_AMBIENT, ambient))
				setColor(ambient, mat.ambient);
			//std::cout << mat.ambient.w << std::endl;
			aiColor3D diffuse;
			if (AI_SUCCESS == material->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse))
				setColor(diffuse, mat.diffuse);

			aiColor3D  speucular;
			if (AI_SUCCESS == material->Get(AI_MATKEY_COLOR_SPECULAR, speucular))
				setColor(speucular, mat.specular);

			aiColor3D  emissive;
			if (AI_SUCCESS == material->Get(AI_MATKEY_COLOR_EMISSIVE, emissive))
				setColor(emissive, mat.emissive);

			//float shininess = 0.0;
			//if(AI_SUCCESS == material->Get(AI_MATKEY_SHININESS, shininess))
			//	mat.shininess = shininess;
			float blend;
			material->Get(AI_MATKEY_OPACITY, blend);
			if (blend < 1.0f)
				setBlend(blend, mat);
			//std::cout << mat.ambient.w << std::endl;

		// 1. Diffuse maps
			std::vector<D3DMesh::Texture> diffuseMaps = this->loadD3DTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
			textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

			// 2. Specular maps
			std::vector<D3DMesh::Texture> specularMaps = this->loadD3DTextures(material, aiTextureType_SPECULAR, "texture_specular");
			textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

			// 3.normal maps
			std::vector<D3DMesh::Texture> normalMaps = this->loadD3DTextures(material, aiTextureType_HEIGHT, "texture_normal");
			textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
		}

		// Return a mesh object created from the extracted mesh data
		std::swap(d3dMesh, D3DMesh(vertices, indices, textures, mat));

	}


	// Checks all material textures of a given type and loads the textures if they're not loaded yet.
// The required info is returned as a Texture struct.
	std::vector<D3DMesh::Texture> D3DModel::loadD3DTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
	{
		std::vector<D3DMesh::Texture> textures;
		for (GLuint i = 0; i < mat->GetTextureCount(type); i++) {
			aiString str;
			mat->GetTexture(type, i, &str);
			D3DMesh::Texture texture;
			texture.pTextureSRV = nullptr;//TextureMgr::getInstance()->loadD3DTexture(pD3D11Device, str.C_Str());
			texture.type = typeName;
			texture.name = str.C_Str();
			textures.push_back(texture);
		}
		return textures;
	}
}