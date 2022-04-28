#pragma once

#include <memory>
#include <vector>


class GameObject;
class Transform;


namespace ProcessingOrder
{
	enum class ComponentProcessingOrder
	{
		Base,
		RigidBody,
		Transform,
	};
}

class Component
	: public std::enable_shared_from_this<Component>
{
public:
	explicit Component(const std::shared_ptr<GameObject>& pGameObject)
		:m_processOrder(ProcessingOrder::ComponentProcessingOrder::Base), m_bActive(true), m_pParentGameobject(pGameObject), m_bCreated(false)
	{}
	virtual ~Component() {}

public:
	/// <summary>
	/// 初期化処理1
	/// </summary>
	virtual void OnPreCreate() {}
	/// <summary>
	/// 初期化処理2
	/// </summary>
	virtual void OnCreate() {}
	/// <summary>
	/// 更新処理
	/// </summary>
	virtual void Update() {}
	/// <summary>
	/// 更新処理2
	/// </summary>
	virtual void LateUpdate() {}
	/// <summary>
	/// 描画準備
	/// </summary>
	virtual void PreDraw() {}
	/// <summary>
	/// 描画処理
	/// </summary>
	virtual void Draw() {}
	/// <summary>
	/// 終了処理
	/// </summary>
	virtual void OnDestroy() {}

	/// <summary>
	/// ゲームオブジェクトを取得
	/// </summary>
	/// <returns>このコンポーネントが付加されているゲームオブジェクト</returns>
	const std::shared_ptr<GameObject> GetGameObject() const
	{
		auto pGameObj = m_pParentGameobject.lock();
		if (pGameObj)
			return pGameObj;
		return nullptr;
	}

	/// <summary>
	/// トリガー衝突開始時イベント
	/// </summary>
	/// <param name="destObjVec"></param>
	virtual void OnTriggerEnter(std::vector<std::shared_ptr<GameObject>>& destObjVec) {}
	/// <summary>
	/// トリガー衝突中イベント
	/// </summary>
	/// <param name="destObjVec"></param>
	virtual void OnTriggerStay(std::vector<std::shared_ptr<GameObject>>& destObjVec) {}
	/// <summary>
	/// トリガー衝突終了時イベント
	/// </summary>
	/// <param name="destObjVec"></param>
	virtual void OnTriggerExit(std::vector<std::shared_ptr<GameObject>>& destObjVec) {}
	/// <summary>
	/// コリジョン衝突時イベント
	/// </summary>
	/// <param name="destObjVec"></param>
	virtual void OnCollisionEnter(std::vector<std::shared_ptr<GameObject>>& destObjVec) {}
	/// <summary>
	/// コリジョン衝突中イベント
	/// </summary>
	/// <param name="destObjVec"></param>
	virtual void OnCollisionStay(std::vector<std::shared_ptr<GameObject>>& destObjVec) {}
	/// <summary>
	/// コリジョン衝突終了イベント
	/// </summary>
	/// <param name="destObjVec"></param>
	virtual void OnCollisionExit(std::vector<std::shared_ptr<GameObject>>& destObjVec) {}

	/// <summary>
	/// 処理順を取得
	/// </summary>
	/// <returns></returns>
	ProcessingOrder::ComponentProcessingOrder GetProcessingOrder() { return m_processOrder; }
	/// <summary>
	/// 処理順を設定
	/// </summary>
	/// <param name="order"></param>
	void SetProcessingOrder(ProcessingOrder::ComponentProcessingOrder order) { m_processOrder = order; }

	/// <summary>
	/// 有効かどうか
	/// </summary>
	/// <returns></returns>
	bool IsActive() { return m_bActive; }
	void SetActive(bool b) { m_bActive = b; }
	/// <summary>
	/// 初期化完了かどうか設定
	/// </summary>
	/// <param name="created"></param>
	void SetCreated(bool created) { m_bCreated = created; }

	/// <summary>
	/// このコンポーネントのshared_ptrを取得
	/// </summary>
	/// <returns></returns>
	template<class T>
	std::shared_ptr<T> GetShared()
	{
		auto pt = std::dynamic_pointer_cast<T>(shared_from_this());
		if (pt)
			return pt;
		return nullptr;
	}

private:
	std::weak_ptr<GameObject> m_pParentGameobject;
	ProcessingOrder::ComponentProcessingOrder m_processOrder;	// 処理順
	bool	m_bActive;
	bool	m_bCreated;
};


