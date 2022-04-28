#include "Ray.h"


Ray::Ray(const Float3 & startPos, const Float3 & direction)
	: m_startPos(startPos)
	, m_direction(direction.normalized())
{
}

bool Ray::IsHitSphere(const Float3 & rayStartPos, const Float3 & rayDirection, const SphereBoundaryVolume & sp, float & refDistance, Float3 & refHitPos)
{
	Ray ray(rayStartPos, rayDirection);
	return ray.IsHitSphere(sp, refDistance, refHitPos);
}

bool Ray::IsHitSphere(const SphereBoundaryVolume & sp, float & refDistance, Float3 & refHitPos)
{
	Float3 SpRay = m_startPos - sp.m_center;
	float dot = Float3::Dot(SpRay, m_direction);
	float d = Float3::Dot(SpRay, SpRay) - sp.m_radius * sp.m_radius;

	if (d > 0.0f && dot > 0.0f)
	{
		return false;
	}
	float discr = dot * dot - d;
	if (discr < 0.0f)
	{
		return false;
	}
	Float3 vecDiscr(discr);
	vecDiscr = DirectX::XMVectorSqrt(vecDiscr);
	refDistance = -dot - vecDiscr.x;
	if (refDistance < 0.0f)
		refDistance = 0.0f;
	refHitPos = m_startPos + m_direction * refDistance;
	return true;
}

#ifdef _DEBUG
void Ray::Draw()
{
}
#endif // DEBUG
