#pragma once
#include "OtobeLib/FrameWork/Singleton/Singleton.h"
#include "OtobeLib/Texture/Interface/ITextureBase.h"
#include "OtobeLib/Texture/TextureBase/TextureBase.h"
namespace OtobeLib
{
	class ResourceManager:public Singleton<ResourceManager>
	{
		friend Singleton<ResourceManager>;
		using MapTexture = std::map<std::string, std::unique_ptr<ITextureBase>>;
	private:
		//読み込まれたテクスチャの情報
		MapTexture			m_TextureList;
	private:
		//コンストラクタ
		ResourceManager();
	public:
		//デストラクタ
		~ResourceManager();

		//リソースを読み込み
		bool LoadResource();

		//指定したテクスチャの情報を取得
		ITextureBase* GetTexture(const std::string name)const
		{
			//指定したキーのテクスチャを探す
			MapTexture::const_iterator iter = m_TextureList.find(name);
			//見つかったらテクスチャの情報を返す
			if (iter != m_TextureList.end())return iter->second.get();
			//見つからなかった
			return nullptr;
		}
	private:
		//テクスチャを読み込み
		bool LoadTexture();

		//テクスチャのキーの挿入
		void AddTex(TextureBase* tex) 
		{ 
			m_TextureList.insert(std::make_pair(tex->GetName(),std::make_unique<TextureBase>(tex)));
		}

		//TextureのMapを取得
		const MapTexture& GetMapTexture()const { return m_TextureList; }
	};
}

