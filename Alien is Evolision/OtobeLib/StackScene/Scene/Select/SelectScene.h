#pragma once
#include "OtobeLib/StackScene/Interface/ISceneBase.h"
#include "OtobeLib/Texture/TextureBase/TextureBase.h"
#include "OtobeLib/Texture/Manager/TextureManager.h"
#include "TemplateState.h"
#include "SELECT.h"
#include <vector>

namespace OtobeLib
{
	class SelectScene:public ISceneBase, StateArray<SELECT, int>
	{
		//描画順(小さいものが先頭に描画される)
		enum class DRAWORDER :int
		{
			WHITEBACK,//四角の画像
			SELECT_FRAME,//選択中の枠
			FRONT_SELECT,//全面の半透明の画像
			BACKGROUND,//背景
		};
	public:
		//配列の数
		static const int ARRAY_NUM = 2;

		//0
		static const int ARRAY_0 = 0;

		//1
		static const int ARRAY_1 = 1;

		//必殺技の名前(爆発)
		static const std::string DEATHBLOW_NAME_EXPROSION;
		static const std::string DEATHBLOW_NAME_EXPROSION_TOP;
		static const std::string DEATHBLOW_NAME_EXPROSION_BOTOM;

		//必殺技の名前(アビリティ)
		static const std::string DEATHBLOW_NAME_ABILITY;
		static const std::string DEATHBLOW_NAME_ABILITY_TOP;
		static const std::string DEATHBLOW_NAME_ABILITY_BOTOM;

		//点滅の間隔の最小値
		static const float BLINK_INTERVAL_MIN;

		//ダークエクスプロージョンのゲージコスト
		static const int GAGE_COST_EXPROSION = 300;

		//リバースアビリティのゲージコスト
		static const int GAGE_COST_REVASEAVIRITY = 100;

	private:
		//画像の管理クラス
		std::unique_ptr<TextureManager>	m_TextureManager;

		//背景の画像
		ITextureBase* m_backTexture;

		//全面の半透明の画像
		std::vector<ITextureBase*>	m_frontTexture;

		//四角の画像
		std::vector<TextureBase*>	m_squareTexture;

		//選択中の周りの枠の画像
		ITextureBase*				m_frameTexture;

	public:
		//コンストラクタ
		SelectScene();

		//デストラクタ
		~SelectScene()override {};

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
		void AddData1(const SELECT& state)override { StateArray::AddData1(state); };
		void AddData2(const int& state)override { StateArray::AddData2(state); };

		//現在の状態を取得する
		const SELECT& GetNowData1()const override { return StateArray::GetNowData1(); };
		const int& GetNowData2()const override { return StateArray::GetNowData2(); };

		//ひとつ前の状態を取得する
		const SELECT& GetPreData1()const override { return StateArray::GetPreData1(); };
		const int& GetPreData2()const override { return StateArray::GetPreData2(); };

	private:
		//ステートの更新処理
		void UpdateState();

		//State_WaitKeyの処理
		void State_WaitKey();

		//State_Calcingの処理
		void State_Calcing();

		//State_Change_CutInの処理
		void State_Change_CutIn();

		//State_Change_Deleteの処理
		void State_Change_Delete();
	};

}

