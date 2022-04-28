#pragma once
#include <XMFLOAT_EX.h>


class SphereBoundaryVolume
{
public:
	SphereBoundaryVolume();
	SphereBoundaryVolume(const Float3& center, float radius);


public:
	Float3 m_center;		// ’†SÀ•W
	float m_radius;			// ”¼Œa
};
