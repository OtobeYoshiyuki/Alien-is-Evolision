#pragma once
#include <string>
namespace OtobeLib
{
	class TimerJudge
	{
	public:
		//1秒
		static const int ONESECOND = 60;
		//1分
		static const int ONEMINUTE = 3600;
	private:
		//フレーム数をカウントする
		float						m_FrameCount;
		//制限時間
		float						m_LimitTimer;
		//タイマーを更新するフラグ
		bool						m_TimerUp;
		//制限時間を超えたか判定するフラグ
		bool						m_Judge;
		//1秒単位の補正値
		float						m_correction;
	public:
		//コンストラクタ
		TimerJudge();

		//デストラクタ
		~TimerJudge(){}

		//初期化処理
		void Initialize(const int& limit, const bool& up);

		//更新処理
		void Update();

	public:
		//--------------------------------セッター-----------------------------------------------------//

		//制限時間を設定する
		void SetLimitTimer(const int& limit) { m_LimitTimer = limit; }

		//タイマーの更新フラグを設定する
		void SetTimerUp(const bool& up) { m_TimerUp = up; }

		//1秒単位の補正値を設定する
		void SetCorrection(const float& cor) { m_correction = cor; }

		//--------------------------------セッター-----------------------------------------------------//

		//--------------------------------ゲッター-----------------------------------------------------//
		
		//制限時間を取得する
		const int& GetLimitTimer()const { return m_LimitTimer; }

		//タイマーの更新フラグを取得する
		const bool& GetTimerUp()const { return m_TimerUp; }

		//制限時間の有無を取得する
		const bool& GetTimerJudge()const { return m_Judge; }
		
		//--------------------------------ゲッター-----------------------------------------------------//

	public:
		//制限時間から、表示する秒数を求める
		const std::string AskTimeStringSecound();

		//制限時間から、表示する分数を求める
		const std::string AskTimeStringMinute();
	};
}

