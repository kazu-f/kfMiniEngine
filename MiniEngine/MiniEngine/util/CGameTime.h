#pragma once

//#include "Stopwatch.h"

namespace Engine {
	/// <summary>
	/// ゲームタイム。
	/// </summary>
	class CGameTime
	{
	public:
		/// <summary>
		/// 1フレームの経過時間を取得(s)
		/// </summary>
		const float GetFrameDeltaTime()const
		{
			//固定FPS
			return 1.0f / 60.0f;
		}
		/// <summary>
		/// 1フレームあたりの経過時間の平均を取る。
		/// </summary>
		/// <param name="deltaTime">現在フレームの経過時間</param>
		void PushFrameDeltaTime(float deltaTime)
		{
			m_frameDeltaTimeQue.push_back(deltaTime);
			if (m_frameDeltaTimeQue.size() > 30.0f) {
				float totalTime = 0.0f;
				for (auto time : m_frameDeltaTimeQue) {
					//合計を計算。
					totalTime += time;
				}
				//平均値を取る。
				//最低でも30fpsは出るようにしている。
				m_frameDeltaTime = min(1.0f / 30.0f, totalTime / m_frameDeltaTimeQue.size());
				//一番古い値を取り出す。
				m_frameDeltaTimeQue.pop_front();
			}
		}
	private:
		std::list<float> m_frameDeltaTimeQue;			//経過時間のリスト。
		float			 m_frameDeltaTime = 1.0f / 60.0f;	//1フレームの経過時間。
	};

}
