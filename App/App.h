#pragma once
//#include <Windows.h>
#include <memory>

#include <ThrowException.h>
#include <System/Resource/Resource.h>
#include <DeviceResource.h>
#include <InputDevice.h>
#include <StepTimer.h>
#include <ObjectFactory.h>
#include <TestScene.h>
#include <Effekseer/Effekseer.h>
#include <Effekseer/EffekseerRendererDX11.h>

#pragma comment(lib,"Effekseer.lib")
#pragma comment(lib,"EffekseerRendererDX11.lib")

#define APP App::GetApp()
#define INPUTDEVICE APP->GetInputDevice()


/// <summary>
/// �Q�[���A�v���P�[�V�������Ǘ��A���삷��
/// </summary>
class App
{
	// �f���[�^�[
	struct AppDeleter
	{
		void operator()(App* p) { delete p; }
	};

public:
	/// <summary>
	/// �V���O���g���\�z�ƃA�N�Z�T
	/// </summary>
	/// <param name="hInstance"></param>
	/// <param name="hWmd"></param>
	/// <param name="fullScreen"></param>
	/// <param name="width"></param>
	/// <param name="height"></param>
	/// <returns></returns>
	static std::unique_ptr<App, AppDeleter>& CreateApp(HINSTANCE hInstance, HWND hWnd, bool fullScreen, UINT width, UINT height);

	/// <summary>
	/// �V���O���g���A�N�Z�T
	/// </summary>
	/// <returns></returns>
	static std::unique_ptr<App, AppDeleter>& GetApp();

	/// <summary>
	/// �����j��
	/// </summary>
	/// <returns></returns>
	static void DeleteApp();

	/// <summary>
	/// ���\�[�X��o�^����
	/// �d��������Ή������Ȃ�
	/// </summary>
	/// <param name="key">���\�[�X�L�[</param>
	/// <param name="resourceObj">���\�[�X</param>
	void RegisterResource(const std::string& key, const std::shared_ptr<BaseResource>& resourceObj);

	/// <summary>
	/// �e�N�X�`����o�^
	/// �����L�[�̃e�N�X�`�����Ȃ���΃t�@�C�����ō쐬���o�^����
	/// �������O�̃e�N�X�`��������΂��̃|�C���^��Ԃ�
	/// </summary>
	/// <param name="key">���\�[�X�L�[</param>
	/// <param name="textureFileName">�e�N�X�`���t�@�C����</param>
	/// <param name="texType">�e�N�X�`���^�C�v</param>
	/// <returns></returns>
	std::shared_ptr<TextureResouce> RegisterTexture(const std::string& key, const std::string& textureFileName, const TextureResouce::FileExtension& texType = TextureResouce::FileExtension::WIC);

	/// <summary>
	/// ���\�[�X�̊m�F
	/// </summary>
	/// <param name="key">���\�[�X�L�[</param>
	/// <returns>�L�[�������true , �Ȃ����false</returns>
	template<class T>
	bool CheckResource(const std::string& key)const
	{
		std::map<std::string, std::shared_ptr<BaseResource>>::const_iterator it;
		it = m_resourceMap.find(key);
		if (it != m_resourceMap.end())
		{
			// ��������
			std::shared_ptr<T> pT = std::dynamic_pointer_cast<T>(it->second);
			if (pT)
			{
				return true;
			}
			else
			{
				throw StandardException(
					"���\�[�X�̕ϊ��Ɏ��s",
					key,
					"App::CheckResource()"
				);
			}
		}
		return false;
	}

	/// <summary>
	/// ���\�[�X���擾
	/// </summary>
	/// <param name="key">���\�[�X�L�[</param>
	/// <returns></returns>
	template<class T>
	std::shared_ptr<T> GetResource(const std::string& key)const
	{
		std::map<std::string, std::shared_ptr<BaseResource>>::const_iterator it;
		it = m_resourceMap.find(key);
		if (it != m_resourceMap.end())
		{
			// ��������
			std::shared_ptr<T> pT = std::dynamic_pointer_cast<T>(it->second);
			if (pT)
			{
				return pT;
			}
			else
			{
				throw StandardException(
					"���\�[�X�̕ϊ��Ɏ��s",
					key,
					"App::GetResource()"
				);
			}
		}
		else
		{
			// ������Ȃ�
			throw StandardException(
				"�w��̃L�[�����݂��Ȃ�",
				key,
				"App::GetResource()"
			);
		}
	}

	/// <summary>
	/// �C���X�^���X�n���h���̎擾
	/// </summary>
	/// <returns></returns>
	HINSTANCE GetHInstance()const { return m_hInstance; }

	/// <summary>
	/// ���C���E�B���h�E�n���h���̎擾
	/// </summary>
	/// <returns></returns>
	HWND GetHWnd()const { return m_hWnd; }

	/// <summary>
	/// �t���X�N���[�����ǂ������擾
	/// </summary>
	/// <returns></returns>
	bool GetFullScreen()const { return m_bFullScreen; }

	/// <summary>
	/// �X�N���[���T�C�Y(��)���擾
	/// </summary>
	/// <returns></returns>
	UINT GetScreenWidth()const { return m_ScreenWidth; }
	/// <summary>
	/// �X�N���[���T�C�Y(�c)���擾
	/// </summary>
	/// <returns></returns>
	UINT GetScreenHeight()const { return m_ScreenHeight; }

