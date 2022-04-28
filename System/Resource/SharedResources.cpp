#include "SharedResources.h"
#include <App.h>
#include <Stage.h>

//===========================================
// カメラ
//===========================================
#pragma region Camera

struct Camera::Param
{
	Float3 m_pos;		// カメラ位置
	Float3 m_look;		// 注視点
	Float3 m_offset;	// オフセット
	Float3 m_up;		// アップベクトル
	bool m_pers;		// 遠近法を使うかどうか
	std::weak_ptr<GameObject> m_cameraObj;	// カメラ代わりのオブジェクト

	float m_fovY;		// 射影角度
	float m_asspect;	// アスペクト比

	float m_width;		// 遠近法を使わない場合の幅
	float m_height;		// 遠近法を使わない場合の高さ
	float m_near;		// ニアクリップ
	float m_far;		// ファークリップ

	Float4x4 m_viewMatrix;	// ビューマトリクス
	Float4x4 m_projMatrix;	// プロジェクションマトリクス

	Param()
		: m_pos(0.0f, 0.0f, -20.0f)
		, m_look(0.0f)
		, m_offset(0.0f, 2.0f, 0.0f)
		, m_up(0.0f, 1.0f, 0.0f)
		, m_pers(true)
		, m_fovY(DirectX::XM_PIDIV4)
		, m_asspect(1.0f)
		, m_width(0.0f)
		, m_height(0.0f)
		, m_near(0.5f)
#ifdef _DEBUG
		, m_far(100.0f)
#else
		, m_far(1000.0f)
#endif // _DEBUG
		, m_viewMatrix()
		, m_projMatrix()
	{
		m_width = (float)APP->GetScreenWidth();
		m_height = (float)APP->GetScreenHeight();
	}
	~Param() {}
};

Camera::Camera(const std::shared_ptr<Stage>& pStage)
	: m_pParam(new Param())
	, GameObject(pStage)
{
	m_processOrder = ProcessingOrder::GameObjectProcessingOrder::Camera;
}


Camera::~Camera()
{
}

void Camera::OnCreate()
{
	CalculateMatrix();
}

void Camera::LateUpdate()
{
	CalculateMatrix();
}

void Camera::Destroy()
{
	GetStage()->RemoveGameObject(GetSharedPtr<Camera>());
}

//=== セッター ===
void Camera::SetPos(const Float3& pos)
{
	m_pParam->m_pos = pos;
	GetTransform()->SetWorldPos(pos);
	CalculateMatrix();
}


void Camera::LookAt(const Float3& look)
{
	m_pParam->m_look = look;
	CalculateMatrix();
}

void Camera::SetOffset(const Float3& offset)
{
	m_pParam->m_offset = offset;
	CalculateMatrix();
}

void Camera::SetViewport(const Viewport & viewport)
{
	m_pParam->m_width = viewport.width;
	m_pParam->m_height = viewport.height;
	m_pParam->m_asspect = m_pParam->m_width / m_pParam->m_height;
	CalculateMatrix();
}


//=== ゲッター ===

/// <summary>
/// ビュープロジェクションマトリクスを取得
/// 転置してないので注意
/// </summary>
/// <returns></returns>
const Float4x4& Camera::GetViewMatrix()const
{
	return m_pParam->m_viewMatrix;
}

/// <summary>
/// プロジェクションマトリクスを取得
/// 転置してないので注意
/// </summary>
/// <returns></returns>
const Float4x4& Camera::GetProjectionMatrix()const
{
	return m_pParam->m_projMatrix;
}

const Float3& Camera::GetPos()const
{
	return m_pParam->m_pos;
}

const Float3 & Camera::GetLook() const
{
	return m_pParam->m_look;
}


const Float3 Camera::GetLookPos()const
{
	return m_pParam->m_look + m_pParam->m_offset;
}

const Float3 & Camera::GetOffset() const
{
	return m_pParam->m_offset;
}

