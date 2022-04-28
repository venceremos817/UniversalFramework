#include "CollisionCapsule.h"
#include <CollisionSphere.h>
#include <CollisionOBB.h>
#include <CollisionRect.h>
#include <App.h>
#include <Transform.h>


struct CollisionCapsule::Param
{
	float makedDiameter;	// �쐬���̒��a
	float makedHeight;		// �쐬���̍���
	size_t hitVolumeIndex;	// �z��{�����[���ƏՓˎ��ɏՓ˂����z�����肷��C���f�b�N�X
	Float4x4 worldM;		// ���݂̃��[���h�}�g���N�X
	Float4x4 beforeWorldM;	// �O�̃��[���h�}�g���N�X
	CapsuleBoundaryVolume capsuleVolume;	// ���݂̃J�v�Z���{�����[��
	CapsuleBoundaryVolume beforeCapsuleVolume;	// �O�̃J�v�Z���{�����[��
	bool firstCalc;			// ���݂̃J�v�Z�����v�Z����K�v�����邩
	bool beforeFirstCalc;	// �O�̃J�v�Z�����v�Z����K�v�����邩

	Param()
		: makedDiameter(1.0f)
		, makedHeight(1.0f)
		, hitVolumeIndex(0)
		, worldM()
		, beforeWorldM()
		, capsuleVolume()
		, beforeCapsuleVolume()
		, firstCalc(true)
		, beforeFirstCalc(true)
	{}
};


CollisionCapsule::CollisionCapsule(const std::shared_ptr<GameObject>& pGameObject)
	: Collision(pGameObject)
	, m_pParam(new Param())
{
}

CollisionCapsule::~CollisionCapsule()
{
}

void CollisionCapsule::CollisionCall(const std::shared_ptr<Collision>& pSrcCol)
{
	pSrcCol->CollisionTest(GetShared<CollisionCapsule>());
}

CapsuleBoundaryVolume CollisionCapsule::GetCapsuleVolume() const
{
	// ���[���h�}�g���N�X���ύX����Ă�A�܂��͑��̗��R������΃J�v�Z���Čv�Z
	if (m_pParam->firstCalc || !GetGameObject()->GetTransform()->IsSameWorldMatrix(m_pParam->worldM))
	{
		m_pParam->worldM = GetGameObject()->GetTransform()->GetWorldMatrix();
		m_pParam->capsuleVolume = CapsuleBoundaryVolume(
			m_pParam->makedDiameter * 0.5f,
			Float3(0.0f, m_pParam->makedHeight * -0.5f, 0.0f),
			Float3(0.0f, m_pParam->makedHeight * 0.5f, 0.0f),
			m_pParam->worldM);
		m_pParam->firstCalc = false;
	}
	return m_pParam->capsuleVolume;
}

CapsuleBoundaryVolume CollisionCapsule::GetBeforeCapsuleVolume() const
{
	// 1�O�̃��[���h�}�g���N�X���ύX����Ă�A�܂��͑��̗��R������΃J�v�Z���Čv�Z
	if (m_pParam->beforeFirstCalc || !GetGameObject()->GetTransform()->IsSameBeforeWorldMatrix(m_pParam->beforeWorldM))
	{
		m_pParam->beforeWorldM = GetGameObject()->GetTransform()->GetPreWolrdMatrix();
		m_pParam->beforeCapsuleVolume = CapsuleBoundaryVolume(
			m_pParam->makedDiameter * 0.5f,
			Float3(0.0f, m_pParam->makedHeight * -0.5f, 0.0f),
			Float3(0.0f, m_pParam->makedHeight * 0.5f, 0.0f),
			m_pParam->beforeWorldM);
		m_pParam->beforeFirstCalc = false;
	}
	return m_pParam->beforeCapsuleVolume;
}

SphereBoundaryVolume CollisionCapsule::GetWrappedSphereVol() const
{
	return GetCapsuleVolume().GetWrappedSphereBoundaryVolume();
}

SphereBoundaryVolume CollisionCapsule::GetEnclosingSphereVol() const
{
	CapsuleBoundaryVolume capVol = GetCapsuleVolume();
	CapsuleBoundaryVolume beforeCapVol = GetBeforeCapsuleVolume();
	SphereBoundaryVolume enclosingSpVol = BoundaryVolume::SphereEnclosingSphere(capVol.GetWrappedSphereBoundaryVolume(), beforeCapVol.GetWrappedSphereBoundaryVolume());
	return enclosingSpVol;
}

