#include "PlayerCamera.h"
#include <App.h>
#include <GameObject.h>
#include <Utility/Math_EX.h>
#include <Stage.h>

struct PlayerCamera::Param
{
	std::shared_ptr<Transform>	m_pPrimary;		// 追従するオブジェクトのトランスフォーム
	float latePosLerp;	// 追従遅れ係数
	float lateLookLerp;	// 注視点遅れ係数
	float radY;		// Y軸回転
	float radXZ;		// XZ平面での回転
	float sensiX;		// 感度X
	float sensiY;		// 感度Y
	float maxUpperRot;	// 上限角度
	float maxUnderRot;	// 下限角度
	float armLen;		// カメラと追従オブジェクトの距離
	float armMaxLen;	// アームの最大長
	float armMinLen;	// アームの最小長

	Param()
		: m_pPrimary(nullptr)
		, latePosLerp(0.5f)
		, lateLookLerp(0.5f)
		, radY(0.5f), radXZ(0.0f)
		, sensiX(1.0f), sensiY(0.5f)
		, maxUpperRot(DirectX::XM_PI / 3.0f)
		, maxUnderRot(-0.2f)
		, armLen(7.0f)
		, armMaxLen(20.0f)
		, armMinLen(0.1f)
	{}
};

PlayerCamera::PlayerCamera(const std::shared_ptr<Stage>& pStage)
	: Camera(pStage)
	, m_param(new Param)
{
	AddTag("playerCamera");
}

PlayerCamera::~PlayerCamera()
{
}

void PlayerCamera::LateUpdate()
{
	// ゲームパッド
	auto gamePad = INPUTDEVICE.GetGamePadState()[0];
	if (gamePad.IsConected())
	{
		// ゲームパッドによる回転
		OperationByGamePad(gamePad);
	}
	else
	{
		// マウスによる回転
		OperationByMouse();
	}
}

void PlayerCamera::Destroy()
{
	GetStage()->RemoveGameObject(GetSharedPtr<PlayerCamera>());
}

void PlayerCamera::SetPrimaryTarget(const std::shared_ptr<Transform>& pPrimary)
{
	m_param->m_pPrimary = pPrimary;
}

void PlayerCamera::SetPrimaryTarget(const std::shared_ptr<GameObject>& pPrimary)
{
	m_param->m_pPrimary = pPrimary->GetTransform();
}

void PlayerCamera::SetPrimaryTarget(const std::shared_ptr<Component>& pPrimary)
{
	m_param->m_pPrimary = pPrimary->GetGameObject()->GetTransform();
}

void PlayerCamera::SetLateLerp(float t)
{
	m_param->latePosLerp = t;
}

void PlayerCamera::SetSensiX(float sensiX)
{
	m_param->sensiX = sensiX;
}

void PlayerCamera::SetSensiY(float sensiY)
{
	m_param->sensiY = sensiY;
}

void PlayerCamera::SetArmLength(float length)
{
	m_param->armLen = length;
}

void PlayerCamera::SetArmMaxLength(float length)
{
	m_param->armMaxLen = length;
}

void PlayerCamera::SetArmMinLength(float length)
{
	m_param->armMinLen = length;
}

Float3 PlayerCamera::GetMoveForward()
{
	Float3 forward;
	forward = GetLookDirection();
	forward.y = 0.0f;
	return forward.Normalize();
}

Float3 PlayerCamera::GetMoveBack()
{
	return (GetMoveForward() * -1.0f);
}

Float3 PlayerCamera::GetMoveLeft()
{
	const Float3 vUP = Float3(0.0f, 1.0f, 0.0f);
	const Float3 vForward = GetMoveForward();
	Float3 vLeft = Float3::Cross(vForward, vUP);
	return vLeft;
}

Float3 PlayerCamera::GetMoveRight()
{
	return (GetMoveLeft() * -1.0f);
}

