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
/// ゲームアプリケーションを管理、操作する
/// </summary>
class App
{
	// デリーター
	struct AppDeleter
	{
		void operator()(App* p) { delete p; }
	};

public:
	/// <summary>
	/// シングルトン構築とアクセサ
	/// </summary>
	/// <param name="hInstance"></param>
	/// <param name="hWmd"></param>
	/// <param name="fullScreen"></param>
	/// <param name="width"></param>
	/// <param name="height"></param>
	/// <returns></returns>
	static std::unique_ptr<App, AppDeleter>& CreateApp(HINSTANCE hInstance, HWND hWnd, bool fullScreen, UINT width, UINT height);

	/// <summary>
	/// シングルトンアクセサ
	/// </summary>
	/// <returns></returns>
	static std::unique_ptr<App, AppDeleter>& GetApp();

	/// <summary>
	/// 強制破棄
	/// </summary>
	/// <returns></returns>
	static void DeleteApp();

	/// <summary>
	/// リソースを登録する
	/// 重複があれば何もしない
	/// </summary>
	/// <param name="key">リソースキー</param>
	/// <param name="resourceObj">リソース</param>
	void RegisterResource(const std::string& key, const std::shared_ptr<BaseResource>& resourceObj);

	/// <summary>
	/// テクスチャを登録
	/// 同じキーのテクスチャがなければファイル名で作成し登録する
	/// 同じ名前のテクスチャがあればそのポインタを返す
	/// </summary>
	/// <param name="key">リソースキー</param>
	/// <param name="textureFileName">テクスチャファイル名</param>
	/// <param name="texType">テクスチャタイプ</param>
	/// <returns></returns>
	std::shared_ptr<TextureResouce> RegisterTexture(const std::string& key, const std::string& textureFileName, const TextureResouce::FileExtension& texType = TextureResouce::FileExtension::WIC);

	/// <summary>
	/// リソースの確認
	/// </summary>
	/// <param name="key">リソースキー</param>
	/// <returns>キーがあればtrue , なければfalse</returns>
	template<class T>
	bool CheckResource(const std::string& key)const
	{
		std::map<std::string, std::shared_ptr<BaseResource>>::const_iterator it;
		it = m_resourceMap.find(key);
		if (it != m_resourceMap.end())
		{
			// 見つかった
			std::shared_ptr<T> pT = std::dynamic_pointer_cast<T>(it->second);
			if (pT)
			{
				return true;
			}
			else
			{
				throw StandardException(
					"リソースの変換に失敗",
					key,
					"App::CheckResource()"
				);
			}
		}
		return false;
	}

	/// <summary>
	/// リソースを取得
	/// </summary>
	/// <param name="key">リソースキー</param>
	/// <returns></returns>
	template<class T>
	std::shared_ptr<T> GetResource(const std::string& key)const
	{
		std::map<std::string, std::shared_ptr<BaseResource>>::const_iterator it;
		it = m_resourceMap.find(key);
		if (it != m_resourceMap.end())
		{
			// 見つかった
			std::shared_ptr<T> pT = std::dynamic_pointer_cast<T>(it->second);
			if (pT)
			{
				return pT;
			}
			else
			{
				throw StandardException(
					"リソースの変換に失敗",
					key,
					"App::GetResource()"
				);
			}
		}
		else
		{
			// 見つからない
			throw StandardException(
				"指定のキーが存在しない",
				key,
				"App::GetResource()"
			);
		}
	}

	/// <summary>
	/// インスタンスハンドルの取得
	/// </summary>
	/// <returns></returns>
	HINSTANCE GetHInstance()const { return m_hInstance; }

	/// <summary>
	/// メインウィンドウハンドルの取得
	/// </summary>
	/// <returns></returns>
	HWND GetHWnd()const { return m_hWnd; }

	/// <summary>
	/// フルスクリーンかどうかを取得
	/// </summary>
	/// <returns></returns>
	bool GetFullScreen()const { return m_bFullScreen; }

	/// <summary>
	/// スクリーンサイズ(幅)を取得
	/// </summary>
	/// <returns></returns>
	UINT GetScreenWidth()const { return m_ScreenWidth; }
	/// <summary>
	/// スクリーンサイズ(縦)を取得
	/// </summary>
	/// <returns></returns>
	UINT GetScreenHeight()const { return m_ScreenHeight; }

	/// <summary>
	/// デバイスリソースを取得
	/// </summary>
	std::shared_ptr<DeviceResources> GetDeviceResources()const { return m_DeviceResources; }