void CollisionCapsule::CollisionTest(const std::shared_ptr<CollisionSphere>& pDestSphere)
{
	// �����m�ɒu�������ďՓ˔�����s���A�Փ˂��Ă��Ȃ��Ȃ�I��
	if (!CollisionWrappedSphere(pDestSphere))
		return;

	auto pDestTransform = pDestSphere->GetGameObject()->GetTransform();
	Float3 srcVelocity = GetGameObject()->GetTransform()->GetVelocity();
	Float3 destVelocity = pDestTransform->GetVelocity();
	Float3 spanVelocity = destVelocity - srcVelocity;	// ���Α��x
	float elapsedTime = APP->GetElapsedTime();
	float hitTime = 0.0f;
	CapsuleBoundaryVolume srcBeforeCapsuleVol = GetBeforeCapsuleVolume();
	SphereBoundaryVolume destBeforeSphereVol = pDestSphere->GetBeforeSphereVolume();

	if (BoundaryVolume::CollisionTestSphereCapsule(destBeforeSphereVol, spanVelocity, srcBeforeCapsuleVol, 0.0f, elapsedTime, hitTime))
	{
		AfterCollisionTemplate_1<CollisionCapsule, CollisionSphere>
			(srcVelocity, destVelocity, elapsedTime, hitTime, GetShared<CollisionCapsule>(), pDestSphere);
	}
}

void CollisionCapsule::CollisionTest(const std::shared_ptr<CollisionOBB>& pDestOBB)
{
	// �����m�̏Փ˔���ɒu�������A�Փ˂��Ă��Ȃ��Ȃ�I��
	if (!CollisionWrappedSphere(pDestOBB))
		return;

	auto pDestTransform = pDestOBB->GetGameObject()->GetTransform();
	Float3 srcVelocity = GetGameObject()->GetTransform()->GetVelocity();
	Float3 destVelocity = pDestTransform->GetVelocity();
	Float3 spanVelocity = srcVelocity - destVelocity;
	float elapsedTime = APP->GetElapsedTime();
	float hitTime = 0.0f;
	CapsuleBoundaryVolume srcBeforeCapsuleVol = GetBeforeCapsuleVolume();
	OBBBoundaryVolume destBeforeCapsuelVol = pDestOBB->GetBeforeOBBVol();

	if (BoundaryVolume::CollisionTestCapsuleOBB(srcBeforeCapsuleVol, spanVelocity, destBeforeCapsuelVol, 0.0f, elapsedTime, hitTime))
	{
		AfterCollisionTemplate_1<CollisionCapsule, CollisionOBB>
			(srcVelocity, destVelocity, elapsedTime, hitTime, GetShared<CollisionCapsule>(), pDestOBB);
	}
}

void CollisionCapsule::CollisionTest(const std::shared_ptr<CollisionCapsule>& pDestCapsule)
{
	// �����m�ɒu�������ďՓ˔�����s���A�Փ˂��Ă��Ȃ��Ȃ�I��
	if (!CollisionWrappedSphere(pDestCapsule))
		return;

	auto pDestTransform = pDestCapsule->GetGameObject()->GetTransform();
	Float3 srcVelocity = GetGameObject()->GetTransform()->GetVelocity();
	Float3 destVelocity = pDestTransform->GetVelocity();
	Float3 spanVelocity = srcVelocity - destVelocity;
	float elapsedTime = APP->GetElapsedTime();
	float hitTime = 0.0f;
	CapsuleBoundaryVolume srcBeforeCapsuleVol = GetBeforeCapsuleVolume();
	CapsuleBoundaryVolume destBeforeCapsuleVol = pDestCapsule->GetBeforeCapsuleVolume();

	if (BoundaryVolume::CollisionTestCapsuleCapsule(srcBeforeCapsuleVol, spanVelocity, destBeforeCapsuleVol, 0.0f, elapsedTime, hitTime))
	{
		AfterCollisionTemplate_1<CollisionCapsule, CollisionCapsule>
			(srcVelocity, destVelocity, elapsedTime, hitTime, GetShared<CollisionCapsule>(), pDestCapsule);
	}
}

void CollisionCapsule::CollisionTest(const std::shared_ptr<CollisionRect>& pDestRect)
{
	auto pDestTransform = pDestRect->GetGameObject()->GetTransform();
	Float3 srcVelocity = GetGameObject()->GetTransform()->GetVelocity();
	Float3 destVelocity = pDestTransform->GetVelocity();
	Float3 spanVelocity = srcVelocity - destVelocity;
	float elapsedTime = APP->GetElapsedTime();
	float hitTime = 0.0f;
	CapsuleBoundaryVolume srcBeforeCapsuleVol = GetBeforeCapsuleVolume();
	RectBoundaryVolume destBeforeRectVol = pDestRect->GetBeforeRectVol();

	if (BoundaryVolume::CollisionTestCapsuleRect(srcBeforeCapsuleVol, spanVelocity, destBeforeRectVol, 0.0f, elapsedTime, hitTime))
	{
		AfterCollisionTemplate_2<CollisionCapsule, CollisionRect>
			(srcVelocity, destVelocity, elapsedTime, hitTime, GetShared<CollisionCapsule>(), pDestRect);
	}
}

