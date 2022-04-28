#include "CollisionOBB.h"
#include <CollisionSphere.h>
#include <CollisionCapsule.h>
#include <CollisionRect.h>
#include <App/App.h>
#include <Transform.h>




struct CollisionOBB::Param
{
	float makedSize;					// 作成時のサイズ
	float chkOnUndarLaySize;
	Float4x4 beforeWorldM;
	Float4x4 worldM;
	OBBBoundaryVolume beforeOBBVol;
	OBBBoundaryVolume OBBVol;
	bool beforeFirstCalc;		// 再計算する必要があるか
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
	// ワールドマトリクスが変更されている、またはほかの理由で再計算が必要なら再計算
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
	// 前のワールドマトリクスが変更されている、または他の理由で再計算が必要なら再計算
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
		return;		// まずは球で衝突判定を行い、当たっていないなら終了

	auto pDestTransform = pDestSphere->GetGameObject()->GetTransform();
	Float3 srcVelocity = GetGameObject()->GetTransform()->GetVelocity();
	Float3 destVelocity = pDestTransform->GetVelocity();
	Float3 spanVelocity = destVelocity - srcVelocity;	// 相対速度
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
		return;		// 球で判定を行いヒットしてないなら終了

	auto pDestTransform = pDestOBB->GetGameObject()->GetTransform();
	Float3 srcVelocity = GetGameObject()->GetTransform()->GetVelocity();
	Float3 destVelocity = pDestTransform->GetVelocity();
	Float3 spanVelocity = srcVelocity - destVelocity;	// 相対速度
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
		return; // 球で判定を行いヒットしていないなら終了

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

	// ヒットしているか確認
	if (BoundaryVolume::Sphere_OBB(destSphereVol, srcOBBVol, closestPos))
	{
		Float3 nowSpan = closestPos - destSphereVol.m_center;
		Float3 newSpan = nowSpan;
		newSpan.Normalize();
		newSpan *= destSphereVol.m_radius;
		Float3 moveSpan = newSpan - nowSpan;
		Float3 pos = GetGameObject()->GetTransform()->GetWorldMatrix().ExtractTransform();
		pos += moveSpan;
		// リセットによってエスケープ
		GetGameObject()->GetTransform()->ResetWorldPos(pos);
	}
}

void CollisionOBB::CollisionEscape(const std::shared_ptr<CollisionOBB>& pDestOBB)
{
	OBBBoundaryVolume srcOBBVol = GetOBBVol();
	OBBBoundaryVolume destOBBVol = pDestOBB->GetOBBVol();

	// ヒットしてなければ終了
	if (!BoundaryVolume::OBB_OBB(srcOBBVol, destOBBVol))
		return;
	Float3 closestPos;
	// 2つのOBBの最接近点を取得
	BoundaryVolume::ClosestPtPointOBB(srcOBBVol.m_center, destOBBVol, closestPos);
	Float3 span = srcOBBVol.m_center - closestPos;
	span.Normalize();
	span *= 0.02f;
	Float3 center = srcOBBVol.m_center;

	// 少しずつsrcOBBをずらしていって当たらなくなるまでループ
	int cnt = 0;
	while (true)
	{
		center += span;
		srcOBBVol.m_center = center;
		// ヒットしなくなったら終了
		if (!BoundaryVolume::OBB_OBB(srcOBBVol, destOBBVol))
			break;

		// 50回ループしたら限界とみなし終了
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

	// ヒットしているか確認
	if (!BoundaryVolume::CAPSULE_OBB(destCapsuleVol, srcOBBVol, closestPos))
		return;

	float t;	// 接ベクトル
	Float3 segPoint;
	// 最接近点とカプセル線分の最接近点を求める
	BoundaryVolume::ClosestPtPointSegment(closestPos, destCapsuleVol.m_bottomPos, destCapsuleVol.m_topPos, t, segPoint);
	Float3 span = closestPos - segPoint;	// 逃げるベクトル
	span.Normalize();
	span *= 0.02f;	// 短くしておく

	int cnt = 0;
	// 衝突しなくなるまで徐々に動かし、エスケープ先を決める
	while (true)
	{
		// 少しずつずらす
		center += span;
		srcOBBVol.m_center = center;
		// ずらした結果ヒットしなくなったらループ終了
		if (!BoundaryVolume::CAPSULE_OBB(destCapsuleVol, srcOBBVol, closestPos))
			break;

		++cnt;
		// 50回ループしたら限界とみなし終了
		if (cnt > 50)
			break;
	}
	// リセットによってエスケープ
	GetGameObject()->GetTransform()->ResetWorldPos(center);
}

void CollisionOBB::CollisionEscape(const std::shared_ptr<CollisionRect>& pDestRect)
{
	OBBBoundaryVolume srcOBBVol = GetOBBVol();
	RectBoundaryVolume destRectVol = pDestRect->GetRectVol();

	// ヒットしていなければ終了
	if (!BoundaryVolume::OBB_RECT(srcOBBVol, destRectVol))
		return;

	//--- 徐々に動かすベクトルを求める
	Float3 span = pDestRect->GetRectVol().GetPlaneVolume().m_normal;	// 矩形の法線方向
	span.Normalize();
	span *= -0.02f;	// 逆に向けて短くする
	Float3 center = srcOBBVol.m_center;

	int cnt = 0;
	// 衝突しなくなるまで徐々に動かし、エスケープ先を決める
	while (true)
	{
		center += span;
		srcOBBVol.m_center = center;

		// ヒットしなくなったら終了
		if (!BoundaryVolume::OBB_RECT(srcOBBVol, destRectVol))
			break;

		++cnt;
		// 50回ループしたら限界とみなし終了
		if (cnt > 50)
			break;
	}
	// リセットによってエスケープ
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
	OBBBoundaryVolume beforeOBBVol = GetBeforeOBBVol();	// 前のOBB
	// 前のOBBからspanTime分移動
	Float3 pos = beforeOBBVol.m_center + totalVelocity * spanTime;
	GetGameObject()->GetTransform()->ResetWorldPos(pos);
}
