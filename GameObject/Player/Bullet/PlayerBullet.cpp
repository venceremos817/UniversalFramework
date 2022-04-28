#include "PlayerBullet.h"
#include <App/App.h>
#include <DrawComponent/DrawComponent.h>
#include <EnemyComponent.h>
#include <EffekseerComponent.h>
#include <Stage.h>

//=====================================================
// NormalBullet
//=====================================================
#pragma region NormalBullet

struct NormalBullet::Param
{
	Float3 direction;	// 飛んでいく方向
	float speed;		// 速さ
	float life;			// 寿命
	std::shared_ptr<EffekseerComponent> heatEffect;
	std::shared_ptr<EffekseerComponent> hitEffect;

	Param()
		: direction(DirectX::g_XMIdentityR2), speed(1.0f), life(2.0f)
	{}
};

NormalBullet::NormalBullet(const std::shared_ptr<Stage>& pStage)
	: GameObject(pStage, ObjectType::PlayerBullet)
	, m_param(new Param)
{
}

NormalBullet::~NormalBullet()
{
}

void NormalBullet::Shot(const Float3 & spawnPos, const Float3 & direction, float speed)
{
	AddTag("bullet");
#ifdef _DEBUG
	AddTag("normalBullet");
#endif // DEBUG

	m_pTransform->ResetPos(spawnPos);

	m_param->direction = direction.normalized();
	m_param->speed = speed;

	GetBehavior<CommonBehavior>()->RotToVecotr(m_param->direction, 1.0f);

	m_param->heatEffect->Play(true, true, spawnPos);
}

void NormalBullet::OnCreate()
{
	m_pTransform->SetScale(0.3f, 0.3f, 0.3f);
	// コンポーネントの追加
	auto rb = AddComponent<RigidBody>();
	auto col = AddComponent<CollisionSphere>();
	m_param->heatEffect = AddComponent<EffekseerComponent>();
	m_param->hitEffect = AddComponent<EffekseerComponent>();

	rb->SetFrictionForce(0.0f);
	rb->SetMinVelocity(Float3(0.0f, 0.0f, 0.0f));
	col->SetFixed(false);
	col->SetHitAction(Collision::HitAction::None);
	col->SetIsTrigger(true);
	m_param->heatEffect->SetEffect(APP->GetResource<EffekseerEffectResource>("HEAT_BULLET_EFFECT"));
	m_param->hitEffect->SetEffect(APP->GetResource<EffekseerEffectResource>("HIT_EFFECT"));
}

void NormalBullet::Update()
{
	float elapsedTime = APP->GetElapsedTime();
	Float3 velo = m_param->direction * m_param->speed * elapsedTime;
	m_pRigidBody->SetVelocity(velo);

	// 寿命が来たら消す
	m_param->life -= elapsedTime;
	if (m_param->life < 0.0f)
		Destroy();
}

void NormalBullet::Destroy()
{
	m_param->heatEffect->Stop();
	GetStage()->RemoveGameObject(GetSharedPtr<NormalBullet>());
}

void NormalBullet::OnTriggerEnter(std::vector<std::shared_ptr<GameObject>>& destObjVec)
{
	for (auto dest : destObjVec)
	{
		auto destType = dest->GetObjectType();
		if (destType == GameObject::ObjectType::Enemy)
			OnEnterEnemy(dest);
	}
}

void NormalBullet::OnEnterEnemy(const std::shared_ptr<GameObject>& pEnemy)
{
	auto enemyCom = pEnemy->GetComponent<EnemyComponent>();
	enemyCom->ReceveDamage(2.0f);
	m_param->hitEffect->Play(false, true, GetTransform()->GetWorldPos());
	Destroy();
}

#pragma endregion


//=====================================================
// HomingBullet
//=====================================================
#pragma region HomingBullet

struct HomingBullet::Param
{
	std::shared_ptr<Transform> target;	// ターゲット
	Float3 velocity;			// 速度
	float maxCentripetalAccel;	// 向心力
	float damping;		// 空気抵抗力
	float propulsion;	// 推進力
	float lifeTime;			// 寿命
	std::shared_ptr<EffekseerComponent> hitEffect;
	std::shared_ptr<EffekseerComponent> drillEffect;

