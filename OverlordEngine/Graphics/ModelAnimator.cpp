//Precompiled Header [ALWAYS ON TOP IN CPP]
#include <stdafx.h>

#include "ModelAnimator.h"
#include "../Diagnostics/Logger.h"


ModelAnimator::ModelAnimator(MeshFilter* pMeshFilter):
m_pMeshFilter(pMeshFilter),
m_Transforms(vector<XMFLOAT4X4>()),
m_IsPlaying(false), 
m_Reversed(false),
m_ClipSet(false),
m_TickCount(0.000000001f),
m_AnimationSpeed(1.0f),
m_EOFClip(false),
m_Loop(true)

{
	SetAnimation(0);
}


ModelAnimator::~ModelAnimator()
{
}

void ModelAnimator::SetAnimation(UINT clipNumber)
{
	//Set m_ClipSet to false
	m_ClipSet = false;
	//Check if clipNumber is smaller than the actual m_AnimationClips vector size
	if (clipNumber < GetClipCount()) {
		SetAnimation(m_pMeshFilter->m_AnimationClips[clipNumber]);
	} else {
		Reset();
		Logger::LogWarning(L"SetAnimation failed: clipnumber exceeds clip count");
		return;
	}


	//If not,
	//	Call Reset
	//	Log a warning with an appropriate message
	//	return
	//else
	//	Retrieve the AnimationClip from the m_AnimationClips vector based on the given clipNumber
	//	Call SetAnimation(AnimationClip clip)
}

void ModelAnimator::SetAnimation(wstring clipName)
{
	m_ClipSet = false;

	for (size_t i = 0; i < GetClipCount(); i++) {
		if (m_pMeshFilter->m_AnimationClips[i].Name == clipName) {
			SetAnimation(m_pMeshFilter->m_AnimationClips[i]);
			return;
		}
	}

	Reset();
	Logger::LogWarning(L"SetAnimation failed: no clip with that name found");

	//Set m_ClipSet to false
	//Iterate the m_AnimationClips vector and search for an AnimationClip with the given name (clipName)
	//If found,
	//	Call SetAnimation(Animation Clip) with the found clip
	//Else
	//	Call Reset
	//	Log a warning with an appropriate message
}

void ModelAnimator::SetAnimation(AnimationClip clip)
{
	//Set m_ClipSet to true
	m_ClipSet = true;

	//Set m_CurrentClip
	m_CurrentClip = clip;
	
	Reset(false);
	//Call Reset(false)
}

void ModelAnimator::Reset(bool pause)
{
	//If pause is true, set m_IsPlaying to false
	m_IsPlaying = !pause;
	m_EOFClip = false;
	m_TickCount = m_Reversed ? m_CurrentClip.Duration : 0;
	m_AnimationSpeed = 1.0f;
	//Set m_TickCount to zero
	//Set m_AnimationSpeed to 1.0f

	if (m_ClipSet) {
		m_Transforms.assign(m_CurrentClip.Keys[0].BoneTransforms.begin(), m_CurrentClip.Keys[0].BoneTransforms.end());
	} else {
		XMFLOAT4X4 id;
		XMStoreFloat4x4(&id, XMMatrixIdentity());
		m_Transforms.assign(1, id);
	}
	//If m_ClipSet is true
	//	Retrieve the BoneTransform from the first Key from the current clip (m_CurrentClip)
	//	Refill the m_Transforms vector with the new BoneTransforms (have a look at vector::assign)
	//Else
	//	Create an IdentityMatrix 
	//	Refill the m_Transforms vector with this IdenityMatrix (Amount = BoneCount) (have a look at vector::assign)
}

void ModelAnimator::Update(const GameContext& gameContext)
{
	//We only update the transforms if the animation is running and the clip is set
	if (m_IsPlaying && m_ClipSet)
	{
		m_EOFClip = false;
		float passedTicks = m_CurrentClip.TicksPerSecond * gameContext.pGameTime->GetElapsed() * m_AnimationSpeed;
		passedTicks = fmod(passedTicks, m_CurrentClip.Duration);
		//Make sure that the passedTicks stay between the m_CurrentClip.Duration bounds (fmod)

		//2. 
		//IF m_Reversed is true
		if (m_Reversed) {
			m_TickCount -= passedTicks;
			if (m_TickCount < 0) {
				m_TickCount += m_CurrentClip.Duration;
			}
		}
		else {
			m_TickCount += passedTicks;
			if (m_TickCount > m_CurrentClip.Duration) {
				m_TickCount -= m_CurrentClip.Duration;
			}
		}

		//Find the enclosing keys
		AnimationKey keyA, keyB;
		if (m_CurrentClip.Keys.size() > 0) keyA = m_CurrentClip.Keys[0], keyB = m_CurrentClip.Keys[0];
		//Iterate all the keys of the clip and find the following keys:
		float aDiff = std::numeric_limits<float>::max(), bDiff = std::numeric_limits<float>::max();

		int keyIndexB = 0;
		for (size_t i = 0; i < m_CurrentClip.Keys.size(); i++) {
			float keyTick = m_CurrentClip.Keys[i].Tick;
			if (keyTick < m_TickCount) {
				if (m_TickCount - keyTick < aDiff) {
					aDiff = m_TickCount - keyTick;
					keyA = m_CurrentClip.Keys[i];
				}
			} else {
				if (keyTick - m_TickCount < bDiff) {
					bDiff = keyTick - m_TickCount;
					keyB = m_CurrentClip.Keys[i];
					keyIndexB = i;
				}
			}
		}

		if (m_Reversed)
		{
			if (keyIndexB <= 1)
			{
				m_EOFClip = true;

				if (!m_Loop)
				{
					m_IsPlaying = false;
					return;
				}
			}
		}
		else
		{
			if (keyIndexB >= m_CurrentClip.Keys.size() - 1)
			{
				m_EOFClip = true;

				if (!m_Loop)
				{
					m_IsPlaying = false;
					return;
				}
			}
		}


		//keyA > Closest Key with Tick before/smaller than m_TickCount
		//keyB > Closest Key with Tick after/bigger than m_TickCount
		
		//4.
		//Interpolate between keys
		//Figure out the BlendFactor (See lab document)
		//Clear the m_Transforms vector
		m_Transforms.clear();
		//FOR every boneTransform in a key (So for every bone)
		for (size_t i = 0; i < m_CurrentClip.Keys[0].BoneTransforms.size(); i++) {
			XMFLOAT4X4 transA;
			transA = keyA.BoneTransforms[i];
			XMFLOAT4X4 transB;
			transB = keyB.BoneTransforms[i];
			float weightA, weightB;
			float totalDiff = aDiff + bDiff;
			weightA = bDiff / totalDiff;
			weightB = aDiff / totalDiff;
			XMMATRIX matA = XMLoadFloat4x4(&transA);
			matA = matA * weightA;

			XMMATRIX matB = XMLoadFloat4x4(&transB);
			matB = matB * weightB;

			XMMATRIX total = matA + matB;
			XMFLOAT4X4 transform;
			XMStoreFloat4x4(&transform, total);
			m_Transforms.push_back(transform);
		}
		//	Retrieve the transform from keyA (transformA)
		//	auto transformA = ...
		// 	Retrieve the transform from keyB (transformB)
		//	auto transformB = ...
		//	Decompose both transforms
		//	Lerp between all the transformations (Position, Scale, Rotation)
		//	Compose a transformation matrix with the lerp-results
		//	Add the resulting matrix to the m_Transforms vector
	}
}
