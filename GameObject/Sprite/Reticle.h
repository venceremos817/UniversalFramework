#pragma once
#include <GameObject.h>

class Reticle
	: public GameObject
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="pStage"></param>
	explicit Reticle(std::shared_ptr<Stage>& pStage);
	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~Reticle();

	/// <summary>
	/// 初期化処理
	/// </summary>
	void OnCreate()override;
	/// <summary>
	/// 更新処理
	/// </summary>
	void Update()override;

	virtual void Destroy()override;

	/// <summary>
	/// カメラを設定
	/// </summary>
	/// <param name="camera"></param>
	void SetCamera(std::shared_ptr<Camera>& camera);

private:
	struct Param;
	std::unique_ptr<Param> m_param;
};