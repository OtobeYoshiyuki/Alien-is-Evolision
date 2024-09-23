#pragma once
#include <string>
#include "OtobeLib/FrameWork/Score/Score.h"
#include "OtobeLib/FrameWork/TimerJudge/TimerJudge.h"
#include "OtobeLib/Texture/Blinking/Blinking.h"
namespace OtobeLib
{
	class Ranking
	{
	private:
		//レベル
		std::string m_level;

		//スコア
		std::unique_ptr<Score> m_score;

		//タイマー
		std::unique_ptr<TimerJudge> m_timer;

		//点滅
		std::unique_ptr<Blinking>	m_blink;

		//色
		DirectX::XMVECTORF32		m_color;

	public:
		//コンストラクタ
		Ranking();

		//デストラクタ
		~Ranking() = default;

		//更新処理
		void Update();

		//レベルを設定
		void SetLevel(const std::string& level) { m_level = level; }

		//色を設定
		void SetColor(const DirectX::XMVECTORF32& color) { m_color = color; }

		//レベルを取得
		const std::string& GetLevel()const { return m_level; }

		//スコアを取得
		Score* GetScore() { return m_score.get(); }

		//タイマーを取得
		TimerJudge* GetTimer() { return m_timer.get(); }

		//点滅を取得
		Blinking* GetBlinking() { return m_blink.get(); }

		//色を取得
		const DirectX::XMVECTORF32& GetColor()const { return m_color; }
	};
}

