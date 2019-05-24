//Precompiled Header [ALWAYS ON TOP IN CPP]
#include <stdafx.h>

#include "PostProcessingMaterial.h"
#include "RenderTarget.h"
#include "../Base/OverlordGame.h"
#include "../Content/ContentManager.h"

ID3D11Buffer* PostProcessingMaterial::m_pVertexBuffer = nullptr;
unsigned int PostProcessingMaterial::m_VertexBufferStride = sizeof(VertexPosTex);

ID3D11InputLayout *PostProcessingMaterial::m_pInputLayout = nullptr;
vector<ILDescription> PostProcessingMaterial::m_pInputLayoutDescriptions = vector<ILDescription>();
UINT PostProcessingMaterial::m_pInputLayoutSize = 0;
UINT PostProcessingMaterial::m_InputLayoutID = 0;

PostProcessingMaterial::PostProcessingMaterial(wstring effectFile, const wstring& technique)
	: m_IsInitialized(false),
	m_pEffect(nullptr),
	m_effectFile(effectFile),
	m_pRenderTarget(nullptr),
	m_pTechnique(nullptr),
	m_TechniqueName(technique)
{
}


PostProcessingMaterial::~PostProcessingMaterial()
{
	//Delete/Release NON-STATIC objects
	SafeDelete(m_pRenderTarget);
}

void PostProcessingMaterial::Destroy()
{
	//Delete/Release/Clear STATIC objects/lists
	SafeRelease(m_pInputLayout);
	m_pInputLayoutDescriptions.clear();

	SafeRelease(m_pVertexBuffer);
}

void PostProcessingMaterial::Initialize(const GameContext& gameContext)
{
	if(!m_IsInitialized)
	{
		m_IsInitialized = true;
		//1. LoadEffect (LoadEffect(...))
		LoadEffect(gameContext, m_effectFile);
		//2. CreateInputLaytout (CreateInputLayout(...))
		CreateInputLayout(gameContext, m_pTechnique);
		//   CreateVertexBuffer (CreateVertexBuffer(...)) > As a TriangleStrip (FullScreen Quad)
		CreateVertexBuffer(gameContext);
		//3. Create RenderTarget (m_pRenderTarget)
		//		Take a look at the class, figure out how to initialize/create a RenderTarget Object
		//		GameSettings > OverlordGame::GetGameSettings()
		m_pRenderTarget = new RenderTarget(gameContext.pDevice);
		auto gameSettings = OverlordGame::GetGameSettings();
		RENDERTARGET_DESC targetDesc = {};
		targetDesc.Height = gameSettings.Window.Height;
		targetDesc.Width = gameSettings.Window.Width;
		targetDesc.GenerateMipMaps = false;
		m_pRenderTarget->Create(targetDesc);
	}
}

bool PostProcessingMaterial::LoadEffect(const GameContext& gameContext, const wstring& effectFile)
{
	UNREFERENCED_PARAMETER(gameContext);

	//Load Effect through ContentManager
	m_pEffect = ContentManager::Load<ID3DX11Effect>(effectFile);
	//Check if m_TechniqueName (default constructor parameter) is set
	if(m_TechniqueName.size() > 0){
		// If SET > Use this Technique (+ check if valid)
		auto techString = string(m_TechniqueName.begin(), m_TechniqueName.end());
		m_pTechnique = m_pEffect->GetTechniqueByName(techString.c_str());
	}else{
		// If !SET > Use Technique with index 0
		m_pTechnique = m_pEffect->GetTechniqueByIndex(0);
	}

	//Call LoadEffectVariables
	LoadEffectVariables();

	return true;
}

