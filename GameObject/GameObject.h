#pragma once

#include <vector>
#include <memory>
#include <string>
#include <map>
#include <typeindex>

#include <Component.h>
#include <ObjectFactory.h>
#include <Transform.h>
#include <RigidBody.h>
#include <CollisionSphere.h>
#include <CollisionOBB.h>
#include <CollisionCapsule.h>
#include <CollisionRect.h>
#include <Behavior/Behavior.h>

class Component;
class Stage;
class Collision;
class Camera;
class Light;



namespace ProcessingOrder
{
	enum class GameObjectProcessingOrder
	{
		Base,
		Collision,
		Camera
	};
}


class GameObject
	: public std::enable_shared_from_this<GameObject>
{
	GameObject() = delete;		// デフォルトコンストラクタ禁止

public:
	enum class ObjectType
	{
		Player,
		Enemy,
		Ground,
		PlayerBullet,
		EnemyBullet,
		None,
	};

public:
	explicit GameObject(const std::shared_ptr<Stage>& pStage, ObjectType objectType = ObjectType::None);
	virtual ~GameObject();

	/// <summary>
	/// このオブジェクトのshred_ptrを取得
	/// </summary>
	/// <returns></returns>
	template<class T>
	std::shared_ptr<T> GetSharedPtr();

	/// <summary>
	/// 初期化処理1
	/// </summary>
	virtual void OnPreCreate();
	/// <summary>
	/// 初期化処理2
	/// </summary>
	virtual void OnCreate() {}
	/// <summary>
	/// 更新処理
	/// </summary>
	virtual void Update() {}
	/// <summary>
	/// 更新処理
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
	/// ターン終了処理
	/// </summary>
	//virtual void TurnEnd() {}
	/// <summary>
	/// 終了処理
	/// </summary>
	virtual void OnDestroy() {}

	/// <summary>
	/// ゲームオブジェクトを破棄
	/// </summary>
	virtual void Destroy() = 0;

	/// <summary>
	/// トリガーコリジョンイベント(衝突開始時
	/// </summary>
	/// <param name="destObjVec"></param>
	virtual void OnTriggerEnter(std::vector<std::shared_ptr<GameObject>>& destObjVec) {}
	/// <summary>
	/// トリガーコリジョンイベント(衝突継続時
	/// </summary>
	/// <param name="destObjVec"></param>
	virtual void OnTriggerStay(std::vector<std::shared_ptr<GameObject>>& destObjVec) {}
	/// <summary>
	/// トリガーコリジョンイベント(衝突終了時
	/// </summary>
	/// <param name="destObjVec"></param>
	virtual void OnTriggerExit(std::vector<std::shared_ptr<GameObject>> & destObjVec) {}

	/// <summary>
	/// 非トリガーコリジョンイベント(衝突開始時
	/// </summary>
	/// <param name="destObjVec"></param>
	virtual void OnCollisionEnter(std::vector<std::shared_ptr<GameObject>>& destObjVec) {}
	/// <summary>
	/// 非トリガーコリジョンイベント(衝突継続時
	/// </summary>
	/// <param name="destObjVec"></param>
	virtual void OnCollisionStay(std::vector<std::shared_ptr<GameObject>>& destObjVec) {}
	/// <summary>
	/// 非トリガーコリジョンイベント(衝突終了時
	/// </summary>
	/// <param name="destObeVec"></param>
	virtual void OnCollisionExit(std::vector<std::shared_ptr<GameObject>>& destObeVec) {}

	/// <summary>
	/// 衝突オブジェクトのリセット
	/// </summary>
	void CollisionReset();

	/// <summary>
	/// 衝突メッセージを受けた時の処理
	/// OnCollision～ OnTrigger～ に処理を分担する
	/// </summary>
	void OnRecivCollisionMessage();

	/// <summary>
	/// コンポーネントの更新
	/// </summary>
	void UpdateComponents();
	/// <summary>
	/// コンポーネントの更新
	/// </summary>
	void LateUpdateComponents();
	/// <summary>
	/// コンポーネントの描画準備
	/// </summary>
	void PreDrawComponents();
	/// <summary>
	/// コンポーネントの描画
	/// </summary>
	void DrawComponents();

	/// <summary>
	/// コンポーネントのトリガー衝突イベント呼び出し(衝突開始時
	/// </summary>
	/// <param name="destObjVec"></param>
	void OnTriggerEnterComponents(std::vector<std::shared_ptr<GameObject>>& destObjVec);
	/// <summary>
	/// コンポーネントのトリガー衝突イベント呼び出し(衝突継続時
	/// </summary>
	/// <param name="destObjVec"></param>
	void OnTriggerStayComponents(std::vector<std::shared_ptr<GameObject>>& destObjVec);
	/// <summary>
	/// コンポーネントのトリガー衝突イベント呼び出し(衝突終了時
	/// </summary>
	/// <param name="destObjVec"></param>
	void OnTriggerExitComponents(std::vector<std::shared_ptr<GameObject>>& destObjVec);

	/// <summary>
	/// コンポーネントの非トリガー衝突イベント呼び出し(衝突開始時
	/// </summary>
	/// <param name="destObjVec"></param>
	void OnCollisionEnterComponents(std::vector<std::shared_ptr<GameObject>>& destObjVec);
	/// <summary>
	/// コンポーネントの非トリガー衝突イベント呼び出し(衝突継続時
	/// </summary>
	/// <param name="destObjVec"></param>
	void OnCollisionStayComponents(std::vector<std::shared_ptr<GameObject>>& destObjVec);
	/// <summary>
	/// コンポーネントの非トリガー衝突イベント呼び出し(衝突終了時
	/// </summary>
	/// <param name="destObjVec"></param>
	void OnCollisionExitComponents(std::vector<std::shared_ptr<GameObject>>& destObjVec);

	bool IsActive() { return m_bActive; }
	void SetIsActive(bool b) { m_bActive = b; }
	bool IsDrawActive() { return m_bDrawActive; }
	void SetDrawActive(bool b) { m_bDrawActive = b; }
	bool IsSpriteDraw() { return m_bSpriteDraw; }
	void SetSpriteDraw(bool b) { m_bSpriteDraw = b; }
	bool IsAlpha() { return m_bAlpha; }
	void SetIsAlpha(bool b) { m_bAlpha = b; }
	int GetDrawLayer() { return m_drawLayer; }

	std::shared_ptr<Transform>	GetTransform()const { return m_pTransform; }
	std::shared_ptr<RigidBody>	GetRigidBody()const { return m_pRigidBody; }
	std::shared_ptr<Collision>	GetCollision()const { return m_pCollision; }

	/// <summary>
	/// コンポーネントを取得
	/// </summary>
	template<class T>
	std::shared_ptr<T> GetComponent()
	{
		for (auto com : m_componentVec)
		{
			auto buff = std::dynamic_pointer_cast<T>(com);
			if (buff)
			{
				return buff;
			}
		}
		return nullptr;
	}

	/// <summary>
	/// コンポーネントを取得
	/// </summary>
	/// <param name="pOut"></param>
	/// <returns>成功ならtrue , 失敗ならfalse</returns>
	template<class T>
	bool TryGetComponent(std::shared_ptr<T>& pOut)
	{
		for (auto com : m_componentVec)
		{
			auto buff = std::dynamic_pointer_cast<T>(com);
			if (buff)
			{
				pOut = buff;
				return true;
			}
		}
		return false;
	}

	template<class T>
	/// <summary>
	/// 指定した方のコンポーネント、またはその派生を持っているかどうか
	/// </summary>
	/// <returns></returns>
	bool HasComponent()
	{
		for (auto com : m_componentVec)
		{
			auto buff = std::dynamic_pointer_cast<T>(com);
			if (buff)
				return true;
		}
		return false;
	}

	/// <summary>
	/// コンポーネントを追加
	/// </summary>
	template<class T>
	std::shared_ptr<T> AddComponent();
	template<class T, class... Ts>
	std::shared_ptr<T> AddComponent(Ts&&... params);
	/// <summary>
	/// トランスフォームの追加
	/// </summary>
	/// <returns></returns>
	template<>
	std::shared_ptr<Transform> AddComponent<Transform>()
	{
		if (m_pTransform)
			return m_pTransform;
		m_pTransform = ObjectFactory::Create<Transform>(GetSharedPtr<GameObject>());
		m_componentVec.push_back(m_pTransform);
		SortComponentsByOrder();
		return m_pTransform;
	}
	/// <summary>
	/// リジッドボディの追加
	/// </summary>
	/// <returns></returns>
	template<>
	std::shared_ptr<RigidBody> AddComponent<RigidBody>()
	{
		if (m_pRigidBody)
			return m_pRigidBody;

		m_pRigidBody = ObjectFactory::Create<RigidBody>(GetSharedPtr<GameObject>());
		m_componentVec.push_back(m_pRigidBody);
		SortComponentsByOrder();
		return m_pRigidBody;
	}
	/// <summary>
	/// 球衝突判定の追加
	/// </summary>
	/// <returns></returns>
	template<>
	std::shared_ptr<CollisionSphere> AddComponent<CollisionSphere>()
	{
		if (m_pCollision)
			return std::dynamic_pointer_cast<CollisionSphere>(m_pCollision);

		auto pBuff = ObjectFactory::Create<CollisionSphere>(GetSharedPtr<GameObject>());
		m_componentVec.push_back(pBuff);
		SortComponentsByOrder();
		m_pCollision = pBuff;
		return pBuff;
	}
	/// <summary>
	/// OBB衝突判定の追加
	/// </summary>
	/// <returns></returns>
	template<>
	std::shared_ptr<CollisionOBB> AddComponent<CollisionOBB>()
	{
		if (m_pCollision)
			return std::dynamic_pointer_cast<CollisionOBB>(m_pCollision);

		auto pBuff = ObjectFactory::Create<CollisionOBB>(GetSharedPtr<GameObject>());
		m_componentVec.push_back(pBuff);
		SortComponentsByOrder();
		m_pCollision = pBuff;
		return pBuff;
	}
	/// <summary>
	/// カプセル衝突判定の追加
	/// </summary>
	/// <returns></returns>
	template<>
	std::shared_ptr<CollisionCapsule> AddComponent<CollisionCapsule>()
	{
		if (m_pCollision)
			return std::dynamic_pointer_cast<CollisionCapsule>(m_pCollision);

		auto pBuff = ObjectFactory::Create<CollisionCapsule>(GetSharedPtr<GameObject>());
		m_componentVec.push_back(pBuff);
		SortComponentsByOrder();
		m_pCollision = pBuff;
		return pBuff;
	}
	/// <summary>
	/// 矩形衝突判定の追加
	/// </summary>
	/// <returns></returns>
	template<>
	std::shared_ptr<CollisionRect> AddComponent<CollisionRect>()
	{
		if (m_pCollision)
			return std::dynamic_pointer_cast<CollisionRect>(m_pCollision);

		auto pBuff = ObjectFactory::Create<CollisionRect>(GetSharedPtr<GameObject>());
		m_componentVec.push_back(pBuff);
		SortComponentsByOrder();
		m_pCollision = pBuff;
		return pBuff;
	}


	/// <summary>
	/// 指定ビヘイビアの取得	存在しなければ生成
	/// </summary>
	/// <returns></returns>
	template<class T>
	std::shared_ptr<T> GetBehavior();

	/// <summary>
	/// ビヘイビアマップに存在するかどうか
	/// </summary>
	template<class T>
	bool FindBehavior();

	void SetCreated(bool create) { m_bCreated = create; }

	/// <summary>
	/// このオブジェクトを管理しているマネージャーを取得
	/// </summary>
	/// <returns></returns>
	const std::shared_ptr<Stage> GetStage()const;

	/// <summary>
	/// タグを追加
	/// </summary>
	/// <param name="tag"></param>
	void AddTag(const std::string& tag);

	/// <summary>
	/// タグをを含むか
	/// </summary>
	/// <param name="tag"></param>
	/// <returns></returns>
	bool IsContainTag(const std::string& tag);

	/// <summary>
	/// オブジェクトタイプを取得
	/// </summary>
	/// <returns></returns>
	const ObjectType GetObjectType()const;
	/// <summary>
	/// 指定したオブジェクトタイプかどうか
	/// </summary>
	/// <param name="objectType"></param>
	/// <returns></returns>
	bool IsObjectType(ObjectType objectType)const;

	/// <summary>
	/// 処理順を取得
	/// </summary>
	/// <returns></returns>
	const ProcessingOrder::GameObjectProcessingOrder GetProcessingOrder() { return m_processOrder; }

	/// <summary>
	/// 描画に利用するカメラを取得
	/// </summary>
	/// <returns></returns>
	const std::shared_ptr<Camera>& GetDrawCamera()const;

	/// <summary>
	/// オブジェクトを描画するためのライトを取得
	/// </summary>
	/// <returns></returns>
	virtual const Light& GetDrawLight()const;

	/// <summary>
	/// 2D描画する際の射影行列を取得
	/// </summary>
	virtual void Get2DDrawProjectionMatrix(Float4x4& projMat)const;

private:
	/// <summary>
	/// ビヘイビアマップから探索して取得
	/// </summary>
	/// <param name="typeIdx"></param>
	/// <returns></returns>
	std::shared_ptr<Behavior> SearchBehavior(std::type_index typeIdx)const;
	/// <summary>
	/// ビヘイビアマップに追加、または更新
	/// </summary>
	/// <param name="typeIdx"></param>
	/// <param name="pBehaveir"></param>
	void AddMakeBehavior(std::type_index typeIdx, const std::shared_ptr<Behavior> pBehaveir);

	/// <summary>
	/// コンポーネントを処理順でソート
	/// </summary>
	void SortComponentsByOrder();

public:
	std::vector<std::shared_ptr<Component>> m_componentVec;


	std::shared_ptr<Transform> m_pTransform;
	std::shared_ptr<RigidBody> m_pRigidBody;
	std::shared_ptr<Collision> m_pCollision;
protected:
	std::vector<std::string> m_tags;
	ObjectType m_objectType;
	bool       m_bActive;			// このオブジェクトがアクティブか
	bool       m_bDrawActive;		// 描画するか
	bool       m_bSpriteDraw;		// 2D描画するか
	bool       m_bAlpha;			// 透過処理を行うか
	bool       m_bCreated;			// このオブジェクトが作成済みか
	std::weak_ptr<Stage>	m_pStage;	// 属するステージ
	ProcessingOrder::GameObjectProcessingOrder m_processOrder;	// 処理順
	int m_drawLayer;

private:
	std::map<std::type_index, std::shared_ptr<Behavior>> m_behaviorMap;

private:
	// コピー禁止
	GameObject(const GameObject&) = delete;
	GameObject& operator=(const GameObject&) = delete;
	// ムーブ禁止
	GameObject(const GameObject&&) = delete;
	GameObject& operator=(const GameObject&&) = delete;
};



