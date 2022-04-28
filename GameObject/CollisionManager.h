#pragma once

#include <GameObject.h>
#include <Collision.h>

class CollisionManager
	: public GameObject
{
public:
	CollisionManager(const std::shared_ptr<Stage>& pStage);
	virtual ~CollisionManager();


	/// <summary>
	/// ����������
	/// </summary>
	//virtual void OnCreate()override;
	/// <summary>
	/// �I������
	/// </summary>
	virtual void OnDestroy()override;

	virtual void Destroy()override;

	/// <summary>
	/// �X�V����
	/// </summary>
	virtual void CollisionUpdate();		// �X�V�^�C�~���O������Ȃ��߁AUpdate�Ɩ��Â��Ȃ�

	/// <summary>
	/// �Փ˔����ɌĂяo���A�Փ˃��b�Z�[�W�𑗂�
	/// </summary>
	virtual void SendCollisionMessage();


private:
	void CollisionSub(rsize_t srcIdx);

private:
	struct CollisionItem
	{
		std::shared_ptr<Collision>	pCollision;
		SphereBoundaryVolume enclosingSphereVol;
		float minX;
		float maxX;
		float minY;
		float maxY;
		float minZ;
		float maxZ;

		bool operator==(const CollisionItem& dest)const
		{
			if (this == &dest)
				return true;
			return false;
		}
	};
	struct Param;
	std::unique_ptr<Param>	m_param;
};