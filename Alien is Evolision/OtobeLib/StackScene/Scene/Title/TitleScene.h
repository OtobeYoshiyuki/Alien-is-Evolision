#pragma once
#include "OtobeLib/StackScene/Interface/ISceneBase.h"
#include "OtobeLib/Texture/Interface/ITextureBase.h"
#include "OtobeLib/Texture/Manager/TextureManager.h"
#include "OtobeLib/Texture/Blinking/Blinking.h"
#include "GameObject/Arrow/Arrow.h"
#include <string>
#include <vector>
namespace OtobeLib
{
	class TitleScene :public ISceneBase
	{
	public:
		//ステージセレクトへ
		static const std::pair<std::string,DirectX::SimpleMath::Vector2> NEXT_STAGESELECT;

		//ランキングへ
		static const std::pair<std::string,DirectX::SimpleMath::Vector2> NEXT_RANKING;

		//オープニングへ
		static const std::pair<std::string, DirectX::SimpleMath::Vector2> NEXT_OPENING;
	private:
		//画像の管理クラス
		std::unique_ptr<TextureManager> m_TextureManager;

		//文字の点滅クラス
		std::unique_ptr<Blinking>	m_blink;

		//Arrowクラス
		Arrow*						m_rightArrowTexture;
		Arrow*						m_leftArrowTexture;

		//表示ターゲット
		std::pair<std::string,DirectX::SimpleMath::Vector2>		m_targetStrings;
	public:
		//コンストラクタ
		TitleScene();

		//デストラクタ
		~TitleScene()override {};

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
	};
}