	Param()
		: lifeTime(3.0f)
	{}
};

HomingBullet::HomingBullet(const std::shared_ptr<Stage>& pStage)
	: GameObject(pStage, ObjectType::PlayerBullet)
	, m_param(new Param)
{
}

HomingBullet::~HomingBullet()
{
}

void HomingBullet::Shot(const Float3 & spawnPos, const Float3 & velocity, std::shared_ptr<Transform> target, float speed, float curvatureRadius, float damping)
{
	AddTag("bullet");
#ifdef _DEBUG
	AddTag("homingBullet");
#endif // DEBUG


	GetTransform()->ResetWorldPos(spawnPos);
	m_param->velocity = velocity;
	float faf = velocity.Length();
	m_param->target = target;
	// 速さv、半径rで円を描くとき、その向心力はv^2/r
	m_param->maxCentripetalAccel = speed * speed / curvatureRadius;
	m_param->damping = damping;
	// 終端速度がspeedになるaccelを求める
	m_param->propulsion = speed * damping;

	// 進行方向を向く
	GetBehavior<CommonBehavior>()->RotToVecotr(m_param->velocity, 1.0f);

	m_param->drillEffect->Play(true, false, spawnPos);
}

void HomingBullet::OnCreate()
{
	m_pTransform->SetScale(0.25f, 0.25f, 0.25f);
	// コンポーネントの追加
	auto rb = AddComponent<RigidBody>();
	auto col = AddComponent<CollisionSphere>();
	m_param->hitEffect = AddComponent<EffekseerComponent>();
	m_param->drillEffect = AddComponent<EffekseerComponent>();

	rb->SetFrictionForce(0.0f);
	rb->SetMinVelocity(Float3(0.0f, 0.0f, 0.0f));
	col->SetFixed(false);
	col->SetHitAction(Collision::HitAction::None);
	col->SetIsTrigger(true);
	m_param->hitEffect->SetEffect(APP->GetResource<EffekseerEffectResource>("HIT_EFFECT"));;
	m_param->drillEffect->SetEffect(APP->GetResource<EffekseerEffectResource>("DRILL_BULLET_EFFECT"));
}

void HomingBullet::Update()
{
	float elapsedTime = APP->GetElapsedTime();
	Float3 pos = GetTransform()->GetWorldPos();

	Float3 toTarget = m_param->target->GetWorldPos() - pos;
	Float3 vn = m_param->velocity.normalized();
	float dot = Float3::Dot(toTarget, vn);
	auto centripetalAccel = toTarget - (vn * dot);	// 向心加速度
	auto centripetalAccelMagnitude = centripetalAccel.Length();
	if (centripetalAccelMagnitude > 1.0f)
		centripetalAccel /= centripetalAccelMagnitude;
	auto force = centripetalAccel * m_param->maxCentripetalAccel;
	force += vn * m_param->propulsion;
	force -= m_param->velocity * m_param->damping;
	m_param->velocity += force * elapsedTime;
	GetRigidBody()->SetVelocity(m_param->velocity);
	pos += m_param->velocity * elapsedTime;

	// 進行方向を向く
	GetBehavior<CommonBehavior>()->RotToVecotr(m_param->velocity, 0.8f);

	// 寿命が来たら消す
	m_param->lifeTime -= elapsedTime;
	if (m_param->lifeTime < 0.0f)
		Destroy();
}

void HomingBullet::Destroy()
{
	m_param->drillEffect->Stop();
	GetStage()->RemoveGameObject(GetSharedPtr<HomingBullet>());
}

void HomingBullet::OnTriggerEnter(std::vector<std::shared_ptr<GameObject>>& destObjVec)
{
	for (auto destObj : destObjVec)
	{
		auto destType = destObj->GetObjectType();
		if (destType == GameObject::ObjectType::Enemy)
		{
			auto enemyCom = destObj->GetComponent<EnemyComponent>();
			enemyCom->ReceveDamage(1.5f);
			m_param->hitEffect->Play(false, true, GetTransform()->GetWorldPos());
			Destroy();
		}
	}
}



#pragma endregion