const Float3 Camera::GetLookDirection()const
{
	return m_pParam->m_look - m_pParam->m_pos;
	//return GetLookPos() - GetPos();
}


const Float3& Camera::GetUp()const
{
	return m_pParam->m_up;
}


float Camera::GetFOVY()const
{
	return m_pParam->m_fovY;
}


float Camera::GetAspect()const
{
	return m_pParam->m_asspect;
}


float Camera::GetNearClip()const
{
	return m_pParam->m_near;
}


float Camera::GetFarClip()const
{
	return m_pParam->m_far;
}

void Camera::CalculateMatrix()
{
	if (auto pObj = m_pParam->m_cameraObj.lock())
	{
		auto pTransform = pObj->GetTransform();
		if (pTransform)
		{
			m_pParam->m_pos = pTransform->GetWorldPos() + m_pParam->m_offset;
			m_pParam->m_viewMatrix = DirectX::XMMatrixLookAtLH(m_pParam->m_pos, m_pParam->m_look, m_pParam->m_up);
			if (m_pParam->m_pers)
				m_pParam->m_projMatrix = DirectX::XMMatrixPerspectiveFovLH(m_pParam->m_fovY, m_pParam->m_asspect, m_pParam->m_near, m_pParam->m_far);
			else
				m_pParam->m_projMatrix = DirectX::XMMatrixOrthographicLH(m_pParam->m_width, m_pParam->m_height, m_pParam->m_near, m_pParam->m_far);
		}
	}
	else
	{
		if (m_pParam->m_pers)
		{
			m_pParam->m_viewMatrix = DirectX::XMMatrixLookAtLH(m_pParam->m_pos, m_pParam->m_look, m_pParam->m_up);
			m_pParam->m_projMatrix = DirectX::XMMatrixPerspectiveFovLH(m_pParam->m_fovY, m_pParam->m_asspect, m_pParam->m_near, m_pParam->m_far);
		}
		else
		{
			m_pParam->m_viewMatrix.Identity();
			m_pParam->m_projMatrix = DirectX::XMMatrixOrthographicLH(m_pParam->m_width, m_pParam->m_height, m_pParam->m_near, m_pParam->m_far);
		}
	}
}

#pragma endregion

//==========================================
// ビュー
//==========================================
#pragma region ViewBase

struct ViewItem
{
	std::shared_ptr<Camera> m_camera;
	Viewport m_viewPort;
	explicit ViewItem(const std::shared_ptr<Stage>& pStage)
	{
		m_viewPort.width = (float)APP->GetScreenWidth();
		m_viewPort.height = (float)APP->GetScreenHeight();
		m_viewPort.topLeftX = 0.0f;
		m_viewPort.topLeftY = 0.0f;
		m_viewPort.minDepth = 0.0f;
		m_viewPort.maxDepth = 1.0f;
		m_camera = ObjectFactory::Create<Camera>(pStage);
		m_camera->SetViewport(m_viewPort);
	}
};

struct ViewBase::Param
{
	bool m_updateActive;
	bool m_bCreated;

	explicit Param() :m_updateActive(true), m_bCreated(false) {}
	~Param() {}
};

ViewBase::ViewBase()
	: m_pParam(new Param())
{
}

ViewBase::~ViewBase()
{
}

bool ViewBase::IsUpdateActive() const
{
	return m_pParam->m_updateActive;
}

void ViewBase::SetUpdateActive(bool active)
{
	m_pParam->m_updateActive = active;
}

void ViewBase::SetCreated(bool b)
{
	m_pParam->m_bCreated = b;
}

#pragma endregion


//===================================
// シングルビュー
//===================================
#pragma region MyRegion
struct SingleView::Param
{
	ViewItem m_viewItem;
	Param(const std::shared_ptr<Stage>& pStage) :m_viewItem(pStage) {}
	~Param() {}
};

