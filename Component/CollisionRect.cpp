#include "CollisionRect.h"
#include <Transform.h>
#include <GameObject/GameObject.h>

struct CollisionRect::Param
{
	float size;		// çÏê¨éûÇÃÉTÉCÉY

	Param() :size(1.0f) {}
};



CollisionRect::CollisionRect(const std::shared_ptr<GameObject>& pGameObject)
	: Collision(pGameObject)
	, m_pParam(new Param())
{
}

CollisionRect::~CollisionRect()
{
}

void CollisionRect::CollisionCall(const std::shared_ptr<Collision>& pSrcCol)
{
	pSrcCol->CollisionTest(GetShared<CollisionRect>());
}

RectBoundaryVolume CollisionRect::GetRectVol() const
{
	RectBoundaryVolume rect(GetMakedSize(), GetMakedSize(), GetGameObject()->GetTransform()->GetWorldMatrix());
	return rect;
}

RectBoundaryVolume CollisionRect::GetBeforeRectVol() const
{
	RectBoundaryVolume rect(GetMakedSize(), GetMakedSize(), GetGameObject()->GetTransform()->GetPreWolrdMatrix());
	return rect;
}

float CollisionRect::GetMakedSize() const
{
	return m_pParam->size;
}

SphereBoundaryVolume CollisionRect::GetWrappedSphereVol() const
{
	return GetRectVol().GetWrappedSphereVolume();
}

SphereBoundaryVolume CollisionRect::GetEnclosingSphereVol() const
{
	RectBoundaryVolume curRectVol = GetRectVol();
	RectBoundaryVolume beforeRectVol = GetBeforeRectVol();

	SphereBoundaryVolume encloseSphereVol = BoundaryVolume::SphereEnclosingSphere(curRectVol.GetWrappedSphereVolume(), beforeRectVol.GetWrappedSphereVolume());
	return encloseSphereVol;
}