void CollisionCapsule::CollisionEscape(const std::shared_ptr<CollisionSphere>& pDestSphere)
{
	CapsuleBoundaryVolume srcCapVol = GetCapsuleVolume();
	SphereBoundaryVolume destSphereVol = pDestSphere->GetSphereVolume();
	Float3 closestPos;

	// �Փ˂��Ă��Ȃ��Ȃ�I��
	if (!BoundaryVolume::SPHERE_CAPSULE(destSphereVol, srcCapVol, closestPos))
		return;

	// �������������ʂ����߂�
	Float3 span = destSphereVol.m_center - closestPos;
	//span.Normalize();
	span *= 0.02f;

	Float3 center = srcCapVol.GetCenter();
	int cnt = 0;
	// �Փ˂��Ȃ��Ȃ�ʒu��������܂ŏ��X�ɓ���
	while (true)
	{
		center += span;
		srcCapVol.SetCenter(center);

		// �Փ˂��Ȃ��Ȃ�����I��
		if (!BoundaryVolume::SPHERE_CAPSULE(destSphereVol, srcCapVol, closestPos))
			break;

		// 50�񃋁[�v��������E�Ƃ݂Ȃ��ďI��
		++cnt;
		if (cnt > 50)
			break;
	}

	// ���Z�b�g�ɂ���ăG�X�P�[�v
	GetGameObject()->GetTransform()->ResetWorldPos(center);
}

void CollisionCapsule::CollisionEscape(const std::shared_ptr<CollisionOBB>& pDestOBB)
{
	CapsuleBoundaryVolume srcCapVol = GetCapsuleVolume();
	OBBBoundaryVolume destOBBVol = pDestOBB->GetOBBVol();
	Float3 closestPos;

	// �Փ˂��Ă��Ȃ��Ȃ�I��
	if (!BoundaryVolume::CAPSULE_OBB(srcCapVol, destOBBVol, closestPos))
		return;

	float t;
	Float3 segmentPoint;
	// �J�v�Z����OBB�̍Őڋߓ_�ƃJ�v�Z�������̍Őڋߓ_���擾
	BoundaryVolume::ClosestPtPointSegment(closestPos, srcCapVol.m_bottomPos, srcCapVol.m_topPos, t, segmentPoint);
	Float3 span = segmentPoint - closestPos;	// �ړ_�ւ̃x�N�g��
	//span.Normalize();
	span *= 0.02f;

	Float3 center = srcCapVol.GetCenter();
	int cnt = 0;
	// ������Ȃ��Ȃ�ʒu��������܂ŏ��X�ɓ���
	while (true)
	{
		center += span;
		srcCapVol.SetCenter(center);

		// �Փ˂��Ȃ��Ȃ�����I��
		if (!BoundaryVolume::CAPSULE_OBB(srcCapVol, destOBBVol, closestPos))
			break;

		// 50�񃋁[�v��������E�Ƃ݂Ȃ��ďI��
		++cnt;
		if (cnt > 50)
			break;
	}

	// ���Z�b�g�ɂ���ăG�X�P�[�v
	GetGameObject()->GetTransform()->ResetWorldPos(center);
}

void CollisionCapsule::CollisionEscape(const std::shared_ptr<CollisionCapsule>& pDestCapsule)
{
	CapsuleBoundaryVolume srcCapVol = GetCapsuleVolume();
	CapsuleBoundaryVolume destCapVol = pDestCapsule->GetCapsuleVolume();

	Float3 srcClosestPos, destClosestPos;
	// �Փ˂��Ă��Ȃ���ΏI��
	if (!BoundaryVolume::CAPSULE_CAPSULE(srcCapVol, destCapVol, srcClosestPos, destClosestPos))
		return;

	float t;
	Float3 segmentPoint;
	// src�J�v�Z���̍Őڋߓ_��src�J�v�Z�������̍Őڋ߂��擾
	BoundaryVolume::ClosestPtPointSegment(srcClosestPos, srcCapVol.m_bottomPos, srcCapVol.m_topPos, t, segmentPoint);

	Float3 span = segmentPoint - srcClosestPos;
	span *= 0.02f;
	Float3 center = srcCapVol.GetCenter();
	int cnt = 0;
	// �Փ˂��Ȃ��Ȃ�܂ŏ��X�ɓ�����
	while (true)
	{
		center += span;
		srcCapVol.SetCenter(center);

		// �Փ˂��Ȃ��Ȃ�����I��
		if (!BoundaryVolume::CAPSULE_CAPSULE(srcCapVol, destCapVol, srcClosestPos, destClosestPos))
			break;

		// 50�񃋁[�v��������E�Ƃ݂Ȃ��I��
		++cnt;
		if (cnt > 50)
			break;
	}

	// ���Z�b�g�ɂ���ăG�X�P�[�v
	GetGameObject()->GetTransform()->ResetWorldPos(center);
}

