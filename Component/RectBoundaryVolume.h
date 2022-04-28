#pragma once
#include <XMFLOAT_EX.h>
#include <PlaneBoundaryVolume.h>
#include <SphereBoundaryVolume.h>

class RectBoundaryVolume
{
public:
	RectBoundaryVolume(float baseXSize, float baseYSize, const Float4x4& matrix);

	/// <summary>
	/// Plane境界ボリュームを取得
	/// </summary>
	/// <returns></returns>
	PlaneBoundaryVolume GetPlaneVolume()const;

	/// <summary>
	/// 包み込むSphere協会ボリュームを取得
	/// </summary>
	/// <returns></returns>
	SphereBoundaryVolume GetWrappedSphereVolume()const;

public:
	Float3	m_center;		// 中心点
	Float3	m_rot[2];		// XYの各座標の傾きを表す方向ベクトル
	float	m_UVec[2];		// XY座標軸に沿った長さの半分(中心点から面までの長さ)
	float	m_baseXSize;	// 作成時のサイズX
	float	m_baseYSize;	// 作成時のサイズY
	Float4x4	m_matrix;
	Float3	m_vertex[4];	// 行列によって変換された頂点座標
};