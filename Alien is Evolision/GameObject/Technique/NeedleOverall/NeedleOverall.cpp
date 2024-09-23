#include "pch.h"
#include "NeedleOverall.h"
#include "OtobeLib/FrameWork/ResourceManager/ResourceManager.h"
#include "OtobeLib/StackScene/Scene/Play/PlayScene.h"
#include "GameContext.h"
#include "GameObject/Player/CoroKun.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace OtobeLib;

NeedleOverall::NeedleOverall():m_Active(false)
{
	//ResourceManagerを取得する
	ResourceManager* resourceManager = ResourceManager::GetInstance();

	//TextureManagerを取得する
	TextureManager* texMana = GameContext::Get<PlayScene>()->GetGameManager().get();

	//CollisionManagerを取得する
	CollisionManager* colMana = GameContext::Get<CollisionManager>();

	//使用するテクスチャを取得する
	ITextureBase* needle = resourceManager->GetTexture("ニードル");

	for (int i = 0; i < NEEDLERANGE_NUM; i++)
	{
		//ニードルを生成
		std::unique_ptr<Needle> pNeedleOver = std::make_unique<Needle>(needle);
		//原点を設定する
		pNeedleOver->SetOrigin(pNeedleOver->AskOrigin());
		//描画を切る
		pNeedleOver->SetState(ITextureBase::STATE::NONE);
		//描画順を変更する
		pNeedleOver->SetDrawOrder(2);
		//入れ替え用のベクトルの初期化をする
		pNeedleOver->SetTempVel(Vector3::Zero);
		//名前を変更する
		pNeedleOver->SetName("shortNeedle");
		//当たり判定を登録する
		colMana->Add(pNeedleOver.get());
		//リストに追加する
		m_NeedleList.push_back(pNeedleOver.get());
		//テクスチャを追加する
		texMana->Add(std::move(pNeedleOver));
	}

	//GameContextに登録する
	GameContext::Register<NeedleOverall>(this);
}

void NeedleOverall::Initialize()
{
}

void NeedleOverall::Update()
{
	//Switch文で処理を切り替える
	switch (this->GetNowState())
	{
	case NEEDLEACTION::MOPE:
		this->State_Mope();
		break;
	case NEEDLEACTION::RELEASE:
		this->State_Release();
		break;
	case NEEDLEACTION::RELOAD:
		this->State_Reload();
		break;
	}
}

void NeedleOverall::State_Mope()
{
	//発射準備のステート
	
	//縦方向のベクトルを初期化する
	Vector3 vel = Vector3::Zero;

	//角度を設定する
	float angle = 0.0f;

	//プレイヤーを取得する
	CoroKun* player = GameContext::Get<CoroKun>();

	//針を1発ずつ装填していく
	for (Needle* needle : m_NeedleList)
	{
		//ベクトルを設定する
		vel = Vector3(0.0f, -70.0f, 0.0f);

		//回転行列を設定する（スクリーン座標のためZ軸回転のみ）
		Matrix rot = Matrix::CreateRotationZ(XMConvertToRadians(angle));

		//角度を設定する
		needle->SetAngle(angle);

		//回転度を30度ずつ更新する
		angle += 30.0f;

		//ベクトルを回転させる
		vel = Vector3::Transform(vel, rot);

		//プレイヤーの周囲に座標を設定する
		needle->SetPosition(player->GetPosition() + vel);

		//針を描画する
		needle->SetState(ITextureBase::STATE::DRAW);

		//ベクトルを正規化する
		vel.Normalize();

		//ベクトルを補正する
		vel *= CoroKun::SPEED * 2;

		//ベクトルを設定する
		needle->SetTempVel(vel);
	}
}

void NeedleOverall::State_Release()
{
	//とげキャノン発射のステート

	//実行関数を落とす
	this->SetActive(false);

	//描画されていない数を数える
	int drawNum = NULL;

	//針を一斉に発射する
	for (Needle* needle:m_NeedleList)
	{
		//ベクトルを設定する
		needle->SetVelocity(needle->GetTempVel());

		if (needle->GetCheck() && needle->GetState() == ITextureBase::STATE::DRAW)
		{
			//針と入れ替え用のベクトルの初期化をする
			needle->SetVelocity(Vector3::Zero);
			needle->SetTempVel(Vector3::Zero);
			//描画を切る
			needle->SetState(ITextureBase::STATE::NONE);
			needle->SetCheck(false);
		}

		if (needle->GetState() == ITextureBase::STATE::NONE)
		{
			//数を更新する
			drawNum++;
		}
	}

	//全部当たっていたら、リロードに移行する
	if (drawNum >= (int)m_NeedleList.size())
	{
		this->SetNowState(NEEDLEACTION::RELOAD);
	}
}

void NeedleOverall::State_Reload()
{
	//とげキャノンのリロードのステート

	this->SetNowState(NEEDLEACTION::NEEDLENONE);
}

