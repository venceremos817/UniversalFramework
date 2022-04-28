#pragma once
#include <XMFLOAT_EX.h>


class PlaneBoundaryVolume
{
public:
	PlaneBoundaryVolume() {}
	/// <summary>
	/// コンストラクタ
	/// 3点を含む平面を作成
	/// </summary>
	/// <param name="pointA"></param>
	/// <param name="pointB"></param>
	/// <param name="pointC"></param>
	PlaneBoundaryVolume(const Float3& pointA, const Float3& pointB, const Float3& pointC);

	~PlaneBoundaryVolume() {}

	void Set(const Float3& pointA, const Float3& pointB, const Float3& pointC);

public:
	Float3	m_normal;	// 法線
	float	m_dotValue;	// 内積値
};
