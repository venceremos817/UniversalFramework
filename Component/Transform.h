#pragma once

#include <Component.h>
#include <Utility/XMFLOAT_EX.h>

class Transform
	: public Component
{
public:
	explicit Transform(const std::shared_ptr<GameObject>& pGameObject);
	~Transform();


	/// <summary>
	/// �X�V����
	/// </summary>
	void Update()override;


	/// <summary>
	/// �O�̃X�P�[�����擾
	/// </summary>
	/// <returns></returns>
	Float3 GetPreScale()const;
	/// <summary>
	/// �X�P�[�����擾
	/// </summary>
	Float3 GetScale()const;
	/// <summary>
	/// �X�P�[����ݒ�
	/// </summary>
	void SetScale(const Float3& scale);
	void SetScale(float x, float y, float z);

	/// <summary>
	/// �O�̏d�S���擾
	/// </summary>
	/// <returns></returns>
	Float3 GetPrePivod()const;
	/// <summary>
	/// �d�S���擾
	/// </summary>
	Float3 GetPivod()const;
	/// <summary>
	/// �d�S��ݒ�
	/// </summary>
	void SetPivod(const Float3& pivot);
	void SetPivot(float x, float y, float z);

	/// <summary>
	/// �O�̉�]���擾(�N�H�[�^�j�I��)
	/// </summary>
	/// <returns></returns>
	Quaternion GetPreQuat()const;
	/// <summary>
	/// ��]���擾(�N�H�[�^�j�I��)
	/// </summary>
	Quaternion GetQuaternion()const;
	/// <summary>
	/// ��]��ݒ�
	/// </summary>
	void SetQuaternion(const Quaternion& quat);
	/// <summary>
	/// �O�̉�]���擾(�x�N�g��)
	/// </summary>
	/// <returns></returns>
	Float3 GetPreRotation()const;
	/// <summary>
	/// ��]���擾(�x�N�g��)
	/// </summary>
	Float3 GetRotation()const;
	/// <summary>
	/// ��]��ݒ肷��(�x�N�g��)
	/// </summary>
	void SetRotation(const Float3& rot);
	void SetRotation(float x, float y, float z);

	/// <summary>
	/// �O�̍��W���擾
	/// </summary>
	/// <returns></returns>
	Float3 GetPrePos()const;
	/// <summary>
	/// ���W���擾
	/// </summary>
	Float3 GetPos()const;
	/// <summary>
	/// ���W��ݒ�
	/// </summary>
	/// <param name="pos"></param>
	void SetPos(const Float3& pos);
	void SetPos(float x, float y, float z);
	/// <summary>
	/// ���W�ƑO�̍��W��ݒ�
	/// </summary>
	void ResetPos(const Float3& pos);

	/// <summary>
	/// ���[���h���W���擾
	/// </summary>
	Float3 GetWorldPos();
	/// <summary>
	/// ���[���h���W��ݒ�
	/// </summary>
	void SetWorldPos(const Float3& pos);
	void SetWorldPos(float x, float y, float z);
	/// <summary>
	/// ���[���h���W�ƑO�̃��[���h���W��ݒ�
	/// </summary>
	void ResetWorldPos(const Float3& pos);

	/// <summary>
	/// �O�̃��[���h�s����擾
	/// </summary>
	/// <returns></returns>
	Float4x4 GetPreWolrdMatrix();
	/// <summary>
	/// ���[���h�}�g���N�X���擾
	/// </summary>
	const Float4x4& GetWorldMatrix();

	const Float4x4& Get2DWorldMatrix();


	/// <summary>
	/// Transform��Ōv�Z�������x���擾
	/// </summary>
	/// <returns></returns>
	Float3 GetVelocity()const;

	/// <summary>
	/// �O�̃^�[���̏����X�V
	/// </summary>
	void SetToBefore();

	/// <summary>
	/// �w�肵������������
	/// </summary>
	/// <param name="lookPos"></param>
	void LookAt(const Float3& lookPos);

	/// <summary>
	/// �O���������擾
	/// </summary>
	/// <returns></returns>
	Float3 GetForward();
	/// <summary>
	/// ��������擾
	/// </summary>
	/// <returns></returns>
	Float3 GetUp();
	/// <summary>
	/// �E�������擾
	/// </summary>
	/// <returns></returns>
	Float3 GetRight();

	/// <summary>
	/// ���݂̃��[���h�}�g���N�X�Ɠ��l��
	/// </summary>
	/// <param name="mat"></param>
	/// <returns></returns>
	bool IsSameWorldMatrix(const Float4x4& mat);
	/// <summary>
	/// 1�O�̃��[���h�s��Ɠ��l��
	/// </summary>
	/// <param name="mat"></param>
	/// <returns></returns>
	bool IsSameBeforeWorldMatrix(const Float4x4& mat);




public:
	struct Param;
	std::unique_ptr<Param> m_param;
};