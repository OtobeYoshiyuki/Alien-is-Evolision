#pragma once
#include "OtobeLib/StackScene/Interface/ISceneBase.h"
#include "OtobeLib/Texture/Interface/ITextureBase.h"
#include "OtobeLib/Texture/Manager/TextureManager.h"
#include "TemplateState.h"
#include "GameObject/Arrow/Arrow.h"
#include "BACKOUT.h"
#include <vector>
#include <string>
#include <functional>
namespace OtobeLib
{
	class StageSelectScene :public ISceneBase,StateArray<int,BACKOUT>
	{
	public:
		enum class STAGEINFO :int
		{
			CLEARTAG,//クリア状況のタグ
			STAGENAME,//ステージ名
			RELEASEINFO,//解放状況
			CLEARINFO,//クリア状況
		};
	public:
		//スクロール前の座標
		static const DirectX::SimpleMath::Vector3 BEFORE_POS;

		//スクロール後の座標
		static const DirectX::SimpleMath::Vector3 AFTER_POS;
	private:
		//画像の管理クラス
		std::unique_ptr<TextureManager>	m_TextureManager;

		//ステージ画像の配列
		std::vector<ITextureBase*> m_stageTexture;

		//イージング関数の経過時間
		float						m_time;

		//ベクトルの向き
		int						m_direct;

		//関数オブジェクト
		std::function<void(int&)> m_ifState;

		//ステージのクリア情報
		std::vector<std::vector<std::string>> m_clearInfo;

		//右矢印
		Arrow* m_rightArrow;

		//左矢印
		Arrow* m_leftArrow;

		//スペースキーの背面のテクスチャ
		TextureBase* m_spaceBackTexture;
	public:
		StageSelectScene();
		~StageSelectScene()override {};
		void Initialize()override;
		void Update()override;
		void Render()override;
		void Finalize()override;
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
		void OtherInitPos(const int& target,const DirectX::SimpleMath::Vector3& pos);
	};
}

