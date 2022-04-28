#include "Stage.h"
#include <algorithm>
#include <set>
#include <App/App.h>
#include <SceneInterface.h>
#include <Camera/PlayerCamera.h>
#include <Player/Player.h>
#include <Enemy/Enemy.h>
#include <Ground.h>
#include <Sprite/Reticle.h>
#include <SkyDome.h>
#include <DrawComponent/DrawComponent.h>
//#include <System/Resource/SharedResources.h>
#include "SampleObject.h"

#define DEBUG_OBJECT
#ifdef DEBUG_OBJECT
#include <DebugObject.h>
#endif // _DEBUG



struct Stage::Param
{
	bool m_bCreated;
	std::vector<std::shared_ptr<GameObject>> m_gameObjectVec;				// ���ݑ��݂��Ă���Q�[���I�u�W�F�N�g
	std::vector<std::shared_ptr<GameObject>> m_waitAddObjectVec;			// �ǉ��҂��̃Q�[���I�u�W�F�N�g
	std::vector<std::shared_ptr<GameObject>> m_waitRemoveObjectVec;			// �폜�҂��̃Q�[���I�u�W�F�N�g

	std::shared_ptr<ViewBase> m_pViewBase;		// �r���[�̎Q��
	std::shared_ptr<LightBase> m_pLightBase;	// ���C�g�̎Q��

	std::weak_ptr<Player> m_pPlayer;	// �v���C���[
	std::shared_ptr<CollisionManager> m_pCollisionManager;		// �R���W�����}�l�[�W���[

	Param() {}
};

Stage::Stage()
	: m_param(new Param())
{
}

Stage::~Stage()
{
	DestroyAllGameObjects();
}

void Stage::OnPreCreate()
{
	// �R���W�����}�l�[�W���[�쐬
	m_param->m_pCollisionManager = AddGameObject<CollisionManager>();
}

void Stage::OnCreate()
{
	// �r���[�쐬
	auto pView = CreateView<SingleView>();
	// �J�����쐬
	auto pCamera = AddGameObject<PlayerCamera>();
	pView->SetCamera(pCamera);
	pCamera->SetPos(Float3(0.0f, 10.0f, -10.0f));
	pCamera->LookAt(Float3(0.0f));
	// ���C�g�쐬
	auto pLight = CreateLight<SingleLight>();
	// �v���C���[�쐬
	auto player = AddGameObject<Player>();
	pCamera->SetPrimaryTarget(player);
	m_param->m_pPlayer = player;
	// �G�l�~�[�쐬
	auto enemy = AddGameObject<Enemy>();
	for (int i = 0; i < 10; ++i)
	{
		enemy = AddGameObject<Enemy>();
		enemy->GetTransform()->ResetWorldPos(Float3((rand() % 30 - 30.0f), 10.0f, (rand() % 30 - 30.0f)));
	}
	// �n�ʍ쐬
	CreateGround();
	// ���e�B�N���쐬
	auto reticle = AddGameObject<Reticle>();
	// �X�J�C�h�[���쐬
	auto skyDome = AddGameObject<SkyDome>();

	//AddGameObject<SampleObject>();

#ifdef DEBUG_OBJECT
	AddGameObject<DebugSprite>();
#endif // _DEBUG
}

