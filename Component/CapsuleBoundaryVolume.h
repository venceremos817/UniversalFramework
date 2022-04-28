#pragma once
#include <XMFLOAT_EX.h>
#include <SphereBoundaryVolume.h>

class CapsuleBoundaryVolume
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// ���a:0.5 bottom:(0,-0.5,0) top(0,0.5,0)
	/// </summary>
	CapsuleBoundaryVolume();
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="radius">���a</param>
	/// <param name="bottomPos">���ԕ������J�n�_</param>
	/// <param name="topPos">���ԕ������I���_</param>
	CapsuleBoundaryVolume(float radius, Float3& bottomPos, Float3& topPos);
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="makedRadius">�쐬���̔��a</param>
	/// <param name="makedBottomPos">�쐬���̒��ԕ������J�n�_</param>
	/// <param name="makedTopPos">�쐬���̒��ԕ������I���_</param>
	/// <param name="matrix">�ϊ��s��</param>
	CapsuleBoundaryVolume(float makedRadius, const Float3& makedBottomPos, const Float3& makedTopPos, const Float4x4& matrix);

	/// <summary>
	/// ��ݍ���Sphere���E�{�����[�����擾
	/// </summary>
	/// <returns></returns>
	SphereBoundaryVolume GetWrappedSphereBoundaryVolume()const;
	

	/// <summary>
	/// ���S�_���擾
	/// </summary>
	/// <returns></returns>
	Float3 GetCenter()const;
	/// <summary>
	/// ���S�_��ݒ�
	/// </summary>
	/// <param name="center"></param>
	void SetCenter(const Float3& center);

public:
	float m_radius;			// ���a
	Float3 m_bottomPos;		// ���ԕ��������J�n�_
	Float3 m_topPos;		// ���ԕ��������I���_
};