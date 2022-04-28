#include "CollisionSphere.h"
#include <CollisionOBB.h>
#include <CollisionCapsule.h>
#include <CollisionRect.h>
#include <BoundaryVolume.h>
#include <Transform.h>
#include <App.h>

struct CollisionSphere::Param
{
	float makedDiameter;		// �쐬���̒��a
	size_t isHitVolumeIdx;		// �z��{�����[���ƏՓˎ��ɏՓ˂����z�����肷��C���f�b�N�X
	CalcScaling calcScaling;	// �v�Z���Ɏg�p����X�P�[��

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
	// ���̃I�u�W�F�N�g��Sphere���E�{�����[�����쐬
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
	// Sphere���E�{�����[�����쐬
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
	Float3 spanVelocity = srcVelocity - destVelocity;	// ���Α��x
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
	// Sphere�{�����[���ŏՓ˔�����s���Փ˂��Ă��Ȃ��Ȃ�I��
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
	// Sphere�{�����[���ŏՓ˔�����s���Փ˂��Ă��Ȃ��Ȃ�I��
	if (!CollisionWrappedSphere(pDestCapsule))
		return;

	auto pDestTransform = pDestCapsule->GetGameObject()->GetTransform();
	Float3 srcVelocity = GetGameObject()->GetTransform()->GetVelocity();
	Float3 destVelocity = pDestTransform->GetVelocity();
	Float3 spanVelocity = srcVelocity - destVelocity;		// ���Α��x
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
	Float3 spanVelocity = srcVelocity - destVelocity;	// ���Α��x
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
		Float3 pos = destSphereVol.m_center + normal;	// �G�X�P�[�v�ʒu
		GetGameObject()->GetTransform()->ResetWorldPos(pos);	// ���Z�b�g�ɂ���Ė߂�
	}
}

void CollisionSphere::CollisionEscape(const std::shared_ptr<CollisionOBB>& pDestOBB)
{
	SphereBoundaryVolume srcSphereVol = GetSphereVolume();
	OBBBoundaryVolume destOBBVol = pDestOBB->GetOBBVol();
	Float3 closestPos;

	if (!BoundaryVolume::Sphere_OBB(srcSphereVol, destOBBVol, closestPos))
		return;		// �������ĂȂ���ΏI��

	// HACK : ���[�v�œ�����Ȃ��Ȃ�܂Ł`�������ق�����������
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
		return;		// �������ĂȂ���ΏI��

	Float3 span = srcSphereVol.m_center - closestPos;
	span.Normalize();
	span *= 0.02f;
	Float3 center = srcSphereVol.m_center;
	int cnt = 0;
	// �q�b�g���Ȃ��Ȃ�܂Ń��[�v
	while (true)
	{
		center += span;
		srcSphereVol.m_center = center;
		// �q�b�g���Ȃ��Ȃ����烋�[�v�I��
		if (!BoundaryVolume::SPHERE_CAPSULE(srcSphereVol, destCapsuleVol, closestPos))
			break;
		cnt++;
		if (cnt > 50)
			break;
	}

	// ���Z�b�g�ɂ���ăG�X�P�[�v
	GetGameObject()->GetTransform()->ResetWorldPos(center);
}

void CollisionSphere::CollisionEscape(const std::shared_ptr<CollisionRect>& pDestRect)
{
	SphereBoundaryVolume srcSphereVol = GetSphereVolume();
	RectBoundaryVolume destRectVol = pDestRect->GetRectVol();
	Float3 closestPos;

	if (!BoundaryVolume::SPHERE_RECT(srcSphereVol, destRectVol, closestPos))
		return;	// �q�b�g���Ă��Ȃ���ΏI��

	Float3 normal = srcSphereVol.m_center - closestPos;
	normal.Normalize();
	normal *= srcSphereVol.m_radius;
	normal += closestPos;

	// ���Z�b�g�ɂ���ăG�X�P�[�v
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
	SphereBoundaryVolume srcBeforeSphereVol = GetBeforeSphereVolume();		// �O�̋��{�����[�����쐬
	Float3 pos = srcBeforeSphereVol.m_center + totalVelocity * spanTime;	// �O�̋��{�����[������w�莞�ԕ����������W
	// ���Z�b�g�ɂ���Ĉړ�
	GetGameObject()->GetTransform()->ResetWorldPos(pos);
}
