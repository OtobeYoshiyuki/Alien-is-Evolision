#pragma once
#include "OtobeLib/StackScene/Interface/ISceneBase.h"
#include "OtobeLib/Texture/Interface/ITextureBase.h"
#include "OtobeLib/Texture/TextureBase/TextureBase.h"
#include "OtobeLib/Texture/Manager/TextureManager.h"
#include <map>
#include <vector>
#include <string>

namespace OtobeLib
{
	class StatusScene:public ISceneBase
	{
		enum class STATUS_INFO :int
		{
			LEVEL = 1,HP,MP,AT,DF,SP,NEXT,TOTAL,
		};
	private:
		//ステータス関連の連想配列
		static std::vector<std::pair<std::string, DirectX::SimpleMath::Vector2>> s_statusInfo;

		//コマンド関連の連想配列
		static const std::vector<std::pair<std::string, DirectX::SimpleMath::Vector2>> COMMAND_INFO;
	private:
		//画像の管理クラス
		std::unique_ptr<TextureManager>	m_TextureManager;

		//スペースキーの裏面の画像
		TextureBase* m_backSpaceTexture;
	public:
		//コンストラクタ
		StatusScene();

		//デストラクタ
		~StatusScene()override {};

		//初期化
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

