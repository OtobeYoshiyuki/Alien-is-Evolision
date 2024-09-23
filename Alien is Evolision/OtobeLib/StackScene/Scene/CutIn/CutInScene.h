#pragma once
#include "OtobeLib/StackScene/Interface/ISceneBase.h"
#include "OtobeLib/Texture/TextureBase/TextureBase.h"
#include "OtobeLib/Texture/Manager/TextureManager.h"
#include "Effect/CutInManager/CutInManager.h"

namespace OtobeLib
{
	class CutInScene :public ISceneBase
	{
	private:
		//カットインの管理クラス
		std::unique_ptr<CutInManager>   m_CutInManager;
		//Textureの管理クラス
		std::unique_ptr<TextureManager>	m_TextureManager;
	public:
		//コンストラクタ
		CutInScene();

		//デストラクタ
		~CutInScene()override = default;

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
		//TextureManagerを取得する
		TextureManager* GetTextureManager() { return m_TextureManager.get(); }

		//CutInManagerを取得する
		CutInManager* GetCutInManager() { return m_CutInManager.get(); }
	};
}

