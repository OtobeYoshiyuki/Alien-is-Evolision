#pragma once
#include "OtobeLib/Texture/Interface/ITextureBase.h"
namespace OtobeLib
{
	class TextureManager
	{	
	private:
		std::list<std::unique_ptr<ITextureBase>>	m_TextureList;//テクスチャのリスト
	public:
		//コンストラクタ
		TextureManager();
		//デストラクタ
		~TextureManager();
		//更新処理
		void Update();	
		//描画処理
		void Render();

	public:
		//追加
		void Add(std::unique_ptr<ITextureBase>&& tex) { m_TextureList.push_back(std::move(tex)); }
		//最後に追加したものを取得する
		ITextureBase* GetBack() { return m_TextureList.back().get(); }
		//リストを取得する
		std::list<std::unique_ptr<ITextureBase>>& GetTextureList() { return m_TextureList; }
	};
}

