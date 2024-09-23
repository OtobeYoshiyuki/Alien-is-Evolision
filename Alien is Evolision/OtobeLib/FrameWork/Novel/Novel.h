#pragma once
#include "OtobeLib/Texture/Interface/ITextureBase.h"
#include "NameWindow/NameWindow.h"
#include "TextWindow/TextWindow.h"
#include "OtobeLib/FrameWork/Story/Story.h"
#include "OtobeLib/Texture/Manager/TextureManager.h"

namespace OtobeLib
{
	class Novel
	{
	private:
		//テキストウィンドウのテクスチャ
		TextWindow* m_textWindow;	

		//名前ウィンドウのテクスチャ
		NameWindow* m_nameWindow;	

		//ストーリー
		std::unique_ptr<Story>	m_story;

	public:
		//コンストラクタ
		Novel();

		//デストラクタ
		~Novel();

		//初期化
		void Initialize(const wchar_t* fileName,TextureManager* texMana);

		//更新
		void Update();

		//終了
		void Finalize();

	public:
		//テキストウィンドウのポインタを取得
		TextWindow* GetTextWindow() { return m_textWindow; }

		//名前ウィンドウのポインタを取得
		NameWindow* GetNameWindow() { return m_nameWindow; }

		//ストーリーのポインタを取得
		Story* GetStory() { return m_story.get(); }
	};
}

