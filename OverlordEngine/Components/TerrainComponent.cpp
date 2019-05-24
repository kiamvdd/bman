//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "stdafx.h"

#include "TerrainComponent.h"

#include "..\Base\GeneralStructs.h"
#include "../Content/ContentManager.h"
#include "../Helpers/EffectHelper.h"
#include "../Helpers/VertexHelper.h"
#include "TransformComponent.h"
#include "ColliderComponent.h"
#include "../Graphics/TextureData.h"
#include "../Physx/PhysxManager.h"
#include "../Physx/PhysxProxy.h"
#include "../Scenegraph/GameObject.h"
#include "../Scenegraph/GameScene.h"


TerrainComponent::TerrainComponent(const wstring& heightMapFile, const wstring& textureFile, unsigned int nrOfRows, unsigned int nrOfColumns, float width, float depth, float maxHeight) :
	m_HeightMapFile(heightMapFile),
	m_TextureFile(textureFile),
	m_NrOfRows(nrOfRows),
	m_NrOfColumns(nrOfColumns),
	m_NrOfVertices(nrOfRows*nrOfColumns),
	m_NrOfTriangles(2 * (nrOfRows - 1) * (nrOfColumns - 1)),
	m_Width(width),
	m_Depth(depth),
	m_MaxHeight(maxHeight)
{
	
}

TerrainComponent::~TerrainComponent()
{
	SafeRelease(m_pInputLayout);
	SafeRelease(m_pVertexBuffer);
	SafeRelease(m_pIndexBuffer);
	//m_HeightField->release();
}

void TerrainComponent::Initialize(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);

	//Load Effect
	m_pEffect = ContentManager::Load<ID3DX11Effect>(L"Resources/Effects/PosNormTex3D.fx");
	m_pTechnique = m_pEffect->GetTechniqueByIndex(0);

	//TODO: Error handling
	m_pMatWorldViewProjVariable = m_pEffect->GetVariableByName("gWorldViewProj")->AsMatrix();
	m_pMatWorldVariable = m_pEffect->GetVariableByName("gWorld")->AsMatrix();
	m_pDiffuseVariable = m_pEffect->GetVariableByName("gDiffuseMap")->AsShaderResource();

	UINT ilSize = 0;
	if (!EffectHelper::BuildInputLayout(gameContext.pDevice, m_pTechnique, &m_pInputLayout, ilSize))
		Logger::LogError(L"TerrainComponent::Initialize >> BuildInputLayout failed!");

	//Texture
	m_pTextureData = ContentManager::Load<TextureData>(m_TextureFile);

	//Load Height Map
	ReadHeightMap();

	//Create Vertices & Triangles
	CalculateVerticesAndIndices();

	//Build Vertexbuffer
	BuildVertexBuffer(gameContext);

	//Build Indexbuffer
	BuildIndexBuffer(gameContext);

	//Create PhysX Heightfield
	CreatePxHeightField();

}

void TerrainComponent::Update(const GameContext& gameContext)
{ 
	UNREFERENCED_PARAMETER(gameContext);
}

void TerrainComponent::Draw(const GameContext& gameContext)
{ 
	XMMATRIX world = XMLoadFloat4x4(&GetTransform()->GetWorld());
	XMMATRIX viewProj = XMLoadFloat4x4(&gameContext.pCamera->GetViewProjection());
	XMMATRIX wvp = XMMatrixMultiply(world, viewProj);
	m_pMatWorldVariable->SetMatrix(reinterpret_cast<float*>(&world));
	m_pMatWorldViewProjVariable->SetMatrix(reinterpret_cast<float*>(&wvp));
	m_pDiffuseVariable->SetResource(m_pTextureData->GetShaderResourceView());

	// Set vertex buffer
	UINT stride = sizeof(VertexPosNormTex);
	UINT offset = 0;
	gameContext.pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);

	// Set index buffer
	gameContext.pDeviceContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the input layout
	gameContext.pDeviceContext->IASetInputLayout(m_pInputLayout);

	// Set primitive topology
	gameContext.pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Render a cube
	D3DX11_TECHNIQUE_DESC techDesc;
	m_pTechnique->GetDesc(&techDesc);
	for (UINT p = 0; p< techDesc.Passes; ++p)
	{
		m_pTechnique->GetPassByIndex(p)->Apply(0, gameContext.pDeviceContext);
		gameContext.pDeviceContext->DrawIndexed(m_NumIndices, 0, 0);
	}
}

