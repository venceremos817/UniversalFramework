#include "PlaneBoundaryVolume.h"

PlaneBoundaryVolume::PlaneBoundaryVolume(const Float3 & pointA, const Float3 & pointB, const Float3 & pointC)
{
	Set(pointA, pointB, pointC);
}

void PlaneBoundaryVolume::Set(const Float3 & pointA, const Float3 & pointB, const Float3 & pointC)
{
	m_normal = Float3::Cross(pointB - pointA, pointC - pointA);
	m_normal.Normalize();
	m_dotValue = Float3::Dot(m_normal, pointA);
}
