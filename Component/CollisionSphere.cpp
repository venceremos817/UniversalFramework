#include "CollisionSphere.h"
#include <CollisionOBB.h>
#include <CollisionCapsule.h>
#include <CollisionRect.h>
#include <BoundaryVolume.h>
#include <Transform.h>
#include <App.h>

struct CollisionSphere::Param
{
	float makedDiameter;		// 作成時の直径
	size_t isHitVolumeIdx;		// 配列ボリュームと衝突時に衝突した配列を特定するインデックス
	CalcScaling calcScaling;	// 計算時に使用するスケール

	Param()
		: makedDiameter(1.0f), isHitVolumeIdx(0), calcScaling(CalcScaling::XScale)
	{}
};


CollisionSphere::CollisionSphere(const std::shared_ptr<GameObject>& pGameObject)
	: Collision(pGameObject)
	, m_pParam(new Param())
{
}

CollisionSphere::~CollisionSphere()
{
}

void CollisionSphere::CollisionCall(const std::shared_ptr<Collision>& pSrcCol)
{
	pSrcCol->CollisionTest(GetShared<CollisionSphere>());
}

float CollisionSphere::GetMakedRadius() const
{
	return m_pParam->makedDiameter * 0.5f;
}

void CollisionSphere::SetMakedRadius(float radius)
{
	m_pParam->makedDiameter = radius * 2.0f;
}

float CollisionSphere::GetMakedDiameter() const
{
	return m_pParam->makedDiameter;
}

void CollisionSphere::SetMakeDiameter(float diameter)
{
	m_pParam->makedDiameter = diameter;
}

Collision::CalcScaling CollisionSphere::GetCalcScaling() const
{
	return m_pParam->calcScaling;
}

void CollisionSphere::SetCalcScaling(CalcScaling s)
{
	m_pParam->calcScaling = s;
}

SphereBoundaryVolume CollisionSphere::GetSphereVolume() const
{
	Float4x4 matBase;
	matBase.Scale(Float3(m_pParam->makedDiameter, m_pParam->makedDiameter, m_pParam->makedDiameter));
	matBase *= GetGameObject()->GetTransform()->GetWorldMatrix();
	// このオブジェクトのSphere境界ボリュームを作成
	SphereBoundaryVolume retSphereVol(matBase.ExtractTransform(), matBase.ExtractQuaternion().x * 0.5f);
	switch (m_pParam->calcScaling)
	{
	case CalcScaling::YScale:
		retSphereVol.m_radius = matBase.ExtractScale().y * 0.5f;
		break;
	case CalcScaling::ZScale:
		retSphereVol.m_radius = matBase.ExtractScale().z * 0.5f;
		break;
	}

	return retSphereVol;
}

SphereBoundaryVolume CollisionSphere::GetBeforeSphereVolume() const
{
	Float4x4 matBase;
	matBase.Scale(Float3(m_pParam->makedDiameter, m_pParam->makedDiameter, m_pParam->makedDiameter));
	matBase *= GetGameObject()->GetTransform()->GetPreWolrdMatrix();
	// Sphere境界ボリュームを作成
	SphereBoundaryVolume retSphereVol(matBase.ExtractTransform(), matBase.ExtractScale().x * 0.5f);
	switch (m_pParam->calcScaling)
	{
	case CalcScaling::YScale:
		retSphereVol.m_radius = matBase.ExtractScale().y * 0.5f;
		break;
	case CalcScaling::ZScale:
		retSphereVol.m_radius = matBase.ExtractScale().z * 0.5f;
		break;
	}

	return retSphereVol;
}

SphereBoundaryVolume CollisionSphere::GetWrappedSphereVol() const
{
	return GetSphereVolume();
}

SphereBoundaryVolume CollisionSphere::GetEnclosingSphereVol() const
{
	SphereBoundaryVolume curSphereVol = GetSphereVolume();
	SphereBoundaryVolume beforeSphereVol = GetBeforeSphereVolume();

	SphereBoundaryVolume encloseVol = BoundaryVolume::SphereEnclosingSphere(curSphereVol, beforeSphereVol);
	return encloseVol;
}

void CollisionSphere::CollisionTest(const std::shared_ptr<CollisionSphere>& pDestSphere)
{
	auto pDestTransform = pDestSphere->GetGameObject()->GetTransform();
	Float3 srcVelocity = GetGameObject()->GetTransform()->GetVelocity();
	Float3 destVelocity = pDestTransform->GetVelocity();
	Float3 spanVelocity = srcVelocity - destVelocity;	// 相対速度
	float elapsedTime = APP->GetElapsedTime();
	float hitTime = 0.0f;
	SphereBoundaryVolume srcBeforeSphereVol = GetBeforeSphereVolume();
	SphereBoundaryVolume destBeforeSphereVol = pDestSphere->GetBeforeSphereVolume();

	if (BoundaryVolume::CollisionTestSphereSphere(srcBeforeSphereVol, spanVelocity, destBeforeSphereVol, 0.0f, elapsedTime, hitTime))
	{
		Float3 sqeewrcVelocity = GetGameObject()->GetTransform()->GetVelocity();
		Float3 deeqewewqstVelocity = pDestTransform->GetVelocity();
		AfterCollisionTemplate_1<CollisionSphere, CollisionSphere>
			(srcVelocity, destVelocity, elapsedTime, hitTime, GetShared<CollisionSphere>(), pDestSphere);
	}
}

