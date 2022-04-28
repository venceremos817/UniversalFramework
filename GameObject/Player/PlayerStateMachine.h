#pragma once
#include <memory>
#include <StateMachine.h>
#include <Behavior/Behavior.h>
#include <InputDevice.h>

class Player;
//================================================================
//	�X�e�[�g�}�V��
//================================================================

/// <summary>
/// �v���C���[�ʏ�X�e�[�g
/// </summary>
class PlayerDefaultState
	:public ObjectState<Player>
{
	PlayerDefaultState() {}

public:
	~PlayerDefaultState();

	/// <summary>
	/// �V���O���g���A�N�Z�T
	/// </summary>
	/// <returns></returns>
	static std::shared_ptr<PlayerDefaultState> GetInstance();

	/// <summary>
	/// �J�n���̏���
	/// </summary>
	/// <param name="pOwner"></param>
	/// <param name="prevState"></param>
	void OnEnter(const std::shared_ptr<Player>& pOwner, const std::shared_ptr<ObjectState<Player>>& prevState = nullptr) override;
	/// <summary>
	/// �X�V���̏���
	/// </summary>
	/// <param name="pOwner"></param>
	void OnUpdate(const std::shared_ptr<Player>& pOwner) override;
	/// <summary>
	/// �I�����̏���
	/// </summary>
	/// <param name="pOwner"></param>
	/// <param name="nextState"></param>
	void OnExit(const std::shared_ptr<Player>& pOwner, const std::shared_ptr<ObjectState<Player>>& nextState = nullptr) override;

private:
};

/// <summary>
/// �v���C���[�W�����v�X�e�[�g
/// </summary>
class PlayerJumpState
	: public ObjectState<Player>
{
	PlayerJumpState() {}

public:
	~PlayerJumpState() {}

	/// <summary>
	/// �V���O���g���A�N�Z�T
	/// </summary>
	/// <returns></returns>
	static std::shared_ptr<PlayerJumpState> GetInstance();

	/// <summary>
	/// �X�e�[�g�J�n���̏���
	/// </summary>
	/// <param name="pOwner"></param>
	/// <param name="prevState"></param>
	void OnEnter(const std::shared_ptr<Player>& pOwner, const std::shared_ptr<ObjectState<Player>>& prevState = nullptr) override;
	/// <summary>
	/// �X�V���̏���
	/// </summary>
	/// <param name="pOwner"></param>
	void OnUpdate(const std::shared_ptr<Player>& pOwner) override;
	/// <summary>
	/// �I�����̏���
	/// </summary>
	/// <param name="pOwner"></param>
	/// <param name="nextState"></param>
	void OnExit(const std::shared_ptr<Player>& pOwner, const std::shared_ptr<ObjectState<Player>>& nextState = nullptr) override;
};


class PlayerStepState
	: public ObjectState<Player>
{
private:
	PlayerStepState();
public:
	~PlayerStepState();

	/// <summary>
	/// �V���O���g���A�N�Z�T
	/// </summary>
	/// <returns></returns>
	static std::shared_ptr<PlayerStepState> GetInstance();

	/// <summary>
	/// �X�e�[�g�J�n���̏���
	/// </summary>
	/// <param name="pPlayer"></param>
	/// <param name="prevState"></param>
	void OnEnter(const std::shared_ptr<Player>& pOwner, const std::shared_ptr<ObjectState<Player>>& prevState = nullptr)override;
	/// <summary>
	/// �X�e�[�g�X�V����
	/// </summary>
	/// <param name="pOwner"></param>
	void OnUpdate(const std::shared_ptr<Player>& pOwner) override;
	/// <summary>
	/// �X�e�[�g�I������
	/// </summary>
	/// <param name="pOwner"></param>
	/// <param name="nextState"></param>
	void OnExit(const std::shared_ptr<Player>& pOwner, const std::shared_ptr<ObjectState<Player>>& nextState = nullptr) override;

private:
	/// <summary>
	/// �J�����ƃQ�[���p�b�h����i�s�������Z�o
	/// </summary>
	/// <returns></returns>
	const Float3 GetStepDirectionByGamePad(const std::shared_ptr<Player>& player, const GamePadState& gamePad)const;
	/// <summary>
	/// �J�����ƃL�[���͂���i�s�������Z�o
	/// </summary>
	/// <returns></returns>
	const Float3 GetStepDirectionByKeybord(const std::shared_ptr<Player>& player)const;
};


