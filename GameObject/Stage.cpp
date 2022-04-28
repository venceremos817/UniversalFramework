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
	std::vector<std::shared_ptr<GameObject>> m_gameObjectVec;				// 現在存在しているゲームオブジェクト
	std::vector<std::shared_ptr<GameObject>> m_waitAddObjectVec;			// 追加待ちのゲームオブジェクト
	std::vector<std::shared_ptr<GameObject>> m_waitRemoveObjectVec;			// 削除待ちのゲームオブジェクト

	std::shared_ptr<ViewBase> m_pViewBase;		// ビューの参照
	std::shared_ptr<LightBase> m_pLightBase;	// ライトの参照

	std::weak_ptr<Player> m_pPlayer;	// プレイヤー
	std::shared_ptr<CollisionManager> m_pCollisionManager;		// コリジョンマネージャー

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
	// コリジョンマネージャー作成
	m_param->m_pCollisionManager = AddGameObject<CollisionManager>();
}

void Stage::OnCreate()
{
	// ビュー作成
	auto pView = CreateView<SingleView>();
	// カメラ作成
	auto pCamera = AddGameObject<PlayerCamera>();
	pView->SetCamera(pCamera);
	pCamera->SetPos(Float3(0.0f, 10.0f, -10.0f));
	pCamera->LookAt(Float3(0.0f));
	// ライト作成
	auto pLight = CreateLight<SingleLight>();
	// プレイヤー作成
	auto player = AddGameObject<Player>();
	pCamera->SetPrimaryTarget(player);
	m_param->m_pPlayer = player;
	// エネミー作成
	auto enemy = AddGameObject<Enemy>();
	for (int i = 0; i < 10; ++i)
	{
		enemy = AddGameObject<Enemy>();
		enemy->GetTransform()->ResetWorldPos(Float3((rand() % 30 - 30.0f), 10.0f, (rand() % 30 - 30.0f)));
	}
	// 地面作成
	CreateGround();
	// レティクル作成
	auto reticle = AddGameObject<Reticle>();
	// スカイドーム作成
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

	// 削除待ちオブジェクトの削除
	ResolveWaitRemoveObjects();
	// 追加待ちオブジェクトの追加
	ResolveWaitAddObjects();

	// Transform,RigidBodyのパラメータ退避
	for (auto obj : GetGameObjectsVec())
	{
		if (obj->IsActive())
		{
			obj->GetTransform()->SetToBefore();
			if (obj->m_pRigidBody)
				obj->m_pRigidBody->SetForce(0.0f, 0.0f, 0.0f);
		}
	}
	// オブジェクトの更新
	for (auto obj : GetGameObjectsVec())
	{
		if (obj->IsActive())
			obj->Update();
	}
	// コンポーネントの更新
	for (auto obj : GetGameObjectsVec())
	{
		if (obj->IsActive())
			obj->UpdateComponents();
	}

	// 衝突判定
	if (m_param->m_pCollisionManager)
	{
		m_param->m_pCollisionManager->CollisionUpdate();
		// 衝突イベント
		m_param->m_pCollisionManager->SendCollisionMessage();
	}

	// オブジェクトのLateUpdate
	for (auto obj : GetGameObjectsVec())
	{
		if (obj->IsActive())
			obj->LateUpdate();
	}
	// コンポーネントのLateUpdate
	for (auto obj : GetGameObjectsVec())
	{
		if (obj->IsActive())
			obj->LateUpdateComponents();
	}
	// 衝突情報をリセット
	for (auto obj : GetGameObjectsVec())
	{
		if (obj->IsActive())
			obj->CollisionReset();
	}

	// Effekseer更新
	APP->GetEffekseerManagerRef()->Update();
}

void Stage::RenderStage()
{
	auto dev = APP->GetDeviceResources();

	//--- デフォルト描画開始 ---
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
		// 既に登録済みならリターン
		if (obj == pGameObj)
			return;
	}

	// シーンが作成済みかどうかで振り分け
	if (m_param->m_bCreated)
		m_param->m_waitAddObjectVec.push_back(pGameObj);
	else
	{
		m_param->m_gameObjectVec.push_back(pGameObj);
		// 処理順でソート
		SortGameObjectsByOrder();
	}
}