void CollisionSphere::CollisionTest(const std::shared_ptr<CollisionOBB>& pDestOBB)
{
	// Sphereボリュームで衝突判定を行い衝突していないなら終了
	if (!CollisionWrappedSphere(pDestOBB))
		return;

	auto pDestTransform = pDestOBB->GetGameObject()->GetTransform();
	Float3 srcVelocity = GetGameObject()->GetTransform()->GetVelocity();
	Float3 destVelocity = pDestTransform->GetVelocity();
	Float3 spanVelocity = srcVelocity - destVelocity;
	float elapsedTime = APP->GetElapsedTime();
	float hitTime = 0.0f;
	SphereBoundaryVolume srcBeforeSphereVol = GetBeforeSphereVolume();
	OBBBoundaryVolume destBeforeOBBVol = pDestOBB->GetBeforeOBBVol();

	if (BoundaryVolume::CollisionTestSphereOBB(srcBeforeSphereVol, spanVelocity, destBeforeOBBVol, 0.0f, elapsedTime, hitTime))
	{
		AfterCollisionTemplate_1<CollisionSphere, CollisionOBB>
			(srcVelocity, destVelocity, elapsedTime, hitTime, GetShared<CollisionSphere>(), pDestOBB);
	}
}

void CollisionSphere::CollisionTest(const std::shared_ptr<CollisionCapsule>& pDestCapsule)
{
	// Sphereボリュームで衝突判定を行い衝突していないなら終了
	if (!CollisionWrappedSphere(pDestCapsule))
		return;

	auto pDestTransform = pDestCapsule->GetGameObject()->GetTransform();
	Float3 srcVelocity = GetGameObject()->GetTransform()->GetVelocity();
	Float3 destVelocity = pDestTransform->GetVelocity();
	Float3 spanVelocity = srcVelocity - destVelocity;		// 相対速度
	float elapsedTime = APP->GetElapsedTime();
	float hitTime = 0.0f;
	SphereBoundaryVolume srcBeforeSphereVol = GetBeforeSphereVolume();
	CapsuleBoundaryVolume destBeforeCapsuleVol = pDestCapsule->GetBeforeCapsuleVolume();
	if (BoundaryVolume::CollisionTestSphereCapsule(srcBeforeSphereVol, spanVelocity, destBeforeCapsuleVol, 0.0f, elapsedTime, hitTime))
	{
		AfterCollisionTemplate_1<CollisionSphere, CollisionCapsule>
			(srcVelocity, destVelocity, elapsedTime, hitTime, GetShared<CollisionSphere>(), pDestCapsule);
	}
}

void CollisionSphere::CollisionTest(const std::shared_ptr<CollisionRect>& pDestRect)
{
	auto pDestTransform = pDestRect->GetGameObject()->GetTransform();
	Float3 srcVelocity = GetGameObject()->GetTransform()->GetVelocity();
	Float3 destVelocity = pDestTransform->GetVelocity();
	Float3 spanVelocity = srcVelocity - destVelocity;	// 相対速度
	float elapsedTime = APP->GetElapsedTime();
	float hitTime = 0.0f;
	SphereBoundaryVolume srcBeforeSphereVol = GetBeforeSphereVolume();
	RectBoundaryVolume destBeforeRectVol = pDestRect->GetBeforeRectVol();
	if (BoundaryVolume::CollisionTestSphereRect(srcBeforeSphereVol, spanVelocity, destBeforeRectVol, 0.0f, elapsedTime, hitTime))
	{
		AfterCollisionTemplate_2<CollisionSphere, CollisionRect	>
			(srcVelocity, destVelocity, elapsedTime, hitTime, GetShared<CollisionSphere>(), pDestRect);
	}
}

void CollisionSphere::CollisionEscape(const std::shared_ptr<CollisionSphere>& pDestSphere)
{
	SphereBoundaryVolume srcSphereVol = GetSphereVolume();
	SphereBoundaryVolume destSphereVol = pDestSphere->GetSphereVolume();
	if (BoundaryVolume::Sphere_Sphere(srcSphereVol, destSphereVol))
	{
		Float3 normal = srcSphereVol.m_center - destSphereVol.m_center;
		normal.Normalize();
		float span = srcSphereVol.m_radius + destSphereVol.m_radius;
		normal *= span;
		Float3 pos = destSphereVol.m_center + normal;	// エスケープ位置
		GetGameObject()->GetTransform()->ResetWorldPos(pos);	// リセットによって戻す
	}
}

