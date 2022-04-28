#pragma once


template <class T>
class ObjectState
{
public:
	ObjectState() {}
	virtual ~ObjectState() {}

	/// <summary>
	/// �X�e�[�g�ɓ������Ƃ��̏���
	/// </summary>
	/// <param name="pOwner"></param>
	virtual void OnEnter(const std::shared_ptr<T>& pOwner, const std::shared_ptr<ObjectState<T>>& prevState = nullptr) = 0;
	/// <summary>
	/// �X�e�[�g��Update����
	/// </summary>
	/// <param name="pOwner"></param>
	virtual void OnUpdate(const std::shared_ptr<T>& pOwner) = 0;
	/// <summary>
	/// �X�e�[�g�I�����̏���
	/// </summary>
	/// <param name="pOwner"></param>
	virtual void OnExit(const std::shared_ptr<T>& pOwner, const std::shared_ptr<ObjectState<T>>& nextState = nullptr) = 0;
};


/// <summary>
/// �X�e�[�g�}�V���e���v���[�g�N���X
/// </summary>
template <class T>
class StateMachine
{
public:
	explicit StateMachine(const std::shared_ptr<T>& pOwner)
		: m_pOwner(pOwner), m_pCurState(), m_pPrevState()
	{}

	virtual ~StateMachine() {}

	/// <summary>
	/// �X�e�[�g���X�V
	/// </summary>
	void Update()const
	{
		auto state = m_pCurState.lock();
		auto owner = m_pOwner.lock();
		if (state && owner)
		{
			state->OnUpdate(owner);
		}
	}

	/// <summary>
	/// ���݂̃X�e�[�g���擾
	/// </summary>
	/// <returns></returns>
	const std::shared_ptr<ObjectState<T>> GetCurrentState()const
	{
		auto curState = m_pCurState.lock();
		if (curState)
			return curState;
		return nullptr;
	}

	/// <summary>
	/// ���݂̃X�e�[�g����������r
	/// </summary>
	/// <param name="pState"></param>
	/// <returns></returns>
	bool IsCurrentState(const std::shared_ptr<ObjectState<T>>& state)const
	{
		auto curState = m_pCurState.lock();
		return (curState == state);
	}

	/// <summary>
	/// �X�e�[�g��ύX
	/// </summary>
	/// <param name="pState"></param>
	void ChangeState(const std::shared_ptr<ObjectState<T>>& pState)
	{
		// ���݂̃X�e�[�g��ޔ�
		m_pPrevState = m_pCurState;
		auto state = m_pCurState.lock();
		auto owner = m_pOwner.lock();
		// ���݂̃X�e�[�g���I��
		if (state && owner)
			m_pCurState.lock()->OnExit(owner);

		m_pCurState = pState;
		state = m_pCurState.lock();
		if (state && owner)
		{
			// �V�����X�e�[�g���J�n
			state->OnEnter(owner);
		}
	}

private:
	std::weak_ptr<T>	m_pOwner;	// �I�[�i�[
	std::weak_ptr<ObjectState<T>>	m_pCurState;	// ���݂̃X�e�[�g
	std::weak_ptr<ObjectState<T>>	m_pPrevState;	// �O�̃X�e�[�g
};
