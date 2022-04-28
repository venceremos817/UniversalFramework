#pragma once

#include <Component.h>


class EnemyComponent
	: public Component
{
public:
	explicit EnemyComponent(const std::shared_ptr<GameObject>& gameObject);
	explicit EnemyComponent(const std::shared_ptr<GameObject>& gameObject, float health);
	virtual ~EnemyComponent();

	/// <summary>
	/// �_���[�W���󂯂�
	/// </summary>
	/// <param name="damage"></param>
	void ReceveDamage(float damage);

private:
	struct Param;
	std::unique_ptr<Param> m_param;
};
