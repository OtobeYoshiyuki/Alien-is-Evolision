#include "pch.h"
#include "Resurrection.h"
#include "OtobeLib/FrameWork/ResourceManager/ResourceManager.h"
#include "OtobeLib/StackScene/Scene/Play/PlayScene.h"
#include "GameContext.h"
#include "GameObject/Player/CoroKun.h"
#include "OtobeLib/FrameWork/StaticData/StaticData.h"
#include "OtobeLib/FrameWork/SoundManager/SoundManager.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace OtobeLib;

//待機から攻撃に移るまでの時間
const float Resurrection::MOVETIME = 60.0f;

Resurrection::Resurrection():m_Active(false),m_Timer(0.0f)
{
	//ResourceManagerを取得する
	ResourceManager* resourceManager = ResourceManager::GetInstance();

	//TextureManagerを取得する
	TextureManager* texMana = GameContext::Get<PlayScene>()->GetGameManager().get();

	//CollisionManagerを取得する
	CollisionManager* colMana = GameContext::Get<CollisionManager>();

	//使用するテクスチャを取得する
	ITextureBase* needle = resourceManager->GetTexture("ニードル");

	for (int i = 0; i < NEEDLESHORT_NUM; i++)
	{
		//ニードルを生成
		std::unique_ptr<Needle> pNeedleWide = std::make_unique<Needle>(needle);
		//原点を設定する
		pNeedleWide->SetOrigin(pNeedleWide->AskOrigin());
		//描画を切る
		pNeedleWide->SetState(ITextureBase::STATE::NONE);
		//描画順を変更する
		pNeedleWide->SetDrawOrder(2);
		//名前を変更する
		pNeedleWide->SetName("longNeedle");
		//リストに追加する
		m_NeedleList.push_back(pNeedleWide.get());
		//テクスチャを追加する
		texMana->Add(std::move(pNeedleWide));
	}

	//GameContextに登録する
	GameContext::Register<Resurrection>(this);
}

void Resurrection::Initialize()
{
}

void Resurrection::Update()
{
	//ステートによって、処理を切り替える
	switch (this->GetNowState())
	{
	case RENEEDLE::LITTLE:
		this->State_Little();
		break;
	case RENEEDLE::ATACK:
		this->State_Atack();
		break;
	case RENEEDLE::END:
		this->State_End();
		break;
	}

	//泡と当たり判定を取る

}

void Resurrection::State_Little()
{
	//待機中のステート

	//SoundManagerを取得する
	SoundManager* soundManager = SoundManager::GetInstance();

	//静的データの取得
	StaticData* staticData = StaticData::GetInstance();

	//ベクトルを初期化する
	Vector3 vel = Vector3::Zero;

	//角度を設定する
	float angle = 0.0f;

	//プレイヤーを取得する
	CoroKun* player = GameContext::Get<CoroKun>();

	//タイマーが制限時間を超えたら、攻撃に移行する
	if (m_Timer >MOVETIME)
	{
		//ステートを切り替える
		this->SetNowState(RENEEDLE::ATACK);
		//タイマーを初期化する
		m_Timer = 0.0f;
		//SEを再生する
		soundManager->Play(CRI_CUESHEET_0__18_針放出);
		return;
	}

	//針を1発ずつ装填していく
	for (Needle* needle : m_NeedleList)
	{
		//ベクトルを設定する
		vel = Vector3(0.0f, -55.0f, 0.0f);

		//回転行列を設定する
		Matrix rot = Matrix::CreateRotationZ(XMConvertToRadians(needle->GetAngle()));

		//ベクトルを回転させる
		vel = Vector3::Transform(vel, rot);

		//角度を設定する
		needle->SetAngle(angle);

		//回転度を30度ずつ更新する
		angle += 30.0f;

		//針の拡大倍率を設定する
		needle->SetScale(Vector3(0.75f));

		//プレイヤーの周囲に座標を設定する
		needle->SetPosition(player->GetPosition() + vel);

		//針を描画する
		needle->SetState(ITextureBase::STATE::DRAW);
	}

	//タイマーを更新する
	m_Timer ++;
}

void Resurrection::State_Atack()
{
	//攻撃中のステート

	//静的データの取得
	StaticData* staticData = StaticData::GetInstance();

	//プレイヤーを取得する
	CoroKun* player = GameContext::Get<CoroKun>();

	//ベクトルを初期化する
	Vector3 vel = Vector3::Zero;

	//タイマーが制限時間を超えたら、攻撃に移行する
	if (m_Timer > MOVETIME)
	{
		//ステートを切り替える
		this->SetNowState(RENEEDLE::END);
		//タイマーを初期化する
		m_Timer = 0.0f;
		return;
	}

	//針を一斉に周囲に出す
	for (Needle* needle : m_NeedleList)
	{
		//ベクトルを設定する
		vel = Vector3(0.0f, -105.0f, 0.0f);

		//回転行列を設定する
		Matrix rot = Matrix::CreateRotationZ(XMConvertToRadians(needle->GetAngle()));

		//ベクトルを回転させる
		vel = Vector3::Transform(vel, rot);

		//針の拡大倍率を設定する
		needle->SetScale(Vector3(1.5f,3.0f,0.75f));

		//プレイヤーの周囲に座標を設定する
		needle->SetPosition(player->GetPosition() + vel);
	}

	//タイマーを更新する
	m_Timer ++;
}

void Resurrection::State_End()
{
	//終了のステート

	//針のパラメーターを元に戻す
	for (Needle* needle : m_NeedleList)
	{
		//描画を切る
		needle->SetState(ITextureBase::STATE::NONE);
	}

	//実行関数を落とす
	m_Active = false;

	//タイマーを初期化する
	m_Timer = 0.0f;

	//ステートを切り替える
	this->SetNowState(RENEEDLE::RENONE);
}