void Stage::Update()
{
	//DirectX::XMMATRIX world = DirectX::XMMatrixIdentity();
	//SetWorld(world);

	// �폜�҂��I�u�W�F�N�g�̍폜
	ResolveWaitRemoveObjects();
	// �ǉ��҂��I�u�W�F�N�g�̒ǉ�
	ResolveWaitAddObjects();

	// Transform,RigidBody�̃p�����[�^�ޔ�
	for (auto obj : GetGameObjectsVec())
	{
		if (obj->IsActive())
		{
			obj->GetTransform()->SetToBefore();
			if (obj->m_pRigidBody)
				obj->m_pRigidBody->SetForce(0.0f, 0.0f, 0.0f);
		}
	}
	// �I�u�W�F�N�g�̍X�V
	for (auto obj : GetGameObjectsVec())
	{
		if (obj->IsActive())
			obj->Update();
	}
	// �R���|�[�l���g�̍X�V
	for (auto obj : GetGameObjectsVec())
	{
		if (obj->IsActive())
			obj->UpdateComponents();
	}

	// �Փ˔���
	if (m_param->m_pCollisionManager)
	{
		m_param->m_pCollisionManager->CollisionUpdate();
		// �Փ˃C�x���g
		m_param->m_pCollisionManager->SendCollisionMessage();
	}

	// �I�u�W�F�N�g��LateUpdate
	for (auto obj : GetGameObjectsVec())
	{
		if (obj->IsActive())
			obj->LateUpdate();
	}
	// �R���|�[�l���g��LateUpdate
	for (auto obj : GetGameObjectsVec())
	{
		if (obj->IsActive())
			obj->LateUpdateComponents();
	}
	// �Փˏ������Z�b�g
	for (auto obj : GetGameObjectsVec())
	{
		if (obj->IsActive())
			obj->CollisionReset();
	}

	// Effekseer�X�V
	APP->GetEffekseerManagerRef()->Update();
}

void Stage::RenderStage()
{
	auto dev = APP->GetDeviceResources();

	//--- �f�t�H���g�`��J�n ---
	dev->StartDefaultDraw();
	ResetViewport(GetView()->GetTargetViewport());
	DrawStage();
	dev->EndDefaultDraw();
}

void Stage::SetCreated(bool b)
{
	m_param->m_bCreated = b;
}

bool Stage::IsCreated() const
{
	return m_param->m_bCreated;
}

void Stage::AddMakedGameObject(const std::shared_ptr<GameObject>& pGameObj)
{
	for (auto obj : m_param->m_waitAddObjectVec)
	{
		// ���ɓo�^�ς݂Ȃ烊�^�[��
		if (obj == pGameObj)
			return;
	}

	// �V�[�����쐬�ς݂��ǂ����ŐU�蕪��
	if (m_param->m_bCreated)
		m_param->m_waitAddObjectVec.push_back(pGameObj);
	else
	{
		m_param->m_gameObjectVec.push_back(pGameObj);
		// �������Ń\�[�g
		SortGameObjectsByOrder();
	}
}

void Stage::RemoveGameObject(const std::shared_ptr<GameObject>& pGameObj)
{
	for (auto obj : m_param->m_waitRemoveObjectVec)
	{
		// ���ɓo�^�ς݂Ȃ烊�^�[��
		if (obj == pGameObj)
			return;
	}

	// �V�[�����쐬�ς݂��ǂ����ŐU�蕪��
	if (m_param->m_bCreated)
		m_param->m_waitRemoveObjectVec.push_back(pGameObj);
	else
	{
		// ���ݑ��݂���Q�[���I�u�W�F�N�g����T��
		for (auto it = m_param->m_gameObjectVec.begin(); it != m_param->m_gameObjectVec.end(); ++it)
		{
			if (*it == pGameObj)
			{
				// �폜
				pGameObj->OnDestroy();
				m_param->m_gameObjectVec.erase(it);
				break;
			}
		}
	}
}

void Stage::SortGameObjectsByOrder()
{
	// �������Ń\�[�g
	auto func = [&](std::shared_ptr<GameObject> left, std::shared_ptr<GameObject> right)->bool
	{
		return (left->GetProcessingOrder() < right->GetProcessingOrder());
	};
	std::sort(m_param->m_gameObjectVec.begin(), m_param->m_gameObjectVec.end(), func);
}

std::vector<std::shared_ptr<GameObject>>& Stage::GetGameObjectsVec() const
{
	return m_param->m_gameObjectVec;
}

std::shared_ptr<Player> Stage::GetPlayer()
{
	return m_param->m_pPlayer.lock();
}

