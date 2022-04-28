#pragma once
#include <GameObject.h>
#include <SceneInterface.h>
#include <CollisionManager.h>


class Player;

/// <summary>
/// ステージ
/// ゲームオブジェクトを管理する
/// </summary>
class Stage
	: public std::enable_shared_from_this<Stage>
{
	//	Stage() = delete;
public:
	explicit Stage();
	~Stage();

	/// <summary>
	/// 前初期化処理
	/// </summary>
	virtual void OnPreCreate();
	/// <summary>
	/// 初期化処理
	/// </summary>
	virtual void OnCreate();
	/// <summary>
	/// ゲームオブジェクトの更新処理
	/// </summary>
	virtual void Update();
	/// <summary>
	/// ステージ固有の描画
	/// </summary>
	virtual void UniqueDraw() {}
	/// <summary>
	/// ゲームオブジェクトの描画
	/// </summary>
	void RenderStage();

	void SetCreated(bool b);
	bool IsCreated()const;

	/// <summary>
	/// ゲームオブジェクトを追加
	/// </summary>
	/// <param name="...params"></param>
	/// <returns></returns>
	template<class T, class... Ts>
	std::shared_ptr<T> AddGameObject(Ts&&... params)
	{
		// ゲームオブジェクトを作成
		auto obj = ObjectFactory::Create<T>(GetSharePtr(), params...);

		// シーンが作成済みかどうかで振り分け
		if (IsCreated())
		{
			// 追加待ちに入れる
			GetWaitAddObjectsVec().push_back(obj);
		}
		else
		{
			GetGameObjectsVec().push_back(obj);
			// 処理順でソート
			SortGameObjectsByOrder();
		}

		return obj;
	}

	/// <summary>
	/// 現在ステージに存在しているゲームオブジェクトの配列を取得
	/// </summary>
	/// <returns></returns>
	std::vector<std::shared_ptr<GameObject>>& GetGameObjectsVec()const;

	/// <summary>
	/// 作成済みゲームオブジェクトを追加
	/// </summary>
	void AddMakedGameObject(const std::shared_ptr<GameObject>& pGameObj);

	/// <summary>
	/// ゲームオブジェクトを削除
	/// </summary>
	/// <param name="pGameObj"></param>
	void RemoveGameObject(const std::shared_ptr<GameObject>& pGameObj);

	/// <summary>
	/// ゲームオブジェクトを処理順でソート
	/// </summary>
	void SortGameObjectsByOrder();

	/// <summary>
	/// すべてのゲームオブジェクトを削除
	/// </summary>
	void DestroyAllGameObjects();


	/// <summary>
	/// ビューを設定
	/// </summary>
	/// <param name="view"></param>
	void SetView(const std::shared_ptr<ViewBase>& view);
	/// <summary>
	/// ビューを取得
	/// </summary>
	/// <returns></returns>
	const std::shared_ptr<ViewBase>& GetView()const;

	/// <summary>
	/// ライトを設定
	/// </summary>
	/// <param name="light"></param>
	void SetLight(const std::shared_ptr<LightBase>& light);
	/// <summary>
	/// ライトを取得
	/// </summary>
	/// <returns></returns>
	const std::shared_ptr<LightBase>& GetLight()const;


	/// <summary>
	/// ビューを作成
	/// </summary>
	/// <param name="...params">可変長パラメータ</param>
	/// <returns></returns>
	template<class T, class... Ts>
	std::shared_ptr<T> CreateView(Ts&&... params)
	{
		std::shared_ptr<T> pView = ObjectFactory::Create<T>(GetSharePtr(), params...);
		SetView(pView);
		return pView;
	}

	/// <summary>
	/// ライトを作成
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
	/// プレイヤーを取得
	/// </summary>
	/// <returns></returns>
	std::shared_ptr<Player> GetPlayer();

private:
	std::vector<std::shared_ptr<GameObject>>& GetWaitAddObjectsVec()const;
	std::vector<std::shared_ptr<GameObject>>& GetWaitRemoveObjectsVec()const;

	/// <summary>
	/// 追加待ちのゲームオブジェクトをシーンに追加
	/// </summary>
	void ResolveWaitAddObjects();

	/// <summary>
	/// 削除待ちのゲームオブジェクトを削除
	/// </summary>
	void ResolveWaitRemoveObjects();

	/// <summary>
	/// ステージ内オブジェクトを描画サブ
	/// </summary>
	void DrawStage();


	void CreateGround();

private:
	/// <summary>
	/// このステージのshared_ptrを取得
	/// </summary>
	/// <returns></returns>
	std::shared_ptr<Stage> GetSharePtr();

private:
	struct Param;
	std::unique_ptr<Param> m_param;
};
