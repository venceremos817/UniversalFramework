#pragma once


#include <GameObject.h>
//#include <PlayerCamera.h>
//#include <Transform.h>
#include <RigidBody.h>
#include <Player/PlayerStateMachine.h>
#include <DrawComponent/DrawComponent.h>
#include <EffekseerComponent.h>

class Player
	: public GameObject
{
public:
	Player(const std::shared_ptr<Stage>& pStage);
	virtual ~Player();

	void OnCreate()override;
	void Update()override;
	void LateUpdate()override;
	void OnDestroy()override;

	virtual void Destroy()override;

	/// <summary>
	/// �X�e�[�g��ύX
	/// </summary>
	/// <param name="pState"></param>
	void ChangeState(const std::shared_ptr<ObjectState<Player>>& pState);

	/// <summary>
	/// ��g���K�[�R���W�����C�x���g(�ՓˊJ�n��
	/// </summary>
	/// <param name="destObjVec"></param>
	virtual void OnCollisionEnter(std::vector<std::shared_ptr<GameObject>>& destObjVec)override;

	/// <summary>
	/// �G�C���A�V�X�g�͂�ݒ�
	/// </summary>
	/// <param name="assist"></param>
	void SetAimAssist(float assist);
	/// <summary>
	/// �G�C���A�C�V�X�Ɨ͂��擾
	/// </summary>
	/// <returns></returns>
	float GetAimAssist()const;

	std::shared_ptr<EffekseerComponent>& GetStepWaveEffect()const;

private:
	/// <summary>
	/// �n�ʂƏՓˎ�
	/// </summary>
	void OnHitGround();

private:
	struct Param;
	std::unique_ptr<Param> m_param;
};

