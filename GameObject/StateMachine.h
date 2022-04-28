#pragma once


template <class T>
class ObjectState
{
public:
	ObjectState() {}
	virtual ~ObjectState() {}

	/// <summary>
	/// ステートに入ったときの処理
	/// </summary>
	/// <param name="pOwner"></param>
	virtual void OnEnter(const std::shared_ptr<T>& pOwner, const std::shared_ptr<ObjectState<T>>& prevState = nullptr) = 0;
	/// <summary>
	/// ステートのUpdate処理
	/// </summary>
	/// <param name="pOwner"></param>
	virtual void OnUpdate(const std::shared_ptr<T>& pOwner) = 0;
	/// <summary>
	/// ステート終了時の処理
	/// </summary>
	/// <param name="pOwner"></param>
	virtual void OnExit(const std::shared_ptr<T>& pOwner, const std::shared_ptr<ObjectState<T>>& nextState = nullptr) = 0;
};


/// <summary>
/// ステートマシンテンプレートクラス
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
	/// ステートを更新
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
	/// 現在のステートを取得
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
	/// 現在のステートが等しい比較
	/// </summary>
	/// <param name="pState"></param>
	/// <returns></returns>
	bool IsCurrentState(const std::shared_ptr<ObjectState<T>>& state)const
	{
		auto curState = m_pCurState.lock();
		return (curState == state);
	}

	/// <summary>
	/// ステートを変更
	/// </summary>
	/// <param name="pState"></param>
	void ChangeState(const std::shared_ptr<ObjectState<T>>& pState)
	{
		// 現在のステートを退避
		m_pPrevState = m_pCurState;
		auto state = m_pCurState.lock();
		auto owner = m_pOwner.lock();
		// 現在のステートを終了
		if (state && owner)
			m_pCurState.lock()->OnExit(owner);

		m_pCurState = pState;
		state = m_pCurState.lock();
		if (state && owner)
		{
			// 新しいステートを開始
			state->OnEnter(owner);
		}
	}

private:
	std::weak_ptr<T>	m_pOwner;	// オーナー
	std::weak_ptr<ObjectState<T>>	m_pCurState;	// 現在のステート
	std::weak_ptr<ObjectState<T>>	m_pPrevState;	// 前のステート
};
