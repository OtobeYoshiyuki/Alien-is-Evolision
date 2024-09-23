#pragma once
#include <string>
#include <map>
#include "AnimationImporter.h"
namespace OtobeLib
{
	//型名が長いので省略
	using AnimationList = std::list<std::unique_ptr<AnimationImporter>>;
	using AnimationMap = std::map<std::string,AnimationList>;

	//アニメーションクラス
	class Animation
	{
	private:
		//指定したキーのアニメーションの情報
		std::map<std::string, RECT>			m_Animation;
		//次に再生するアニメーションの情報
		std::map<std::string, std::string>	m_NextAnimation;
		//アニメーションの終了時間
		std::map<std::string, int>			m_EndTime;
		//現在のアニメーション
		RECT								m_NowRect;
		//現在のアニメーションの情報
		std::string							m_NowInfo;
		//アニメーションのループ情報
		std::string							m_LoopInfo;
		//アニメーションカウント
		int									m_AniCount;
		//アニメーションの更新フラグ
		bool								m_AniFlag;
		//アニメーションのループフラグ
		bool								m_LoopFlag;
		//アニメーションのグループを設定する
		static	AnimationMap s_AniGrop;
	public:
		//コンストラクタ
		Animation();
		//デストラクタ
		~Animation(){}
		//初期化
		void Initialize();
		//更新
		void Update();
		//終了
		void Finalize();
		//--------------------セッター------------------------------------------//
		
		//アニメーションの情報を設定する
		virtual void SetAnimation(const std::string& name, const RECT& rect) { m_Animation[name] = rect; }

		//次に再生するアニメーションの情報を設定する
		virtual void SetNextAnimation(const std::string& now, const std::string& next) { m_NextAnimation[now] = next; }

		//アニメーションの終了時間を設定する
		virtual void SetEndTime(const std::string& name, const int& time) { m_EndTime[name] = time; }

		//現在のアニメーションを設定する
		virtual void SetNowAnimation(const RECT& rect) { m_NowRect = rect; }

		//現在のアニメーションの情報を設定する
		virtual void SetNowInfo(const std::string& info) { m_NowInfo = info; }

		//アニメーションのループ情報を設定する
		virtual void SetLoopInfo(const std::string& loop) { m_LoopInfo = loop; }

		//アニメーションの更新フラグを設定する
		virtual void SetAniFlag(const bool& flag) { m_AniFlag = flag; }

		//アニメーションのループフラグを設定する
		virtual void SetLoopFlag(const bool& flag) { m_LoopFlag = flag; }

		//アニメーションのパラメーターを引き継ぐ
		virtual void SetNewAnimation(Animation* ani);

		//アニメーションのタイマーを初期化する
		virtual void SetInitTimer(const int& timer) { m_AniCount = timer; }

		//--------------------セッター------------------------------------------//

		//--------------------ゲッター------------------------------------------//
		
		//アニメーションの情報を取得する
		virtual const RECT& GetAnimation(const std::string& name) { return m_Animation[name]; }

		//次に再生するアニメーションの情報を取得する
		virtual const std::string& GetNextAnimation(const std::string& now) { return m_NextAnimation[now]; }

		//アニメーションの終了時間を設定する
		virtual const int& GetEndTime(const std::string& name) { return m_EndTime[name]; }

		//現在のアニメーションを取得する
		virtual const RECT& GetNowAnimation()const { return m_NowRect; }

		//現在のアニメーションの情報を設定する
		virtual const std::string& GetNowInfo()const { return m_NowInfo; }

		//アニメーションのループ情報を設定する
		virtual const std::string& GetLoopInfo()const {return m_LoopInfo; }

		//アニメーションの更新フラグを取得する
		virtual const bool& GetAniFlag()const { return m_AniFlag; }

		//アニメーションのループフラグを取得する
		virtual const bool& GetLoopFlag()const { return m_LoopFlag; }

		//アニメーションカウントを取得する
		virtual const int& GetAniCount()const { return m_AniCount; }

		//アニメーションのMapを取得する
		virtual const std::map<std::string, RECT>& GetAnimationMap()const { return m_Animation; }

		//次のアニメーションのMapを取得する
		virtual const std::map<std::string, std::string>& GetNextAnimationMap()const { return m_NextAnimation; }

		//アニメーションの終了時間のMapを取得する
		virtual const std::map<std::string, int>& GetEndTimeMap()const { return m_EndTime; }

		//自分自身を取得する
		virtual Animation* GetThisAnimation() { return this; }

		//アニメーションの初期化関数を呼ぶ
		virtual void InitMiniMapAnimation();
		
		//--------------------ゲッター------------------------------------------//

	public:
		//テクスチャの情報を追加する
		static void Add(const std::vector<AnimationImporter>& Importer)
		{
			for (const AnimationImporter& importer : Importer)
			{
				//グループ名を取得する
				std::string grop = importer.GetGrop();

				//インスタンスを生成する
				s_AniGrop[grop].push_back(std::make_unique<AnimationImporter>(importer));
			}
		}

		//アニメーションコンテナを取得する
		static const AnimationMap& GetAnimeMap() { return s_AniGrop; }

		//コンテナの中のリストを取得する
		static const AnimationList& GetAnimeList(const std::string& grop)
		{
			return s_AniGrop[grop];
		}
	};
}

