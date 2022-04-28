#include "App.h"
#include <string>
#include <time.h>

std::unique_ptr<App, App::AppDeleter> App::m_App;


std::unique_ptr<App, App::AppDeleter>& App::CreateApp(HINSTANCE hInstance, HWND hWnd, bool fullScreen, UINT width, UINT height)
{
	if (m_App.get() == 0)
	{
		// 自分自身の構築
		m_App.reset(new App(hInstance, hWnd, fullScreen, width, height));
	}
	return m_App;
}

std::unique_ptr<App, App::AppDeleter>& App::GetApp()
{
	if (m_App.get() == 0)
	{
		throw;
	}
	return m_App;
}

void App::DeleteApp()
{
	if (m_App.get())
	{
		m_App->GetSceneInterface()->OnDestroy();
		m_App.reset();
	}
}

void App::RegisterResource(const std::string & key, const std::shared_ptr<BaseResource>& resourceObj)
{
	std::map<std::string, std::shared_ptr<BaseResource>>::iterator it;
	// 重複ポインタ調査
	for (it = m_resourceMap.begin(); it != m_resourceMap.end(); ++it)
	{
		if (it->second == resourceObj)
		{
			// ポインタが重複していてもキーが同じなら、すでに登録されているのでリターン
			if (it->first == key)
				return;
		}
	}
	// 重複キー調査
	it = m_resourceMap.find(key);
	if (it == m_resourceMap.end())
	{
		// 見つからないのでリソースペアを追加
		m_resourceMap[key] = resourceObj;
	}
}

std::shared_ptr<TextureResouce> App::RegisterTexture(const std::string & key, const std::string & textureFileName, const TextureResouce::FileExtension & texType)
{
	// すでに登録されていればそれを返す
	if (CheckResource<TextureResouce>(key))
		return GetResource<TextureResouce>(key);
	// なければ作成して登録し、返す
	auto pTexture = TextureResouce::CreateTextureResouce(textureFileName, texType);
	RegisterResource(key, pTexture);
	return pTexture;
}

float App::GetElapsedTime()
{
	double elapsedTime = m_Timer.GetElapsedSeconds();
	if (elapsedTime < 0.001f)
	{
		elapsedTime = 0.001f;
	}

	return (float)elapsedTime;
}

bool App::ResetInpuState(HWND hWnd, USE_KEY_VEC & useKeyVec)
{
	if (useKeyVec.size() == 0)
		return false;
	return m_InputDevice.ResetInputState(hWnd, useKeyVec);
}

void App::UpdateDraw(unsigned int syncInterval)
{
	if (!m_sceneInterface)
	{
		// シーンが無効なら
		return;
	}

	// 入力更新
//	UpdateInput();
	m_InputDevice.ResetControllerState();

	// シーンオブジェクトを更新
	m_Timer.Tick([&]()
	{
		m_sceneInterface->Update();
	});
	// 初回更新時にレンダリングは行わない
	if (GetFrameCount() == 0)
	{
		return;
	}

	// 描画
	m_sceneInterface->Draw();
	m_DeviceResources->Present(syncInterval, 0);
}

void App::SetEffekseerCameraMatrix(const std::shared_ptr<Camera>& camera)
{
	// 行列
	Float3 cameraPos = camera->GetPos();
	Float3 cameraLook = camera->GetLook();
	Float3 cameraUp = camera->GetUp();
	Effekseer::Vector3D eye = ::Effekseer::Vector3D(cameraPos.x, cameraPos.y, cameraPos.z);
	Effekseer::Vector3D look = ::Effekseer::Vector3D(cameraLook.x, cameraLook.y, cameraLook.z);
	Effekseer::Vector3D up = ::Effekseer::Vector3D(cameraUp.x, cameraUp.y, cameraUp.z);
	// 投影行列を設定
	m_effekseerRendererRef->SetProjectionMatrix(
		::Effekseer::Matrix44().PerspectiveFovLH(
			camera->GetFOVY(), camera->GetAspect(), camera->GetNearClip(), camera->GetFarClip()
		)
	);
	// カメラ行列を設定
	m_effekseerRendererRef->SetCameraMatrix(::Effekseer::Matrix44().LookAtLH(eye, look, up));
}

std::shared_ptr<EffekseerEffectResource> App::RegisterEffect(const std::string & key, const std::u16string & fileName)
{
	// すでに登録されていればそれを返す
	if (CheckResource<EffekseerEffectResource>(key))
		return GetResource<EffekseerEffectResource>(key);
	auto effect = EffekseerEffectResource::CreateEffekseerResource(fileName);
	RegisterResource(key, effect);
	return effect;
}

App::App(HINSTANCE hInstance, HWND hWnd, bool fullScreen, UINT width, UINT height)
	: m_hInstance(hInstance)
	, m_hWnd(hWnd)
	, m_bFullScreen(fullScreen)
	, m_ScreenWidth(width)
	, m_ScreenHeight(height)
	, m_Timer()
{
	try
	{
		// 乱数初期化
		srand((unsigned)time(nullptr));
		// デバイスリソース構築
		m_DeviceResources = std::shared_ptr<DeviceResources>(new DeviceResources(hWnd, fullScreen, width, height));

		// Effekseer初期化
		m_effekseerRendererRef = EffekseerRendererDX11::Renderer::Create(
			m_DeviceResources->GetD3DDevice(),
			m_DeviceResources->GetD3DDeviceContext(),
			8000);
		m_effekseerManagerRef = Effekseer::Manager::Create(8000);
		// 描画モジュールの設定
		m_effekseerManagerRef->SetSpriteRenderer(m_effekseerRendererRef->CreateSpriteRenderer());
		m_effekseerManagerRef->SetRibbonRenderer(m_effekseerRendererRef->CreateRibbonRenderer());
		m_effekseerManagerRef->SetRingRenderer(m_effekseerRendererRef->CreateRingRenderer());
		m_effekseerManagerRef->SetTrackRenderer(m_effekseerRendererRef->CreateTrackRenderer());
		m_effekseerManagerRef->SetModelRenderer(m_effekseerRendererRef->CreateModelRenderer());
		// テクスチャ、モデル、カーブ、マテリアルローダーの設定をする
		// ユーザーが独自で拡張できる。現在はファイルから読み込む
		m_effekseerManagerRef->SetTextureLoader(m_effekseerRendererRef->CreateTextureLoader());
		m_effekseerManagerRef->SetModelLoader(m_effekseerRendererRef->CreateModelLoader());
		m_effekseerManagerRef->SetMaterialLoader(m_effekseerRendererRef->CreateMaterialLoader());
		m_effekseerManagerRef->SetCurveLoader(Effekseer::MakeRefPtr<Effekseer::CurveLoader>());
	}
	catch (...)
	{
		throw;
	}
}

App::~App()
{
	// Effekseer破棄
	m_effekseerManagerRef.Reset();
	m_effekseerRendererRef.Reset();
}
