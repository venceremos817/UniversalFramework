#pragma once

#include <GameObject.h>
#include <System/Resource/SharedResources.h>

class SceneInterface
{
public:
	/// <summary>
	/// 初期化処理
	/// コンポーネント追加等
	/// </summary>
	virtual void OnPreCreate() {}
	/// <summary>
	/// 初期化処理
	/// コンポーネント設定等
	/// </summary>
	virtual void OnCreate() {}
	/// <summary>
	/// 更新処理
	/// </summary>
	virtual void Update() {}
	/// <summary>
	/// 遅れアップデート
	/// </summary>
	virtual void LateUpDate() {}
	/// <summary>
	/// 描画処理
	/// </summary>
	virtual void Draw();
	/// <summary>
	/// 終了処理
	/// </summary>
	virtual void OnDestroy() {}

	/// <summary>
	/// 作成済みフラグ設定
	/// </summary>
	/// <param name="created"></param>
	void SetCreated(bool created);
	/// <summary>
	/// 作成済みかどうかを取得
	/// </summary>
	/// <returns></returns>
	bool IsCreated();

	/// <summary>
	/// ステージを設定
	/// </summary>
	/// <param name="stage"></param>
	void SetStage(const std::shared_ptr<Stage>& stage);
	/// <summary>
	/// ステージを取得
	/// </summary>
	/// <returns></returns>
	std::shared_ptr<Stage> GetStage()const;


protected:
	SceneInterface();
	virtual ~SceneInterface();

private:
	struct Param;
	std::unique_ptr<Param> m_param;
};
