#pragma once
#include <GameObject.h>
#include <SceneInterface.h>
#include <CollisionManager.h>


class Player;

/// <summary>
/// �X�e�[�W
/// �Q�[���I�u�W�F�N�g���Ǘ�����
/// </summary>
class Stage
	: public std::enable_shared_from_this<Stage>
{
	//	Stage() = delete;
public:
	explicit Stage();
	~Stage();

	/// <summary>
	/// �O����������
	/// </summary>
	virtual void OnPreCreate();
	/// <summary>
	/// ����������
	/// </summary>
	virtual void OnCreate();
	/// <summary>
	/// �Q�[���I�u�W�F�N�g�̍X�V����
	/// </summary>
	virtual void Update();
	/// <summary>
	/// �X�e�[�W�ŗL�̕`��
	/// </summary>
	virtual void UniqueDraw() {}
	/// <summary>
	/// �Q�[���I�u�W�F�N�g�̕`��
	/// </summary>
	void RenderStage();

	void SetCreated(bool b);
	bool IsCreated()const;

	/// <summary>
	/// �Q�[���I�u�W�F�N�g��ǉ�
	/// </summary>
	/// <param name="...params"></param>
	/// <returns></returns>
	template<class T, class... Ts>
	std::shared_ptr<T> AddGameObject(Ts&&... params)
	{
		// �Q�[���I�u�W�F�N�g���쐬
		auto obj = ObjectFactory::Create<T>(GetSharePtr(), params...);

		// �V�[�����쐬�ς݂��ǂ����ŐU�蕪��
		if (IsCreated())
		{
			// �ǉ��҂��ɓ����
			GetWaitAddObjectsVec().push_back(obj);
		}
		else
		{
			GetGameObjectsVec().push_back(obj);
			// �������Ń\�[�g
			SortGameObjectsByOrder();
		}

		return obj;
	}

	/// <summary>
	/// ���݃X�e�[�W�ɑ��݂��Ă���Q�[���I�u�W�F�N�g�̔z����擾
	/// </summary>
	/// <returns></returns>
	std::vector<std::shared_ptr<GameObject>>& GetGameObjectsVec()const;

	/// <summary>
	/// �쐬�ς݃Q�[���I�u�W�F�N�g��ǉ�
	/// </summary>
	void AddMakedGameObject(const std::shared_ptr<GameObject>& pGameObj);

	/// <summary>
	/// �Q�[���I�u�W�F�N�g���폜
	/// </summary>
	/// <param name="pGameObj"></param>
	void RemoveGameObject(const std::shared_ptr<GameObject>& pGameObj);

	/// <summary>
	/// �Q�[���I�u�W�F�N�g���������Ń\�[�g
	/// </summary>
	void SortGameObjectsByOrder();

	/// <summary>
	/// ���ׂẴQ�[���I�u�W�F�N�g���폜
	/// </summary>
	void DestroyAllGameObjects();


	/// <summary>
	/// �r���[��ݒ�
	/// </summary>
	/// <param name="view"></param>
	void SetView(const std::shared_ptr<ViewBase>& view);
	/// <summary>
	/// �r���[���擾
	/// </summary>
	/// <returns></returns>
	const std::shared_ptr<ViewBase>& GetView()const;

	/// <summary>
	/// ���C�g��ݒ�
	/// </summary>
	/// <param name="light"></param>
	void SetLight(const std::shared_ptr<LightBase>& light);
	/// <summary>
	/// ���C�g���擾
	/// </summary>
	/// <returns></returns>
	const std::shared_ptr<LightBase>& GetLight()const;


	/// <summary>
	/// �r���[���쐬
	/// </summary>
	/// <param name="...params">�ϒ��p�����[�^</param>
	/// <returns></returns>
	template<class T, class... Ts>
	std::shared_ptr<T> CreateView(Ts&&... params)
	{
		std::shared_ptr<T> pView = ObjectFactory::Create<T>(GetSharePtr(), params...);
		SetView(pView);
		return pView;
	}

	/// <summary>
	/// ���C�g���쐬
	/// </summary>
	/// <param name="...params"></param>
	/// <returns></returns>
	template<class T, class... Ts>
	std::shared_ptr<T> CreateLight(Ts&&... params)
	{
		auto pLight = ObjectFactory::Create<T>(GetSharePtr(), params...);
		SetLight(pLight);
		return pLight;
	}

	/// <summary>
	/// �v���C���[���擾
	/// </summary>
	/// <returns></returns>
	std::shared_ptr<Player> GetPlayer();

private:
	std::vector<std::shared_ptr<GameObject>>& GetWaitAddObjectsVec()const;
	std::vector<std::shared_ptr<GameObject>>& GetWaitRemoveObjectsVec()const;

	/// <summary>
	/// �ǉ��҂��̃Q�[���I�u�W�F�N�g���V�[���ɒǉ�
	/// </summary>
	void ResolveWaitAddObjects();

	/// <summary>
	/// �폜�҂��̃Q�[���I�u�W�F�N�g���폜
	/// </summary>
	void ResolveWaitRemoveObjects();

	/// <summary>
	/// �X�e�[�W���I�u�W�F�N�g��`��T�u
	/// </summary>
	void DrawStage();


	void CreateGround();

private:
	/// <summary>
	/// ���̃X�e�[�W��shared_ptr���擾
	/// </summary>
	/// <returns></returns>
	std::shared_ptr<Stage> GetSharePtr();

private:
	struct Param;
	std::unique_ptr<Param> m_param;
};
