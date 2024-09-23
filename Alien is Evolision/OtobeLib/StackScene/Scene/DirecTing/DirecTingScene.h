#pragma once
#include "OtobeLib/StackScene/Interface/ISceneBase.h"
#include "OtobeLib/Texture/Interface/ITextureBase.h"
#include "OtobeLib/Texture/Manager/TextureManager.h"
#include "OtobeLib/Texture/AnimeTexture/AnimeTexture.h"
#include "GameObject/DamageUI/FadeState.h"
#include "TemplateState.h"
#include "DIRECTING.h"
#include <vector>

namespace OtobeLib
{
	//必殺技の演出を行うシーン
	class DirecTingScene:public ISceneBase,StateArray<DIRECTING,int,FADE_STATE>
	{
		//描画順(小さいものが先頭に描画される)
		enum class DRAWORDER :int
		{
			EXPROSION,//爆発
			PLAYER,//プレイヤー
			PLAYER_BACK,//プレイヤーの裏面
			WAVE,//オーラ
			BACKGROUND,//背景
		};
	public:
		//配列の大きさ
		static const int EXPROSION_ARRAY_NUM = 3;
		
		//ステートを切り替える時間
		static const float CHANGE_TIME_PLAYER;

		//ステートを切り替える時間
		static const float CHANGE_TIME_EXPROSION;

		//プレイヤーのアニメーションの待ち時間
		static const float WAIT_TIME_FACE;

		//プレイヤーの背景のアニメーションの待ち時間
		static const float WAIT_TIME_BACKFADE;

		//0
		static const int ARRAY_0 = 0;

		//1
		static const int ARRAY_1 = 1;

		//2
		static const int ARRAY_2 = 2;

		//ゲージの回復量(コマンド技)
		static const int TECHNIQUE_HEAL_COMMAND = 50;

		//ゲージの回復量(必殺技)
		static const int TECHNIQUE_HEAL_SPECIAL = 100;

		//取得スコア
		static const int BASE_SCORE = 1000;
	private:
		//画像の管理クラス
		std::unique_ptr<TextureManager>	m_TextureManager;

		//背景
		ITextureBase*			m_backTexture;

		//プレイヤーのアニメーション用クラス
		AnimeTexture*			m_playerAnimeTex;

		//オーラのアニメーション用クラス
		AnimeTexture*			m_waveAnimeTex;

		//爆発エフェクトのアニメーション用クラス(複数あるので、std::vectorにする)
		std::vector<AnimeTexture*>	m_exproAnimeTex;

		//裏面のテクスチャ
		ITextureBase*			m_backPlayer;

		//経過時間
		float					m_timer;

	public:
		//コンストラクタ
		DirecTingScene();

		//デストラクタ
		~DirecTingScene()override {};

		//初期化
		void Initialize()override;

		//更新処理
		void Update()override;

		//描画処理
		void Render()override;

		//終了処理
		void Finalize()override;

		//生成
		void Create()override;

	public:
		//現在の状態を設定する
		void AddData1(const DIRECTING& state)override { StateArray::AddData1(state); };
		void AddData2(const int& state)override { StateArray::AddData2(state); };
		void AddData3(const FADE_STATE& state)override { StateArray::AddData3(state); };

		//現在の状態を取得する
		const DIRECTING& GetNowData1()const override { return StateArray::GetNowData1(); };
		const int& GetNowData2()const override { return StateArray::GetNowData2(); };
		const FADE_STATE& GetNowData3()const override { return StateArray::GetNowData3(); };

		//ひとつ前の状態を取得する
		const DIRECTING& GetPreData1()const override { return StateArray::GetPreData1(); };
		const int& GetPreData2()const override { return StateArray::GetPreData2(); };
		const FADE_STATE& GetPreData3()const override { return StateArray::GetPreData3(); };

	private:
		//ステートの更新処理
		void UpdateState();

		//State_Noneの処理
		void State_None();

		//State_WaveAnimeの処理
		void State_WaveAnime();

		//State_TimerUpの処理
		void State_TimerUp();

		//State_PlayerAnimeの処理
		void State_PlayerAnime();

		//State_BackFadeの処理
		void State_BackFade();

		//State_ExproAnimeの処理
		void State_ExproAnime();

		//State_Endの処理
		void State_End();
	};
}