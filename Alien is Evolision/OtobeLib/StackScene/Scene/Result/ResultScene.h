#pragma once
#include "OtobeLib/StackScene/Interface/ISceneBase.h"
#include "OtobeLib/Texture/Interface/ITextureBase.h"
#include "OtobeLib/Texture/Manager/TextureManager.h"
#include "OtobeLib/FrameWork/TimerJudge/TimerJudge.h"
#include "OtobeLib/FrameWork/Score/Score.h"
#include "OtobeLib/FrameWork/Ranking/Ranking.h"
#include "TemplateState.h"
#include "CLEARRESULT.h"
#include <functional>
namespace OtobeLib
{
	class ResultScene :public ISceneBase,State<CLEARSTATE>
	{
	public:
		enum RANKING_INFO :int
		{
			STAGE1 = 1,
			STAGE2 = 11,
			STAGE3 = 21,
			STAGE4 = 31,
			BOSSSTAGE = 41,
			STAGENUM = 5,
		};
		enum class RESULT_INFO :int
		{
			LEVEL,//レベル
			SCORE,//スコア
			CLEATIME,//クリアタイム
			RANKING_NUM,//ランキングの数
		};
	public:
		//ステージ用のデータを追加する間隔
		static const int STAGE_INTERVAL = 3;

		//クリアデータの範囲
		static const int CREAR_RESULT_RANGE = 9;

		//最初に表示されるUIとスクロールするUIの距離
		static const float UI_INTERVAL;
	private:
		//画像の管理クラス
		std::unique_ptr<TextureManager>	m_TextureManager;

		//クリアタイム
		std::unique_ptr<TimerJudge>		m_timerJudge;

		//獲得スコア
		std::unique_ptr<Score>			m_score;

		//トータルスコア
		std::unique_ptr<Score>			m_totalScore;

		//スペースキーの点滅
		std::unique_ptr<Blinking>		m_spacekeyBlink;

		//経過時間
		float							m_time;

		//キーを押した回数
		int								m_pushCount;

		//関数オブジェクト
		std::function<void()>			m_func;

		//ランキングデータ
		std::vector<std::vector<std::string>>	m_rankingData;

		//ランキングデータ
		std::vector<std::unique_ptr<Ranking>>	m_rankings;

		//フォントのベースの座標X
		float			m_fontX;

	public:
		ResultScene();
		~ResultScene()override;
		void Initialize()override;
		void Update()override;
		void Render()override;
		void Finalize()override;
		void Create()override;

	private:
		//ステートの更新処理
		void UpdateState();

		//State_Noneの処理
		void State_None();

		//State_WaitKeyの処理
		void State_WaitKey();

		//State_Nextの処理
		void State_Next();

		//State_Admissonの処理
		void State_Admisson();

		//State_Rankingの処理
		void State_Ranking();

	public:
		//現在の状態を設定する
		void SetNowState(const CLEARSTATE& now)override { State::SetNowState(now); };

		//現在の状態を取得する
		const CLEARSTATE& GetNowState()const override { return State::GetNowState(); };

		//ひとつ前の状態を取得する
		const CLEARSTATE& GetPreState()const override { return State::GetPreState(); };

		//指定したデータを配列にして返す(std::vector<>)
		const std::vector<std::string> GetSaveDataArray(const int& data);

		//指定した範囲のデータを2次元配列にして返す(std::vector<std::vector<>>)
		const std::vector<std::vector<std::string>> GetSaveDataDoubleArray(const int& first, const int& finishi,const int& target, const int interval = 1);


	};
}

