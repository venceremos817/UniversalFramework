#include "CapsuleBoundaryVolume.h"

CapsuleBoundaryVolume::CapsuleBoundaryVolume()
	: m_radius(0.5f)
	, m_bottomPos(0.0f, -0.5f, 0.0f)
	, m_topPos(0.0f, 0.5f, 0.0f)
{
}

CapsuleBoundaryVolume::CapsuleBoundaryVolume(float radius, Float3 & bottomPos, Float3 & topPos)
	: m_radius(radius)
	, m_bottomPos(bottomPos)
	, m_topPos(topPos)
{
}

CapsuleBoundaryVolume::CapsuleBoundaryVolume(float makedRadius, const Float3 & makedBottomPos, const Float3 & makedTopPos, const Float4x4 & matrix)
	: m_radius(makedRadius)
	, m_bottomPos(makedBottomPos)
	, m_topPos(makedTopPos)
{
	m_bottomPos *= matrix;
	m_topPos *= matrix;
	// xスケーリングを適用
	Float3 scale = matrix.ExtractScale();
	m_radius *= scale.x;
}

SphereBoundaryVolume CapsuleBoundaryVolume::GetWrappedSphereBoundaryVolume() const
{
	SphereBoundaryVolume wrappedSphere;
	float size = Float3(m_topPos - m_bottomPos).Length();
	size *= 0.5f;
	size += m_radius;
	wrappedSphere.m_radius = size;
	wrappedSphere.m_center = GetCenter();
	return wrappedSphere;
}

Float3 CapsuleBoundaryVolume::GetCenter() const
{
	return m_bottomPos + ((m_topPos - m_bottomPos) * 0.5f);
}

void CapsuleBoundaryVolume::SetCenter(const Float3 & center)
{
	Float3 centerToPosA = (m_bottomPos - m_topPos) * 0.5f;
	Float3 centerToPosB = (m_topPos - m_bottomPos) * 0.5f;
	m_bottomPos = center + centerToPosA;
	m_topPos = center + centerToPosB;
}
