#include "CollisionOBB.h"
#include <CollisionSphere.h>
#include <CollisionCapsule.h>
#include <CollisionRect.h>
#include <App/App.h>
#include <Transform.h>




struct CollisionOBB::Param
{
	float makedSize;					// �쐬���̃T�C�Y
	float chkOnUndarLaySize;
	Float4x4 beforeWorldM;
	Float4x4 worldM;
	OBBBoundaryVolume beforeOBBVol;
	OBBBoundaryVolume OBBVol;
	bool beforeFirstCalc;		// �Čv�Z����K�v�����邩
	bool firstCalc;

	Param()
		: makedSize(1.0f)
		, chkOnUndarLaySize(0.1f)
		, beforeWorldM()
		, worldM()
		, beforeOBBVol()
		, OBBVol()
		, beforeFirstCalc(true)
		, firstCalc(true)
	{}
};


CollisionOBB::CollisionOBB(const std::shared_ptr<GameObject>& pGameObject)
	: Collision(pGameObject)
	, m_pParam(new Param())
{
}

CollisionOBB::~CollisionOBB()
{
}

void CollisionOBB::CollisionCall(const std::shared_ptr<Collision>& pSrcCol)
{
	pSrcCol->CollisionTest(GetShared<CollisionOBB>());
}

float CollisionOBB::GetMakedSize() const
{
	return m_pParam->makedSize;
}

void CollisionOBB::SetMakedSize(float len) const
{
	m_pParam->makedSize = len;
}

OBBBoundaryVolume CollisionOBB::GetOBBVol() const
{
	// ���[���h�}�g���N�X���ύX����Ă���A�܂��͂ق��̗��R�ōČv�Z���K�v�Ȃ�Čv�Z
	if (m_pParam->firstCalc || !GetGameObject()->GetTransform()->IsSameWorldMatrix(m_pParam->worldM))
	{
		m_pParam->worldM = GetGameObject()->GetTransform()->GetWorldMatrix();
		Float4x4 matBase;
		matBase.Scale(Float3(m_pParam->makedSize, m_pParam->makedSize, m_pParam->makedSize));
		matBase *= m_pParam->worldM;
		m_pParam->OBBVol = OBBBoundaryVolume(Float3(m_pParam->makedSize, m_pParam->makedSize, m_pParam->makedSize), matBase);
		m_pParam->firstCalc = false;
	}

	return m_pParam->OBBVol;
}

OBBBoundaryVolume CollisionOBB::GetBeforeOBBVol() const
{
	// �O�̃��[���h�}�g���N�X���ύX����Ă���A�܂��͑��̗��R�ōČv�Z���K�v�Ȃ�Čv�Z
	if (m_pParam->beforeFirstCalc || !GetGameObject()->GetTransform()->IsSameBeforeWorldMatrix(m_pParam->beforeWorldM))
	{
		m_pParam->beforeWorldM = GetGameObject()->GetTransform()->GetPreWolrdMatrix();
		Float4x4 matBase;
		matBase.Scale(Float3(m_pParam->makedSize, m_pParam->makedSize, m_pParam->makedSize));
		matBase *= m_pParam->beforeWorldM;
		m_pParam->beforeOBBVol = OBBBoundaryVolume(Float3(m_pParam->makedSize, m_pParam->makedSize, m_pParam->makedSize), matBase);
		m_pParam->beforeFirstCalc = false;
	}

	return m_pParam->beforeOBBVol;
}

SphereBoundaryVolume CollisionOBB::GetWrappedSphereVol() const
{
	return GetOBBVol().GetWrappedSphereVolume();
}

SphereBoundaryVolume CollisionOBB::GetEnclosingSphereVol() const
{
	OBBBoundaryVolume curOBBVol = GetOBBVol();
	OBBBoundaryVolume beforeOBBVol = GetBeforeOBBVol();

	SphereBoundaryVolume encloseSphereVol = BoundaryVolume::SphereEnclosingSphere(curOBBVol.GetWrappedSphereVolume(), beforeOBBVol.GetWrappedSphereVolume());
	return encloseSphereVol;
}