	/// <summary>
	/// タイマーの取得
	/// </summary>
	/// <returns></returns>
	StepTimer& GetStepTimer() { return m_Timer; }

	/// <summary>
	/// 前回のターンからの経過時間を取得(秒)
	/// </summary>
	/// <returns></returns>
	float GetElapsedTime();

	/// <summary>
	/// プログラム開始からの合計更新回数を取得
	/// </summary>
	/// <returns></returns>
	unsigned int GetFrameCount() { return (unsigned int)m_Timer.GetFrameCount(); }

	/// <summary>
	/// 入力機器の取得
	/// </summary>
	/// <returns></returns>
	const InputDevice& GetInputDevice()const { return m_InputDevice; }

	/// <summary>
	/// マウスとキーボードの情報を取得
	/// </summary>
	/// <param name="hWnd"></param>
	/// <param name="useKeyVec"></param>
	/// <returns></returns>
	bool ResetInpuState(HWND hWnd, USE_KEY_VEC& useKeyVec);

	/// <summary>
	/// シーンインターフェイスの取得
	/// </summary>
	/// <returns></returns>
	std::shared_ptr<SceneInterface> GetSceneInterface()const { return m_sceneInterface; }

	/// <summary>
	/// シーンの作成
	/// </summary>
	/// <param name="...params">可変長パラメータ</param>
	/// <returns>シーン</returns>
	template<typename T, typename... Ts>
	std::shared_ptr<T> CreateScene(Ts&&... params)
	{
		// シーンの作成
		auto Ptr = ObjectFactory::Create<T>(params...);
		m_sceneInterface = Ptr;
		return Ptr;
	}


	/// <summary>
	/// シーンの更新と描画
	/// </summary>
	/// <param name="syncInterval">垂直同期インターバル値</param>
	void UpdateDraw(unsigned int syncInterval);

	/// <summary>
	/// Effekseerレンダラーの参照を取得
	/// </summary>
	/// <returns></returns>
	EffekseerRendererDX11::RendererRef GetEffekseerRendererRef() { return m_effekseerRendererRef; }
	/// <summary>
	/// Effekseerマネージャーの参照を取得
	/// </summary>
	/// <returns></returns>
	Effekseer::ManagerRef GetEffekseerManagerRef() { return m_effekseerManagerRef; }

	/// <summary>
	/// Effekseerカメラ行列を設定
	/// </summary>
	/// <param name="camera"></param>
	void SetEffekseerCameraMatrix(const std::shared_ptr<Camera>& camera);

	/// <summary>
	/// Effekseerエフェクトを登録
	/// </summary>
	/// <param name="key"></param>
	/// <param name="fileName"></param>
	/// <returns></returns>
	std::shared_ptr<EffekseerEffectResource> RegisterEffect(const std::string& key, const std::u16string& fileName);

private:
	App(HINSTANCE hInstance, HWND hWnd, bool fullScreen, UINT width, UINT height);
	virtual ~App();

private:
	static std::unique_ptr<App, AppDeleter>	m_App;	// 自分自身
	HWND		m_hWnd;			// メインウィンドウハンドル
	HINSTANCE	m_hInstance;	// インスタンスハンドル
	bool		m_bFullScreen;	// フルスクリーンかどうか
	UINT		m_ScreenWidth;	// スクリーンサイズ(幅)
	UINT		m_ScreenHeight;	// スクリーンサイズ(縦)
	StepTimer	m_Timer;		// タイマー
	std::map<std::string, std::shared_ptr<BaseResource>> m_resourceMap;	// キーとリソースを結びつけるマップ
	std::shared_ptr<DeviceResources> m_DeviceResources;	// デバイス
	InputDevice	 m_InputDevice;		// 入力機器
	std::shared_ptr<SceneInterface>	 m_sceneInterface;	// シーン
	EffekseerRendererDX11::RendererRef m_effekseerRendererRef;	// Effekseerレンダラー
	Effekseer::ManagerRef m_effekseerManagerRef;				// Effekseerマネージャー
};



/// <summary>
/// バックアップデータ(親)
/// </summary>
struct BackupDataBase
{
	BackupDataBase() {}
	virtual ~BackupDataBase() {}
};

/// <summary>
/// バックアップデータ
/// </summary>
template<class T>
struct BackupData
	: public BackupDataBase
{
	std::vector<T> m_vertices;
	std::vector<uint16_t> m_indexes;
};