//Static Function
void PostProcessingMaterial::Draw(const GameContext& gameContext, RenderTarget* previousRendertarget)
{
	//1. Clear the object's RenderTarget (m_pRenderTarget) [Check RenderTarget Class]
	FLOAT color[] = { 0,0,0,0 };
	m_pRenderTarget->Clear(gameContext, color);
	//2. Call UpdateEffectVariables(...)
	UpdateEffectVariables(previousRendertarget);
	//3. Set InputLayout
	gameContext.pDeviceContext->IASetInputLayout(m_pInputLayout);
	//4. Set VertexBuffer
	UINT stride = sizeof(VertexPosTex);
	UINT offset = 0;
	gameContext.pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
	//5. Set PrimitiveTopology (TRIANGLESTRIP!!)
	gameContext.pDeviceContext->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	//6. Draw (NOT Indexed!)
	D3DX11_TECHNIQUE_DESC techDesc;
	m_pTechnique->GetDesc(&techDesc);

	for (int i = 0; i < techDesc.Passes; ++i)
	{
		m_pTechnique->GetPassByIndex(i)->Apply(0, gameContext.pDeviceContext);
	}
	gameContext.pDeviceContext->Draw(NUM_VERTS, 0);

	//7. Unbind the PreviousRendertarget's ShaderResource view from the pipeline. Because we'll need to use it as RenderTarget later on. 
	//   A resource can't be bound as RenderTarget & ShaderResource at the same time. See what happens when you comment these last lines.

	// Unbind SRV from PS-Stage > So it can be used as a RenderTarget later on
	ID3D11ShaderResourceView *const pSRV[] = {nullptr};
    gameContext.pDeviceContext->PSSetShaderResources(0, 1,pSRV);
}

//Static Function
void PostProcessingMaterial::CreateVertexBuffer(const GameContext& gameContext)
{
	//Static Buffer Object (Created once, shared between other PPMaterials)
	if (m_pVertexBuffer)
		return;

	//Create a vertex buffer for a full screen quad. Use the VertexPosTex struct (m_pVertexBuffer)
	//Primitive Topology: TriangleStrip
	//Vertex Amount: NUM_VERTS (4)
	//Vertex Struct: VertexPosTex
	//Use NDC to define your vertex positions

	// Create vertex array containing 4 elements in system memory
	vector<VertexPosTex> vertices;
	vertices.push_back(VertexPosTex(XMFLOAT3(-1.f, 1.f, 0.f), XMFLOAT2(0.f, 0.f)));
	vertices.push_back(VertexPosTex(XMFLOAT3(1.f, 1.f, 0.f), XMFLOAT2(1.f, 0.f)));
	vertices.push_back(VertexPosTex(XMFLOAT3(-1.f, -1.f, 0.f), XMFLOAT2(0.f, 1.f)));
	vertices.push_back(VertexPosTex(XMFLOAT3(1.f, -1.f, 0.f), XMFLOAT2(1.f, 1.f)));

	D3D11_BUFFER_DESC desc = {};
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	desc.ByteWidth = sizeof(VertexPosTex) * NUM_VERTS;
	desc.MiscFlags = 0;
	desc.CPUAccessFlags = D3D10_CPU_ACCESS_FLAG::D3D10_CPU_ACCESS_WRITE;
	desc.Usage = D3D11_USAGE_DYNAMIC;

	D3D11_SUBRESOURCE_DATA data = {};
	data.pSysMem = vertices.data();

	HRESULT error = gameContext.pDevice->CreateBuffer(&desc, &data, &m_pVertexBuffer);

	//+ Check for errors (HRESULT)
	if (FAILED(error)){Logger::LogWarning(L"PostProcessingMaterial::CreateVertexBuffer() > Failed to create vertex buffer!");}

	//Set 'm_VertexBufferStride' (Size in bytes of VertexPosTex)
	m_VertexBufferStride = sizeof(VertexPosTex);


}

void PostProcessingMaterial::CreateInputLayout(const GameContext& gameContext, ID3DX11EffectTechnique* pTechnique)
{
	//Static InputLayout Object (Created once, shared between other PPMaterials)
	if (m_pInputLayout)
		return;

	//Create a simple InputLayout for these effects
	//2 Elements, POSITION & TEXCOORD (Check PP-Shaders)
	D3D11_INPUT_ELEMENT_DESC inputLayout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	UINT numElements = sizeof(inputLayout) / sizeof(inputLayout[0]);

	D3DX11_PASS_DESC PassDesc;
	pTechnique->GetPassByIndex(0)->GetDesc(&PassDesc);
	HRESULT error = gameContext.pDevice->CreateInputLayout(inputLayout, numElements, PassDesc.pIAInputSignature, PassDesc.IAInputSignatureSize, &m_pInputLayout);

	//+ Check for errors (HRESULT)
	if (FAILED(error)) { Logger::LogWarning(L"PostProcessingMaterial::CreateInputLayout() > Failed to create input layout!"); }

}

RenderTarget* PostProcessingMaterial::GetRenderTarget() const
{
	return m_pRenderTarget;
}