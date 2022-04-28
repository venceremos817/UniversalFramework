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
	/// ����������1
	/// </summary>
	virtual void OnPreCreate() {}
	/// <summary>
	/// ����������2
	/// </summary>
	virtual void OnCreate() {}
	/// <summary>
	/// �X�V����
	/// </summary>
	virtual void Update() {}
	/// <summary>
	/// �X�V����2
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
	/// �I������
	/// </summary>
	virtual void OnDestroy() {}

	/// <summary>
	/// �Q�[���I�u�W�F�N�g���擾
	/// </summary>
	/// <returns>���̃R���|�[�l���g���t������Ă���Q�[���I�u�W�F�N�g</returns>
	const std::shared_ptr<GameObject> GetGameObject() const
	{
		auto pGameObj = m_pParentGameobject.lock();
		if (pGameObj)
			return pGameObj;
		return nullptr;
	}

	/// <summary>
	/// �g���K�[�ՓˊJ�n���C�x���g
	/// </summary>
	/// <param name="destObjVec"></param>
	virtual void OnTriggerEnter(std::vector<std::shared_ptr<GameObject>>& destObjVec) {}
	/// <summary>
	/// �g���K�[�Փ˒��C�x���g
	/// </summary>
	/// <param name="destObjVec"></param>
	virtual void OnTriggerStay(std::vector<std::shared_ptr<GameObject>>& destObjVec) {}
	/// <summary>
	/// �g���K�[�ՓˏI�����C�x���g
	/// </summary>
	/// <param name="destObjVec"></param>
	virtual void OnTriggerExit(std::vector<std::shared_ptr<GameObject>>& destObjVec) {}
	/// <summary>
	/// �R���W�����Փˎ��C�x���g
	/// </summary>
	/// <param name="destObjVec"></param>
	virtual void OnCollisionEnter(std::vector<std::shared_ptr<GameObject>>& destObjVec) {}
	/// <summary>
	/// �R���W�����Փ˒��C�x���g
	/// </summary>
	/// <param name="destObjVec"></param>
	virtual void OnCollisionStay(std::vector<std::shared_ptr<GameObject>>& destObjVec) {}
	/// <summary>
	/// �R���W�����ՓˏI���C�x���g
	/// </summary>
	/// <param name="destObjVec"></param>
	virtual void OnCollisionExit(std::vector<std::shared_ptr<GameObject>>& destObjVec) {}

	/// <summary>
	/// ���������擾
	/// </summary>
	/// <returns></returns>
	ProcessingOrder::ComponentProcessingOrder GetProcessingOrder() { return m_processOrder; }
	/// <summary>
	/// ��������ݒ�
	/// </summary>
	/// <param name="order"></param>
	void SetProcessingOrder(ProcessingOrder::ComponentProcessingOrder order) { m_processOrder = order; }

	/// <summary>
	/// �L�����ǂ���
	/// </summary>
	/// <returns></returns>
	bool IsActive() { return m_bActive; }
	void SetActive(bool b) { m_bActive = b; }
	/// <summary>
	/// �������������ǂ����ݒ�
	/// </summary>
	/// <param name="created"></param>
	void SetCreated(bool created) { m_bCreated = created; }

	/// <summary>
	/// ���̃R���|�[�l���g��shared_ptr���擾
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
	ProcessingOrder::ComponentProcessingOrder m_processOrder;	// ������
	bool	m_bActive;
	bool	m_bCreated;
};


