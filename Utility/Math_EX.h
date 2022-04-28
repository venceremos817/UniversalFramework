#pragma once
#include <float.h>
#include <cmath>


// float比較時誤差用
_NODISCARD static inline bool AdjEqual(const float num1, const float num2)
{
	return fabsf(num1 - num2) <= DBL_EPSILON * fmaxf(1, fmaxf(fabsf(num1), fabsf(num2)));
}

/// <summary>
/// 0.0f～1.0fの乱数を取得
/// </summary>
/// <returns></returns>
_NODISCARD static inline float RandZeroToOne()
{
	double prob = (double)rand() / (double)RAND_MAX;
	if (prob <= 0.0)
	{
		prob = 0.0;
	}
	return (float)prob;
}

enum class LerpMode
{
	Linear,		// 1次関数(線形)
	EaseIn,		// 2次関数 ゆっくり上昇
	EaseOut,	// 2次関数 最後がゆっくり
	Cube,		// 3次関数
	Cos,		// コサイン
};

/// <summary>
/// 補完計算
/// </summary>
/// <param name="start">開始値</param>
/// <param name="end">終了値</param>
/// <param name="startTime">開始時間</param>
/// <param name="endTime">終了時間</param>
/// <param name="nowTime">現在時間</param>
/// <param name="r">計算方法</param>
/// <returns>補完した結果</returns>
template<class T>
static inline T CalculateLerp(const T & start, const T & end, float startTime, float endTime, float nowTime, LerpMode r = LerpMode::Linear)
{
	float t = (nowTime - startTime) / (endTime - startTime);
	if (t < 0.0f)
		t = 0.0f;
	else if (t > 1.0f)
		t = 1.0f;

	//-- 補間値を設定
	float cal_rate;	// 1次関数補完値に変換(デフォルト
	switch (r)
	{
	case LerpMode::Linear:
		cal_rate = t;
		break;
	case LerpMode::EaseIn:
		cal_rate = t * t;
		break;
	case LerpMode::EaseOut:
		cal_rate = t * (2.0f - t);
		break;
	case LerpMode::Cube:
		cal_rate = t * t * (3.0f - 2.0f * t);
		break;
	case LerpMode::Cos:
		cal_rate = (1.0f - cos(t*DirectX::XM_PI)) * 0.5f;
		break;
	}

	return start * (1.0f - cal_rate) + end * cal_rate;
}
