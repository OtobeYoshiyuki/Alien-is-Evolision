#pragma once
#include "OtobeLib/FrameWork/Collision/Collision.h"
#include "OtobeLib/FrameWork/Collision/CollisionTemplate/ICollisionBase.h"
#include "OtobeLib/Texture/Interface/ITextureBase.h"
#include <vector>
#include <list>
#include <map>
#include <string>
#include <functional>
namespace OtobeLib
{
	class Collision;
	class CollisionManager
	{
	private:
		//連動させるIDを設定する
		std::multimap<std::string, std::string>		m_IDGroup;

		//当たるものを設定する
		std::map<std::string, std::vector<std::string>>		m_CollisionGrop;

		//プレイヤー側の当たり判定
		std::vector<ITextureBase*>					m_HeroCollision;	

		//テクスチャのリスト
		std::vector<ITextureBase*>					m_TextureList;		

		//当たり判定のリスト
		std::map<std::string, std::vector<ITextureBase*>> m_CollisionList;

		//プレイヤー側の使う当たり判定
		std::vector<ITextureBase*>					m_UseHero;

		//他の使う当たり判定
		std::map<std::string, std::vector<ITextureBase*>> m_UseOther;

		//呼び出す関数オブジェクト
		std::function<bool(ITextureBase*, ITextureBase*)>	m_Collision;

	public:
		//コンストラクタ
		CollisionManager();	

		//デストラクタ
		~CollisionManager() {}	

		//初期化
		void Initialize();

		//更新処理
		void Update();			

		//テクスチャリストに追加
		void Add(ITextureBase* obj) { m_TextureList.push_back(obj);}	

		//使うものを追加する
		void AddUseHero(ITextureBase* obj)
		{ 
			if (obj->GetState() == ITextureBase::STATE::DRAW)
			{
				m_UseHero.push_back(obj);
			}
		}

		void AddUseOther(ITextureBase* obj, const std::string& heroName) 
		{
			if (obj->GetState() == ITextureBase::STATE::DRAW)
			{
				m_UseOther[heroName].push_back(obj);
			}
		}
		void AddUseOther(ITextureBase* other, ITextureBase* hero, const float& length);

		//IDの判定を取るものを追加する
		void SetIDGroup(const std::string& A, const std::string& B)			
		{	
			m_IDGroup.insert(std::pair<std::string, std::string>(A, B));
			m_IDGroup.insert(std::pair<std::string, std::string>(B, A));
		}

		//当たるものを設定する
		void SetCollisionGrop(const std::string& Hero, const std::string& Other)
		{
			m_CollisionGrop[Hero].push_back(Other);
		}

		//当たり判定の主役をリセットする
		void ResetUseHero() { m_UseHero.clear(); }

	private:

		//2Dオブジェクトの当たり判定の更新
		void Object2D();												
	};
}