std::vector <std::shared_ptr<GameObject>>& Stage::GetWaitAddObjectsVec() const
{
	return m_param->m_waitAddObjectVec;
}

std::vector<std::shared_ptr<GameObject>>& Stage::GetWaitRemoveObjectsVec() const
{
	return m_param->m_waitRemoveObjectVec;
}

void Stage::ResolveWaitAddObjects()
{
	// �ǉ��҂����Ȃ���΃��^�[��
	if (m_param->m_waitAddObjectVec.empty())
		return;

	for (auto waitObj : m_param->m_waitAddObjectVec)
	{
		m_param->m_gameObjectVec.push_back(waitObj);
	}

	// �������Ń\�[�g
	SortGameObjectsByOrder();

	// �ǉ��҂��z����N���A
	m_param->m_waitAddObjectVec.clear();
}

void Stage::ResolveWaitRemoveObjects()
{
	// �폜�҂����Ȃ���΃��^�[��
	if (m_param->m_waitRemoveObjectVec.empty())
		return;

	for (auto waitObj : m_param->m_waitRemoveObjectVec)
	{
		// ���ݑ��݂���Q�[���I�u�W�F�N�g����T��
		auto it = m_param->m_gameObjectVec.begin();
		while (it != m_param->m_gameObjectVec.end())
		{
			if (*it == waitObj)
			{
				// �폜
				waitObj->OnDestroy();
				m_param->m_gameObjectVec.erase(it);
				break;
			}
			++it;
		}
	}
	//m_param->m_gameObjectVec.shrink_to_fit();
	// �폜�҂��z����N���A
	m_param->m_waitRemoveObjectVec.clear();
}

void Stage::DrawStage()
{
	std::vector<std::shared_ptr<GameObject>> objSpriteVec;	// 2D�`��I�u�W�F�N�g
	std::vector<std::shared_ptr<GameObject>> obj3DNormalVec;	// �s����3D�I�u�W�F�N�g
	std::vector<std::shared_ptr<GameObject>> obj3DAlphaVec;	// ����3D�I�u�W�F�N�g

	// ���C���Ń\�[�g
	std::set<int> layers;
	for (auto obj : m_param->m_gameObjectVec)
	{
		if (obj->IsDrawActive())
		{
			layers.insert(obj->GetDrawLayer());
			// 2D�\����3D�\���𕪂���
			if (/*obj->HasComponent<SpriteBase>() ||*/ obj->IsSpriteDraw())
			{
				objSpriteVec.push_back(obj);
			}
			else
			{
				// ����/�s�����ŕ�����
				if (obj->IsAlpha())
				{
					obj3DAlphaVec.push_back(obj);
				}
				else
				{
					obj3DNormalVec.push_back(obj);
				}
			}
		}
	}

	auto camera = m_param->m_pViewBase->GetTargetCamera();
	// �����I�u�W�F�N�g���J���������Ń\�[�g
	const Float3 cameraPos = camera->GetPos();
	auto sortFunc = [&](std::shared_ptr<GameObject> left, std::shared_ptr<GameObject> right)->bool
	{
		Float3 leftPos = left->GetTransform()->GetWorldPos();
		Float3 rightPos = right->GetTransform()->GetWorldPos();

		float leftDistance = Float3::Distance(leftPos, cameraPos);
		float rightDistance = Float3::Distance(rightPos, cameraPos);

		return leftDistance < rightDistance;
	};
	std::sort(obj3DAlphaVec.begin(), obj3DAlphaVec.end(), sortFunc);

	// �`�揀��
	for (auto obj : obj3DNormalVec)
	{
		obj->PreDraw();
		obj->PreDrawComponents();
	}
	for (auto obj : obj3DAlphaVec)
	{
		obj->PreDraw();
		obj->PreDrawComponents();
	}
	for (auto obj : objSpriteVec)
	{
		obj->PreDraw();
		obj->PreDrawComponents();
	}

	// �X�v���C�g��Z���W�Ń\�[�g
	auto spriteSort = [&](std::shared_ptr<GameObject> left, std::shared_ptr<GameObject> right)->bool
	{
		Float3 leftPos = left->GetTransform()->GetWorldPos();
		Float3 rightPos = right->GetTransform()->GetWorldPos();
		float leftZ = leftPos.z;
		float rightZ = rightPos.z;

		return leftZ > rightZ;
	};
	std::sort(objSpriteVec.begin(), objSpriteVec.end(), spriteSort);


	for (auto layerIt = layers.begin(); layerIt != layers.end(); ++layerIt)
	{
		int layer = *layerIt;
		// �񓧉�3D�I�u�W�F�N�g�`��
		for (auto obj : obj3DNormalVec)
		{
			if (obj->GetDrawLayer() == layer)
			{
				obj->Draw();
				obj->DrawComponents();
			}
		}
		// ����3D�I�u�W�F�N�g�`��
		for (auto obj : obj3DAlphaVec)
		{
			if (obj->GetDrawLayer() == layer)
			{
				obj->Draw();
				obj->DrawComponents();
			}
		}

		// Effekseer�G�t�F�N�g�`��
		APP->SetEffekseerCameraMatrix(GetView()->GetTargetCamera());
		auto effekseerRenderer = APP->GetEffekseerRendererRef();
		effekseerRenderer->BeginRendering();
		APP->GetEffekseerManagerRef()->Draw();
		effekseerRenderer->EndRendering();

		// �X�v���C�g�`��
		for (auto obj : objSpriteVec)
		{
			if (obj->GetDrawLayer() == layer)
			{
				obj->Draw();
				obj->DrawComponents();
			}
		}
	}



	// �X�e�[�W�ŗL�̕`��Ăяo��
	UniqueDraw();
}

