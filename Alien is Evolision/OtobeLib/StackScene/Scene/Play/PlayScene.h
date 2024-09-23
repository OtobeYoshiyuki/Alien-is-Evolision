#pragma once
#include "OtobeLib/StackScene/Interface/ISceneBase.h"
#include "OtobeLib/FrameWork/Collision/CollisionManager/CollisionManager.h"
#include "OtobeLib/Texture/Interface/ITextureBase.h"
#include "OtobeLib/Texture/Manager/TextureManager.h"
#include "GameObject/BackGround/BackGround.h"
#include "GameObject/MiniMap/MiniMap.h"
#include "GameObject/Stage/Stage.h"
#include "GameObject/DamageUI/DamageUI.h"
#include "OtobeLib/FrameWork/TimerJudge/TimerJudge.h"
#include "GameObject/TimeLag/TimeLag.h"
#include "Effect/Manager/StarManager.h"
#include "Effect/LevelUp/LevelUp.h"
#include "OtobeLib/Camera/Manager/CameraManager.h"
#include "OtobeLib/FrameWork/Score/Score.h"
#include "GameObject/Item/Manager/HealManager.h"
#include "TemplateState.h"
#include "BACKOUT.h"

namespace OtobeLib
{
	class PlayScene :public ISceneBase,State<BACKOUT>
	{
	public:
		enum class SAVE_DATA :int
		{
			PLAYER_STATUS = 2,//プレイヤーのステータス
			HEAL_NAME = 4,//回復アイテムの名前
			HEAL_INFO,//回復アイテム
		};

		enum class PLAYER_STATUS :int
		{
			LEVEL,//レベル
			HP,//HP
			MP,//MP
			ATACK,//攻撃力
			DEFENCE,//防御力
			SPECIAL,//スペシャルポイント
			NEXT,//次のレベルまでの経験値
			MAX,//配列の大きさ
		};

	private:
		//当たり判定管理クラス
		std::unique_ptr<CollisionManager>		m_CollisionManager;

		//ゲーム用画像管理クラス
		std::unique_ptr<TextureManager>			m_GameManager;

		//UI用メイン画像管理クラス
		std::unique_ptr<TextureManager>			m_MainInfoManager;

		//UI用サブ画像管理クラス
		std::unique_ptr<TextureManager>			m_SubInfoManager;

		//ミニマップ管理クラス
		std::unique_ptr<TextureManager>			m_MiniMapManager;

		//カメラの管理クラス
		std::unique_ptr<CameraManager>			m_cameraManager;

		//背景クラス
		BackGround*								m_BG;

		//ミニマップクラス
		std::unique_ptr<MiniMap>				m_MiniMap;

		//ステージクラス
		std::unique_ptr<Stage>					m_Stage;

		//タイマークラス
		std::unique_ptr<TimerJudge>				m_TimerJudge;

		//タイムラグエフェクト
		std::unique_ptr<TimeLag>				m_LagStart;
		std::unique_ptr<TimeLag>				m_LagGo;
		std::unique_ptr<TimeLag>				m_LagFinish;

		//レベルアップエフェクト
		LevelUp*								m_LevelUp;

		//StarEffectManager
		std::unique_ptr<StarManager>			m_StarManager;

		//セーブデータ
		std::vector<std::vector<std::string>>	m_saveData;

		//スコア
		std::unique_ptr<Score>					m_score;

		//回復アイテムの管理クラス
		std::unique_ptr<HealManager>			m_healManager;

		//経過時間
		float									m_easingTime;

	public:
		PlayScene();		//コンストラクタ

		~PlayScene()override {};		//デストラクタ

		void Initialize()override;		//初期化処理

		void Update()override;		//更新処理

		void Render()override;		//描画処理

		void Finalize()override;	//終了処理

		void Create()override;		//生成

		//ゲーム用のTextureManagerを取得
		std::unique_ptr<TextureManager>& GetGameManager() { return m_GameManager; }

		//UI用のTextureManagerを取得
		std::unique_ptr<TextureManager>& GetMainInfoManager() { return m_MainInfoManager; }

		//UI用のTextureManagerを取得
		std::unique_ptr<TextureManager>& GetSubInfoManager() { return m_SubInfoManager; }

		//ミニマップ用のTextureManagerを取得
		std::unique_ptr<TextureManager>& GetMiniMapManager() { return m_MiniMapManager; }

		//LagStartを取得
		std::unique_ptr<TimeLag>& GetLagStart() { return m_LagStart; }

		//LagGoを取得
		std::unique_ptr<TimeLag>& GetLagGo() { return m_LagGo; }

		//LagFinishを取得
		std::unique_ptr<TimeLag>& GetLagFinish() { return m_LagFinish; }

		//LevelUpを取得
		LevelUp* GetLevelUp() { return m_LevelUp; }

		//スコアを取得
		Score* GetScore() { return m_score.get(); }

		//HealManagerを取得
		HealManager* GetHealManager() { return m_healManager.get(); }

		//指定したデータを配列にして返す(std::vector<>)
		const std::vector<std::string> GetSaveDataArray(const int& data);

	public:
		//現在の状態を設定する
		void SetNowState(const BACKOUT& now)override { State::SetNowState(now); };

		//現在の状態を取得する
		const BACKOUT& GetNowState()const override { return State::GetNowState(); };

		//ひとつ前の状態を取得する
		const BACKOUT& GetPreState()const override { return State::GetPreState(); };

	private:
		//Managerクラスを生成
		void CreateManager();	

		//背景を生成
		void CreateBack();		

		//ステージを生成
		void CreateStage();		

		//カメラを生成
		void CreateCamera();	

		//タイマーを生成
		void CreateTimer();		

		//エフェクトを生成
		void CreateEffect();		

		//スコアを生成
		void CreateScore();		

		//LagEffectの更新
		void UpdateLag();		
	};
}