void CollisionOBB::CollisionTest(const std::shared_ptr<CollisionSphere>& pDestSphere)
{
	if (!CollisionWrappedSphere(pDestSphere))
		return;		// �܂��͋��ŏՓ˔�����s���A�������Ă��Ȃ��Ȃ�I��

	auto pDestTransform = pDestSphere->GetGameObject()->GetTransform();
	Float3 srcVelocity = GetGameObject()->GetTransform()->GetVelocity();
	Float3 destVelocity = pDestTransform->GetVelocity();
	Float3 spanVelocity = destVelocity - srcVelocity;	// ���Α��x
	float elapsedTime = APP->GetElapsedTime();
	float hitTime = 0.0f;
	OBBBoundaryVolume srcBeforeOBB = GetBeforeOBBVol();
	SphereBoundaryVolume destBeforeSphereVol = pDestSphere->GetBeforeSphereVolume();

	if (BoundaryVolume::CollisionTestSphereOBB(destBeforeSphereVol, spanVelocity, srcBeforeOBB, 0.0f, elapsedTime, hitTime))
	{
		AfterCollisionTemplate_1<CollisionOBB, CollisionSphere>
			(srcVelocity, destVelocity, elapsedTime, hitTime, GetShared<CollisionOBB>(), pDestSphere);
	}
}

void CollisionOBB::CollisionTest(const std::shared_ptr<CollisionOBB>& pDestOBB)
{
	if (!CollisionWrappedSphere(pDestOBB))
		return;		// ���Ŕ�����s���q�b�g���ĂȂ��Ȃ�I��

	auto pDestTransform = pDestOBB->GetGameObject()->GetTransform();
	Float3 srcVelocity = GetGameObject()->GetTransform()->GetVelocity();
	Float3 destVelocity = pDestTransform->GetVelocity();
	Float3 spanVelocity = srcVelocity - destVelocity;	// ���Α��x
	float elaplsedTime = APP->GetElapsedTime();
	float hitTime = 0.0f;
	OBBBoundaryVolume srcBeforeOBB = GetBeforeOBBVol();
	OBBBoundaryVolume destBeforeOBB = pDestOBB->GetBeforeOBBVol();

	if (BoundaryVolume::CollisionTestOBBOBB(srcBeforeOBB, spanVelocity, destBeforeOBB, 0.0f, elaplsedTime, hitTime))
	{
		AfterCollisionTemplate_1<CollisionOBB, CollisionOBB>
			(srcVelocity, destVelocity, elaplsedTime, hitTime, GetShared<CollisionOBB>(), pDestOBB);
	}
}

void CollisionOBB::CollisionTest(const std::shared_ptr<CollisionCapsule>& pDestCapsule)
{
	if (!CollisionWrappedSphere(pDestCapsule))
		return; // ���Ŕ�����s���q�b�g���Ă��Ȃ��Ȃ�I��

	auto pDestTransform = pDestCapsule->GetGameObject()->GetTransform();
	Float3 srcVelocity = GetGameObject()->GetTransform()->GetVelocity();
	Float3 destVelocity = pDestTransform->GetVelocity();
	Float3 spanVelocity = destVelocity - srcVelocity;
	float elapsedTime = APP->GetElapsedTime();
	float hitTime = 0.0f;
	OBBBoundaryVolume srcBeforeOBBVol = GetBeforeOBBVol();
	CapsuleBoundaryVolume destBeforeCapVol = pDestCapsule->GetBeforeCapsuleVolume();

	if (BoundaryVolume::CollisionTestCapsuleOBB(destBeforeCapVol, spanVelocity, srcBeforeOBBVol, 0.0f, elapsedTime, hitTime))
	{
		AfterCollisionTemplate_1<CollisionOBB, CollisionCapsule>
			(srcVelocity, destVelocity, elapsedTime, hitTime, GetShared<CollisionOBB>(), pDestCapsule);
	}
}

