#include "pch.h"
#include "NormalBullet.h"
#include "GameContext.h"
#include "OtobeLib/StackScene/Scene/Play/PlayScene.h"
#include "OtobeLib/FrameWork/Collision/CollisionManager/CollisionManager.h"
#include "OtobeLib/FrameWork/ResourceManager/ResourceManager.h"
#include "GameObject/Player/NormalTechnique/NormalTechnique.h"
#include "GameObject/Gage/NormalGage/NormalGage.h"
#include "OtobeLib/FrameWork/SoundManager/SoundManager.h"
#include "Utils.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace OtobeLib;

NormalBullet::NormalBullet(ITextureBase* texture):Particle(texture),m_Temp(Vector3::Zero),m_PlayerTarget(Vector3::Zero)
{

}

void NormalBullet::Initialize()
{
	this->SetNowState(ENERGY_CHAGE::ENERGYNONE);

	this->SetVelocity(Vector3::Zero);

	this->SetState(STATE::NONE);
}

void NormalBullet::Update()
{
	//CoroKunを取得する
	CoroKun* player = GameContext::Get<CoroKun>();

	//NormalTechniqueを取得する
	NormalTechnique* Ntch = GameContext::Get<NormalTechnique>();

	//パーティクルの更新を行う
	Particle::Update();

	//当たり判定を追加する
	GameContext::Get<CollisionManager>()->AddUseHero(this);

	//２つの座標からベクトルを算出する

	//PlayerTargetが設定されていないときは、何もしない
	if (m_PlayerTarget == Vector3::Zero)return;

	Vector3 Len = Length(m_PlayerTarget, this->GetPosition());

	if (Len.x <= 0.0f && this->GetNowState() == ENERGY_CHAGE::ENERGYATACK)
	{
		this->SetState(ITextureBase::STATE::NONE);

		this->SetVelocity(Vector3::Zero);

		this->SetNowState(ENERGY_CHAGE::ENERGYNONE);

		//PlayerTargetを初期化する
		m_PlayerTarget = Vector3::Zero;
	}
}

void NormalBullet::Render()
{
	//パーティクルの描画を行う
	Particle::Render();
}

void NormalBullet::Finalize()
{
}

void NormalBullet::OnCollisionEnter(ITextureBase* obj)
{
	//NormalTechniqueを取得する
	NormalTechnique* Ntch = GameContext::Get<NormalTechnique>();

	//弾にキャストする
	NormalBullet* normalBullet = dynamic_cast<NormalBullet*>(this);

	//SoundManagerを取得する
	SoundManager* soundManager = SoundManager::GetInstance();

	Particle::OnCollisionEnter(obj);

	//現在打っている弾と、先頭の弾が一緒の時、初期化する
	if (Ntch->GetFrontBullet() == normalBullet)
	{
		//ゲージの初期化
		Ntch->GetNormalGage()->Initialize();

		//弾の初期化
		Ntch->GetFrontBullet()->Initialize();

		//SEを止める
		soundManager->Stop(CRI_CUESHEET_0__14_チャージ);
	}

	//チャージ中に当たった
	if (this->GetNowState() == ENERGY_CHAGE::ENERGYNONE)
	{
		//フラグを起こす
		Ntch->SetCheckPush(true);

		//フラグを起こす
		Ntch->SetCheckRelease(true);
	}

	//描画を消す
	this->SetState(STATE::NONE);

	//ベクトルを初期化する
	this->SetVelocity(Vector3::Zero);

	this->SetNowState(ENERGY_CHAGE::ENERGYNONE);

	//PlayerTargetを初期化する
	m_PlayerTarget = Vector3::Zero;
}

void NormalBullet::NonCollisionEnter(ITextureBase* obj)
{
	Particle::NonCollisionEnter(obj);
}