//Exercise - Heightmap
void TerrainComponent::ReadHeightMap()
{
	m_VecHeightValues.resize(m_NrOfVertices, 0);

	std::ifstream in;
	in.open(m_HeightMapFile.c_str(), std::ios_base::binary);

	if (!in) {
		Logger::LogFormat(LogLevel::Error, L"Loading terrain \'%ls\' failed!", m_HeightMapFile.c_str());
		return;
	}

	in.read(reinterpret_cast<char*>(&m_VecHeightValues[0]), static_cast<std::streamsize>(m_NrOfVertices * sizeof(unsigned short)));
	in.close();
}

//Exercise - Flat Grid
void TerrainComponent::CalculateVerticesAndIndices()
{
	ReadHeightMap();
	//**VERTICES
	//Reserve spots in the buffer
	m_VecVertices.resize(m_NrOfColumns * m_NrOfRows);

	float cellWidth = m_Width / m_NrOfColumns;
	float cellDepth = m_Depth / m_NrOfRows;
	float cellXPos = -m_Width * 0.5f;
	float cellZPos = m_Depth * 0.5f;

	for (int row = 0; row < m_NrOfRows; row++) {
		cellXPos = -m_Width * 0.5f;
		for (int col = 0; col < m_NrOfColumns; col++) {
			int vertexId = row * m_NrOfColumns + col;
			m_VecVertices[vertexId].Position.x = cellXPos;
			m_VecVertices[vertexId].Position.y = m_VecHeightValues[vertexId] / (float)USHRT_MAX * m_MaxHeight;
			m_VecVertices[vertexId].Position.z = cellZPos;

			m_VecVertices[vertexId].Normal = XMFLOAT3(0, 1, 0);

			m_VecVertices[vertexId].TexCoord.x = col / (float)m_NrOfColumns;
			m_VecVertices[vertexId].TexCoord.y = row / (float)m_NrOfRows;

			cellXPos += cellWidth;
		}

		cellZPos -= cellDepth;
	}


	int nrQuadsRow = m_NrOfRows - 1;
	int nrQuadsColumn = m_NrOfColumns - 1;
	for (int row = 0; row < nrQuadsRow; ++row) {
		for (int col = 0; col < nrQuadsColumn; ++col) {
			int a = row * m_NrOfColumns + col;
			int b = a + 1;
			int c = a + m_NrOfColumns;
			int d = c + 1;
			AddQuad(a, b, c, d);
		}
	}

	for (int row = 0; row < m_NrOfRows; row++) {
		for (int col = 0; col < m_NrOfColumns; col++) {
			int vertexId = row * m_NrOfColumns + col;
			int tempId = 0;
			XMVECTOR pC = XMLoadFloat3(&m_VecVertices[vertexId].Position);
			XMVECTOR avgNormal = { 0, 0, 0, 0 };
			int surfaceCount = 6;

			XMVECTOR p1;
			XMVECTOR p2;
			XMVECTOR v1;
			XMVECTOR v2;
			XMVECTOR n;

			// FACE 1
			if (row == 0 || col == 0) {
				surfaceCount -= 1;
			} else {
				tempId = (row - 1) * m_NrOfColumns + (col - 1);
				p1 = XMLoadFloat3(&m_VecVertices[tempId].Position);
				tempId = (row - 1) * m_NrOfColumns + col;
				p2 = XMLoadFloat3(&m_VecVertices[tempId].Position);

				v1 = p2 - p1;
				v2 = pC - p1;
				n = XMVector3Cross(v1, v2);

				avgNormal += n;
			}


			// FACE 2
			if (row == 0 || col == m_NrOfColumns - 1) {
				surfaceCount -= 1;
			} else {
				tempId = (row - 1) * m_NrOfColumns + col;
				p1 = XMLoadFloat3(&m_VecVertices[tempId].Position);
				tempId = row * m_NrOfColumns + (col + 1);
				p2 = XMLoadFloat3(&m_VecVertices[tempId].Position);

				v1 = p2 - p1;
				v2 = pC - p1;
				n = XMVector3Cross(v1, v2);

				avgNormal += XMVector3Normalize(n);
			}

			// FACE 3
			if (row == m_NrOfRows - 1 || col == m_NrOfColumns - 1) {
				surfaceCount -= 1;
			} else {
				tempId = row * m_NrOfColumns + (col + 1);
				p1 = XMLoadFloat3(&m_VecVertices[tempId].Position);
				tempId = (row + 1) * m_NrOfColumns + (col + 1);
				p2 = XMLoadFloat3(&m_VecVertices[tempId].Position);

				v1 = p2 - p1;
				v2 = pC - p1;
				n = XMVector3Cross(v1, v2);

				avgNormal += XMVector3Normalize(n);
			}



			// FACE 4
			if (row == m_NrOfRows - 1 || col == m_NrOfColumns - 1) {
				surfaceCount -= 1;
			} else {
				tempId = (row + 1) * m_NrOfColumns + (col + 1);
				p1 = XMLoadFloat3(&m_VecVertices[tempId].Position);
				tempId = (row + 1) * m_NrOfColumns + col;
				p2 = XMLoadFloat3(&m_VecVertices[tempId].Position);

				v1 = p2 - p1;
				v2 = pC - p1;
				n = XMVector3Cross(v1, v2);

				avgNormal += XMVector3Normalize(n);
			}


			// FACE 5
			if (row == m_NrOfRows - 1 || col == 0) {
				surfaceCount -= 1;
			} else {
				tempId = (row + 1) * m_NrOfColumns + col;
				p1 = XMLoadFloat3(&m_VecVertices[tempId].Position);
				tempId = row * m_NrOfColumns + (col - 1);
				p2 = XMLoadFloat3(&m_VecVertices[tempId].Position);

				v1 = p2 - p1;
				v2 = pC - p1;
				n = XMVector3Cross(v1, v2);

				avgNormal += XMVector3Normalize(n);
			}

			// FACE 6
			if (row == 0 || col == 0) {
				surfaceCount -= 1;
			} else {
				tempId = row * m_NrOfColumns + (col - 1);
				p1 = XMLoadFloat3(&m_VecVertices[tempId].Position);
				tempId = (row - 1) * m_NrOfColumns + (col - 1);
				p2 = XMLoadFloat3(&m_VecVertices[tempId].Position);

				v1 = p2 - p1;
				v2 = pC - p1;
				n = XMVector3Cross(v1, v2);

				avgNormal += XMVector3Normalize(n);
			}

			XMVECTOR normal = XMVector3Normalize(avgNormal);

			XMStoreFloat3(&m_VecVertices[vertexId].Normal, normal);
		}
	}
}