void CollisionOBB::CollisionTest(const std::shared_ptr<CollisionRect>& pDestRect)
{
	auto pDestTransform = pDestRect->GetGameObject()->GetTransform();
	Float3 srcVelocity = GetGameObject()->GetTransform()->GetVelocity();
	Float3 destVelocity = pDestTransform->GetVelocity();
	Float3 spanVelocity = srcVelocity - destVelocity;
	float elapsedTime = APP->GetElapsedTime();
	float hitTime = 0.0f;
	OBBBoundaryVolume srcBeforeOBBVol = GetBeforeOBBVol();
	RectBoundaryVolume destBeforeRectVol = pDestRect->GetBeforeRectVol();
	if (BoundaryVolume::CollisionTestOBBRect(srcBeforeOBBVol, spanVelocity, destBeforeRectVol, 0.0f, elapsedTime, hitTime))
	{
		AfterCollisionTemplate_2<CollisionOBB, CollisionRect>
			(srcVelocity, destVelocity, elapsedTime, hitTime, GetShared<CollisionOBB>(), pDestRect);
	}
}

void CollisionOBB::CollisionEscape(const std::shared_ptr<CollisionSphere>& pDestSphere)
{
	OBBBoundaryVolume srcOBBVol = GetOBBVol();
	SphereBoundaryVolume destSphereVol = pDestSphere->GetSphereVolume();
	Float3 closestPos;

	// �q�b�g���Ă��邩�m�F
	if (BoundaryVolume::Sphere_OBB(destSphereVol, srcOBBVol, closestPos))
	{
		Float3 nowSpan = closestPos - destSphereVol.m_center;
		Float3 newSpan = nowSpan;
		newSpan.Normalize();
		newSpan *= destSphereVol.m_radius;
		Float3 moveSpan = newSpan - nowSpan;
		Float3 pos = GetGameObject()->GetTransform()->GetWorldMatrix().ExtractTransform();
		pos += moveSpan;
		// ���Z�b�g�ɂ���ăG�X�P�[�v
		GetGameObject()->GetTransform()->ResetWorldPos(pos);
	}
}

void CollisionOBB::CollisionEscape(const std::shared_ptr<CollisionOBB>& pDestOBB)
{
	OBBBoundaryVolume srcOBBVol = GetOBBVol();
	OBBBoundaryVolume destOBBVol = pDestOBB->GetOBBVol();

	// �q�b�g���ĂȂ���ΏI��
	if (!BoundaryVolume::OBB_OBB(srcOBBVol, destOBBVol))
		return;
	Float3 closestPos;
	// 2��OBB�̍Őڋߓ_���擾
	BoundaryVolume::ClosestPtPointOBB(srcOBBVol.m_center, destOBBVol, closestPos);
	Float3 span = srcOBBVol.m_center - closestPos;
	span.Normalize();
	span *= 0.02f;
	Float3 center = srcOBBVol.m_center;

	// ��������srcOBB�����炵�Ă����ē�����Ȃ��Ȃ�܂Ń��[�v
	int cnt = 0;
	while (true)
	{
		center += span;
		srcOBBVol.m_center = center;
		// �q�b�g���Ȃ��Ȃ�����I��
		if (!BoundaryVolume::OBB_OBB(srcOBBVol, destOBBVol))
			break;

		// 50�񃋁[�v��������E�Ƃ݂Ȃ��I��
		++cnt;
		if (cnt > 50)
			break;
	}
}

void CollisionOBB::CollisionEscape(const std::shared_ptr<CollisionCapsule>& pDestCapsule)
{
	OBBBoundaryVolume srcOBBVol = GetOBBVol();
	Float3 center = srcOBBVol.m_center;
	CapsuleBoundaryVolume destCapsuleVol = pDestCapsule->GetCapsuleVolume();
	Float3 closestPos;

	// �q�b�g���Ă��邩�m�F
	if (!BoundaryVolume::CAPSULE_OBB(destCapsuleVol, srcOBBVol, closestPos))
		return;

	float t;	// �ڃx�N�g��
	Float3 segPoint;
	// �Őڋߓ_�ƃJ�v�Z�������̍Őڋߓ_�����߂�
	BoundaryVolume::ClosestPtPointSegment(closestPos, destCapsuleVol.m_bottomPos, destCapsuleVol.m_topPos, t, segPoint);
	Float3 span = closestPos - segPoint;	// ������x�N�g��
	span.Normalize();
	span *= 0.02f;	// �Z�����Ă���

	int cnt = 0;
	// �Փ˂��Ȃ��Ȃ�܂ŏ��X�ɓ������A�G�X�P�[�v������߂�
	while (true)
	{
		// ���������炷
		center += span;
		srcOBBVol.m_center = center;
		// ���炵�����ʃq�b�g���Ȃ��Ȃ����烋�[�v�I��
		if (!BoundaryVolume::CAPSULE_OBB(destCapsuleVol, srcOBBVol, closestPos))
			break;

		++cnt;
		// 50�񃋁[�v��������E�Ƃ݂Ȃ��I��
		if (cnt > 50)
			break;
	}
	// ���Z�b�g�ɂ���ăG�X�P�[�v
	GetGameObject()->GetTransform()->ResetWorldPos(center);
}

