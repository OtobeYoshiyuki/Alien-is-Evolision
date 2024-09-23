#include "pch.h"
#include "NormalTechnique.h"
#include "GameContext.h"
#include "OtobeLib/StackScene/Scene/Play/PlayScene.h"
#include "OtobeLib/FrameWork/Collision/CollisionManager/CollisionManager.h"
#include "OtobeLib/FrameWork/ResourceManager/ResourceManager.h"
#include "GameObject/Player/CoroKun.h"
#include "GameObject/Gage/NormalGage/NormalGage.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace OtobeLib;

NormalTechnique::NormalTechnique():
	m_BackGage(nullptr),//ゲージの裏面
	m_NormalGage(nullptr),//ゲージの表面
	m_FrontBullet(nullptr),//先頭の弾
	m_Active(false),//実行可能
	m_checkPush(false),//ダメージ判定用のフラグ
	m_checkRelease(false),//ダメージ判定用のフラグ
	m_time(0.0f)//時間
{
	//ResourceManagerを取得する
	ResourceManager* resourceManager = ResourceManager::GetInstance();

	//TextureManagerを取得する
	TextureManager* gameManager = GameContext::Get<PlayScene>()->GetGameManager().get();

	//使うテクスチャを取得する
	ITextureBase* gagetex = resourceManager->GetTexture("WhiteBack");
	ITextureBase* particle = resourceManager->GetTexture("粒子");

	//技ゲージを生成する
	std::unique_ptr<NormalGage> pNormalGage = std::make_unique<NormalGage>(gagetex);
	pNormalGage->SetState(ITextureBase::STATE::DRAW);
	pNormalGage->SetDrawOrder(0);
	pNormalGage->SetRect(RECT{ 0,0,0,20 });
	pNormalGage->SetColor(Colors::Blue);
	pNormalGage->SetNowState(NORMALSTATE::LEFTUP);
	m_NormalGage = pNormalGage.get();

	//技ゲージの裏面を生成する
	std::unique_ptr<ITextureBase> pBackGage = std::make_unique<TextureBase>(gagetex);
	pBackGage->SetState(ITextureBase::STATE::DRAW);
	pBackGage->SetDrawOrder(1);
	pBackGage->SetRect(RECT{ 0,0,100,20 });
	pBackGage->SetColor(Colors::White);
	m_BackGage = pBackGage.get();

	//TextureManagerに追加する
	gameManager->Add(std::move(pNormalGage));
	gameManager->Add(std::move(pBackGage));

	//通常弾を10弾生成する
	for (int i = 0; i < BULLET_NUM; i++)
	{
		std::unique_ptr<NormalBullet>pBullet = std::make_unique<NormalBullet>(particle);
		pBullet->SetDrawOrder(2);
		pBullet->SetState(ITextureBase::STATE::NONE);
		pBullet->SetOrigin(pBullet->AskOrigin());
		pBullet->SetName("通常弾");
		m_NormalBulletsList.push_back(pBullet.get());
		gameManager->Add(std::move(pBullet));
	}

	//GameContextに登録する
	GameContext::Register<NormalTechnique>(this);

	//m_FrontBullet = this->GetAllBulletFront();
}

void NormalTechnique::Initialize()
{
}

void NormalTechnique::Update()
{
	//プレイヤーを取得する
	CoroKun* player = GameContext::Get<CoroKun>();

	//プレイヤーの座標をもとに、ゲージの座標を決定する
	m_NormalGage->SetPosition(player->GetPosition() - Vector2(50.0f,70.0f));
	m_BackGage->SetPosition(player->GetPosition() - Vector2(50.0f,70.0f));
}

NormalBullet* NormalTechnique::GetAllBulletFront()
{
	//発射待機している先頭のポインタを取得する

	for (NormalBullet* nBullet : m_NormalBulletsList)
	{
		if (nBullet->GetState() == ITextureBase::STATE::NONE)
		{
			return nBullet;
		}
	}

	return nullptr;
}
