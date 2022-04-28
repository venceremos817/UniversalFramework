#include "EnemyComponent.h"
#include <GameObject.h>
#include <GameObject/Enemy/Enemy.h>

struct EnemyComponent::Param
{
	float health;

	Param() :health(10.0f)
	{}
	Param(float h) :health(h)
	{}
};

EnemyComponent::EnemyComponent(const std::shared_ptr<GameObject>& gameObject)
	: Component(gameObject)
	, m_param(new Param())
{
}

EnemyComponent::EnemyComponent(const std::shared_ptr<GameObject>& gameObject, float health)
	: Component(gameObject)
	, m_param(new Param(health))
{
}

EnemyComponent::~EnemyComponent()
{
}

void EnemyComponent::ReceveDamage(float damage)
{
	m_param->health -= damage;

	if (m_param->health <= 0.0f)
	{
		// Œ‚”j
		auto enemyObj = GetGameObject()->GetSharedPtr<Enemy>();
		auto stateMachine = enemyObj->GetStateMachine();
		stateMachine->ChangeState(EnemyDestroyState::GetInstance());
	}
}