	/// <summary>
	/// �f�o�C�X���\�[�X���擾
	/// </summary>
	std::shared_ptr<DeviceResources> GetDeviceResources()const { return m_DeviceResources; }

	/// <summary>
	/// �^�C�}�[�̎擾
	/// </summary>
	/// <returns></returns>
	StepTimer& GetStepTimer() { return m_Timer; }

	/// <summary>
	/// �O��̃^�[������̌o�ߎ��Ԃ��擾(�b)
	/// </summary>
	/// <returns></returns>
	float GetElapsedTime();

	/// <summary>
	/// �v���O�����J�n����̍��v�X�V�񐔂��擾
	/// </summary>
	/// <returns></returns>
	unsigned int GetFrameCount() { return (unsigned int)m_Timer.GetFrameCount(); }

	/// <summary>
	/// ���͋@��̎擾
	/// </summary>
	/// <returns></returns>
	const InputDevice& GetInputDevice()const { return m_InputDevice; }

	/// <summary>
	/// �}�E�X�ƃL�[�{�[�h�̏����擾
	/// </summary>
	/// <param name="hWnd"></param>
	/// <param name="useKeyVec"></param>
	/// <returns></returns>
	bool ResetInpuState(HWND hWnd, USE_KEY_VEC& useKeyVec);

	/// <summary>
	/// �V�[���C���^�[�t�F�C�X�̎擾
	/// </summary>
	/// <returns></returns>
	std::shared_ptr<SceneInterface> GetSceneInterface()const { return m_sceneInterface; }

	/// <summary>
	/// �V�[���̍쐬
	/// </summary>
	/// <param name="...params">�ϒ��p�����[�^</param>
	/// <returns>�V�[��</returns>
	template<typename T, typename... Ts>
	std::shared_ptr<T> CreateScene(Ts&&... params)
	{
		// �V�[���̍쐬
		auto Ptr = ObjectFactory::Create<T>(params...);
		m_sceneInterface = Ptr;
		return Ptr;
	}


	/// <summary>
	/// �V�[���̍X�V�ƕ`��
	/// </summary>
	/// <param name="syncInterval">���������C���^�[�o���l</param>
	void UpdateDraw(unsigned int syncInterval);

	/// <summary>
	/// Effekseer�����_���[�̎Q�Ƃ��擾
	/// </summary>
	/// <returns></returns>
	EffekseerRendererDX11::RendererRef GetEffekseerRendererRef() { return m_effekseerRendererRef; }
	/// <summary>
	/// Effekseer�}�l�[�W���[�̎Q�Ƃ��擾
	/// </summary>
	/// <returns></returns>
	Effekseer::ManagerRef GetEffekseerManagerRef() { return m_effekseerManagerRef; }

	/// <summary>
	/// Effekseer�J�����s���ݒ�
	/// </summary>
	/// <param name="camera"></param>
	void SetEffekseerCameraMatrix(const std::shared_ptr<Camera>& camera);

	/// <summary>
	/// Effekseer�G�t�F�N�g��o�^
	/// </summary>
	/// <param name="key"></param>
	/// <param name="fileName"></param>
	/// <returns></returns>
	std::shared_ptr<EffekseerEffectResource> RegisterEffect(const std::string& key, const std::u16string& fileName);

private:
	App(HINSTANCE hInstance, HWND hWnd, bool fullScreen, UINT width, UINT height);
	virtual ~App();

private:
	static std::unique_ptr<App, AppDeleter>	m_App;	// �������g
	HWND		m_hWnd;			// ���C���E�B���h�E�n���h��
	HINSTANCE	m_hInstance;	// �C���X�^���X�n���h��
	bool		m_bFullScreen;	// �t���X�N���[�����ǂ���
	UINT		m_ScreenWidth;	// �X�N���[���T�C�Y(��)
	UINT		m_ScreenHeight;	// �X�N���[���T�C�Y(�c)
	StepTimer	m_Timer;		// �^�C�}�[
	std::map<std::string, std::shared_ptr<BaseResource>> m_resourceMap;	// �L�[�ƃ��\�[�X�����т���}�b�v
	std::shared_ptr<DeviceResources> m_DeviceResources;	// �f�o�C�X
	InputDevice	 m_InputDevice;		// ���͋@��
	std::shared_ptr<SceneInterface>	 m_sceneInterface;	// �V�[��
	EffekseerRendererDX11::RendererRef m_effekseerRendererRef;	// Effekseer�����_���[
	Effekseer::ManagerRef m_effekseerManagerRef;				// Effekseer�}�l�[�W���[
};



/// <summary>
/// �o�b�N�A�b�v�f�[�^(�e)
/// </summary>
struct BackupDataBase
{
	BackupDataBase() {}
	virtual ~BackupDataBase() {}
};

/// <summary>
/// �o�b�N�A�b�v�f�[�^
/// </summary>
template<class T>
struct BackupData
	: public BackupDataBase
{
	std::vector<T> m_vertices;
	std::vector<uint16_t> m_indexes;
};