SingleView::SingleView(const std::shared_ptr<Stage>& pStage)
	: m_pParam(new Param(pStage))
{
}

SingleView::~SingleView()
{
}

void SingleView::Update()
{
	// カメラの計算
	m_pParam->m_viewItem.m_camera->CalculateMatrix();
}

void SingleView::SetViewport(const Viewport & viewport)
{
	m_pParam->m_viewItem.m_viewPort = viewport;
	m_pParam->m_viewItem.m_camera->SetViewport(viewport);
}

const Viewport & SingleView::GetViewport() const
{
	return m_pParam->m_viewItem.m_viewPort;
}

const void SingleView::SetCamera(const std::shared_ptr<Camera>& camera)
{
	m_pParam->m_viewItem.m_camera = camera;
	m_pParam->m_viewItem.m_camera->SetViewport(GetViewport());
	// カメラを計算
	m_pParam->m_viewItem.m_camera->CalculateMatrix();
}

const std::shared_ptr<Camera>& SingleView::GetTargetCamera() const
{
	return m_pParam->m_viewItem.m_camera;
}

const Viewport & SingleView::GetTargetViewport() const
{
	return m_pParam->m_viewItem.m_viewPort;
}

#pragma endregion


//============================================
// ライトオブジェクトベース
//============================================
#pragma region LightBase

//============================================
// ライト構造体
//============================================
#pragma region Light
Light::Light()
	: m_direction(0.0f, -1.0f, 0.0f)
	, m_diffuse(1.0f)
	, m_specular(1.0f)
	, m_ambient(0.0f)
{
}

Light::Light(const Float3 & dir, const Float4 & dif, const Float4 & spe, const Float4& amb)
	: m_direction(dir.normalized())
	, m_diffuse(dif)
	, m_specular(spe)
	, m_ambient(amb)
{
}

void Light::SetPositionToDirectional(const Float3 & position)
{
	m_direction = position * -1.0f;
	m_direction.Normalize();
}

#pragma endregion


LightBase::LightBase(const std::shared_ptr<Stage>& pStage)
	: GameObject(pStage)
{
}

LightBase::~LightBase()
{
}

#pragma endregion

//==================================================
// シングルライト
//==================================================
#pragma region MyRegion
struct SingleLight::Param
{
	Light m_light;
};


SingleLight::SingleLight(const std::shared_ptr<Stage>& pStage)
	: LightBase(pStage)
	, m_pParam(new Param())
{
}

SingleLight::~SingleLight()
{
}

void SingleLight::Destroy()
{
	GetStage()->RemoveGameObject(GetSharedPtr<SingleLight>());
}

void SingleLight::SetLight(const Light & light)
{
	m_pParam->m_light = light;
}

const Light& SingleLight::GetLight()const
{
	return m_pParam->m_light;
}

Light& SingleLight::GetLight()
{
	return m_pParam->m_light;
}

const Light & SingleLight::GetTargetLight() const
{
	return m_pParam->m_light;
}



#pragma endregion

void ResetViewport(const Viewport & vp)
{
	auto pID3D11DeviceContext = APP->GetDeviceResources()->GetD3DDeviceContext();
	D3D11_VIEWPORT vp11;
	vp11.Height = vp.height;
	vp11.Width = vp.width;
	vp11.MinDepth = vp.minDepth;
	vp11.MaxDepth = vp.maxDepth;
	vp11.TopLeftX = vp.topLeftX;
	vp11.TopLeftY = vp.topLeftY;
	pID3D11DeviceContext->RSSetViewports(1, &vp11);
	CD3D11_RECT rect(
		(LONG)vp11.TopLeftX,
		(LONG)vp11.TopLeftY,
		(LONG)vp11.TopLeftX + (LONG)vp11.Width,
		(LONG)vp11.TopLeftY + (LONG)vp11.Height
	);
	pID3D11DeviceContext->RSSetScissorRects(1, &rect);
}
