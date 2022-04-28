#pragma once
#include <XMFLOAT_EX.h>


class PlaneBoundaryVolume
{
public:
	PlaneBoundaryVolume() {}
	/// <summary>
	/// �R���X�g���N�^
	/// 3�_���܂ޕ��ʂ��쐬
	/// </summary>
	/// <param name="pointA"></param>
	/// <param name="pointB"></param>
	/// <param name="pointC"></param>
	PlaneBoundaryVolume(const Float3& pointA, const Float3& pointB, const Float3& pointC);

	~PlaneBoundaryVolume() {}

	void Set(const Float3& pointA, const Float3& pointB, const Float3& pointC);

public:
	Float3	m_normal;	// �@��
	float	m_dotValue;	// ���ϒl
};