//================================================================
// �r�w�C�r�A
//================================================================
/// <summary>
/// �v���C���[�̒ʏ펞(�ړ����Ȃ�)�r�w�C�r�A
/// </summary>
class PlayerDefaultBehavior
	:public Behavior
{
public:
	explicit PlayerDefaultBehavior(const std::shared_ptr<GameObject>& pGameObject);
	virtual ~PlayerDefaultBehavior() {}

	/// <summary>
	/// �ړ������s
	/// </summary>
	void Move();


private:
	/// <summary>
	/// �J�����ƃQ�[���p�b�h����i�s�������Z�o
	/// </summary>
	/// <returns></returns>
	const Float3 GetMoveVectorByGamePad(const GamePadState& gamePad)const;
	/// <summary>
	/// �J�����ƃL�[���͂���i�s�������Z�o
	/// </summary>
	/// <returns></returns>
	const Float3 GetMoveVectorByKeybord()const;

private:
	float m_maxSpeed;
};

/// <summary>
/// �ʏ�ˌ��r�w�C�r�A
/// �v���C���[�̌����Ă�������ɒʏ�e���΂�
/// </summary>
class PlayerShotBehavior
	:public Behavior
{
public:
	explicit PlayerShotBehavior(const std::shared_ptr<GameObject>& pGameObject) :Behavior(pGameObject) {}
	virtual ~PlayerShotBehavior() {}

	/// <summary>
	/// �ˌ����s
	/// </summary>
	void Shot();
};

/// <summary>
/// �A�ˎˌ��r�w�C�r�A
/// </summary>
class PlayerRapidFireBehavior
	: public Behavior
{
public:
	explicit PlayerRapidFireBehavior(const std::shared_ptr<GameObject>& pGameObject);
	virtual ~PlayerRapidFireBehavior() {}

	/// <summary>
	/// �ˌ�
	/// </summary>
	void Shot(std::shared_ptr<Player> player);

private:
	float m_rate;		// �ˌ����[�g(1�b������̎ˌ���)
	float m_spanTime;
};

class PlayerHomingShotBehavior
	: public Behavior
{
public:
	explicit PlayerHomingShotBehavior(const std::shared_ptr<GameObject>& pGameObject);
	virtual ~PlayerHomingShotBehavior() {}

	/// <summary>
	/// ����
	/// </summary>
	void Shot(std::shared_ptr<Player> player);

private:
	float m_rate;
	float m_spanTime;
};

/// <summary>
/// �v���C���[�X�e�b�v�r�w�C�r�A
/// </summary>
class PlayerStepBehavior
	: public Behavior
{
public:
	explicit PlayerStepBehavior(const std::shared_ptr<GameObject>& pGameObject);
	virtual ~PlayerStepBehavior();



	/// <summary>
	/// �p�����[�^������
	/// </summary>
	/// <param name="velocity">�X�e�b�v���鑬�x</param>
	/// <param name="stepMoveTime">�X�e�b�v�ňړ����鎞��</param>
	/// <param name="rigidityTime">�d������</param>
	void Reset(const Float3& velocity, float stepMoveTime, float rigidityTime);
	/// <summary>
	/// ���s
	/// </summary>
	/// <returns>�X�e�b�v���Ԃ��I��������false</returns>
	bool Execute();

private:
	struct Param;
	std::unique_ptr<Param> m_param;
};