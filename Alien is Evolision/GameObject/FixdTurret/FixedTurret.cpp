#include "pch.h"
#include "FixedTurret.h"
#include "OtobeLib/FrameWork/ResourceManager/ResourceManager.h"
#include "OtobeLib/StackScene/Scene/Play/PlayScene.h"
#include "GameObject/Enemy/Super/SuperEnemy.h"
#include "OtobeLib/FrameWork/Rand/Rand.h"
#include "OtobeLib/FrameWork/Collision/Collision.h"
#include "OtobeLib/FrameWork/StaticData/StaticData.h"
#include "Utils.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace OtobeLib;

//生存時間
const float FixedTurret::SURVIVAL_TIME = 10.0f;

FixedTurret::FixedTurret():
	m_parentBabble(nullptr),//親の泡
	m_JumpState(nullptr),//ジャンプステート
	m_AutoMoveState(nullptr),//自動移動のステート
	m_SurvivalTime(0.0f),//生存時間
	m_bulletFiring(false)//弾が発射されたフラグ
{
}

void FixedTurret::Initialize()
{
	//ResourceManagerを取得する
	ResourceManager* resourceManager = ResourceManager::GetInstance();
	ITextureBase* shabondama1 = resourceManager->GetTexture("シャボン玉1");

	//GameManagerを取得する
	TextureManager* gameManager = GameContext::Get<PlayScene>()->GetGameManager().get();

	//Stageを取得する
	Stage* stage = GameContext::Get<Stage>();

	//マップ情報を取得する
	std::vector<std::vector<ITextureBase*>>& mapAll = stage->GetMap();

	//親の泡を生成する
	std::unique_ptr<SoapBabble> pbabble = std::make_unique<SoapBabble>(shabondama1);

	//所有者のポインタを設定する
	pbabble->SetOwner(this->GetOwner());

	//親の泡の拡大倍率を変更する
	pbabble->SetScale(Vector3(3.0f,3.0f,0.0f));

	//当たり判定の大きさは画像の2倍の大きさとする
	pbabble->SetRadius(Vector3(pbabble->AskOrigin()));

	//座標を設定する
	pbabble->SetPosition(this->GetOwner()->GetPosition());

	//ベクトルを初期化する
	pbabble->SetVelocity(Vector3::Zero);
	pbabble->SetNowState(Vector3::Zero);

	//名前を変更する
	pbabble->SetName("Supersyabondama");

	//原点を設定する
	pbabble->SetOrigin(pbabble->AskOrigin());

	//変数に生ポインタを登録する
	m_parentBabble = pbabble.get();

	//リストに生ポインタを追加する
	m_allBabble.push_back(pbabble.get());

	//マップ情報を追加する
	mapAll[mapAll.size() - 1].push_back(pbabble.get());

	//GameManagerに追加する
	gameManager->Add(std::move(pbabble));

	for (int i = 0; i < CHILD_NUM; i++)
	{
		//子供の泡を生成する
		std::unique_ptr<SoapBabble> pchild = std::make_unique<SoapBabble>(shabondama1);

		//所有者のポインタを設定する
		pchild->SetOwner(this->GetOwner());

		//座標を設定する
		pchild->SetPosition(this->GetOwner()->GetPosition());

		//名前を変更する
		pchild->SetName("Supersyabondama");

		//ベクトルを初期化する
		pchild->SetVelocity(Vector3::Zero);
		pchild->SetNowState(Vector3::Zero);

		//原点を設定する
		pchild->SetOrigin(pchild->AskOrigin());

		//リストに生ポインタを追加する
		m_allBabble.push_back(pchild.get());
		m_childBabbles.push_back(pchild.get());
		//マップ情報を追加する
		mapAll[mapAll.size() - 1].push_back(pchild.get());

		//GameManagerに追加する
		gameManager->Add(std::move(pchild));
	}
	
	//AutoMoveStateを生成
	m_AutoMoveState = std::make_unique<AutoMoveState>();
	m_AutoMoveState->SetExecute(false);
	m_AutoMoveState->SetSpeed(Vector3(4.0f, 0.0f, 0.0f));
	m_AutoMoveState->SetNowState(DIRECT::LEFT);

	//JumpStateを生成
	m_JumpState = std::make_unique<JumpState>();
	m_JumpState->SetExecute(false);
	m_JumpState->SetJumpLimit(200.0f);
	m_JumpState->SetJumpSpeed(4.0f);
	m_JumpState->SetCorrection(0.4f);
	m_JumpState->SetJumpLimitCount(1);
	m_JumpState->SetJumpOption(true);

	//リストにステートを追加する
	m_StateList.push_back(m_AutoMoveState.get());
	m_StateList.push_back(m_JumpState.get());
}

void FixedTurret::Update()
{
	//ステートの更新処理
	this->UpdateState();

	//泡の更新処理
	this->UpdateBabble();
}

void FixedTurret::RandShake(SoapBabble* child)
{
	//RandMachineを取得する
	Rand* random = Rand::GetInstance();

	//Xの乱数を振る
	double x = random->RandomMachine(-3.0, 3.0);

	//Yの乱数を振る
	double y = random->RandomMachine(-3.0, 3.0);

	//ベクトルを設定する
	child->SetNowState(Vector3((float)x, (float)y, 0.0f));
}

void FixedTurret::InitTurret()
{
	//生存時間を初期化する
	m_SurvivalTime = 0.0f;

	//フラグを落とす
	m_bulletFiring = false;

	for (SoapBabble* babble : m_allBabble)
	{
		//泡を消す
		babble->SetState(ITextureBase::STATE::NONE);

		//入れ替え用のベクトルを初期化する
		babble->SetNowState(Vector3::Zero);
	}
}

void FixedTurret::UpdateState()
{
	//ベクトルの初期化
	Vector3 vel = Vector3::Zero;

	for (MoveState* state : m_StateList)
	{
		//実行中のベクトルを取得する
		vel += state->Update();
	}

	//親から子まで全て動かす
	for (SoapBabble* babble : m_allBabble)
	{
		//入れ替え用のベクトルを取得する
		Vector3 temp = babble->GetNowState();

		//ベクトルを設定する
		babble->SetVelocity(vel + temp);
	}
}

void FixedTurret::UpdateBabble()
{
	//静的データの取得
	StaticData* staticData = StaticData::GetInstance();

	//描画されている数
	int drawNum = NULL;

	//泡を全部発射しているとき
	for (SoapBabble* child : m_childBabbles)
	{
		//描画されている
		if (child->GetState() == ITextureBase::STATE::DRAW)drawNum++;
	}

	//全弾発射していたら、フラグを起動させる(フラグが起動していないときのみ)
	if (drawNum == (int)m_childBabbles.size() && !m_bulletFiring)m_bulletFiring = true;

	//弾を全部発射していたら、
	if (m_bulletFiring)
	{
		//思考時間が経過したら、元に戻す
		if (m_SurvivalTime > SURVIVAL_TIME)
		{
			//情報を初期化する
			this->InitTurret();

			//思考時間の更新を行わない
			return;
		}

		//思考時間を更新する
		m_SurvivalTime += staticData->Get_FloatData("elapsedTime");
	}
}
