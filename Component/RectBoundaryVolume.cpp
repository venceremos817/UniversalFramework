#include "RectBoundaryVolume.h"

RectBoundaryVolume::RectBoundaryVolume(float baseXSize, float baseYSize, const Float4x4 & matrix)
	: m_baseXSize(baseXSize)
	, m_baseYSize(baseYSize)
	, m_matrix(matrix)
{
	// TODO : 簡略化
	m_center = matrix.ExtractTransform();
	Float3 vecX(matrix._11, matrix._12, matrix._13);
	Float3 vecY(matrix._21, matrix._22, matrix._23);
	m_UVec[0] = baseXSize * vecX.Length() * 0.5f;
	m_UVec[1] = baseYSize * vecY.Length() * 0.5f;
	// 回転を得る
	m_rot[0] = vecX.Normalize();
	m_rot[1] = vecY.Normalize();
	// 頂点の設定
	float halfX = m_baseXSize * 0.5f;
	float halfY = m_baseYSize * 0.5f;
	m_vertex[0] = Float3(-halfX, halfY, 0.0f);
	m_vertex[1] = Float3(halfX, halfY, 0.0f);
	m_vertex[2] = Float3(-halfX, -halfY, 0.0f);
	m_vertex[3] = Float3(halfX, -halfY, 0.0f);
	// マトリクスを適用
	for (auto& v : m_vertex)
		v *= m_matrix;
}

PlaneBoundaryVolume RectBoundaryVolume::GetPlaneVolume() const
{
	// 表面上の3つの点を使ってPlaneを作成
	Float3 point0 = m_center;

	float makedHalfX = m_baseXSize * 0.5f;
	float makedHalfY = m_baseYSize * 0.5f;
	Float3 point1(-makedHalfX, -makedHalfY, 0.0f);
	point1 = m_matrix * point1;

	Float3 point2(makedHalfX, -makedHalfY, 0.0f);
	point2 = m_matrix * point2;

	PlaneBoundaryVolume ret(point0, point1, point2);
	return ret;
}

SphereBoundaryVolume RectBoundaryVolume::GetWrappedSphereVolume() const
{
	SphereBoundaryVolume sphereVol;
	float size = Float3(m_vertex[1] - m_vertex[2]).Length();
	size *= 0.5f;
	sphereVol.m_radius = size;
	sphereVol.m_center = m_center;
	return sphereVol;
}
