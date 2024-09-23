#include "pch.h"
#include "Item.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace OtobeLib;

Item::Item(ITextureBase* texture):
	TextureBase(texture),//基底クラスに情報を送信する
	m_getFlag(false)//取得済みフラグ
{
}

void Item::Initialize()
{
}

void Item::Update()
{
	//画像の更新処理
	TextureBase::Update();
}

void Item::Render()
{
	//画像の描画処理
	TextureBase::Render();
}

void Item::Finalize()
{
}

void Item::OnCollisionEnter(ITextureBase* obj)
{
	//当たり判定の処理
	CollisionBase::OnCollisionEnter(obj);
}

void Item::NonCollisionEnter(ITextureBase* obj)
{
	//当たり判定の処理
	CollisionBase::NonCollisionEnter(obj);
}
