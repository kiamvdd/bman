//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "stdafx.h"

#include "CameraComponent.h"
#include "../Base/OverlordGame.h"
#include "../Components/TransformComponent.h"
#include "../Diagnostics/Logger.h"
#include "../Physx/PhysxManager.h"
#include "../Physx/PhysxProxy.h"
#include "../Scenegraph/GameObject.h"
#include "../Scenegraph/GameScene.h"
#include "../Base/GeneralStructs.h"

CameraComponent::CameraComponent(void):
	m_FOV(XM_PIDIV4),
	m_NearPlane(0.1f),
	m_FarPlane(2500.0f),
	m_Size(25.0f),
	m_PerspectiveProjection(true),
	m_IsActive(true)
{
	XMStoreFloat4x4(&m_Projection, XMMatrixIdentity());
	XMStoreFloat4x4(&m_View, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ViewInverse, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ViewProjection, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ViewProjectionInverse, XMMatrixIdentity());
}


CameraComponent::~CameraComponent(void)
{
}

void CameraComponent::Initialize(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
}

void CameraComponent::Update(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
	auto windowSettings = OverlordGame::GetGameSettings().Window;
	XMMATRIX projection, view, viewInv, viewProjectionInv;

	if(m_PerspectiveProjection)
	{
		projection = XMMatrixPerspectiveFovLH(m_FOV, windowSettings.AspectRatio ,m_NearPlane, m_FarPlane);
	}
	else
	{
		float viewWidth = (m_Size>0) ? m_Size * windowSettings.AspectRatio : windowSettings.Width;
		float viewHeight = (m_Size>0) ? m_Size : windowSettings.Height;
		projection = XMMatrixOrthographicLH(viewWidth, viewHeight, m_NearPlane, m_FarPlane);
	}

	XMVECTOR worldPosition = XMLoadFloat3(&GetTransform()->GetWorldPosition());
	XMVECTOR lookAt = XMLoadFloat3(&GetTransform()->GetForward());
	XMVECTOR upVec = XMLoadFloat3(&GetTransform()->GetUp());

	view = XMMatrixLookAtLH(worldPosition, worldPosition + lookAt, upVec);
	viewInv = XMMatrixInverse(nullptr, view);
	viewProjectionInv = XMMatrixInverse(nullptr, view * projection);

	XMStoreFloat4x4(&m_Projection, projection);
	XMStoreFloat4x4(&m_View, view);
	XMStoreFloat4x4(&m_ViewInverse, viewInv);
	XMStoreFloat4x4(&m_ViewProjection, view * projection);
	XMStoreFloat4x4(&m_ViewProjectionInverse, viewProjectionInv);
}

void CameraComponent::Draw(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
}

void CameraComponent::SetActive()
{
	auto gameObject = GetGameObject();
	if(gameObject == nullptr)
	{
		Logger::LogError(L"[CameraComponent] Failed to set active camera. Parent game object is null");
		return;
	}

	auto gameScene = gameObject->GetScene();
	if(gameScene == nullptr)
	{
		Logger::LogError(L"[CameraComponent] Failed to set active camera. Parent game scene is null");
		return;
	}

	gameScene->SetActiveCamera(this);
}

GameObject* CameraComponent::Pick(const GameContext& gameContext, CollisionGroupFlag ignoreGroups) const
{
	//TODO
	float wHeight = OverlordGame::GetGameSettings().Window.Height;
	float wWidth= OverlordGame::GetGameSettings().Window.Width;
	float wHalfWidth = wWidth * 0.5f;
	float wHalfHeight = wHeight * 0.5f;

	POINT mousePos = gameContext.pInput->GetMousePosition();

	float xNDC = (mousePos.x - wHalfWidth) / wHalfWidth;
	float yNDC = (wHalfHeight - mousePos.y) / wHalfHeight;

	XMFLOAT4 point = XMFLOAT4{ xNDC, yNDC, 0, 0 };
	XMVECTOR vec = XMLoadFloat4(&point);
	XMMATRIX VI = XMLoadFloat4x4(&(gameContext.pCamera->GetViewInverse()));
	XMVECTOR result = XMVector4Transform(vec, VI);
	XMFLOAT4 nearPoint;
	XMStoreFloat4(&nearPoint, result);

	point = XMFLOAT4{ xNDC, yNDC, 0, 0 };
	vec = XMLoadFloat4(&point);
	result = XMVector4Transform(vec, VI);
	XMFLOAT4 farPoint;
	XMStoreFloat4(&farPoint, result);
	
	auto pPhysxProxy = m_pGameObject->GetScene()->GetPhysxProxy();
	PxQueryFilterData filterData;
	filterData.data.word0 = ~ignoreGroups;

	PxVec3 rayStart = PxVec3(nearPoint.x, nearPoint.y, nearPoint.z);
	PxVec3 rayDir = PxVec3(farPoint.x, farPoint.y, farPoint.z);
	rayDir = rayDir = rayStart;
	rayDir.normalize();
	PxRaycastBuffer hit;

	if (pPhysxProxy->Raycast(rayStart, rayDir, PX_MAX_F32, hit, PxHitFlag::eDEFAULT, filterData)) {
		
	}

	return nullptr;
}