//std::vector<GameObject*> GetGameObjectVec();

template<class T>
inline std::shared_ptr<T> GameObject::GetSharedPtr()
{
	auto ptr = std::dynamic_pointer_cast<T>(shared_from_this());
	if (ptr)
		return ptr;
	return nullptr;
}

template<class T>
inline std::shared_ptr<T> GameObject::AddComponent()
{
	auto buff = ObjectFactory::Create<T>(GetSharedPtr<GameObject>());
	m_componentVec.push_back(buff);
	// 処理順でソート
	SortComponentsByOrder();

	return buff;
}

template<class T, class ...Ts>
inline std::shared_ptr<T> GameObject::AddComponent(Ts && ...params)
{
	auto buff = ObjectFactory::Create<T>(GetSharedPtr<GameObject>(), params...);
	m_componentVec.push_back(buff);
	// 処理順でソート
	SortComponentsByOrder();

	return buff;
}

template<class T>
inline std::shared_ptr<T> GameObject::GetBehavior()
{
	// ビヘイビアマップから探索
	auto ptr = SearchBehavior(std::type_index(typeid(T)));
	if (ptr)
	{
		auto retPtr = std::dynamic_pointer_cast<T>(ptr);
		if (retPtr != nullptr)
			return retPtr;	// 見つかれば返す
		else
			return nullptr;	// キャストできない
	}
	// 見つからなければ新規作成
	std::shared_ptr<T> pBehavior = ObjectFactory::Create<T>(GetSharedPtr<GameObject>());
	AddMakeBehavior(std::type_index(typeid(T)), pBehavior);
	return pBehavior;
}

template<class T>
inline bool GameObject::FindBehavior()
{
	auto ptr = SearchBehavior(std::type_index(typeid(T)));
	if (ptr)
	{
		auto retPtr = std::dynamic_pointer_cast<T>(ptr);
		if (retPtr != nullptr)
			return true;
		else
			return false;
	}
	return false;
}
