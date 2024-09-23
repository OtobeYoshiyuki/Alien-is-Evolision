#include "pch.h"
#include "CollisionManager.h"
#include <map>
#include <utility>

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace OtobeLib;

CollisionManager::CollisionManager():m_Collision(nullptr)
{
}

void CollisionManager::Initialize()
{
	for (std::map<std::string,std::vector<std::string>>::const_iterator iter = m_CollisionGrop.begin();
		iter != m_CollisionGrop.end(); iter++)
	{
		//当たり判定の主役を取得する
		const std::string& first = iter->first;

		//当たり判定の主人公を設定する
		for (ITextureBase* tex : m_TextureList)
		{
			if (first == tex->GetName())
			{
				m_HeroCollision.push_back(tex);
			}
		}

		//当たるものを設定する
		std::vector<std::string> otherList = m_CollisionGrop.find(first)->second;

		for (const std::string& other:otherList)
		{
			//当たる他のものを設定する
			for (ITextureBase* tex : m_TextureList)
			{
				if (other == tex->GetName())
				{
					m_CollisionList[first].push_back(tex);
				}
			}
		}
	}
}

void CollisionManager::Update()
{
	//2Dの当たり判定の処理
	this->Object2D();
}

void CollisionManager::AddUseOther(ITextureBase* other, ITextureBase* hero, const float& length)
{
	//始点終点の取得
	RECT rect1 = hero->GetRect();
	RECT rect2 = other->GetRect();

	//画像の半径の取得
	Vector3 Size1 = Vector3(rect1.right - rect1.left, rect1.bottom - rect1.top, 0.0f);
	Vector3 Size2 = Vector3(rect2.right - rect2.left, rect2.bottom - rect2.top, 0.0f);

	Size1 *= 0.5f;
	Size2 *= 0.5f;

	//それぞれの座標を求める（座標は画像の中心で統一する）
	Vector3 Pos1 = hero->GetPosition() + Size1 - hero->GetOrigin();
	Vector3 Pos2 = other->GetPosition() + Size2 - other->GetOrigin();

	//距離を算出する
	Vector3 Length = Pos1 - Pos2;

	//絶対値に変換する
	Length = Vector3(fabsf(Length.x), fabsf(Length.y), fabsf(Length.z));

	//指定した距離より短い場合は、追加しない
	if (Length.x < length && Length.y < length && Length.z < length)
	{
		if (other->GetState() == ITextureBase::STATE::DRAW &&
			hero->GetState() == ITextureBase::STATE::DRAW)
		{
			m_UseOther[hero->GetName()].push_back(other);
		}
	}
}


void CollisionManager::Object2D()
{
	for (ITextureBase* object1 : m_UseHero)
	{
		//設定してある当たるオブジェクトのリストを取得する
		std::vector<ITextureBase*> objectList = m_UseOther[object1->GetName()];

		for (ITextureBase* object2 : objectList)
		{
			ICollisionBase<ITextureBase>* collision1 = object1->GetThisCollision();
			ICollisionBase<ITextureBase>* collision2 = object2->GetThisCollision();

			if (!collision1 || !collision2)continue;

			//当たっているオブジェクトが自分自身じゃないとき
			if (object1 != object2)
			{
				m_Collision = Collision::HitCheck_OBB;

				//描画されていなければ、当たり判定を取らない
				if (object1->GetState() != ITextureBase::STATE::DRAW)continue;
				if (object2->GetState() != ITextureBase::STATE::DRAW)continue;

				if (Collision::HitCheck_OBB(object1, object2))
				{
					//当たり判定の実行関数
					collision1->OnCollisionEnter(object2);

					//当たり判定の実行関数
					collision2->OnCollisionEnter(object1);

					if (m_IDGroup.find(object1->GetName()) != m_IDGroup.end() &&
						m_IDGroup.find(object2->GetName()) != m_IDGroup.end())
					{
						if (collision1->Find(collision2->GetID()) == -1)
						{
							collision1->Add(collision2->GetID());
						}
						if (collision2->Find(collision1->GetID()) == -1)
						{
							collision2->Add(collision1->GetID());
						}
							
					}
				}
				else
				{
					//当たり判定の実行関数
					collision1->NonCollisionEnter(object2);

					//当たり判定の実行関数
					collision2->NonCollisionEnter(object1);
				}
			}
		}

	}

	//毎フレームコンテナの中身をクリアする
	m_UseHero.clear();
	m_UseOther.clear();
}

