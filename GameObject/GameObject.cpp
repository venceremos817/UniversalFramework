#include <GameObject.h>
#include <algorithm>
#include <Collision.h>
#include <Stage.h>



GameObject::GameObject(const std::shared_ptr<Stage>& pStage, GameObject::ObjectType objectType)
	: m_processOrder(ProcessingOrder::GameObjectProcessingOrder::Base)
	, m_bActive(true)
	, m_bDrawActive(false)
	, m_bSpriteDraw(false)
	, m_bAlpha(false)
	, m_pTransform(nullptr)
	, m_pRigidBody(nullptr)
	, m_pCollision(nullptr)
	, m_bCreated(false)
	, m_pStage(pStage)
	, m_objectType(objectType)
	, m_drawLayer(0)
{
}

GameObject::~GameObject()
{
}

void GameObject::OnPreCreate()
{
	m_pTransform = AddComponent<Transform>();
}

void GameObject::CollisionReset()
{
	if (m_pCollision)
	{
		m_pCollision->SetToBeforeHitObject();
		m_pCollision->ClearHitObject();
	}
}

void GameObject::OnRecivCollisionMessage()
{
	std::shared_ptr<Collision> pCollision;
	bool haveCollision = TryGetComponent<Collision>(pCollision);
	// コリジョンコンポーネントを持ってなければリターン
	if (!haveCollision)
		return;

	//=== トリガーイベント ===
	// 衝突し始め
	auto triggerEnterVec = pCollision->GetTriggerEnterObjectVec();
	if (!triggerEnterVec.empty())
	{
		OnTriggerEnter(triggerEnterVec);
		OnTriggerEnterComponents(triggerEnterVec);
	}

	// 継続的に衝突
	auto& triggerStayVec = pCollision->GetTriggerStayObjectVec();
	if (!triggerStayVec.empty())
	{
		OnTriggerStay(triggerStayVec);
		OnTriggerStayComponents(triggerStayVec);
	}

	// 衝突から離れた
	auto& triggerExitVec = pCollision->GetTriggerExitObjectVec();
	if (!triggerExitVec.empty())
	{
		OnTriggerExit(triggerExitVec);
		OnTriggerExitComponents(triggerExitVec);
	}

	//=== 非トリガーイベント ===
	// 衝突し始め
	auto& collisionEnterVec = pCollision->GetCollisionEnterObjectVec();
	if (!collisionEnterVec.empty())
	{
		OnCollisionEnter(collisionEnterVec);
		OnCollisionEnterComponents(collisionEnterVec);
	}

	// 継続的に衝突
	auto& collisionStayVec = pCollision->GetCollisionStayObjectVec();
	if (!collisionStayVec.empty())
	{
		OnCollisionStay(collisionStayVec);
		OnCollisionStayComponents(collisionStayVec);
	}

	// 衝突から離れた
	auto& collisionExitVec = pCollision->GetCollisionExitObjectVec();
	if (!collisionExitVec.empty())
	{
		OnCollisionExit(collisionExitVec);
		OnCollisionExitComponents(collisionExitVec);
	}
}

void GameObject::UpdateComponents()
{
	auto buff = m_componentVec;
	for (auto com : buff)
		if (com->IsActive())
			com->Update();
}

void GameObject::LateUpdateComponents()
{
	auto buff = m_componentVec;
	for (auto com : buff)
		com->LateUpdate();
}

void GameObject::PreDrawComponents()
{
	auto buff = m_componentVec;
	for (auto com : buff)
		if (com->IsActive())
			com->PreDraw();
}

void GameObject::DrawComponents()
{
	auto buff = m_componentVec;
	for (auto com : buff)
		if (com->IsActive())
			com->Draw();
}

void GameObject::OnTriggerEnterComponents(std::vector<std::shared_ptr<GameObject>>& destObjVec)
{
	auto buff = m_componentVec;
	for (auto com : buff)
		com->OnTriggerEnter(destObjVec);
}

void GameObject::OnTriggerStayComponents(std::vector<std::shared_ptr<GameObject>>& destObjVec)
{
	auto buff = m_componentVec;
	for (auto com : buff)
		com->OnTriggerStay(destObjVec);
}

void GameObject::OnTriggerExitComponents(std::vector<std::shared_ptr<GameObject>>& destObjVec)
{
	auto buff = m_componentVec;
	for (auto com : buff)
		com->OnTriggerExit(destObjVec);
}

void GameObject::OnCollisionEnterComponents(std::vector<std::shared_ptr<GameObject>>& destObjVec)
{
	auto buff = m_componentVec;
	for (auto com : buff)
		com->OnCollisionEnter(destObjVec);
}

void GameObject::OnCollisionStayComponents(std::vector<std::shared_ptr<GameObject>>& destObjVec)
{
	auto buff = m_componentVec;
	for (auto com : buff)
		com->OnCollisionStay(destObjVec);
}

void GameObject::OnCollisionExitComponents(std::vector<std::shared_ptr<GameObject>>& destObjVec)
{
	auto buff = m_componentVec;
	for (auto com : buff)
		com->OnCollisionExit(destObjVec);
}

const std::shared_ptr<Stage> GameObject::GetStage()const
{
	return m_pStage.lock();
}

void GameObject::AddTag(const std::string & tag)
{
	// まだタグを含んでいなければ追加
	if (!IsContainTag(tag))
		m_tags.push_back(tag);
}

bool GameObject::IsContainTag(const std::string& tag)
{
	for (auto v : m_tags)
	{
		if (v == tag)
			return true;
	}
	return false;
}

const GameObject::ObjectType GameObject::GetObjectType() const
{
	return m_objectType;
}

bool GameObject::IsObjectType(ObjectType objectType) const
{
	return m_objectType == objectType;
}

const std::shared_ptr<Camera>& GameObject::GetDrawCamera() const
{
	// ステージのカメラを使う
	return GetStage()->GetView()->GetTargetCamera();
}

const Light & GameObject::GetDrawLight() const
{
	// ステージからライトを取得
	return GetStage()->GetLight()->GetTargetLight();
}

void GameObject::Get2DDrawProjectionMatrix(Float4x4& projMat) const
{
	auto viewport = m_pStage.lock()->GetView()->GetTargetViewport();
	float width = viewport.width;
	float height = viewport.height;
	projMat = (Float4x4)DirectX::XMMatrixOrthographicLH(
		viewport.width, viewport.height,
		viewport.minDepth, viewport.maxDepth);
}

std::shared_ptr<Behavior> GameObject::SearchBehavior(std::type_index typeIdx) const
{
	auto it = m_behaviorMap.find(typeIdx);
	if (it != m_behaviorMap.end())
		return it->second;
	return nullptr;
}

void GameObject::AddMakeBehavior(std::type_index typeIdx, const std::shared_ptr<Behavior> pBehaveir)
{
	m_behaviorMap[typeIdx] = pBehaveir;
}

void GameObject::SortComponentsByOrder()
{
	// 処理順でソート
	auto func = [&](std::shared_ptr<Component> left, std::shared_ptr<Component> right)->bool
	{
		return (left->GetProcessingOrder() < right->GetProcessingOrder());
	};
	std::sort(m_componentVec.begin(), m_componentVec.end(), func);
}
