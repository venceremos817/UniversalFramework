#include "StepTimer.h"

StepTimer::StepTimer()
	: m_elapsedTicks((uint64)0.0)
	, m_totalTicks((uint64)0.0)
	, m_leftOverTicks((uint64)0.0)
	, m_frameCount((uint32)0.0)
	, m_framesPerSecond((uint32)0.0)
	, m_framesThisSecond((uint32)0.0)
	, m_qpcSecondCounter((uint64)0.0)
	, m_isFixedTimeStep(false)
	, m_targetElapsedTicks((uint64)(TicksPerSecond / 60.0))
{
	if (!QueryPerformanceFrequency(&m_qpcFrequency))	// システムの1秒間のカウント数(周波数)を64bi値で取得
	{
		// TODO : エラー処理
	}

	if (!QueryPerformanceCounter(&m_qpcLastTime))	// 呼び出した瞬間のカウント数を64bit値で取得
	{
		// TODO : エラー処理
	}

	// 最大デルタを1秒の1/10に初期化する
	m_qpcMaxDelta = m_qpcFrequency.QuadPart / 10;
}

void StepTimer::ResetElapsedTime()
{
	if (QueryPerformanceCounter(&m_qpcLastTime))	// 呼び出した瞬間のカウント数を64bit値で取得
	{
		// TODO : エラー処理
	}

	m_leftOverTicks = 0;
	m_framesPerSecond = 0;
	m_framesThisSecond = 0;
	m_qpcSecondCounter = 0;
}