void Stage::RemoveGameObject(const std::shared_ptr<GameObject>& pGameObj)
{
	for (auto obj : m_param->m_waitRemoveObjectVec)
	{
		// 既に登録済みならリターン
		if (obj == pGameObj)
			return;
	}

	// シーンが作成済みかどうかで振り分け
	if (m_param->m_bCreated)
		m_param->m_waitRemoveObjectVec.push_back(pGameObj);
	else
	{
		// 現在存在するゲームオブジェクトから探索
		for (auto it = m_param->m_gameObjectVec.begin(); it != m_param->m_gameObjectVec.end(); ++it)
		{
			if (*it == pGameObj)
			{
				// 削除
				pGameObj->OnDestroy();
				m_param->m_gameObjectVec.erase(it);
				break;
			}
		}
	}
}

void Stage::SortGameObjectsByOrder()
{
	// 処理順でソート
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
	// 追加待ちがなければリターン
	if (m_param->m_waitAddObjectVec.empty())
		return;

	for (auto waitObj : m_param->m_waitAddObjectVec)
	{
		m_param->m_gameObjectVec.push_back(waitObj);
	}

	// 処理順でソート
	SortGameObjectsByOrder();

	// 追加待ち配列をクリア
	m_param->m_waitAddObjectVec.clear();
}

void Stage::ResolveWaitRemoveObjects()
{
	// 削除待ちがなければリターン
	if (m_param->m_waitRemoveObjectVec.empty())
		return;

	for (auto waitObj : m_param->m_waitRemoveObjectVec)
	{
		// 現在存在するゲームオブジェクトから探索
		auto it = m_param->m_gameObjectVec.begin();
		while (it != m_param->m_gameObjectVec.end())
		{
			if (*it == waitObj)
			{
				// 削除
				waitObj->OnDestroy();
				m_param->m_gameObjectVec.erase(it);
				break;
			}
			++it;
		}
	}
	//m_param->m_gameObjectVec.shrink_to_fit();
	// 削除待ち配列をクリア
	m_param->m_waitRemoveObjectVec.clear();
}

void Stage::DrawStage()
{
	std::vector<std::shared_ptr<GameObject>> objSpriteVec;	// 2D描画オブジェクト
	std::vector<std::shared_ptr<GameObject>> obj3DNormalVec;	// 不透明3Dオブジェクト
	std::vector<std::shared_ptr<GameObject>> obj3DAlphaVec;	// 透明3Dオブジェクト

	// レイヤでソート
	std::set<int> layers;
	for (auto obj : m_param->m_gameObjectVec)
	{
		if (obj->IsDrawActive())
		{
			layers.insert(obj->GetDrawLayer());
			// 2D表示と3D表示を分ける
			if (/*obj->HasComponent<SpriteBase>() ||*/ obj->IsSpriteDraw())
			{
				objSpriteVec.push_back(obj);
			}
			else
			{
				// 透明/不透明で分ける
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
	// 透明オブジェクトをカメラ距離でソート
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

	// 描画準備
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

	// スプライトはZ座標でソート
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
		// 非透過3Dオブジェクト描画
		for (auto obj : obj3DNormalVec)
		{
			if (obj->GetDrawLayer() == layer)
			{
				obj->Draw();
				obj->DrawComponents();
			}
		}
		// 透過3Dオブジェクト描画
		for (auto obj : obj3DAlphaVec)
		{
			if (obj->GetDrawLayer() == layer)
			{
				obj->Draw();
				obj->DrawComponents();
			}
		}

		// Effekseerエフェクト描画
		APP->SetEffekseerCameraMatrix(GetView()->GetTargetCamera());
		auto effekseerRenderer = APP->GetEffekseerRendererRef();
		effekseerRenderer->BeginRendering();
		APP->GetEffekseerManagerRef()->Draw();
		effekseerRenderer->EndRendering();

		// スプライト描画
		for (auto obj : objSpriteVec)
		{
			if (obj->GetDrawLayer() == layer)
			{
				obj->Draw();
				obj->DrawComponents();
			}
		}
	}



	// ステージ固有の描画呼び出し
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
	// 追加待ちを削除
	while (!m_param->m_waitAddObjectVec.empty())
	{
		auto it = m_param->m_waitAddObjectVec.begin();
		(*it)->OnDestroy();
		m_param->m_waitAddObjectVec.erase(it);
	}
	m_param->m_waitAddObjectVec.clear();

	// 削除待ちを削除
	ResolveWaitRemoveObjects();

	// 現在存在しているオブジェクトを削除
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