void CollisionSphere::CollisionEscape(const std::shared_ptr<CollisionOBB>& pDestOBB)
{
	SphereBoundaryVolume srcSphereVol = GetSphereVolume();
	OBBBoundaryVolume destOBBVol = pDestOBB->GetOBBVol();
	Float3 closestPos;

	if (!BoundaryVolume::Sphere_OBB(srcSphereVol, destOBBVol, closestPos))
		return;		// 当たってなければ終了

	// HACK : ループで当たらなくなるまで～をしたほうがいいかも
	Float3 span = srcSphereVol.m_center - closestPos;
	span.Normalize();
	span *= srcSphereVol.m_radius;
	span += closestPos;
	GetGameObject()->GetTransform()->ResetWorldPos(span);
}

void CollisionSphere::CollisionEscape(const std::shared_ptr<CollisionCapsule>& pDestCapsule)
{
	SphereBoundaryVolume srcSphereVol = GetSphereVolume();
	CapsuleBoundaryVolume destCapsuleVol = pDestCapsule->GetCapsuleVolume();
	Float3 closestPos;

	bool isHit = BoundaryVolume::SPHERE_CAPSULE(srcSphereVol, destCapsuleVol, closestPos);
	if (!isHit)
		return;		// 当たってなければ終了

	Float3 span = srcSphereVol.m_center - closestPos;
	span.Normalize();
	span *= 0.02f;
	Float3 center = srcSphereVol.m_center;
	int cnt = 0;
	// ヒットしなくなるまでループ
	while (true)
	{
		center += span;
		srcSphereVol.m_center = center;
		// ヒットしなくなったらループ終了
		if (!BoundaryVolume::SPHERE_CAPSULE(srcSphereVol, destCapsuleVol, closestPos))
			break;
		cnt++;
		if (cnt > 50)
			break;
	}

	// リセットによってエスケープ
	GetGameObject()->GetTransform()->ResetWorldPos(center);
}

void CollisionSphere::CollisionEscape(const std::shared_ptr<CollisionRect>& pDestRect)
{
	SphereBoundaryVolume srcSphereVol = GetSphereVolume();
	RectBoundaryVolume destRectVol = pDestRect->GetRectVol();
	Float3 closestPos;

	if (!BoundaryVolume::SPHERE_RECT(srcSphereVol, destRectVol, closestPos))
		return;	// ヒットしていなければ終了

	Float3 normal = srcSphereVol.m_center - closestPos;
	normal.Normalize();
	normal *= srcSphereVol.m_radius;
	normal += closestPos;

	// リセットによってエスケープ
	GetGameObject()->GetTransform()->ResetWorldPos(normal);
}

void CollisionSphere::GetHitNormal(const std::shared_ptr<CollisionSphere>& pDestSphere, Float3 & retNormal) const
{
	SphereBoundaryVolume srcSphereVol = GetSphereVolume();
	SphereBoundaryVolume destSphereVol = pDestSphere->GetSphereVolume();
	retNormal = destSphereVol.m_center - srcSphereVol.m_center;
	retNormal.Normalize();
}

void CollisionSphere::GetHitNormal(const std::shared_ptr<CollisionOBB>& pDestOBB, Float3 & retNormal) const
{
	SphereBoundaryVolume srcSphereVol = GetSphereVolume();
	OBBBoundaryVolume destOBBVol = pDestOBB->GetOBBVol();
	Float3 closestPos;
	BoundaryVolume::Sphere_OBB(srcSphereVol, destOBBVol, closestPos);
	retNormal = closestPos - srcSphereVol.m_center;
	retNormal.Normalize();
}

void CollisionSphere::GetHitNormal(const std::shared_ptr<CollisionCapsule>& pDestCapsule, Float3 & retNormal) const
{
	SphereBoundaryVolume srcSphereVol = GetSphereVolume();
	CapsuleBoundaryVolume destCapVol = pDestCapsule->GetCapsuleVolume();
	Float3 closestPos;
	BoundaryVolume::SPHERE_CAPSULE(srcSphereVol, destCapVol, closestPos);
	retNormal = closestPos - srcSphereVol.m_center;
	retNormal.Normalize();
}

void CollisionSphere::GetHitNormal(const std::shared_ptr<CollisionRect>& pDestRect, Float3 & retNormal) const
{
	SphereBoundaryVolume srcSphereVol = GetSphereVolume();
	RectBoundaryVolume destRectVol = pDestRect->GetRectVol();
	PlaneBoundaryVolume plane = destRectVol.GetPlaneVolume();
	retNormal = plane.m_normal;
	retNormal.Normalize();
}

void CollisionSphere::BackToBefore(const Float3 & totalVelocity, float spanTime)
{
	SphereBoundaryVolume srcBeforeSphereVol = GetBeforeSphereVolume();		// 前の球ボリュームを作成
	Float3 pos = srcBeforeSphereVol.m_center + totalVelocity * spanTime;	// 前の球ボリュームから指定時間分動いた座標
	// リセットによって移動
	GetGameObject()->GetTransform()->ResetWorldPos(pos);
}
