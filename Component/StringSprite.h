#pragma once
#include <Component.h>
#include <dwrite.h>
#include <wrl/client.h>
#include <string>
#include <XMFLOAT_EX.h>

#pragma comment(lib,"Dwrite.lib")

/// <summary>
/// 文字列描画コンポーネント
/// </summary>
class StringSprite
	:public Component
{
public:
	enum class TextAlignment
	{
		Left,		// 左合わせ
		Center,		// 中央合わせ
		Right,		// 右合わせ
	};

public:
	explicit StringSprite(const std::shared_ptr<GameObject>& pGameObject);
	virtual ~StringSprite();

	void OnCreate()override;
	/// <summary>
	/// 更新処理
	/// </summary>
	void Update()override;
	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw()override;


	/// <summary>
	/// フォントを設定
	/// </summary>
	/// <param name="fontName">フォント名</param>
	/// <param name="fontSize"></param>
	void SetFont(const std::wstring& fontName, float fontSize);
	/// <summary>
	/// フォント名を取得
	/// </summary>
	/// <returns></returns>
	const std::wstring& GetFontName()const;
	/// <summary>
	/// フォントサイズを取得
	/// </summary>
	/// <returns></returns>
	float GetFontSize()const;

	/// <summary>
	/// テキストアライメントを設定
	/// </summary>
	/// <param name="alignment"></param>
	void SetTextAlignment(TextAlignment alignment);
	/// <summary>
	/// テキストのアライメントを取得
	/// </summary>
	/// <returns></returns>
	TextAlignment GetTextAlignment()const;

	/// <summary>
	/// 文字列を取得
	/// </summary>
	/// <returns></returns>
	const std::wstring& GetText()const;
	/// <summary>
	/// 文字列を設定
	/// </summary>
	/// <param name="str"></param>
	void SetText(const std::wstring& str);
	/// <summary>
	/// 文字列を追加
	/// </summary>
	/// <param name="str"></param>
	void AddText(const std::wstring& str);

	/// <summary>
	/// フォント色を設定
	/// </summary>
	/// <param name="color"></param>
	void SetFontColor(const Float4& color);
	/// <summary>
	/// フォント色を取得
	/// </summary>
	/// <returns></returns>
	const Float4& GetFontColor()const;
	/// <summary>
	/// 背景色を設定
	/// </summary>
	void SetBackColor(const Float4& color);
	/// <summary>
	/// 背景色を取得
	/// </summary>
	/// <returns></returns>
	const Float4& GetBackColor()const;

	/// <summary>
	/// 背景のテキストマージンを設定
	/// </summary>
	/// <param name="margine"></param>
	void SetBackTextMargine(const Float2& margine);
	/// <summary>
	/// 背景のテキストマージンを取得
	/// </summary>
	/// <returns></returns>
	Float2 GetBackTextMargine()const;

	/// <summary>
	/// テキストブロックの幅を設定
	/// </summary>
	/// <param name="widh"></param>
	void SetTextBlockWidth(float widh);
	/// <summary>
	/// テキストブロックの高さを設定
	/// </summary>
	/// <param name="height"></param>
	void SetTextBlockHeight(float height);

	/// <summary>
	/// テキストレイアウトを取得
	/// </summary>
	/// <returns></returns>
	Microsoft::WRL::ComPtr<IDWriteTextLayout>& GetTextLayout()const;
	/// <summary>
	/// テキストフォーマットを取得
	/// </summary>
	/// <returns></returns>
	Microsoft::WRL::ComPtr<IDWriteTextFormat>& GetTextFormat()const;

	/// <summary>
	/// テキストメトリクスを取得
	/// </summary>
	/// <returns></returns>
	const DWRITE_TEXT_METRICS& GetDriteTextMetrics()const;

	/// <summary>
	/// スタート位置を設定
	/// </summary>
	/// <param name="pos"></param>
	void SetStartPosition(const Float2& pos);

	/// <summary>
	/// テキストの矩形を設定
	/// </summary>
	/// <param name="rect"></param>
	void SetTextRect(float left, float top, float right, float bottom);


public:
	/// <summary>
	/// floatを文字列に変換する形式
	/// </summary>
	enum class FloatModify
	{
		Default,	// 浮動小数点
		Fixed,		// 数字を出力
		Scientific,	// e+09などの形式
	};
	/// <summary>
	/// 浮動小数を文字列(wstring)に変換する
	/// </summary>
	/// <param name="val"></param>
	/// <param name="precision">精度</param>
	/// <param name="modify">形式</param>
	/// <returns></returns>
	static std::wstring FloatToWstr(float val, std::streamsize precision = 0, FloatModify modify = FloatModify::Default);

	/// <summary>
	/// 整数を文字列に変換する形式
	/// </summary>
	enum class NumModify
	{
		Oct,	// 8進数
		Dec,	// 10進数
		Hex,	// 16進数
	};
	/// <summary>
	/// UINTをwstringに変換
	/// </summary>
	/// <param name="val"></param>
	/// <param name="modify">進数</param>
	/// <returns></returns>
	static std::wstring UintToWstr(UINT val, NumModify modify = NumModify::Dec);
	/// <summary>
	/// 
	/// </summary>
	/// <param name="val"></param>
	/// <param name="modify"></param>
	/// <returns></returns>
	static std::wstring IntToWstr(int val, NumModify modify = NumModify::Dec);

private:
	struct Param;
	std::unique_ptr<Param> m_param;
};