void CollisionOBB::CollisionEscape(const std::shared_ptr<CollisionRect>& pDestRect)
{
	OBBBoundaryVolume srcOBBVol = GetOBBVol();
	RectBoundaryVolume destRectVol = pDestRect->GetRectVol();

	// �q�b�g���Ă��Ȃ���ΏI��
	if (!BoundaryVolume::OBB_RECT(srcOBBVol, destRectVol))
		return;

	//--- ���X�ɓ������x�N�g�������߂�
	Float3 span = pDestRect->GetRectVol().GetPlaneVolume().m_normal;	// ��`�̖@������
	span.Normalize();
	span *= -0.02f;	// �t�Ɍ����ĒZ������
	Float3 center = srcOBBVol.m_center;

	int cnt = 0;
	// �Փ˂��Ȃ��Ȃ�܂ŏ��X�ɓ������A�G�X�P�[�v������߂�
	while (true)
	{
		center += span;
		srcOBBVol.m_center = center;

		// �q�b�g���Ȃ��Ȃ�����I��
		if (!BoundaryVolume::OBB_RECT(srcOBBVol, destRectVol))
			break;

		++cnt;
		// 50�񃋁[�v��������E�Ƃ݂Ȃ��I��
		if (cnt > 50)
			break;
	}
	// ���Z�b�g�ɂ���ăG�X�P�[�v
	GetGameObject()->GetTransform()->ResetWorldPos(center);
}

void CollisionOBB::GetHitNormal(const std::shared_ptr<CollisionSphere>& pDestSphere, Float3 & retNormal) const
{
	OBBBoundaryVolume srcOBBVol = GetOBBVol();
	SphereBoundaryVolume destSphereVol = pDestSphere->GetSphereVolume();
	Float3 closestPos;
	BoundaryVolume::Sphere_OBB(destSphereVol, srcOBBVol, closestPos);
	retNormal = destSphereVol.m_center - closestPos;
	retNormal.Normalize();
}

void CollisionOBB::GetHitNormal(const std::shared_ptr<CollisionOBB>& pDestOBB, Float3 & retNormal) const
{
	OBBBoundaryVolume srcOBBVol = GetOBBVol();
	OBBBoundaryVolume destOBBVol = pDestOBB->GetOBBVol();
	Float3 closestPos;
	BoundaryVolume::ClosestPtPointOBB(srcOBBVol.m_center, destOBBVol, closestPos);
	retNormal = closestPos - srcOBBVol.m_center;
	retNormal.Normalize();
}

void CollisionOBB::GetHitNormal(const std::shared_ptr<CollisionCapsule>& pDestCapsule, Float3 & retNormal) const
{
	OBBBoundaryVolume srcOBBVol = GetOBBVol();
	CapsuleBoundaryVolume destCapVol = pDestCapsule->GetCapsuleVolume();
	Float3 closestPos;
	BoundaryVolume::CAPSULE_OBB(destCapVol, srcOBBVol, closestPos);
	retNormal = destCapVol.GetCenter() - closestPos;
	retNormal.Normalize();
}

void CollisionOBB::GetHitNormal(const std::shared_ptr<CollisionRect>& pDestRect, Float3 & retNormal) const
{
	retNormal = pDestRect->GetRectVol().GetPlaneVolume().m_normal;
	retNormal.Normalize();
}

void CollisionOBB::BackToBefore(const Float3 & totalVelocity, float spanTime)
{
	OBBBoundaryVolume beforeOBBVol = GetBeforeOBBVol();	// �O��OBB
	// �O��OBB����spanTime���ړ�
	Float3 pos = beforeOBBVol.m_center + totalVelocity * spanTime;
	GetGameObject()->GetTransform()->ResetWorldPos(pos);
}
