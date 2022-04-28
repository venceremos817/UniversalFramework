#pragma once


#include <Component.h>
#include <XMFLOAT_EX.h>

class GameObject;

/// <summary>
/// �r�w�C�r�A�x�[�X�N���X
/// </summary>
class Behavior
	:public Component
{
public:
	explicit Behavior(const std::shared_ptr<GameObject>& pGameObject) : Component(pGameObject) {}
	virtual ~Behavior() {}

protected:
};




/// <summary>
/// �d�͂�������r�w�C�r�A	(Collision�ł����p����̂ŁA�d�͂����������ꍇ�͊�{�I�ɂ��̃r�w�C�r�A���g��
/// </summary>
class GravityBehavior
	:public Behavior
{
public:
	explicit GravityBehavior(const std::shared_ptr<GameObject>& pGameObject);
	virtual ~GravityBehavior();


	/// <summary>
	/// ���݂̏d�͂��擾
	/// </summary>
	/// <returns></returns>
	const Float3& GetGravity()const;

	/// <summary>
	/// ���݂̏d�͂�ݒ肷��
	/// </summary>
	/// <param name="gravity"></param>
	void SetGravity(const Float3& gravity);
	void SetGravity(float x, float y, float z);

	/// <summary>
	/// �w�肵�����ԕ��̏d�͂�������
	/// </summary>
	/// <param name="time"></param>
	void UpdateFromTime(float time);

	/// <summary>
	/// �W�����v�̊J�n
	/// </summary>
	/// <param name="startVelocity">�����x</param>
	/// <param name="escapeSpan">�߂荞�ݕ␳</param>
	void Jump(const Float3& startVelocity, float escapeSpan = 0.01f);

	/// <summary>
	/// �s�������s
	/// </summary>
	void Execute();


private:
	struct Param;
	std::unique_ptr<Param> m_param;
};


/// <summary>
/// ���ʃr�w�C�r�A
/// </summary>
class CommonBehavior
	: public Behavior
{
public:
	explicit CommonBehavior(const std::shared_ptr<GameObject>& pGameObject) : Behavior(pGameObject) {}
	virtual ~CommonBehavior() {}

	/// <summary>
	/// �w�肵������(�x�N�g��)�ɕ�Ԃɂ���Č���
	/// </summary>
	/// <param name="vector"></param>
	/// <param name="lerpFactor">��ԌW��</param>
	void RotToVecotr(const Float3& vector, float lerpFactor);
};
