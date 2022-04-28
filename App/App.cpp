#include "App.h"
#include <string>
#include <time.h>

std::unique_ptr<App, App::AppDeleter> App::m_App;


std::unique_ptr<App, App::AppDeleter>& App::CreateApp(HINSTANCE hInstance, HWND hWnd, bool fullScreen, UINT width, UINT height)
{
	if (m_App.get() == 0)
	{
		// �������g�̍\�z
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
	// �d���|�C���^����
	for (it = m_resourceMap.begin(); it != m_resourceMap.end(); ++it)
	{
		if (it->second == resourceObj)
		{
			// �|�C���^���d�����Ă��Ă��L�[�������Ȃ�A���łɓo�^����Ă���̂Ń��^�[��
			if (it->first == key)
				return;
		}
	}
	// �d���L�[����
	it = m_resourceMap.find(key);
	if (it == m_resourceMap.end())
	{
		// ������Ȃ��̂Ń��\�[�X�y�A��ǉ�
		m_resourceMap[key] = resourceObj;
	}
}

std::shared_ptr<TextureResouce> App::RegisterTexture(const std::string & key, const std::string & textureFileName, const TextureResouce::FileExtension & texType)
{
	// ���łɓo�^����Ă���΂����Ԃ�
	if (CheckResource<TextureResouce>(key))
		return GetResource<TextureResouce>(key);
	// �Ȃ���΍쐬���ēo�^���A�Ԃ�
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
		// �V�[���������Ȃ�
		return;
	}

	// ���͍X�V
//	UpdateInput();
	m_InputDevice.ResetControllerState();

	// �V�[���I�u�W�F�N�g���X�V
	m_Timer.Tick([&]()
	{
		m_sceneInterface->Update();
	});
	// ����X�V���Ƀ����_�����O�͍s��Ȃ�
	if (GetFrameCount() == 0)
	{
		return;
	}

	// �`��
	m_sceneInterface->Draw();
	m_DeviceResources->Present(syncInterval, 0);
}

void App::SetEffekseerCameraMatrix(const std::shared_ptr<Camera>& camera)
{
	// �s��
	Float3 cameraPos = camera->GetPos();
	Float3 cameraLook = camera->GetLook();
	Float3 cameraUp = camera->GetUp();
	Effekseer::Vector3D eye = ::Effekseer::Vector3D(cameraPos.x, cameraPos.y, cameraPos.z);
	Effekseer::Vector3D look = ::Effekseer::Vector3D(cameraLook.x, cameraLook.y, cameraLook.z);
	Effekseer::Vector3D up = ::Effekseer::Vector3D(cameraUp.x, cameraUp.y, cameraUp.z);
	// ���e�s���ݒ�
	m_effekseerRendererRef->SetProjectionMatrix(
		::Effekseer::Matrix44().PerspectiveFovLH(
			camera->GetFOVY(), camera->GetAspect(), camera->GetNearClip(), camera->GetFarClip()
		)
	);
	// �J�����s���ݒ�
	m_effekseerRendererRef->SetCameraMatrix(::Effekseer::Matrix44().LookAtLH(eye, look, up));
}

std::shared_ptr<EffekseerEffectResource> App::RegisterEffect(const std::string & key, const std::u16string & fileName)
{
	// ���łɓo�^����Ă���΂����Ԃ�
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
		// ����������
		srand((unsigned)time(nullptr));
		// �f�o�C�X���\�[�X�\�z
		m_DeviceResources = std::shared_ptr<DeviceResources>(new DeviceResources(hWnd, fullScreen, width, height));

		// Effekseer������
		m_effekseerRendererRef = EffekseerRendererDX11::Renderer::Create(
			m_DeviceResources->GetD3DDevice(),
			m_DeviceResources->GetD3DDeviceContext(),
			8000);
		m_effekseerManagerRef = Effekseer::Manager::Create(8000);
		// �`�惂�W���[���̐ݒ�
		m_effekseerManagerRef->SetSpriteRenderer(m_effekseerRendererRef->CreateSpriteRenderer());
		m_effekseerManagerRef->SetRibbonRenderer(m_effekseerRendererRef->CreateRibbonRenderer());
		m_effekseerManagerRef->SetRingRenderer(m_effekseerRendererRef->CreateRingRenderer());
		m_effekseerManagerRef->SetTrackRenderer(m_effekseerRendererRef->CreateTrackRenderer());
		m_effekseerManagerRef->SetModelRenderer(m_effekseerRendererRef->CreateModelRenderer());
		// �e�N�X�`���A���f���A�J�[�u�A�}�e���A�����[�_�[�̐ݒ������
		// ���[�U�[���Ǝ��Ŋg���ł���B���݂̓t�@�C������ǂݍ���
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
	// Effekseer�j��
	m_effekseerManagerRef.Reset();
	m_effekseerRendererRef.Reset();
}
