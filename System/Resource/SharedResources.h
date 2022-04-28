#pragma once
#include <memory>
#include <XMFLOAT_EX.h>
#include <GameObject.h>


struct Viewport
{
	float topLeftX;
	float topLeftY;
	float width;
	float height;
	float minDepth;
	float maxDepth;
};
/// <summary>
/// ビューポートとシーサー矩形設定
/// </summary>
void ResetViewport(const Viewport& vp);


/// <summary>
/// カメラオブジェクト
/// </summary>
class Camera
	: public GameObject
{
public:
	struct ViewProjection
	{
		Float4x4 view;
		Float4x4 proj;
	};
	struct Angle
	{
		float y, z;
	};

public:
	Camera(const std::shared_ptr<Stage>& pStage);
	virtual ~Camera();

	void OnCreate()override;
	void LateUpdate()override;

	virtual void Destroy()override;

	//=== セッター ===
	void SetPos(const Float3& pos);
	void LookAt(const Float3& lookPos);
	void SetOffset(const Float3& offset);
	void SetViewport(const Viewport& viewport);


	//=== ゲッター ===
	const Float4x4& GetViewMatrix()const;
	const Float4x4& GetProjectionMatrix()const;

	const Float3& GetPos()const;
	const Float3& GetLook()const;
	const Float3 GetLookPos()const;
	const Float3& GetOffset()const;
	const Float3 GetLookDirection()const;
	const Float3& GetUp()const;
	float	GetFOVY()const;
	float	GetAspect()const;
	float	GetNearClip()const;
	float	GetFarClip()const;


	/// <summary>
	/// ビューマトリクス・プロジェクションマトリクスを計算
	/// </summary>
	void CalculateMatrix();

protected:
	struct Param;
	std::unique_ptr<Param> m_pParam;
};


/// <summary>
/// ビュー
/// </summary>
class ViewBase
{
protected:
	/// <summary>
	/// プロテクトコンストラクタ
	/// </summary>
	explicit ViewBase();
	/// <summary>
	/// プロテクトデストラクタ
	/// </summary>
	virtual ~ViewBase();

public:
	/// <summary>
	/// 更新処理が有効かどうか
	/// </summary>
	/// <returns></returns>
	bool IsUpdateActive()const;
	/// <summary>
	/// 更新処理が有効かどうか設定
	/// </summary>
	void SetUpdateActive(bool active);

	virtual void OnPreCreate() {}
	virtual void OnCreate() {}
	virtual void Update() {}

	virtual void SetCreated(bool b);

	/// <summary>
	/// 描画に使用するカメラを取得
	/// </summary>
	/// <returns></returns>
	virtual const std::shared_ptr<Camera>& GetTargetCamera()const = 0;

	/// <summary>
	/// 描画するビューポートを取得
	/// </summary>
	/// <returns></returns>
	virtual const Viewport& GetTargetViewport()const = 0;


private:
	struct Param;
	std::shared_ptr<Param> m_pParam;
};


/// <summary>
/// シングルビュー
/// </summary>
class SingleView
	: public ViewBase
{
public:
	explicit SingleView(const std::shared_ptr<Stage>& pStage);
	virtual ~SingleView();

	/// <summary>
	/// 更新処理
	/// </summary>
	virtual void Update()override;

	/// <summary>
	/// ビューポートを設定
	/// </summary>
	/// <param name="v"></param>
	void SetViewport(const Viewport& viewport);
	/// <summary>
	/// ビューポートを取得
	/// </summary>
	/// <returns></returns>
	const Viewport& GetViewport()const;
	/// <summary>
	/// カメラを設定
	/// </summary>
	const void SetCamera(const std::shared_ptr<Camera>& camera);
	/// <summary>
	/// カメラを取得
	/// </summary>
	virtual const std::shared_ptr<Camera>& GetTargetCamera()const override;
	/// <summary>
	/// ビューポートを取得
	/// </summary>
	virtual const Viewport& GetTargetViewport()const override;

private:
	struct Param;
	std::unique_ptr<Param> m_pParam;
};

/// <summary>
/// ライト構造体
/// </summary>
class Light
{
public:
	Float3 m_direction;	// ライトの向き
	Float4 m_diffuse;	// ディフューズ
	Float4 m_specular;	// スペキュラー
	Float4 m_ambient;	// アンビエント

	Light();
	Light(const Float3& dir, const Float4& dif, const Float4& spe,const Float4& amb);
	~Light() {}

	/// <summary>
	/// ライト位置からの向きを設定
	/// </summary>
	/// <param name="position">ライトの位置</param>
	void SetPositionToDirectional(const Float3& position);
};

/// <summary>
/// ライトオブジェクトベース
/// </summary>
class LightBase
	: public GameObject
{
public:

protected:
	/// <summary>
	/// プロテクトコンストラクタ
	/// </summary>
	/// <param name="pStage"></param>
	explicit LightBase(const std::shared_ptr<Stage>& pStage);
	/// <summary>
	/// プロテクトデストラクタ
	/// </summary>
	virtual ~LightBase();

public:
	/// <summary>
	/// 描画に使用するライトを取得
	/// </summary>
	/// <returns></returns>
	virtual const Light& GetTargetLight()const = 0;

private:
};


/// <summary>
/// シングルライト
/// </summary>
class SingleLight
	: public LightBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="pStage"></param>
	explicit SingleLight(const std::shared_ptr<Stage>& pStage);
	virtual ~SingleLight();

	virtual void Destroy()override;

	/// <summary>
	/// ライトを設定
	/// </summary>
	void SetLight(const Light& light);
	const Light& GetLight()const;
	Light& GetLight();
	/// <summary>
	/// 描画に使用するライトを取得
	/// </summary>
	const Light& GetTargetLight()const override;

private:
	struct Param;
	std::unique_ptr<Param> m_pParam;
};