void PlayerCamera::OperationByGamePad(const GamePadState & gamePad)
{
	float elapsedTime = APP->GetElapsedTime();
	Float2 RStick = gamePad.GetThumbR();
	Float3 newPos = GetPos();
	Float3 newLook = GetLookPos();
	Float3 arm = newPos - newLook;
	arm.Normalize();

	// Y角度
	if (RStick.y >= 0.1f)
	{
		m_param->radY -= RStick.y * m_param->sensiY * elapsedTime;
	}
	else if (RStick.y <= -0.1f)
	{
		m_param->radY -= RStick.y * m_param->sensiY * elapsedTime;
	}
	if (m_param->radY >= m_param->maxUpperRot)
	{
		m_param->radY = m_param->maxUpperRot;
	}
	else if (m_param->radY <= m_param->maxUnderRot)
	{// 下限に行ったらそれ以上下がらないようにする
		m_param->radY = m_param->maxUnderRot;
	}
	arm.y = sin(m_param->radY);
	// Y回転軸を作成
	if (RStick.x != 0)
	{
		m_param->radXZ += RStick.x * elapsedTime * m_param->sensiX;
		// 1週したら0
		if (abs(m_param->radXZ) >= DirectX::XM_2PI)
			m_param->radXZ = 0.0f;
	}

	// クォータニオンでY回転
	Quaternion qtXZ;
	qtXZ.Rotaion(m_param->radXZ, Float3(0.0f, 1.0f, 0.0f));
	qtXZ.Normalize();
	// 移動先行の行列計算をすることで、回転後のXZ座標の値を取得
	Float4x4 mat;
	mat.strTransformation(
		Float3(1.0f),
		Float3(0.0f, 0.0f, -1.0f),
		qtXZ
	);
	Float3 posXZ = mat.ExtractTransform();
	arm.x = posXZ.x;
	arm.z = posXZ.z;
	arm.Normalize();

	if (m_param->m_pPrimary)
	{
		Float3 toPrimary = m_param->m_pPrimary->GetWorldPos();
		toPrimary += GetOffset();
		newLook = CalculateLerp(GetLook(), toPrimary, 0.0f, 1.0f, 1.0f);
	}

#ifdef _DEBUG
	auto key = INPUTDEVICE.GetKeybordMouseState();
	if (key.IsKeyPress(VK_DOWN))
	{
		m_param->armLen += 0.05f;
		if (m_param->armLen > m_param->armMaxLen)
			m_param->armLen = m_param->armMaxLen;
	}
	if (key.IsKeyPress(VK_UP))
	{
		m_param->armLen -= 0.05f;
		if (m_param->armLen < m_param->armMinLen)
			m_param->armLen = m_param->armMinLen;
	}
#endif // _DEBUG

	Float3 toCamera = newLook + arm * m_param->armLen;
	newPos = CalculateLerp(GetPos(), toCamera, 0.0f, 1.0f, m_param->latePosLerp);
	newLook = CalculateLerp(GetLook(), newLook, 0.0f, 1.0f, m_param->lateLookLerp);

	LookAt(newLook);
	SetPos(newPos);
}

void PlayerCamera::OperationByMouse()
{
	auto mouse = INPUTDEVICE.GetKeybordMouseState();
	POINT move = mouse.GetMouseMove();

	float elapsedTime = APP->GetElapsedTime();
	Float3 newPos = GetPos();
	Float3 newLook = GetLookPos();
	Float3 arm = newPos - newLook;
	arm.Normalize();

	// Y角度
	if (move.y >= 0.1f)
	{
		m_param->radY += move.y * m_param->sensiY * elapsedTime;
	}
	else if (move.y <= -0.1f)
	{
		m_param->radY += move.y * m_param->sensiY * elapsedTime;
	}
	if (m_param->radY >= m_param->maxUpperRot)
	{
		m_param->radY = m_param->maxUpperRot;
	}
	else if (m_param->radY <= m_param->maxUnderRot)
	{// 下限に行ったらそれ以上下がらないようにする
		m_param->radY = m_param->maxUnderRot;
	}
	arm.y = sin(m_param->radY);
	// Y回転軸を作成
	if (move.x != 0)
	{
		m_param->radXZ += move.x * elapsedTime * m_param->sensiX;
		// 1週したら0
		if (abs(m_param->radXZ) >= DirectX::XM_2PI)
			m_param->radXZ = 0.0f;
	}

	// クォータニオンでY回転
	Quaternion qtXZ;
	qtXZ.Rotaion(m_param->radXZ, Float3(0.0f, 1.0f, 0.0f));
	qtXZ.Normalize();
	// 移動先行の行列計算をすることで、回転後のXZ座標の値を取得
	Float4x4 mat;
	mat.strTransformation(
		Float3(1.0f),
		Float3(0.0f, 0.0f, -1.0f),
		qtXZ
	);
	Float3 posXZ = mat.ExtractTransform();
	arm.x = posXZ.x;
	arm.z = posXZ.z;
	arm.Normalize();

	if (m_param->m_pPrimary)
	{
		Float3 toPrimary = m_param->m_pPrimary->GetWorldPos();
		toPrimary += GetOffset();
		newLook = CalculateLerp(GetLook(), toPrimary, 0.0f, 1.0f, 1.0f);
	}

#ifdef _DEBUG
	if (mouse.IsKeyPress(VK_DOWN))
	{
		m_param->armLen += 0.05f;
		if (m_param->armLen > m_param->armMaxLen)
			m_param->armLen = m_param->armMaxLen;
	}
	if (mouse.IsKeyPress(VK_UP))
	{
		m_param->armLen -= 0.05f;
		if (m_param->armLen < m_param->armMinLen)
			m_param->armLen = m_param->armMinLen;
	}
#endif // _DEBUG

	newLook = CalculateLerp(GetLook(), newLook, 0.0f, 1.0f, m_param->lateLookLerp);
	Float3 toCamera = newLook + arm * m_param->armLen;
	newPos = CalculateLerp(GetPos(), toCamera, 0.0f, 1.0f, m_param->latePosLerp);

	LookAt(newLook);
	SetPos(newPos);
}



