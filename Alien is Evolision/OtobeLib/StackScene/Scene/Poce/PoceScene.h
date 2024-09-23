#pragma once
#include "OtobeLib/StackScene/Interface/ISceneBase.h"
#include "OtobeLib/Texture/Interface/ITextureBase.h"
#include "OtobeLib/Texture/Manager/TextureManager.h"
#include "TemplateState.h"
#include <string>
#include <vector>
namespace OtobeLib
{
	class PoceScene :public ISceneBase,State<int>
	{
	public:
		//フォントの情報
		static const std::vector<std::pair<std::string, DirectX::SimpleMath::Vector2>> FONT_INFO;
	private:
		//画像の管理クラス
		std::unique_ptr<TextureManager>	m_TextureManager;

		//各シーンへの遷移画像
		std::vector<ITextureBase*>		m_sceneTexture;
	public:
		PoceScene();
		~PoceScene()override {};
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
	};
}