//Exercise - Flat Grid
void TerrainComponent::AddTriangle(unsigned int a, unsigned int b, unsigned c)
{
	m_VecIndices.push_back(DWORD(a));
	m_VecIndices.push_back(DWORD(b));
	m_VecIndices.push_back(DWORD(c));
}

//Exercise - Flat Grid
void TerrainComponent::AddQuad(unsigned int a, unsigned int b, unsigned c, unsigned d)
{
	AddTriangle(a, d, c);
	AddTriangle(a, b, d);
}

void TerrainComponent::BuildVertexBuffer(const GameContext& gameContext)
{
	//Vertexbuffer	
	D3D11_BUFFER_DESC bd = {};
	D3D11_SUBRESOURCE_DATA initData = { 0 };
	bd.Usage = D3D11_USAGE_IMMUTABLE;
	bd.ByteWidth = sizeof(VertexPosNormTex) * m_NrOfVertices;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	initData.pSysMem = m_VecVertices.data();
	HRESULT hr = gameContext.pDevice->CreateBuffer(&bd, &initData, &m_pVertexBuffer);
	Logger::LogHResult(hr, L"Failed to Create Vertexbuffer");
}

void TerrainComponent::BuildIndexBuffer(const GameContext& gameContext)
{
	D3D11_BUFFER_DESC bd = {};
	D3D11_SUBRESOURCE_DATA initData = { 0 };
	bd.Usage = D3D11_USAGE_IMMUTABLE;
	bd.ByteWidth = sizeof(DWORD) * m_VecIndices.size();
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	initData.pSysMem = m_VecIndices.data();
	HRESULT hr = gameContext.pDevice->CreateBuffer(&bd, &initData, &m_pIndexBuffer);
	Logger::LogHResult(hr, L"Failed to Create Indexbuffer");
	m_NumIndices = m_VecIndices.size();
}

//Exercise - PhysX
void TerrainComponent::CreatePxHeightField()
{
	//Implement PhysX HeightField
	//auto physX = PhysxManager::GetInstance()->GetPhysics();
	//PxHeightFieldDesc desc;
	//desc.setToDefault();
	//desc.nbRows = m_NrOfRows;
	//desc.nbColumns = m_NrOfColumns;
	//PxStridedData samples;
	//std::cout << sizeof(m_VecHeightValues) << std::endl;
	//std::vector<PxHeightFieldSample*> vec_Samples;
	//vec_Samples.resize(m_VecHeightValues.size());

	//for (size_t i = 0; i < vec_Samples.size(); i++) {
	//	vec_Samples[i] = new PxHeightFieldSample();
	//	vec_Samples[i]->height = m_VecHeightValues[i];
	//}

	//samples.stride = sizeof(vec_Samples);
	//samples.data = vec_Samples.data();
	//desc.samples = samples;

	//m_HeightField = physX->createHeightField(desc);
}
