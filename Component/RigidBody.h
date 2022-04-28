#pragma once

#include <Component.h>
#include <XMFLOAT_EX.h>



class RigidBody
	: public Component
{
public:
	explicit RigidBody(const std::shared_ptr<GameObject>& pGameObject);
	virtual ~RigidBody();



	/// <summary>
	/// �X�V����
	/// </summary>
	void Update()override;




	/// <summary>
	/// ���x���擾
	/// </summary>
	/// <returns></returns>
	const Float3& GetVelocity()const;
	/// <summary>
	/// ���x��ݒ�
	/// </summary>
	/// <param name="velocity"></param>
	const Float3& SetVelocity(const Float3& velocity);
	const Float3& SetVelocity(float x, float y, float z);
	/// <summary>
	/// ���x��0�ɐݒ�
	/// </summary>
	void SetVelocityZero();
	
	/// <summary>
	/// �d�͑��x���擾
	/// </summary>
	/// <returns></returns>
	const Float3& GetGravityVelocity()const;
	/// <summary>
	/// �d�͑��x��ݒ�
	/// </summary>
	/// <param name="velocity"></param>
	void SetGravityVelocity(const Float3& velocity);
	void SetGravityVelocity(float x, float y, float z);
	/// <summary>
	/// �d�͑��x��0�ɐݒ�
	/// </summary>
	void SetGravityVelocityZero();

	/// <summary>
	/// �ō����x���擾
	/// </summary>
	/// <returns></returns>
	float GetMaxSpeed()const;
	/// <summary>
	/// �ō����x��ݒ�
	/// </summary>
	/// <param name="speed"></param>
	void SetMaxSpeed(float speed);
	/// <summary>
	/// �Œᑬ�x���擾
	/// </summary>
	const Float3& GetMinVelocity()const;
	/// <summary>
	/// �Œᑬ�x��ݒ�
	/// </summary>
	/// <param name="velocity"></param>
	void SetMinVelocity(const Float3& velocity);
	void SetMinVelocity(float x, float y, float z);

	/// <summary>
	/// ��]���x���擾
	/// </summary>
	/// <returns></returns>
	const Float3& GetAngularVelocity()const;
	/// <summary>
	/// ��]���x��ݒ�
	/// </summary>
	/// <param name="angularVelocity"></param>
	void SetAngularVelocity(const Float3& angularVelocity);
	void SetAngularVelocity(float x, float y, float z);

	/// <summary>
	/// ���ʂ��擾
	/// </summary>
	/// <returns></returns>
	float GetMass()const;
	/// <summary>
	/// ���ʂ�ݒ�
	/// </summary>
	/// <param name="mass"></param>
	void SetMass(float mass);

	/// <summary>
	/// �����W���𓾂�
	/// </summary>
	/// <returns></returns>
	float GetReflection()const;
	/// <summary>
	/// �����W����ݒ�
	/// </summary>
	/// <param name="reflection"></param>
	void SetReflection(float reflection);

	/// <summary>
	/// �t�H�[�X���擾
	/// </summary>
	/// <returns></returns>
	const Float3& GetForce()const;
	/// <summary>
	/// �t�H�[�X��ݒ�
	/// </summary>
	/// <param name="force"></param>
	void SetForce(const Float3& force);
	void SetForce(float x, float y, float z);

	/// <summary>
	/// �����t�H�[�X���擾
	/// </summary>
	/// <returns></returns>
	float GetFrictionForce()const;
	/// <summary>
	/// �����t�H�[�X��ݒ�
	/// </summary>
	/// <param name="frictionF"></param>
	void SetFrictionForce(float frictionF);

	/// <summary>
	/// �ő�t�H�[�X���擾
	/// </summary>
	/// <returns></returns>
	float GetMaxForce()const;
	/// <summary>
	/// �ő�t�H�[�X��ݒ�
	/// </summary>
	/// <param name="f"></param>
	void SetMaxForce(float f);

	/// <summary>
	/// �w�肵�����ԕ����x�ɂ��������Ĉړ�����
	/// </summary>
	/// <param name="calcTime"></param>
	void UpdateFromTime(float calcTime);



public:
	struct Param;
	std::unique_ptr<Param> m_pParam;
};


