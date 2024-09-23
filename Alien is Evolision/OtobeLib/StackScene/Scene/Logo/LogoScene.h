#pragma once
#include "OtobeLib/StackScene/Interface/ISceneBase.h"
#include "OtobeLib/Texture/TextureBase/TextureBase.h"
#include "OtobeLib/Texture/Manager/TextureManager.h"

namespace OtobeLib
{
	class LogoScene :public ISceneBase
	{
	private:
		//画像の管理クラス
		std::unique_ptr<TextureManager>	m_TextureManager;
	public:
		//コンストラクタ
		LogoScene();

		//デストラクタ
		~LogoScene()override {};

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

