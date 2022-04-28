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
	GameObject() = delete;		// �f�t�H���g�R���X�g���N�^�֎~

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
	/// ���̃I�u�W�F�N�g��shred_ptr���擾
	/// </summary>
	/// <returns></returns>
	template<class T>
	std::shared_ptr<T> GetSharedPtr();

	/// <summary>
	/// ����������1
	/// </summary>
	virtual void OnPreCreate();
	/// <summary>
	/// ����������2
	/// </summary>
	virtual void OnCreate() {}
	/// <summary>
	/// �X�V����
	/// </summary>
	virtual void Update() {}
	/// <summary>
	/// �X�V����
	/// </summary>
	virtual void LateUpdate() {}
	/// <summary>
	/// �`�揀��
	/// </summary>
	virtual void PreDraw() {}
	/// <summary>
	/// �`�揈��
	/// </summary>
	virtual void Draw() {}
	/// <summary>
	/// �^�[���I������
	/// </summary>
	//virtual void TurnEnd() {}
	/// <summary>
	/// �I������
	/// </summary>
	virtual void OnDestroy() {}

	/// <summary>
	/// �Q�[���I�u�W�F�N�g��j��
	/// </summary>
	virtual void Destroy() = 0;

	/// <summary>
	/// �g���K�[�R���W�����C�x���g(�ՓˊJ�n��
	/// </summary>
	/// <param name="destObjVec"></param>
	virtual void OnTriggerEnter(std::vector<std::shared_ptr<GameObject>>& destObjVec) {}
	/// <summary>
	/// �g���K�[�R���W�����C�x���g(�Փˌp����
	/// </summary>
	/// <param name="destObjVec"></param>
	virtual void OnTriggerStay(std::vector<std::shared_ptr<GameObject>>& destObjVec) {}
	/// <summary>
	/// �g���K�[�R���W�����C�x���g(�ՓˏI����
	/// </summary>
	/// <param name="destObjVec"></param>
	virtual void OnTriggerExit(std::vector<std::shared_ptr<GameObject>> & destObjVec) {}

	/// <summary>
	/// ��g���K�[�R���W�����C�x���g(�ՓˊJ�n��
	/// </summary>
	/// <param name="destObjVec"></param>
	virtual void OnCollisionEnter(std::vector<std::shared_ptr<GameObject>>& destObjVec) {}
	/// <summary>
	/// ��g���K�[�R���W�����C�x���g(�Փˌp����
	/// </summary>
	/// <param name="destObjVec"></param>
	virtual void OnCollisionStay(std::vector<std::shared_ptr<GameObject>>& destObjVec) {}
	/// <summary>
	/// ��g���K�[�R���W�����C�x���g(�ՓˏI����
	/// </summary>
	/// <param name="destObeVec"></param>
	virtual void OnCollisionExit(std::vector<std::shared_ptr<GameObject>>& destObeVec) {}

	/// <summary>
	/// �Փ˃I�u�W�F�N�g�̃��Z�b�g
	/// </summary>
	void CollisionReset();

	/// <summary>
	/// �Փ˃��b�Z�[�W���󂯂����̏���
	/// OnCollision�` OnTrigger�` �ɏ����𕪒S����
	/// </summary>
	void OnRecivCollisionMessage();

	/// <summary>
	/// �R���|�[�l���g�̍X�V
	/// </summary>
	void UpdateComponents();
	/// <summary>
	/// �R���|�[�l���g�̍X�V
	/// </summary>
	void LateUpdateComponents();
	/// <summary>
	/// �R���|�[�l���g�̕`�揀��
	/// </summary>
	void PreDrawComponents();
	/// <summary>
	/// �R���|�[�l���g�̕`��
	/// </summary>
	void DrawComponents();

	/// <summary>
	/// �R���|�[�l���g�̃g���K�[�Փ˃C�x���g�Ăяo��(�ՓˊJ�n��
	/// </summary>
	/// <param name="destObjVec"></param>
	void OnTriggerEnterComponents(std::vector<std::shared_ptr<GameObject>>& destObjVec);
	/// <summary>
	/// �R���|�[�l���g�̃g���K�[�Փ˃C�x���g�Ăяo��(�Փˌp����
	/// </summary>
	/// <param name="destObjVec"></param>
	void OnTriggerStayComponents(std::vector<std::shared_ptr<GameObject>>& destObjVec);
	/// <summary>
	/// �R���|�[�l���g�̃g���K�[�Փ˃C�x���g�Ăяo��(�ՓˏI����
	/// </summary>
	/// <param name="destObjVec"></param>
	void OnTriggerExitComponents(std::vector<std::shared_ptr<GameObject>>& destObjVec);

	/// <summary>
	/// �R���|�[�l���g�̔�g���K�[�Փ˃C�x���g�Ăяo��(�ՓˊJ�n��
	/// </summary>
	/// <param name="destObjVec"></param>
	void OnCollisionEnterComponents(std::vector<std::shared_ptr<GameObject>>& destObjVec);
	/// <summary>
	/// �R���|�[�l���g�̔�g���K�[�Փ˃C�x���g�Ăяo��(�Փˌp����
	/// </summary>
	/// <param name="destObjVec"></param>
	void OnCollisionStayComponents(std::vector<std::shared_ptr<GameObject>>& destObjVec);
	/// <summary>
	/// �R���|�[�l���g�̔�g���K�[�Փ˃C�x���g�Ăяo��(�ՓˏI����
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
	/// �R���|�[�l���g���擾
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
	/// �R���|�[�l���g���擾
	/// </summary>
	/// <param name="pOut"></param>
	/// <returns>�����Ȃ�true , ���s�Ȃ�false</returns>
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
	/// �w�肵�����̃R���|�[�l���g�A�܂��͂��̔h���������Ă��邩�ǂ���
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
	/// �R���|�[�l���g��ǉ�
	/// </summary>
	template<class T>
	std::shared_ptr<T> AddComponent();
	template<class T, class... Ts>
	std::shared_ptr<T> AddComponent(Ts&&... params);
	/// <summary>
	/// �g�����X�t�H�[���̒ǉ�
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
	/// ���W�b�h�{�f�B�̒ǉ�
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
	/// ���Փ˔���̒ǉ�
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
	/// OBB�Փ˔���̒ǉ�
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
	/// �J�v�Z���Փ˔���̒ǉ�
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
	/// ��`�Փ˔���̒ǉ�
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
	/// �w��r�w�C�r�A�̎擾	���݂��Ȃ���ΐ���
	/// </summary>
	/// <returns></returns>
	template<class T>
	std::shared_ptr<T> GetBehavior();

	/// <summary>
	/// �r�w�C�r�A�}�b�v�ɑ��݂��邩�ǂ���
	/// </summary>
	template<class T>
	bool FindBehavior();

	void SetCreated(bool create) { m_bCreated = create; }

	/// <summary>
	/// ���̃I�u�W�F�N�g���Ǘ����Ă���}�l�[�W���[���擾
	/// </summary>
	/// <returns></returns>
	const std::shared_ptr<Stage> GetStage()const;

	/// <summary>
	/// �^�O��ǉ�
	/// </summary>
	/// <param name="tag"></param>
	void AddTag(const std::string& tag);

	/// <summary>
	/// �^�O�����܂ނ�
	/// </summary>
	/// <param name="tag"></param>
	/// <returns></returns>
	bool IsContainTag(const std::string& tag);

	/// <summary>
	/// �I�u�W�F�N�g�^�C�v���擾
	/// </summary>
	/// <returns></returns>
	const ObjectType GetObjectType()const;
	/// <summary>
	/// �w�肵���I�u�W�F�N�g�^�C�v���ǂ���
	/// </summary>
	/// <param name="objectType"></param>
	/// <returns></returns>
	bool IsObjectType(ObjectType objectType)const;

	/// <summary>
	/// ���������擾
	/// </summary>
	/// <returns></returns>
	const ProcessingOrder::GameObjectProcessingOrder GetProcessingOrder() { return m_processOrder; }

	/// <summary>
	/// �`��ɗ��p����J�������擾
	/// </summary>
	/// <returns></returns>
	const std::shared_ptr<Camera>& GetDrawCamera()const;

	/// <summary>
	/// �I�u�W�F�N�g��`�悷�邽�߂̃��C�g���擾
	/// </summary>
	/// <returns></returns>
	virtual const Light& GetDrawLight()const;

	/// <summary>
	/// 2D�`�悷��ۂ̎ˉe�s����擾
	/// </summary>
	virtual void Get2DDrawProjectionMatrix(Float4x4& projMat)const;

