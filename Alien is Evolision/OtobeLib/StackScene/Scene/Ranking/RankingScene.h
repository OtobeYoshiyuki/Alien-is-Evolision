#pragma once
#include "OtobeLib/StackScene/Interface/ISceneBase.h"
#include "OtobeLib/Texture/TextureBase/TextureBase.h"
#include "OtobeLib/Texture/Manager/TextureManager.h"
#include "OtobeLib/FrameWork/Ranking/Ranking.h"
#include "TemplateState.h"
#include "GameObject/Arrow/Arrow.h"
#include "BACKOUT.h"
#include <vector>

namespace OtobeLib
{
	class RankingScene :public ISceneBase,StateArray<int,BACKOUT>
	{
	public:
		//スクロール前の座標
		static const DirectX::SimpleMath::Vector3 BEFORE_POS;

		//スクロール後の座標
		static const DirectX::SimpleMath::Vector3 AFTER_POS;
	private:
		//画像の管理クラス
		std::unique_ptr<TextureManager>	m_TextureManager;

		//ランキングデータ
		std::vector<std::vector<std::unique_ptr<Ranking>>>	m_rankingDatas;

		//ランキングデータ
		std::vector<std::vector<std::string>>	m_rankingData;

		//ランキングの背面の画像
		std::vector<ITextureBase*> m_rankingBack;

		//ステージのクリア情報
		std::vector<std::vector<std::string>> m_clearInfo;

		//右矢印
		Arrow*						m_rightArrow;

		//左矢印
		Arrow*						m_leftArrow;

		//Escapeキーの背面のテクスチャ
		TextureBase*				m_escapeBack;

		//イージング関数の経過時間
		float						m_time;

		//ベクトルの向き
		int						m_direct;

		//関数オブジェクト
		std::function<void(int&)> m_ifState;

	public:
		//コンストラクタ
		RankingScene();

		//デストラクタ
		~RankingScene()override {};

		//初期化処理
		void Initialize()override;

		//更新処理
		void Update()override;

		//描画処理
		void Render()override;

		//終了処理
		void Finalize()override;

		//生成処理
		void Create()override;

		private:
		//指定したデータを配列にして返す(std::vector<>)
		const std::vector<std::string> GetSaveDataArray(const int& data);

		//指定した範囲のデータを2次元配列にして返す(std::vector<std::vector<>>)
		const std::vector<std::vector<std::string>> GetSaveDataDoubleArray(const int& first, const int& finishi, const int& target, const int interval = 1);

	public:
		//現在の状態を設定する
		void AddData1(const int& state)override { StateArray::AddData1(state); };
		void AddData2(const BACKOUT& state)override { StateArray::AddData2(state); };

		//現在の状態を取得する
		const int& GetNowData1()const override { return StateArray::GetNowData1(); };
		const BACKOUT& GetNowData2()const override { return StateArray::GetNowData2(); };

		//ひとつ前の状態を取得する
		const int& GetPreData1()const override { return StateArray::GetPreData1(); };
		const BACKOUT& GetPreData2()const override { return StateArray::GetPreData2(); };

	private:
		//イージング関数の更新
		void UpdateEasing();

		//State_Noneの処理
		void State_None();

		//State_Admissonの処理
		void State_Admisson();

		//State_Exitの処理
		void State_Exit();

		//State_Resultの処理
		void State_Result();

		//スクロール対象以外の座標を初期化する
		void OtherInitPos(const int& target, const DirectX::SimpleMath::Vector3& pos);

	};
}


