#pragma once

#include <GameObject.h>
#include <Enemy/EnemyStateMachine.h>
#include <EffekseerComponent.h>

class Enemy
	: public GameObject
{
public:
	Enemy(const std::shared_ptr<Stage>& pStage);
	virtual ~Enemy();

	void OnCreate()override;
	void Update()override;
	void OnDestroy()override;

	virtual void Destroy()override;

	void OnCollisionEnter(std::vector<std::shared_ptr<GameObject>>& destObjVec)override;

	/// <summary>
	/// �팂�j���̃G�t�F�N�g���擾
	/// </summary>
	/// <returns></returns>
	const std::shared_ptr<EffekseerComponent>& GetDestroyEffect()const;

	const std::shared_ptr<StateMachine<Enemy>>& GetStateMachine()const;

private:
	struct Param;
	std::unique_ptr<Param> m_param;
};




/// <summary>
/// �G�l�~�[���Ǘ�����
/// �V���O���g��
/// </summary>
using EnemyVector = std::vector<std::shared_ptr<Enemy>>;
class EnemyManager
{
private:
	EnemyManager();
public:
	virtual ~EnemyManager();

	/// <summary>
	/// �V���O���g���A�N�Z�T
	/// </summary>
	/// <returns></returns>
	static std::shared_ptr<EnemyManager> GetInstance();

	/// <summary>
	/// �Ǘ��ɒǉ�
	/// </summary>
	std::shared_ptr<Enemy> AddEnemy(std::shared_ptr<Enemy> pEnemy);

	/// <summary>
	/// �Ǘ�����O��
	/// </summary>
	/// <param name="pEnemy"></param>
	void EraseEnemy(const std::shared_ptr<Enemy>& pEnemy);

	/// <summary>
	/// �Ǘ����Ă���G�l�~�[�z����擾
	/// </summary>
	const EnemyVector GetEnemyVector()const;

	/// <summary>
	/// ��_�����ԋ߂��G�l�~�[���擾
	/// </summary>
	/// <returns></returns>
	const std::shared_ptr<Enemy> GetNearlestEnemy(const Float3& base)const;

	/// <summary>
	/// �Ǘ����Ă���G�l�~�[����_����߂����Ƀ\�[�g�����z����擾
	/// </summary>
	/// <param name="base">��_</param>
	/// <returns></returns>
	const EnemyVector GetEnemyVectorByDistance(const Float3& base)const;

	/// <summary>
	/// �Ǘ����Ă���z����N���A
	/// </summary>
	void ClearVector();

private:
	EnemyVector	m_enemiesVector;	// �Ǘ�����G�l�~�[��ێ�����z��
};

