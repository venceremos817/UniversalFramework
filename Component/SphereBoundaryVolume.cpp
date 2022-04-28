#include "SphereBoundaryVolume.h"

SphereBoundaryVolume::SphereBoundaryVolume()
	: m_center{0.0f,0.0f,0.0f}
	, m_radius(0.5f)
{
}

SphereBoundaryVolume::SphereBoundaryVolume(const Float3 & center, float radius)
	: m_center(center)
	, m_radius(radius)
{
}