void Stage::CreateGround()
{
#ifdef _DEBUG
	const int tileNum = 5;
#else
	const int tileNum = 50;
#endif // _DEBUG
	const float tileSize = 100.0f;
	for (int x = 0; x < tileNum; ++x)
	{
		for (int z = 0; z < tileNum; ++z)
		{
			auto ground = AddGameObject<Ground>();
			ground->GetTransform()->SetWorldPos(Float3((x - tileNum * 0.5f)*tileSize, 0.0f, (z - tileNum * 0.5f)* tileSize));
		}
	}
}

std::shared_ptr<Stage> Stage::GetSharePtr()
{
	auto ptr = std::dynamic_pointer_cast<Stage>(shared_from_this());
	if (ptr)
		return ptr;
	return nullptr;
}

void Stage::DestroyAllGameObjects()
{
	// �ǉ��҂����폜
	while (!m_param->m_waitAddObjectVec.empty())
	{
		auto it = m_param->m_waitAddObjectVec.begin();
		(*it)->OnDestroy();
		m_param->m_waitAddObjectVec.erase(it);
	}
	m_param->m_waitAddObjectVec.clear();

	// �폜�҂����폜
	ResolveWaitRemoveObjects();

	// ���ݑ��݂��Ă���I�u�W�F�N�g���폜
	while (!m_param->m_gameObjectVec.empty())
	{
		auto it = m_param->m_gameObjectVec.begin();
		(*it)->OnDestroy();
		m_param->m_gameObjectVec.erase(it);
	}
	m_param->m_gameObjectVec.clear();


}

void Stage::SetView(const std::shared_ptr<ViewBase>& view)
{
	m_param->m_pViewBase = view;
}

const std::shared_ptr<ViewBase> & Stage::GetView() const
{
	return m_param->m_pViewBase;
}

void Stage::SetLight(const std::shared_ptr<LightBase>& light)
{
	m_param->m_pLightBase = light;
}

const std::shared_ptr<LightBase>& Stage::GetLight() const
{
	return m_param->m_pLightBase;
}
