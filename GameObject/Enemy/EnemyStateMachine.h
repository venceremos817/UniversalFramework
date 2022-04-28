#pragma once
#include <memory>
#include <StateMachine.h>
#include <Behavior/Behavior.h>

class Enemy;

//=======================================================
//	�X�e�[�g�}�V��
//=======================================================
/// <summary>
/// �G�l�~�[�ʏ펞�X�e�[�g
/// </summary>
class EnemyDefaultState
	: public ObjectState<Enemy>
{
private:
	EnemyDefaultState() {}
public:
	virtual ~EnemyDefaultState() {}

	/// <summary>
	/// �V���O���g���A�N�Z�T
	/// </summary>
	/// <returns></returns>
	static std::shared_ptr<EnemyDefaultState> GetInstance();


	/// <summary>
	/// �J�n���̏���
	/// </summary>
	/// <param name="pOwner"></param>
	/// <param name="prevState"></param>
	virtual void OnEnter(const std::shared_ptr<Enemy>& pOwner, const std::shared_ptr<ObjectState<Enemy>>& prevState = nullptr) override;
	/// <summary>
	/// �X�V���̏���
	/// </summary>
	/// <param name="pOwner"></param>
	virtual void OnUpdate(const std::shared_ptr<Enemy>& pOwner) override;
	/// <summary>
	/// �I�����̏���
	/// </summary>
	/// <param name="pOwner"></param>
	/// <param name="nextState"></param>
	virtual void OnExit(const std::shared_ptr<Enemy>& pOwner, const std::shared_ptr<ObjectState<Enemy>>& nextState = nullptr) override;

private:

};

/// <summary>
/// �G�l�~�[�p�j�X�e�[�g
/// </summary>
class EnemyWanderState
	: public ObjectState<Enemy>
{
private:
	EnemyWanderState() {}
public:
	virtual ~EnemyWanderState() {}

	/// <summary>
	/// �V���O���g���A�N�Z�T
	/// </summary>
	/// <returns></returns>
	static std::shared_ptr<EnemyWanderState> GetInstance();


	/// <summary>
	/// �J�n���̏���
	/// </summary>
	/// <param name="pOwner"></param>
	/// <param name="prevState"></param>
	virtual void OnEnter(const std::shared_ptr<Enemy>& pOwner, const std::shared_ptr<ObjectState<Enemy>>& prevState = nullptr) override;
	/// <summary>
	/// �X�V���̏���
	/// </summary>
	/// <param name="pOwner"></param>
	virtual void OnUpdate(const std::shared_ptr<Enemy>& pOwner) override;
	/// <summary>
	/// �I�����̏���
	/// </summary>
	/// <param name="pOwner"></param>
	/// <param name="nextState"></param>
	virtual void OnExit(const std::shared_ptr<Enemy>& pOwner, const std::shared_ptr<ObjectState<Enemy>>& nextState = nullptr) override;
};

/// <summary>
/// �G�l�~�[�팂�j���X�e�[�g
/// </summary>
class EnemyDestroyState
	: public ObjectState<Enemy>
{
private:
	EnemyDestroyState() {};
public:
	virtual ~EnemyDestroyState() {};

	static std::shared_ptr<EnemyDestroyState> GetInstance();

	virtual void OnEnter(const std::shared_ptr<Enemy>& pOwner, const std::shared_ptr<ObjectState<Enemy>>& prevState = nullptr) override;

	virtual void OnUpdate(const std::shared_ptr<Enemy>& pOwner) override;

	virtual void OnExit(const std::shared_ptr<Enemy>& pOwner, const std::shared_ptr<ObjectState<Enemy>>& nextState = nullptr) override;

};

/// <summary>
/// �p�j�r�w�C�r�A
/// </summary>
class WanderBehavior
	: public Behavior
{
public:
	WanderBehavior(const std::shared_ptr<GameObject>& pGameObject);
	virtual ~WanderBehavior();

	/// <summary>
	/// �p�j���S�_��ݒ�
	/// </summary>
	/// <param name="center"></param>
	void SetWanderCenter(const Float3& center);
	/// <summary>
	/// �p�j���a��ݒ�
	/// </summary>
	/// <param name="radius"></param>
	void SetWanderRadius(const float radius);
	/// <summary>
	/// �p�j�~�܂ł̋�����ݒ�
	/// </summary>
	/// <param name="distance"></param>
	void SetWanderDistance(const float distance);
	/// <summary>
	/// �����_���ψق̍ő�l
	/// </summary>
	/// <param name="jitter"></param>
	void SetWanderJitter(const float jitter);

	/// <summary>
	/// �ڕW�n�_��ݒ�
	/// </summary>
	/// <param name="targetPos"></param>
	void SetRandomTargetPos();

	/// <summary>
	/// ���s
	/// </summary>
	void Execute();

private:
	Float3 Wander(const Float4x4& worldMat, const float wanderRadius, const float wanderDistance, const float wanderJitter, Float3& wanderTarget);
	bool AccumulateForce(Float3& force, const Float3& forceToAdd, const float maxForce);
private:
	struct Param;
	std::unique_ptr<Param> m_pParam;
};

/// <summary>
/// �G�l�~�[�팂�j�r�w�C�r�A
/// </summary>
class EnemyDefeatedBehavior
	: public Behavior
{
public:
	EnemyDefeatedBehavior(const std::shared_ptr<GameObject>& pGameObject);
	virtual ~EnemyDefeatedBehavior();

	void StartDefeat(const std::shared_ptr<Enemy>& pOwner);
	void Update(const std::shared_ptr<Enemy>& pOwner);
	
private:
};