#pragma once

#include <Windows.h>


class StepTimer
{
	typedef long long int64;
	typedef unsigned long long uint64;
	typedef unsigned int uint32;
public:
	StepTimer();
	~StepTimer() {};

	/// <summary>
	/// 前のUpdate呼び出しから経過したカウンタを取得
	/// </summary>
	/// <returns></returns>
	uint64	GetElapsedTicks()const { return m_elapsedTicks; }

	/// <summary>
	/// 前のUpdate呼び出しから経過した秒数を取得
	/// </summary>
	/// <returns></returns>
	double	GetElapsedSeconds()const { return TicksToSeconds(m_elapsedTicks); }

	/// <summary>
	/// プログラムが開始してから経過したカウンタを取得
	/// </summary>
	/// <returns></returns>
	uint64	GetTotalTicks()const { return m_totalTicks; }

	/// <summary>
	/// プログラムが開始してから経過した秒数を取得
	/// </summary>
	/// <returns></returns>
	double	GetTotalSeconds()const { return TicksToSeconds(m_totalTicks); }

	/// <summary>
	/// プログラム開始からの合計更新回数を取得
	/// </summary>
	/// <returns></returns>
	uint32	GetFrameCount()const { return m_frameCount; }

	/// <summary>
	/// 現在のフレームレートを取得
	/// </summary>
	/// <returns></returns>
	uint32	GetFPS()const { return m_framesPerSecond; }

	/// <summary>
	/// タイムステップのモードを固定/可変にする
	/// </summary>
	/// <param name="isFixedTimeStep"></param>
	void SetFixedTimeStep(bool isFixedTimeStep) { m_isFixedTimeStep = isFixedTimeStep; }

	/// <summary>
	/// 固定タイムステップモードでUpdateの呼び出し頻度を設定
	/// </summary>
	/// <param name="targetElapsed">呼び出し頻度(カウンタ)</param>
	void SetTargetElapsedTicks(uint64 targetElapsed) { m_targetElapsedTicks = targetElapsed; }

	/// <summary>
	/// 固定タイムステップモードでUpdateの呼び出し頻度を設定
	/// </summary>
	/// <param name="targetElapsed">呼び出し頻度(時間)</param>
	void SetTargetElapsedSeconds(double targetElapsed) { m_targetElapsedTicks = SecondsToTicks(targetElapsed); }

	/// <summary>
	/// 呼び出しの更新
	/// 意図的なタイミングの不連続性のあと(ブロックIO操作など)これを呼び出すと、
	/// 固定タイムステップロジックによって一連のキャッチアップが施行されるのを回避できる
	/// </summary>
	void ResetElapsedTime();

	/// <summary>
	/// タイマー状態を更新し、指定のUpdate関数を適切な回数だけ呼び出す
	/// </summary>
	/// <param name="update">呼び出すべきUpdate関数</param>
	template<typename TUpdate>
	void Tick(const TUpdate& update);


public:
	/// <summary>
	/// ティックを時間に変換
	/// </summary>
	/// <param name="ticks"></param>
	/// <returns></returns>
	static double	TicksToSeconds(uint64 ticks) { return static_cast<double>(ticks) / TicksPerSecond; }

	/// <summary>
	/// 時間をティックに変換
	/// </summary>
	/// <param name="seconds"></param>
	/// <returns></returns>
	static uint64	SecondsToTicks(double seconds) { return static_cast<uint64>(seconds*TicksPerSecond); }


public:
	static const uint64 TicksPerSecond = 10000000;	// 1秒当たり10000000ティック


private:
	LARGE_INTEGER		m_qpcFrequency;
	LARGE_INTEGER		m_qpcLastTime;
	uint64	m_qpcMaxDelta;

	uint64	m_elapsedTicks;
	uint64	m_totalTicks;
	uint64	m_leftOverTicks;

	uint32	m_frameCount;
	uint32	m_framesPerSecond;
	uint32	m_framesThisSecond;
	uint64	m_qpcSecondCounter;

	bool	m_isFixedTimeStep;
	uint64	m_targetElapsedTicks;

	StepTimer(const StepTimer&) = delete;
	StepTimer& operator=(const StepTimer&) = delete;
	StepTimer(const StepTimer&&) = delete;
	StepTimer& operator=(const StepTimer&&) = delete;
};

template<typename TUpdate>
inline void StepTimer::Tick(const TUpdate & update)
{
	// 現在の時刻をクエリする
	LARGE_INTEGER currentTime;

	if (!QueryPerformanceCounter(&currentTime))	// 呼び出した瞬間のカウント数を64bit値で取得
	{
		// TODO : エラー処理
	}

	uint64 timeDelta = currentTime.QuadPart - m_qpcLastTime.QuadPart;

	m_qpcLastTime = currentTime;
	m_qpcSecondCounter += timeDelta;

	// 極端に大きな時間差をクランプ
	if (timeDelta > m_qpcMaxDelta)
	{
		timeDelta = m_qpcMaxDelta;
	}

	// QPC単位を標準の目盛り形式に変換する。
	timeDelta *= TicksPerSecond;
	timeDelta /= m_qpcFrequency.QuadPart;

	uint32 lastFrameCount = m_frameCount;

	if (m_isFixedTimeStep)
	{
		/*	固定タイムステップ更新
		クランプしたターゲット経過時間(1/4ms以内)に接近して、アプリケーションが実行されている場合
		ターゲット値に正確に一致させるクロック。これにより、小さい無関係なエラーの発生を防ぐ。
		このクランプなしで60fpsを要求したゲームは固定された更新(59.94NTSCディスプレイ上で有効なvsyncで実行)は
		最終的にはフレームをドロップするのに十分な小さいエラーを累積する。
		わずかな僅差をゼロに持っていきスムーズに進行するようにする。
		*/

		if (abs(static_cast<int64>(timeDelta - m_targetElapsedTicks)) < TicksPerSecond / 4000)
		{
			timeDelta = m_targetElapsedTicks;
		}

		m_leftOverTicks += timeDelta;

		while (m_leftOverTicks >= m_targetElapsedTicks)
		{
			m_elapsedTicks = m_targetElapsedTicks;
			m_totalTicks += m_targetElapsedTicks;
			m_leftOverTicks -= m_targetElapsedTicks;
			++m_frameCount;

			update();
		}
	}
	else
	{
		// 可変タイムステップ更新ロジック
		m_elapsedTicks = timeDelta;
		m_totalTicks += timeDelta;
		m_leftOverTicks = 0;
		++m_frameCount;

		update();
	}

	// 現在のフレームレートを追跡
	if (m_frameCount != lastFrameCount)
	{
		++m_framesThisSecond;
	}

	if (m_qpcSecondCounter >= static_cast<uint64>(m_qpcFrequency.QuadPart))
	{
		m_framesPerSecond = m_framesThisSecond;
		m_framesThisSecond = 0;
		m_qpcSecondCounter %= m_qpcFrequency.QuadPart;
	}
}
