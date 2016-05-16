//
//  BallStaticMeshComponent.cpp
//  Chilli Source
//  Created by Scott Downie on 07/10/2010.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2010 Tag Games Limited
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//  THE SOFTWARE.
//

#include <Game/Ball/BallStaticMeshComponent.h>

#include <CSProfiling/CSProfiling.h>

#include <ChilliSource/Rendering/Material/Material.h>
#include <ChilliSource/Rendering/Material/MaterialFactory.h>
#include <ChilliSource/Rendering/Base/RenderSystem.h>
#include <ChilliSource/Rendering/Sprite/DynamicSpriteBatcher.h>

#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Delegate/MakeDelegate.h>
#include <ChilliSource/Core/Entity/Entity.h>
#include <ChilliSource/Core/Math/Matrix4.h>

#include <ChilliSource/Rendering/Model/SubMesh.h>

#include <algorithm>
#include <limits>

namespace CSPong
{
	CS_DEFINE_NAMEDTYPE(BallStaticMeshComponent);

	BallStaticMeshComponent::BallStaticMeshComponent()
		: m_isBSValid(false), m_isAABBValid(false), m_isOOBBValid(false)
	{
		mMaterials.push_back(mpMaterial);
	}
	//----------------------------------------------------------
	/// Is A
	//----------------------------------------------------------
	bool BallStaticMeshComponent::IsA(CSCore::InterfaceIDType inInterfaceID) const
	{
		return  (inInterfaceID == BallStaticMeshComponent::InterfaceID) ||
			(inInterfaceID == RenderComponent::InterfaceID) ||
			(inInterfaceID == VolumeComponent::InterfaceID);
	}
	//----------------------------------------------------
	/// Get Axis Aligned Bounding Box
	//----------------------------------------------------
	const CSCore::AABB& BallStaticMeshComponent::GetAABB()
	{
		if (GetEntity() && !m_isAABBValid)
		{
			m_isAABBValid = true;

			//Rebuild the box
			const CSCore::AABB& cAABB = mpModel->GetAABB();
			const CSCore::Matrix4& matWorld = GetEntity()->GetTransform().GetWorldTransform();
			CSCore::Vector3 vBackBottomLeft(cAABB.BackBottomLeft() * matWorld);
			CSCore::Vector3 vBackBottomRight(cAABB.BackBottomRight() * matWorld);
			CSCore::Vector3 vBackTopLeft(cAABB.BackTopLeft() * matWorld);
			CSCore::Vector3 vBackTopRight(cAABB.BackTopRight() * matWorld);
			CSCore::Vector3 vFrontBottomLeft(cAABB.FrontBottomLeft() * matWorld);
			CSCore::Vector3 vFrontBottomRight(cAABB.FrontBottomRight() * matWorld);
			CSCore::Vector3 vFrontTopLeft(cAABB.FrontTopLeft() *matWorld);
			CSCore::Vector3 vFrontTopRight(cAABB.FrontTopRight() * matWorld);

			CSCore::Vector3 vMin(std::numeric_limits<f32>::infinity(), std::numeric_limits<f32>::infinity(), std::numeric_limits<f32>::infinity());
			vMin.x = std::min(vMin.x, vBackBottomLeft.x);
			vMin.x = std::min(vMin.x, vBackBottomRight.x);
			vMin.x = std::min(vMin.x, vBackTopLeft.x);
			vMin.x = std::min(vMin.x, vBackTopRight.x);
			vMin.x = std::min(vMin.x, vFrontBottomLeft.x);
			vMin.x = std::min(vMin.x, vFrontBottomRight.x);
			vMin.x = std::min(vMin.x, vFrontTopLeft.x);
			vMin.x = std::min(vMin.x, vFrontTopRight.x);

			vMin.y = std::min(vMin.y, vBackBottomLeft.y);
			vMin.y = std::min(vMin.y, vBackBottomRight.y);
			vMin.y = std::min(vMin.y, vBackTopLeft.y);
			vMin.y = std::min(vMin.y, vBackTopRight.y);
			vMin.y = std::min(vMin.y, vFrontBottomLeft.y);
			vMin.y = std::min(vMin.y, vFrontBottomRight.y);
			vMin.y = std::min(vMin.y, vFrontTopLeft.y);
			vMin.y = std::min(vMin.y, vFrontTopRight.y);

			vMin.z = std::min(vMin.z, vBackBottomLeft.z);
			vMin.z = std::min(vMin.z, vBackBottomRight.z);
			vMin.z = std::min(vMin.z, vBackTopLeft.z);
			vMin.z = std::min(vMin.z, vBackTopRight.z);
			vMin.z = std::min(vMin.z, vFrontBottomLeft.z);
			vMin.z = std::min(vMin.z, vFrontBottomRight.z);
			vMin.z = std::min(vMin.z, vFrontTopLeft.z);
			vMin.z = std::min(vMin.z, vFrontTopRight.z);

			CSCore::Vector3 vMax(-std::numeric_limits<f32>::infinity(), -std::numeric_limits<f32>::infinity(), -std::numeric_limits<f32>::infinity());
			vMax.x = std::max(vMax.x, vBackBottomLeft.x);
			vMax.x = std::max(vMax.x, vBackBottomRight.x);
			vMax.x = std::max(vMax.x, vBackTopLeft.x);
			vMax.x = std::max(vMax.x, vBackTopRight.x);
			vMax.x = std::max(vMax.x, vFrontBottomLeft.x);
			vMax.x = std::max(vMax.x, vFrontBottomRight.x);
			vMax.x = std::max(vMax.x, vFrontTopLeft.x);
			vMax.x = std::max(vMax.x, vFrontTopRight.x);

			vMax.y = std::max(vMax.y, vBackBottomLeft.y);
			vMax.y = std::max(vMax.y, vBackBottomRight.y);
			vMax.y = std::max(vMax.y, vBackTopLeft.y);
			vMax.y = std::max(vMax.y, vBackTopRight.y);
			vMax.y = std::max(vMax.y, vFrontBottomLeft.y);
			vMax.y = std::max(vMax.y, vFrontBottomRight.y);
			vMax.y = std::max(vMax.y, vFrontTopLeft.y);
			vMax.y = std::max(vMax.y, vFrontTopRight.y);

			vMax.z = std::max(vMax.z, vBackBottomLeft.z);
			vMax.z = std::max(vMax.z, vBackBottomRight.z);
			vMax.z = std::max(vMax.z, vBackTopLeft.z);
			vMax.z = std::max(vMax.z, vBackTopRight.z);
			vMax.z = std::max(vMax.z, vFrontBottomLeft.z);
			vMax.z = std::max(vMax.z, vFrontBottomRight.z);
			vMax.z = std::max(vMax.z, vFrontTopLeft.z);
			vMax.z = std::max(vMax.z, vFrontTopRight.z);

			mBoundingBox.SetSize(vMax - vMin);
			mBoundingBox.SetOrigin(cAABB.Centre() * matWorld);
		}

		return mBoundingBox;
	}
	//----------------------------------------------------
	/// Get Object Oriented Bounding Box
	//----------------------------------------------------
	const CSCore::OOBB& BallStaticMeshComponent::GetOOBB()
	{
		if (GetEntity() && !m_isOOBBValid)
		{
			m_isOOBBValid = true;

			mOBBoundingBox.SetTransform(GetEntity()->GetTransform().GetWorldTransform());
			// Origin and Size updated in AttachMesh
		}
		return mOBBoundingBox;
	}
	//----------------------------------------------------
	/// Get Bounding Sphere
	//----------------------------------------------------
	const CSCore::Sphere& BallStaticMeshComponent::GetBoundingSphere()
	{
		if (GetEntity() && !m_isBSValid)
		{
			m_isBSValid = true;

			const CSCore::AABB& sAABB = GetAABB();
			mBoundingSphere.vOrigin = sAABB.GetOrigin();
			mBoundingSphere.fRadius = (sAABB.BackTopRight() - sAABB.FrontBottomLeft()).Length() * 0.5f;
		}
		return mBoundingSphere;
	}
	//-----------------------------------------------------------
	/// Is Transparent
	//-----------------------------------------------------------
	bool BallStaticMeshComponent::IsTransparent()
	{
		for (u32 i = 0; i < mMaterials.size(); ++i)
		{
			if (mMaterials[i]->IsTransparencyEnabled() == true)
				return true;
		}
		return false;
	}
	//-----------------------------------------------------------
	/// Set Material
	//-----------------------------------------------------------
	void BallStaticMeshComponent::SetMaterial(const CSRendering::MaterialCSPtr& inpMaterial)
	{
		mpMaterial = inpMaterial;

		//apply to all materials
		for (u32 i = 0; i < mMaterials.size(); i++)
		{
			mMaterials[i] = mpMaterial;
		}
	}
	//-----------------------------------------------------------
	/// Set Material For Sub Mesh
	//-----------------------------------------------------------
	void BallStaticMeshComponent::SetMaterialForSubMesh(const CSRendering::MaterialCSPtr& inpMaterial, u32 indwSubMeshIndex)
	{
		if (indwSubMeshIndex < mMaterials.size())
		{
			mMaterials[indwSubMeshIndex] = inpMaterial;

			if (indwSubMeshIndex == 0)
			{
				mpMaterial = inpMaterial;
			}
		}
	}
	//-----------------------------------------------------------
	/// Set Material For Sub Mesh
	//-----------------------------------------------------------
	void BallStaticMeshComponent::SetMaterialForSubMesh(const CSRendering::MaterialCSPtr& inpMaterial, const std::string& instrSubMeshName)
	{
		if (nullptr != mpModel)
		{
			s32 indwIndex = mpModel->GetSubMeshIndexByName(instrSubMeshName);
			if (indwIndex >= 0 && indwIndex < (s32)mMaterials.size())
			{
				mMaterials[indwIndex] = inpMaterial;

				if (indwIndex == 0)
				{
					mpMaterial = inpMaterial;
				}
			}
		}
	}
	//-----------------------------------------------------------
	/// Get Material Of Sub Mesh
	//-----------------------------------------------------------
	CSRendering::MaterialCSPtr BallStaticMeshComponent::GetMaterialOfSubMesh(u32 indwSubMeshIndex) const
	{
		if (indwSubMeshIndex < mMaterials.size())
		{
			return mMaterials[indwSubMeshIndex];
		}

		CS_LOG_ERROR("Failed to get material from sub mesh " + CSCore::ToString(indwSubMeshIndex));
		return nullptr;
	}
	//-----------------------------------------------------------
	/// Get Material Of Sub Mesh
	//-----------------------------------------------------------
	CSRendering::MaterialCSPtr BallStaticMeshComponent::GetMaterialOfSubMesh(const std::string& instrSubMeshName) const
	{
		if (nullptr != mpModel)
		{
			s32 indwIndex = mpModel->GetSubMeshIndexByName(instrSubMeshName);

			if (indwIndex >= 0 && indwIndex < (s32)mMaterials.size())
			{
				return mMaterials[indwIndex];
			}
		}

		CS_LOG_ERROR("Failed to get material from sub mesh " + instrSubMeshName);
		return nullptr;
	}
	//----------------------------------------------------------
	/// Attach Mesh
	//----------------------------------------------------------
	void BallStaticMeshComponent::AttachMesh(const CSRendering::MeshCSPtr& inpModel)
	{
		mpModel = inpModel;

		// Update OOBB
		mOBBoundingBox.SetSize(mpModel->GetAABB().GetSize());
		mOBBoundingBox.SetOrigin(mpModel->GetAABB().GetOrigin());
	}
	//----------------------------------------------------------
	/// Attach Mesh
	//----------------------------------------------------------
	void BallStaticMeshComponent::AttachMesh(const CSRendering::MeshCSPtr& inpModel, const CSRendering::MaterialCSPtr& inpMaterial)
	{
		mpModel = inpModel;
		mpMaterial = inpMaterial;

		// Update OOBB
		mOBBoundingBox.SetSize(mpModel->GetAABB().GetSize());
		mOBBoundingBox.SetOrigin(mpModel->GetAABB().GetOrigin());

		SetMaterial(inpMaterial);
	}
	//----------------------------------------------------------
	/// Get Mesh
	//----------------------------------------------------------
	const CSRendering::MeshCSPtr& BallStaticMeshComponent::GetMesh() const
	{
		return mpModel;
	}
	//----------------------------------------------------------
	/// Render
	//----------------------------------------------------------
	void BallStaticMeshComponent::Render(CSRendering::RenderSystem* inpRenderSystem, CSRendering::CameraComponent* inpCam, CSRendering::ShaderPass ineShaderPass)
	{
		if (IsTransparent())
		{
			//Flush the sprite cache to maintain order
			inpRenderSystem->GetDynamicSpriteBatchPtr()->ForceRender();
		}

		mpModel->Render(inpRenderSystem, GetEntity()->GetTransform().GetWorldTransform(), mMaterials, ineShaderPass);

		//Increment the number of times the ball is rendered
		CSCore::Application::Get()->GetSystem<CSProfiling::MetricsSystem>()->IncrementRenderBallCalled();
	}
	//----------------------------------------------------------
	/// Render Shadow Map
	//----------------------------------------------------------
	void BallStaticMeshComponent::RenderShadowMap(CSRendering::RenderSystem* inpRenderSystem, CSRendering::CameraComponent* inpCam, const CSRendering::MaterialCSPtr& in_staticShadowMap, const CSRendering::MaterialCSPtr& in_animShadowMap)
	{
		mpModel->Render(inpRenderSystem, GetEntity()->GetTransform().GetWorldTransform(), { in_staticShadowMap }, CSRendering::ShaderPass::k_ambient);
	}
	//----------------------------------------------------
	//----------------------------------------------------
	void BallStaticMeshComponent::OnAddedToScene()
	{
		m_transformChangedConnection = GetEntity()->GetTransform().GetTransformChangedEvent().OpenConnection(CSCore::MakeDelegate(this, &BallStaticMeshComponent::OnEntityTransformChanged));

		OnEntityTransformChanged();
	}
	//----------------------------------------------------
	/// On Entity Transform Changed
	//----------------------------------------------------
	void BallStaticMeshComponent::OnEntityTransformChanged()
	{
		m_isBSValid = false;
		m_isAABBValid = false;
		m_isOOBBValid = false;
	}
	//----------------------------------------------------
	//----------------------------------------------------
	void BallStaticMeshComponent::OnRemovedFromScene()
	{
		m_transformChangedConnection = nullptr;
	}
}
