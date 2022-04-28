#pragma once
#include <GameObject.h>

/// <summary>
/// �ʏ�e
/// �܂��������ł���
/// </summary>
class NormalBullet
	: public GameObject
{
public:
	NormalBullet(const std::shared_ptr<Stage>& pStage);
	~NormalBullet();

	/// <summary>
	/// �������A�w�肵�������ɔ��ł���
	/// </summary>
	/// <param name="direction"></param>
	void Shot(const Float3& spawnPos, const Float3& direction, float speed);

	void OnCreate()override;
	void Update()override;

	virtual void Destroy()override;

	void OnTriggerEnter(std::vector<std::shared_ptr<GameObject>>& destObjVec)override;


private:
	void OnEnterEnemy(const std::shared_ptr<GameObject>& pEney);

private:
	struct Param;
	std::unique_ptr<Param> m_param;
};


class HomingBullet
	: public GameObject
{
public:
	HomingBullet(const std::shared_ptr<Stage>& pStage);
	~HomingBullet();

	/// <summary>
	/// ����
	/// </summary>
	/// <param name="spawnPos">�����ʒu</param>
	/// <param name="velocity">�����x</param>
	/// <param name="target">�^�[�Q�b�g</param>
	/// <param name="speed">����</param>
	/// <param name="curvatureRadius">�Ȑ����a</param>
	/// <param name="damping">��C��R</param>
	void Shot(const Float3& spawnPos, const Float3& velocity, std::shared_ptr<Transform> target, float speed, float curvatureRadius, float damping);

	void OnCreate()override;
	void Update()override;

	virtual void Destroy()override;

	void OnTriggerEnter(std::vector<std::shared_ptr<GameObject>>& destObjVec)override;

private:
	struct Param;
	std::unique_ptr<Param> m_param;
};