private:
	/// <summary>
	/// �r�w�C�r�A�}�b�v����T�����Ď擾
	/// </summary>
	/// <param name="typeIdx"></param>
	/// <returns></returns>
	std::shared_ptr<Behavior> SearchBehavior(std::type_index typeIdx)const;
	/// <summary>
	/// �r�w�C�r�A�}�b�v�ɒǉ��A�܂��͍X�V
	/// </summary>
	/// <param name="typeIdx"></param>
	/// <param name="pBehaveir"></param>
	void AddMakeBehavior(std::type_index typeIdx, const std::shared_ptr<Behavior> pBehaveir);

	/// <summary>
	/// �R���|�[�l���g���������Ń\�[�g
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
	bool       m_bActive;			// ���̃I�u�W�F�N�g���A�N�e�B�u��
	bool       m_bDrawActive;		// �`�悷�邩
	bool       m_bSpriteDraw;		// 2D�`�悷�邩
	bool       m_bAlpha;			// ���ߏ������s����
	bool       m_bCreated;			// ���̃I�u�W�F�N�g���쐬�ς݂�
	std::weak_ptr<Stage>	m_pStage;	// ������X�e�[�W
	ProcessingOrder::GameObjectProcessingOrder m_processOrder;	// ������
	int m_drawLayer;

private:
	std::map<std::type_index, std::shared_ptr<Behavior>> m_behaviorMap;

private:
	// �R�s�[�֎~
	GameObject(const GameObject&) = delete;
	GameObject& operator=(const GameObject&) = delete;
	// ���[�u�֎~
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
	// �������Ń\�[�g
	SortComponentsByOrder();

	return buff;
}

template<class T, class ...Ts>
inline std::shared_ptr<T> GameObject::AddComponent(Ts && ...params)
{
	auto buff = ObjectFactory::Create<T>(GetSharedPtr<GameObject>(), params...);
	m_componentVec.push_back(buff);
	// �������Ń\�[�g
	SortComponentsByOrder();

	return buff;
}

template<class T>
inline std::shared_ptr<T> GameObject::GetBehavior()
{
	// �r�w�C�r�A�}�b�v����T��
	auto ptr = SearchBehavior(std::type_index(typeid(T)));
	if (ptr)
	{
		auto retPtr = std::dynamic_pointer_cast<T>(ptr);
		if (retPtr != nullptr)
			return retPtr;	// ������ΕԂ�
		else
			return nullptr;	// �L���X�g�ł��Ȃ�
	}
	// ������Ȃ���ΐV�K�쐬
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
