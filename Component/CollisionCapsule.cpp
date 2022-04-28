#include "CollisionCapsule.h"
#include <CollisionSphere.h>
#include <CollisionOBB.h>
#include <CollisionRect.h>
#include <App.h>
#include <Transform.h>


struct CollisionCapsule::Param
{
	float makedDiameter;	// 作成時の直径
	float makedHeight;		// 作成時の高さ
	size_t hitVolumeIndex;	// 配列ボリュームと衝突時に衝突した配列を特定するインデックス
	Float4x4 worldM;		// 現在のワールドマトリクス
	Float4x4 beforeWorldM;	// 前のワールドマトリクス
	CapsuleBoundaryVolume capsuleVolume;	// 現在のカプセルボリューム
	CapsuleBoundaryVolume beforeCapsuleVolume;	// 前のカプセルボリューム
	bool firstCalc;			// 現在のカプセルを計算する必要があるか
	bool beforeFirstCalc;	// 前のカプセルを計算する必要があるか

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
	// ワールドマトリクスが変更されてる、または他の理由があればカプセル再計算
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
	// 1つ前のワールドマトリクスが変更されてる、または他の理由があればカプセル再計算
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
	// 球同士に置き換えて衝突判定を行い、衝突していないなら終了
	if (!CollisionWrappedSphere(pDestSphere))
		return;

	auto pDestTransform = pDestSphere->GetGameObject()->GetTransform();
	Float3 srcVelocity = GetGameObject()->GetTransform()->GetVelocity();
	Float3 destVelocity = pDestTransform->GetVelocity();
	Float3 spanVelocity = destVelocity - srcVelocity;	// 相対速度
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
	// 球同士の衝突判定に置き換え、衝突していないなら終了
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
	// 球同士に置き換えて衝突判定を行い、衝突していないなら終了
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

	// 衝突していないなら終了
	if (!BoundaryVolume::SPHERE_CAPSULE(destSphereVol, srcCapVol, closestPos))
		return;

	// 少しずつ動かす量を求める
	Float3 span = destSphereVol.m_center - closestPos;
	//span.Normalize();
	span *= 0.02f;

	Float3 center = srcCapVol.GetCenter();
	int cnt = 0;
	// 衝突しなくなる位置が見つかるまで徐々に動く
	while (true)
	{
		center += span;
		srcCapVol.SetCenter(center);

		// 衝突しなくなったら終了
		if (!BoundaryVolume::SPHERE_CAPSULE(destSphereVol, srcCapVol, closestPos))
			break;

		// 50回ループしたら限界とみなして終了
		++cnt;
		if (cnt > 50)
			break;
	}

	// リセットによってエスケープ
	GetGameObject()->GetTransform()->ResetWorldPos(center);
}

void CollisionCapsule::CollisionEscape(const std::shared_ptr<CollisionOBB>& pDestOBB)
{
	CapsuleBoundaryVolume srcCapVol = GetCapsuleVolume();
	OBBBoundaryVolume destOBBVol = pDestOBB->GetOBBVol();
	Float3 closestPos;

	// 衝突していないなら終了
	if (!BoundaryVolume::CAPSULE_OBB(srcCapVol, destOBBVol, closestPos))
		return;

	float t;
	Float3 segmentPoint;
	// カプセルとOBBの最接近点とカプセル線分の最接近点を取得
	BoundaryVolume::ClosestPtPointSegment(closestPos, srcCapVol.m_bottomPos, srcCapVol.m_topPos, t, segmentPoint);
	Float3 span = segmentPoint - closestPos;	// 接点へのベクトル
	//span.Normalize();
	span *= 0.02f;

	Float3 center = srcCapVol.GetCenter();
	int cnt = 0;
	// 当たらなくなる位置が見つかるまで徐々に動く
	while (true)
	{
		center += span;
		srcCapVol.SetCenter(center);

		// 衝突しなくなったら終了
		if (!BoundaryVolume::CAPSULE_OBB(srcCapVol, destOBBVol, closestPos))
			break;

		// 50回ループしたら限界とみなして終了
		++cnt;
		if (cnt > 50)
			break;
	}

	// リセットによってエスケープ
	GetGameObject()->GetTransform()->ResetWorldPos(center);
}

void CollisionCapsule::CollisionEscape(const std::shared_ptr<CollisionCapsule>& pDestCapsule)
{
	CapsuleBoundaryVolume srcCapVol = GetCapsuleVolume();
	CapsuleBoundaryVolume destCapVol = pDestCapsule->GetCapsuleVolume();

	Float3 srcClosestPos, destClosestPos;
	// 衝突していなければ終了
	if (!BoundaryVolume::CAPSULE_CAPSULE(srcCapVol, destCapVol, srcClosestPos, destClosestPos))
		return;

	float t;
	Float3 segmentPoint;
	// srcカプセルの最接近点とsrcカプセル線分の最接近を取得
	BoundaryVolume::ClosestPtPointSegment(srcClosestPos, srcCapVol.m_bottomPos, srcCapVol.m_topPos, t, segmentPoint);

	Float3 span = segmentPoint - srcClosestPos;
	span *= 0.02f;
	Float3 center = srcCapVol.GetCenter();
	int cnt = 0;
	// 衝突しなくなるまで徐々に動かす
	while (true)
	{
		center += span;
		srcCapVol.SetCenter(center);

		// 衝突しなくなったら終了
		if (!BoundaryVolume::CAPSULE_CAPSULE(srcCapVol, destCapVol, srcClosestPos, destClosestPos))
			break;

		// 50回ループしたら限界とみなし終了
		++cnt;
		if (cnt > 50)
			break;
	}

	// リセットによってエスケープ
	GetGameObject()->GetTransform()->ResetWorldPos(center);
}

void CollisionCapsule::CollisionEscape(const std::shared_ptr<CollisionRect>& pDestRect)
{
	CapsuleBoundaryVolume srcCapVol = GetCapsuleVolume();
	RectBoundaryVolume destRectVol = pDestRect->GetRectVol();
	Float3 closestPos;

	// 衝突していないなら終了
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

		// 衝突しなくなったら終了
		if (!BoundaryVolume::CAPSULE_RECT(srcCapVol, destRectVol, closestPos))
			break;

		// 50回ループしたら限界とみなし終了
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
	// カプセルとOBBの最接近点を求める
	Float3 closestPos;
	BoundaryVolume::CAPSULE_OBB(srcCapVol, destOBBVol, closestPos);
	// カプセルから最接近点へのベクトルを求める
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
	// 相手のカプセル線分
	Float3 start = destCapVol.m_bottomPos;
	Float3 end = destCapVol.m_topPos;
	// 相手のカプセル線分とカプセル同士の最接近点との最接近点を求める
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
