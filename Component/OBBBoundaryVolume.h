#pragma once
#include <XMFLOAT_EX.h>
#include <SphereBoundaryVolume.h>


class OBBBoundaryVolume
{
public:
	/// <summary>
	/// �f�t�H���g�R���X�g���N�^
	/// 1��1.0��OBB
	/// </summary>
	OBBBoundaryVolume();
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="size">�T�C�Y</param>
	/// <param name="rot">��]�x�N�g��</param>
	/// <param name="center">���S</param>
	OBBBoundaryVolume(const Float3& size, const Float3& rot, Float3& center);
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="size">�T�C�Y</param>
	/// <param name="quat">��]�N�H�[�^�j�I��</param>
	/// <param name="center">���S</param>
	OBBBoundaryVolume(const Float3& size, const Quaternion& quat, const Float3& center);
	/// <summary>
	/// �R���X�g���N�^
	/// �쐬���ꂽ�T�C�Y�ƕϊ��s�񂩂�OBB���쐬
	/// </summary>
	/// <param name="size">�T�C�Y</param>
	/// <param name="matrix">�ϊ��s��</param>
	OBBBoundaryVolume(const Float3& size, const Float4x4 matrix);

	/// <summary>
	/// OBB�{�����[�����ݍ���SphereVolume���擾
	/// </summary>
	/// <returns></returns>
	SphereBoundaryVolume GetWrappedSphereVolume();

	/// <summary>
	/// �T�C�Y�ƕϊ��s�񂩂�OBB�����
	/// </summary>
	/// <param name="size">�T�C�Y</param>
	/// <param name="matrix">�ϊ��s��</param>
	void Create(const Float3& size, const Float4x4& matrix);

	/// <summary>
	/// ��]�s��𓾂�
	/// </summary>
	/// <returns></returns>
	Float4x4 GetRotMatrix()const;

	/// <summary>
	/// ���[���h�s��𓾂�
	/// </summary>
	/// <returns></returns>
	Float4x4 GetWorldMatrix()const;

	/// <summary>
	/// ��ԑ傫���ӂ̒����𓾂�
	/// </summary>
	/// <returns></returns>
	float GetMaxSize()const;
	/// <summary>
	/// ��ԑ傫���ӂ̔����̒����𓾂�
	/// </summary>
	/// <returns></returns>
	float GetHalfMaxSize()const;
	/// <summary>
	/// ��ԏ������ӂ̒����𓾂�
	/// </summary>
	/// <returns></returns>
	float GetMinSize()const;
	/// <summary>
	/// ��ԏ������ӂ̔����̒����𓾂�
	/// </summary>
	/// <returns></returns>
	float GetHalfMinSize()const;


public:
	Float3 m_center;	// ���S
	Float3 m_rot[3];	// �e���W���̌X����\�������x�N�g��
	Float3 m_size;	// �e���W���ɉ����������̔���(���S�_����ʂ܂ł̒���
};