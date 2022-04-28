#pragma once
#include <XMFLOAT_EX.h>
#include <PlaneBoundaryVolume.h>
#include <SphereBoundaryVolume.h>

class RectBoundaryVolume
{
public:
	RectBoundaryVolume(float baseXSize, float baseYSize, const Float4x4& matrix);

	/// <summary>
	/// Plane���E�{�����[�����擾
	/// </summary>
	/// <returns></returns>
	PlaneBoundaryVolume GetPlaneVolume()const;

	/// <summary>
	/// ��ݍ���Sphere����{�����[�����擾
	/// </summary>
	/// <returns></returns>
	SphereBoundaryVolume GetWrappedSphereVolume()const;

public:
	Float3	m_center;		// ���S�_
	Float3	m_rot[2];		// XY�̊e���W�̌X����\�������x�N�g��
	float	m_UVec[2];		// XY���W���ɉ����������̔���(���S�_����ʂ܂ł̒���)
	float	m_baseXSize;	// �쐬���̃T�C�YX
	float	m_baseYSize;	// �쐬���̃T�C�YY
	Float4x4	m_matrix;
	Float3	m_vertex[4];	// �s��ɂ���ĕϊ����ꂽ���_���W
};