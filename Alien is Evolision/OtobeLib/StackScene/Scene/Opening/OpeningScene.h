#pragma once
#include "OtobeLib/StackScene/Interface/ISceneBase.h"
#include "OtobeLib/Texture/TextureBase/TextureBase.h"
#include "OtobeLib/Texture/Manager/TextureManager.h"
#include "TemplateState.h"
#include "GameObject/Arrow/Arrow.h"
#include "BACKOUT.h"
#include "OtobeLib/FrameWork/Novel/Novel.h"

namespace OtobeLib
{
	class OpeningScene :public ISceneBase, StateArray<int, BACKOUT>
	{
	public:
		//スクロール前の座標
		static const DirectX::SimpleMath::Vector3 BEFORE_POS;

		//スクロール後の座標
		static const DirectX::SimpleMath::Vector3 AFTER_POS;

		//スクロールする画像の数
		static const int SCROLL_NUM = 7;

	private:
		//画像の管理クラス
		std::unique_ptr<TextureManager>	m_textureManager;

		//スクロールする画像
		std::vector<ITextureBase*> m_scrollTexture;

		//イージング関数の経過時間
		float						m_time;

		//ベクトルの向き
		int						m_direct;

		//関数オブジェクト
		std::function<void(int&)> m_ifState;

		//ノベルの管理クラス
		std::unique_ptr<Novel>	m_novel;

	public:
		//コンストラクタ
		OpeningScene();

		//デストラクタ
		~OpeningScene()override {};

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

		//キーの更新処理
		void UpdateInput();

	};

}

