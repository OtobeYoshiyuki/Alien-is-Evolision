#include "pch.h"
#include "TextureManager.h"
#include "OtobeLib/FrameWork/Graphics/Graphics.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace OtobeLib;

TextureManager::TextureManager()
{
}

TextureManager::~TextureManager()
{
}

void TextureManager::Update()
{
	for (std::list<std::unique_ptr<ITextureBase>>::iterator iter = m_TextureList.begin();
		iter != m_TextureList.end(); iter++)
	{
		//削除フラグがONになった
		if ((*iter)->GetIsVal())m_TextureList.erase(iter);
	}

	for (std::unique_ptr<ITextureBase>& tex : m_TextureList)
	{
		//更新処理
		tex->Update();
	}
}

void TextureManager::Render()
{
	// 描画順反映パターン
	std::list<ITextureBase*> pTextures;

	std::list<std::unique_ptr<ITextureBase>>::iterator it = m_TextureList.begin();
	std::list<std::unique_ptr<ITextureBase>>::iterator end = m_TextureList.end();

	while (it != end)
	{
		pTextures.push_back((*it).get());
		++it;
	}

	pTextures.sort([](const ITextureBase* a, const ITextureBase* b) {
		return a->GetDrawOrder() > b->GetDrawOrder(); });

	for (ITextureBase* texture : pTextures)
	{
		texture->Render();
	}

}
