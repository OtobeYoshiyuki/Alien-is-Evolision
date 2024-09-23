#pragma once
#include "OtobeLib/StackScene/Interface/ISceneBase.h"
#include "OtobeLib/Texture/Interface/ITextureBase.h"
#include "OtobeLib/Texture/TextureBase/TextureBase.h"
#include "OtobeLib/Texture/Manager/TextureManager.h"
#include "OtobeLib/Texture/FadeTexture/FadeTexture.h"
#include <TemplateState.h>
#include <vector>
#include <functional>

namespace OtobeLib
{
	class MenuScene:public ISceneBase,State<int>
	{
	public:
		enum class SELECT_INFO :int
		{
			NONE = -1,//何も選択していない
			STATUS,//ステータスを選択中
			ITEM,//アイテムを選択中
			CHARBOOK,//キャラクターブックを選択中
		};
	public:
		//画像の座標
		static const std::vector<DirectX::SimpleMath::Vector3> TEXPOS;

		//ステータスの座標
		static const DirectX::SimpleMath::Vector3 STATUS_POS;

		//アイテムの座標
		static const DirectX::SimpleMath::Vector3 ITEM_POS;

		//キャラブックの座標
		static const DirectX::SimpleMath::Vector3 CHARBOOK_POS;

		//各Sceneの情報
		static const std::vector<std::string> SCENES_INFO;
	private:
		//画像の管理クラス
		std::unique_ptr<TextureManager>			m_TextureManager;

		//選択用の画像
		std::vector<OtobeLib::FadeTexture*>		m_selectTextures;

		//スペースキーの裏面の画像
		TextureBase*							m_backSpaceTexture;
	public:
		//コンストラクタ
		MenuScene();

		//デストラクタ
		~MenuScene()override {};
		void Initialize()override;
		void Update()override;
		void Render()override;
		void Finalize()override;
		void Create()override;

	public:
		//現在の状態を設定する
		void SetNowState(const int& now)override { State::SetNowState(now); };

		//現在の状態を取得する
		const int& GetNowState()const override { return State::GetNowState(); };

		//ひとつ前の状態を取得する
		const int& GetPreState()const override { return State::GetPreState(); };

	private:
		//キー入力による選択
		void InputSelect(const int& add, std::function<void(int&)> func);

		//選択を決定する
		void InputEntry();

	public:
		//画像の管理クラスを取得する
		TextureManager* GetTextureManager() { return m_TextureManager.get(); }
	};
}