void CollisionCapsule::CollisionEscape(const std::shared_ptr<CollisionRect>& pDestRect)
{
	CapsuleBoundaryVolume srcCapVol = GetCapsuleVolume();
	RectBoundaryVolume destRectVol = pDestRect->GetRectVol();
	Float3 closestPos;

	// �Փ˂��Ă��Ȃ��Ȃ�I��
	if (!BoundaryVolume::CAPSULE_RECT(srcCapVol, destRectVol, closestPos))
		return;

	PlaneBoundaryVolume destPlaneVol = destRectVol.GetPlaneVolume();
	Float3 span = destPlaneVol.m_normal;
	span.Normalize();
	span *= -0.02f;
	Float3 center = srcCapVol.GetCenter();
	int cnt = 0;
	while (true)
	{
		center += span;
		srcCapVol.SetCenter(center);

		// �Փ˂��Ȃ��Ȃ�����I��
		if (!BoundaryVolume::CAPSULE_RECT(srcCapVol, destRectVol, closestPos))
			break;

		// 50�񃋁[�v��������E�Ƃ݂Ȃ��I��
		++cnt;
		if (cnt > 50)
			break;
	}

	GetGameObject()->GetTransform()->ResetWorldPos(center);
}

void CollisionCapsule::GetHitNormal(const std::shared_ptr<CollisionSphere>& pDestSphere, Float3 & retNormal) const
{
	CapsuleBoundaryVolume srcCapVol = GetCapsuleVolume();
	SphereBoundaryVolume destSphereVol = pDestSphere->GetSphereVolume();
	Float3 closesetPos;
	BoundaryVolume::SPHERE_CAPSULE(destSphereVol, srcCapVol, closesetPos);
	retNormal = destSphereVol.m_center - closesetPos;
	retNormal.Normalize();
}

void CollisionCapsule::GetHitNormal(const std::shared_ptr<CollisionOBB>& pDestOBB, Float3 & retNormal) const
{
	CapsuleBoundaryVolume srcCapVol = GetCapsuleVolume();
	OBBBoundaryVolume destOBBVol = pDestOBB->GetOBBVol();
	// �J�v�Z����OBB�̍Őڋߓ_�����߂�
	Float3 closestPos;
	BoundaryVolume::CAPSULE_OBB(srcCapVol, destOBBVol, closestPos);
	// �J�v�Z������Őڋߓ_�ւ̃x�N�g�������߂�
	float t;
	Float3 segPoint;
	BoundaryVolume::ClosestPtPointSegment(closestPos, srcCapVol.m_bottomPos, srcCapVol.m_topPos, t, segPoint);
	retNormal = closestPos - segPoint;
	retNormal.Normalize();
}

void CollisionCapsule::GetHitNormal(const std::shared_ptr<CollisionCapsule>& pDestCapsule, Float3 & retNormal) const
{
	CapsuleBoundaryVolume srcCapVol = GetCapsuleVolume();
	CapsuleBoundaryVolume destCapVol = pDestCapsule->GetCapsuleVolume();
	Float3 closestPosA, closestPosB;
	BoundaryVolume::CAPSULE_CAPSULE(srcCapVol, destCapVol, closestPosA, closestPosB);
	// ����̃J�v�Z������
	Float3 start = destCapVol.m_bottomPos;
	Float3 end = destCapVol.m_topPos;
	// ����̃J�v�Z�������ƃJ�v�Z�����m�̍Őڋߓ_�Ƃ̍Őڋߓ_�����߂�
	float t;
	Float3 closestPos;
	BoundaryVolume::ClosestPtPointSegment(closestPosA, start, end, t, closestPos);
	retNormal = closestPos - closestPosA;
	retNormal.Normalize();
}

void CollisionCapsule::GetHitNormal(const std::shared_ptr<CollisionRect>& pDestRect, Float3 & retNormal) const
{
	RectBoundaryVolume destRectVol = pDestRect->GetRectVol();
	PlaneBoundaryVolume destPlane = destRectVol.GetPlaneVolume();
	retNormal = destPlane.m_normal;
	retNormal.Normalize();
}

void CollisionCapsule::BackToBefore(const Float3 & totalVelocity, float spanTime)
{
	CapsuleBoundaryVolume beforeCapVol = GetBeforeCapsuleVolume();
	Float3 pos = beforeCapVol.GetCenter() + totalVelocity * spanTime;
	GetGameObject()->GetTransform()->ResetWorldPos(pos);
}
