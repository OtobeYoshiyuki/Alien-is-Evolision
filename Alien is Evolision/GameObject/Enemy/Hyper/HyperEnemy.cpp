#include "HyperEnemy.h"
#include "pch.h"
#include "HyperEnemy.h"
#include "OtobeLib/FrameWork/ResourceManager/ResourceManager.h"
#include "OtobeLib/StackScene/Scene/Play/PlayScene.h"
#include "GameObject/Stage/Stage.h"
#include "GameObject/Stage/Maptip/Floor/Floor.h"
#include "GameObject/Stage/Maptip/MapTip.h"
#include "GameObject/Player/CoroKun.h"
#include "OtobeLib/FrameWork/Rand/Rand.h"
#include "OtobeLib/FrameWork/StaticData/StaticData.h"
#include "GameContext.h"	
#include "OtobeLib/Status/StatusCalc/StatusCalc.h"
#include "OtobeLib/FrameWork/Score/Score.h"
#include "GameObject/Item/Drop/ItemDrop.h"
#include "OtobeLib/FrameWork/SoundManager/SoundManager.h"
#include "Utils.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace OtobeLib;

//敵の思考時間
const float HyperEnemy::THINKTIME = 5.0f;

//敵の強さ(経験値に影響する)
const float HyperEnemy::STRENGTH = 300.0f;

HyperEnemy::HyperEnemy(ITextureBase* texture)
	:TextureBase(texture),//基底クラスに情報を送信する
	m_JumpState(nullptr),//JumpState
	m_AutoMoveState(nullptr),//AutoMoveState
	m_AtackTime(NULL),//攻撃時間
	m_ArrayBabble(-1),//BabulListの参照用の添え字
	m_ThinkingTime(0.0f),//敵の思考時間
	m_HpGage(nullptr),//HPゲージ
	m_HpBack(nullptr),//HPゲージの裏面
	m_Result(false),//終了フラグ
	m_experience(nullptr),//経験値の画像
	m_masicEffectManager(nullptr),//技のエフェクトの管理クラス
	m_frameCount(NULL),//フレーム数のカウント
	m_damageCheck(false)//ダメージのチェックフラグ
{
	//ResourcesManagerを取得する
	ResourceManager* resourceManager = ResourceManager::GetInstance();
	
	//GameManagerを取得する
	TextureManager* gameManager = GameContext::Get<PlayScene>()->GetGameManager().get();

	//描画順を変更
	this->SetDrawOrder(5);

	//アニメーションの設定
	for (const std::unique_ptr<AnimationImporter>& importer : Animation::GetAnimeList("HyperEnemy"))
	{
		//アニメーションの使用するテクスチャを取得する
		ITextureBase* useTexture = resourceManager->GetTexture(importer->GetRefference());

		//アニメーション情報を設定する
		this->SetAnimation(importer->GetAnimation(), useTexture->GetRect());

		//次のアニメーションの情報を設定する
		this->SetNextAnimation(importer->GetAnimation(), importer->GetNext());

		//アニメーションの終了時間を設定する
		this->SetEndTime(importer->GetAnimation(), importer->GetEnd());
	}

	//アニメーションのフラグを設定する
	this->SetAniFlag(true);
	this->SetLoopFlag(true);

	//現在のアニメーション情報を設定する
	this->SetNowInfo("ハイパー2");
	this->SetLoopInfo("ハイパー4");
	this->SetNowAnimation(this->GetNowAnimation());

	//ステートの生成
	m_AutoMoveState = std::make_unique<AutoMoveState>();
	m_JumpState = std::make_unique<JumpState>();

	//ステートをリストに追加する
	m_MoveStateList.push_back(m_AutoMoveState.get());
	m_MoveStateList.push_back(m_JumpState.get());

	//各ステートの設定を行う

	//自動移動のステート
	m_AutoMoveState->SetExecute(true);
	m_AutoMoveState->SetNowState(DIRECT::LEFT);
	m_AutoMoveState->SetSpeed(Vector3(2.0f, 0.0f, 0.0f));

	//ジャンプのステート
	m_JumpState->SetExecute(false);
	m_JumpState->SetJumpOption(false);
	m_JumpState->SetJumpLimit(200.0f);
	m_JumpState->SetJumpSpeed(10.0f);
	m_JumpState->SetCorrection(0.5f);
	m_JumpState->GetOM()->SetOwner(this);

	//点滅の間隔を設定する
	TextureBase::SetTarget(0.1f);

	//点滅の終了の設定をする
	TextureBase::SetFinal(1);
	TextureBase::SetFinalOption(true);

	//実行関数を起こす
	this->SetActive(true);

	ITextureBase* Gage = resourceManager->GetTexture("WhiteBack");
	ITextureBase* ex = resourceManager->GetTexture("経験値");

	//回復アイテムの画像を取得する
	ITextureBase* healTex = resourceManager->GetTexture("回復3");

	//HPゲージを生成する
	std::unique_ptr<HPGage>pHpGage = std::make_unique<HPGage>(Gage);
	pHpGage->SetColor(Colors::GreenYellow);
	pHpGage->SetRect(RECT{ 0,0,80,10 });
	pHpGage->SetPosition(this->GetPosition() - Vector3(0, 20, 0));
	pHpGage->SetMaxTexLength(80);
	pHpGage->SetDrawOrder(4);
	pHpGage->SetState(STATE::DRAW);
	m_HpGage = pHpGage.get();

	//HPゲージの裏側を生成する
	std::unique_ptr<ITextureBase>pHpBack = std::make_unique<TextureBase>(Gage);
	pHpBack->SetColor(Colors::White);
	pHpBack->SetRect(RECT{ 0,0,80,10 });
	pHpBack->SetPosition(this->GetPosition() - Vector3(0, 20, 0));
	pHpBack->SetDrawOrder(6);
	m_HpBack = pHpBack.get();

	//経験値を生成する
	std::unique_ptr<Experience> pex = std::make_unique<Experience>(ex);
	pex->SetDrawOrder(4);
	pex->SetState(STATE::NONE);
	m_experience = pex.get();

	//TextureManagerに追加する
	gameManager->Add(std::move(pHpGage));
	gameManager->Add(std::move(pHpBack));
	gameManager->Add(std::move(pex));

	//画像の大きさを当たり判定に使用する
	this->SetRadius(Vector3(this->AskOrigin() * 2));

	//HealManagerを取得する
	HealManager* healManager = GameContext::Get<PlayScene>()->GetHealManager();

	//回復アイテムを生成する
	healManager->CreateItem(healTex, [&](Item* item)
		{	Heal* healItem = dynamic_cast<Heal*>(item);
	m_healItem = healItem; });
}

void HyperEnemy::Initialize()
{
	//ResourceManagerを取得する
	ResourceManager* resourceManager = ResourceManager::GetInstance();
	ITextureBase* shabondama1 = resourceManager->GetTexture("シャボン玉1");

	//静的データを取得する
	StaticData* staticData = StaticData::GetInstance();

	//GameManagerを取得する
	TextureManager* gameManager = GameContext::Get<PlayScene>()->GetGameManager().get();

	//Stageを取得する
	Stage* stage = GameContext::Get<Stage>();

	//Playerを取得する
	CoroKun* player = GameContext::Get<CoroKun>();

	//マップ情報を取得する
	std::vector<std::vector<ITextureBase*>>& mapAll = stage->GetMap();

	//シャボン玉の生成を行う
	for (int i = 0; i < SOAPBABBLENUM; i++)
	{
		//シャボン玉を生成する
		std::unique_ptr<SoapBabble> babble = std::make_unique<SoapBabble>(shabondama1);

		//座標を設定する
		babble->SetPosition(this->GetPosition());

		//描画を切る
		babble->SetState(STATE::NONE);

		//原点を変更する
		babble->SetOrigin(this->AskOrigin());

		//名前を変更する
		babble->SetName("Hypersyabondama");

		//自身のポインタを設定する
		babble->SetOwner(this);

		//生ポインタをリストに追加する
		m_SoapBubbleList.push_back(babble.get());

		//マップ情報を追加する
		mapAll[mapAll.size() - 1].push_back(babble.get());

		//Managerクラスに登録する
		gameManager->Add(std::move(babble));
	}

	//経験値の追尾用のステートの設定をする

	//ベクトルの補正値を設定する
	m_experience->GetTrackingState()->SetCorrection(Vector3(CoroKun::SPEED));

	//追跡目標を設定する
	m_experience->GetTrackingState()->SetTarget(&player->GetPosition());

	//自身の座標を設定する
	m_experience->GetTrackingState()->SetThis(&m_experience->GetPosition());

	//所有者のポインタを設定する
	m_experience->SetOwner(this);

	//MasicEffectManagerを生成する
	m_masicEffectManager = std::make_unique<MasicEffectManager>();
	m_masicEffectManager->Initialize(this);

	//敵のレベル情報を取得する
	int level = staticData->Get_IntData(this->GetName());

	//ステータスを設定する
	StatusImporter* importer = StatusBase::FindStatus(this->GetName(), level);
	this->SetStatus(importer->GetHp(), importer->GetMp(), importer->GetAtack(), importer->GetDefence(),
		importer->GetSpecial(), importer->GetLevel(), importer->GetNextLevelPoint());
	this->SetMaxStatus(importer->GetHp(), importer->GetMp(), importer->GetAtack(), importer->GetDefence(),
		importer->GetSpecial(), MAX_LEVEL, importer->GetNextLevelPoint());

	//経験値の補正値を設定する
	this->SetCompensation(STRENGTH);

	//常に敵の上側にHPゲージが来るようにする
	m_HpGage->SetPosition(this->GetPosition() - Vector3(0, 20, 0));
	m_HpBack->SetPosition(this->GetPosition() - Vector3(0, 20, 0));

	//HPゲージの初期化
	m_HpGage->CreateGage(gameManager);
}

void HyperEnemy::Update()
{
	//UIの表示が終わるまで何もしない
	TimeLag* go = GameContext::Get<PlayScene>()->GetLagGo().get();
	if (!go->GetFinishFlag())return;

	//実行関数が落ちているときは、何もしない
	if (!this->GetActive())return;

	//CameraManagerを取得する
	CameraManager* cameraManager = GameContext::Get<CameraManager>();

	//現在稼働しているカメラを取得する
	CameraBase* nowCamera = cameraManager->GetNowCamera();

	//カメラがスクロールカメラの時は、更新を止める
	if (!nowCamera->GetObjectMove())return;

	//アニメーションの更新処理
	Animation::Update();

	//攻撃の更新処理
	this->UpdateAtack();

	//ゲージの更新処理
	this->UpdateGage();

	//ステートの更新処理
	this->UpdateState();

	//画像の更新処理
	TextureBase::Update();

	//画像の切り出し位置を更新する
	this->SetRect(this->GetNowAnimation());

	//当たり判定の座標を設定する
	this->SetCenter(this->GetPosition());

	//点滅中の時
	if (this->GetBlink())
	{
		switch (this->GetNowData2())
		{
		case COLOR_CHANGE::NONE:
			//色を白にする
			this->SetColor(Colors::White);

			//5フレームごとに色を切り替える
			if (m_frameCount % 5 == NULL && m_frameCount != NULL)
			{
				//ステートを切り替える
				this->AddData2(COLOR_CHANGE::CHANGE);
			}

			break;
		case COLOR_CHANGE::CHANGE:
			//色を赤にする
			this->SetColor(Colors::Red);

			//5フレームごとに色を切り替える
			if (m_frameCount % 5 == NULL && m_frameCount != NULL)
			{
				//ステートを切り替える
				this->AddData2(COLOR_CHANGE::NONE);
			}

			break;
		}

		//フレーム数を更新する
		m_frameCount++;
	}
	else
	{
		//点滅していないとき
		this->SetColor(Colors::White);

		//フレーム数を初期化する
		m_frameCount = NULL;
	}


	//当たり判定を追加する
	GameContext::Get<CollisionManager>()->AddUseHero(this);

	//プレイヤーを取得する
	CoroKun* player = GameContext::Get<CoroKun>();
	
	//死亡時以外は当たり判定を設定する
	if (this->GetHP() > NULL)
	{
		//プレイヤーが生存しているとき
		if (player->GetHP() > NULL)
		{
			//プレイヤーとの当たり判定を設定する
			GameContext::Get<CollisionManager>()->AddUseOther(player, this, 100.0f);

			//周囲に生える針との当たり判定を設定する
			std::vector<Needle*> needleRe = GameContext::Get<Resurrection>()->GetNeedleList();

			for (Needle* needle : needleRe)
			{
				//当たり判定を設定する
				GameContext::Get<CollisionManager>()->AddUseOther(needle, this, 200.0f);
			}

			//長距離に飛ぶ針との当たり判定を設定する
			std::vector<Needle*> needleOver = GameContext::Get<NeedleOverall>()->GetNeedleList();

			for (Needle* needle : needleOver)
			{
				//当たり判定を設定する
				GameContext::Get<CollisionManager>()->AddUseOther(needle, this, 100.0f);
			}

			//通常弾を取得する
			std::vector<NormalBullet*> normalBulletList = GameContext::Get<NormalTechnique>()->GetBulletsList();

			for (NormalBullet* bullet : normalBulletList)
			{
				//当たり判定を設定する
				GameContext::Get<CollisionManager>()->AddUseOther(bullet, this, 200.0f);
			}
		}
	}
}

void HyperEnemy::Render()
{
	//画像の描画処理
	TextureBase::Render();
}

void HyperEnemy::Finalize()
{
}

void HyperEnemy::OnCollisionEnter(ITextureBase* obj)
{
	CollisionBase::OnCollisionEnter(obj);

	if (obj->GetName() == "SickChar")this->OnCollision_Player(obj);

	if (obj->GetName() == "floor")this->OnCollision_Floor(obj);

	if (obj->GetName() == "wall")this->OnCollision_Wall(obj);

	if (obj->GetName() == "WhiteBack")this->OnCollision_WhiteBack(obj);

	//パネルによって、呼ぶ関数を変える
	if (obj->GetName() == "LeftPanel")this->OnCollision_LeftPanel(obj);

	if (obj->GetName() == "RightPanel")this->OnCollision_RightPanel(obj);

	if (obj->GetName() == "LeftJumpPanel")this->OnCollision_LeftJumpPanel(obj);

	if (obj->GetName() == "RightJumpPanel")this->OnCollision_RightJumpPanel(obj);

	if (obj->GetName() == "shortNeedle")this->OnCollision_NeedleOverAll(obj);

	if (obj->GetName() == "longNeedle")this->OnCollision_Resurrection(obj);

	if (obj->GetName() == "ウイルスビーム")this->OnCollision_VirusBeam(obj);

	if (obj->GetName() == "通常弾")this->OnCollision_NormalBullet(obj);
}

void HyperEnemy::NonCollisionEnter(ITextureBase* obj)
{
	CollisionBase::NonCollisionEnter(obj);
}

void HyperEnemy::OnDamage(const int& atack, const int& defence, const float correction)
{
	//スコアを取得する
	Score* score = GameContext::Get<PlayScene>()->GetScore();

	//HPを取得する
	int hp = this->GetHP();

	//rectを取得する
	RECT rect = m_HpGage->GetRect();

	//ダメージを計算する
	StatusCalc::CalcDamage(hp, atack, defence,
		m_HpGage->GetMaxTexLength(), this->GetMaxHP(), rect.right, correction);

	//HPを設定する
	this->SetHP(hp);

	//RECTを設定する
	m_HpGage->SetRect(rect);

	//SoundManagerを取得する
	SoundManager* soundManager = SoundManager::GetInstance();

	//SEを鳴らす
	soundManager->Play(CRI_CUESHEET_0__11_ダメージ);

	if (score->GetSaving() == NULL)
	{
		//SEを鳴らす
		soundManager->Play(CRI_CUESHEET_0__19_スコア加算);
	}

	//ダメージ用のフラグを立てる
	m_damageCheck = true;

	//HPゲージの裏面のテクスチャを取得する
	ITextureBase* backHpTexture = m_HpGage->GetBackGageTexture();

	//HPゲージの裏面の色を設定する
	backHpTexture->SetColor(Colors::Red);

	//ダメージのカウントを初期化する
	m_HpGage->SetEasingTime(0.0f);
}

void HyperEnemy::UpdateState()
{
	//ベクトルの初期化
	Vector3 vel = Vector3::Zero;

	//Stageを取得
	Stage* stage = GameContext::Get<Stage>();

	//静的データの取得
	StaticData* staticData = StaticData::GetInstance();

	//対象の下にあるものを数える
	int botomNum = NULL;

	for (Floor* pfloor : stage->GetFloors())
	{
		if (stage->DirectlyBlowMap(this, pfloor) == nullptr)
		{
			//下に何もないとき、カウントを更新する
			botomNum++;
		}
	}

	//着地してから、下に何もないとき、落下する
	if (botomNum == (int)stage->GetFloors().size())
	{
		if (m_JumpState->GetPreState() == JUMP::LANDING)
		{
			m_JumpState->SetNowState(JUMP::FALL);
			m_JumpState->SetJumpSpeed(3.0f);
			m_JumpState->AddFall();
			//終了処理が行われていないとき
			if (!m_Result)
			{
				this->SetNowInfo("ハイパー5");
				this->SetLoopInfo("ハイパー5");
			}
		}
	}

	//実行中のステートからベクトルを取得する
	for (MoveState*& state : m_MoveStateList)
	{
		vel += state->Update();
	}

	//ベクトルを設定する
	this->SetVelocity(vel);

	//データが存在するかどうか、確認する
	if (!staticData->Request_Data("ハイパーエネミーダメージ"))return;

	//ステートの更新処理
	if (!m_Result)
	{
		if (this->GetFrameCount() > this->GetFinal())
		{
			m_AutoMoveState->SetExecute(true);
			this->SetNowInfo("ハイパー2");
			this->SetLoopInfo("ハイパー4");
			this->SetInitTimer(0);
		}
	}
}

void HyperEnemy::UpdateAtack()
{
	//プレイヤーのインスタンスを取得する
	CoroKun* player = GameContext::Get<CoroKun>();

	//ダメージを受けているときは、何もしない
	if (this->GetBlink())return;

	//プレイヤーが攻撃中の時は、何もしない
	if (player->ALLTechniqueExe())return;

	//体力が尽きたら、やられる
	//強制的に死亡ステートに切り替える
	if (this->GetHP() <= NULL && !m_Result)
	{
		this->AddData1(ENEMYSTATE::DEADSTART);
	}

	//ステートによって、処理を分ける
	switch (this->GetNowData1())
	{
	case ENEMYSTATE::ENONE:
		this->State_AtackNone();
		break;
	case ENEMYSTATE::ATACKSTART:
		this->State_AtackStart();
		break;
	case ENEMYSTATE::ATACKING:
		this->State_Atacking();
		break;
	case ENEMYSTATE::ATACKRESUTL:
		this->State_AtackResult();
		break;
	case ENEMYSTATE::NEXTSTATE:
		this->State_AtackNext();
		break;
	case ENEMYSTATE::DEADSTART:
		this->State_DeadStart();
		break;
	case ENEMYSTATE::DEADTING:
		this->State_DeadTing();
		break;
	case ENEMYSTATE::DEAD:
		this->State_Dead();
		break;
	}
}

void HyperEnemy::UpdateGage()
{
	//常に敵の上側にHPゲージが来るようにする
	m_HpGage->SetPosition(this->GetPosition() - Vector3(0, 20, 0));
	m_HpBack->SetPosition(this->GetPosition() - Vector3(0, 20, 0));
	m_HpGage->GetBackGageTexture()->SetPosition(m_HpGage->GetPosition());

	//静的データを取得する
	StaticData* staticData = StaticData::GetInstance();

	//前回のフレームからの経過時間を取得する
	float elapsedTime = staticData->Get_FloatData("elapsedTime");

	//ダメージを受けているとき
	if (m_damageCheck)
	{
		//背面のテクスチャを取得する
		ITextureBase* backTexture = m_HpGage->GetBackGageTexture();

		//HPゲージの裏面の切り出し位置を取得する
		RECT back = m_HpGage->GetBackGageTexture()->GetRect();

		//HPゲージの切り出し位置を取得する
		RECT front = m_HpGage->GetRect();

		//ダメージの差分を求める
		int diffrent = (int)(back.right - front.right);

		//現在の時間を取得する
		float time = m_HpGage->GetEasingTime();

		//時間を更新する
		time += elapsedTime;

		//ダメージをスカラ補正する
		diffrent *= time;

		//ゲージに動きをつける
		back.right -= diffrent;

		//切り出し位置を再設定する
		m_HpGage->GetBackGageTexture()->SetRect(back);

		//時間を再設定する
		m_HpGage->SetEasingTime(time);

		if (m_HpGage->GetEasingTime() >= 1.0f)
		{
			//1秒経過したら、フラグを切る
			m_damageCheck = false;

			//切り出し位置を設定する
			m_HpGage->GetBackGageTexture()->SetRect(m_HpGage->GetRect());
		}
	}
}

void HyperEnemy::HealItemDrop()
{
	//乱数を取得する
	Rand* rand = Rand::GetInstance();

	//ドロップする確立を求める
	int pro = ItemDrop::GetProbability(this->GetName(), this->GetLevel());

	//0から100で乱数を振る
	int result = rand->RandomMachine(0, 100);

	//乱数の結果がドロップする確率以下の場合に、ドロップさせる
	if (pro >= result)
	{
		m_healItem->OnFall(this->GetPosition());
	}
}

void HyperEnemy::OnCollision_LeftPanel(ITextureBase* obj)
{
	//ステートを変更する
	m_AutoMoveState->SetNowState(DIRECT::LEFT);

	//描画を反転させる
	this->SetSpriteEffect(SpriteEffects::SpriteEffects_None);
}

void HyperEnemy::OnCollision_Player(ITextureBase* obj)
{
	//プレイヤーと当たった時の処理
	CoroKun* player = dynamic_cast<CoroKun*>(obj);

	//静的データの取得
	StaticData* staticData = StaticData::GetInstance();

	//スコアを取得する
	Score* score = GameContext::Get<PlayScene>()->GetScore();

	//ダメージを受けていないとき
	if (!TextureBase::GetBlink())
	{
		//プレイヤーが体当たり中のとき
		if (player->GetBodyBlow()->GetActive())
		{
			//点滅させる
			this->SetBlink(true);

			//シャボン玉を消す
			for (SoapBabble* babble : m_SoapBubbleList)
			{
				babble->SetState(STATE::NONE);
			}

			//自動移動のステートを変更する
			m_AutoMoveState->SetExecute(false);

			//アニメーションを切り替える
			this->SetNowInfo("ハイパー1");

			//ダメージステートの情報を送信する
			staticData->Add("ハイパーエネミーダメージ", obj->GetName());

			//ステートを切り替える(ダメージを受けたら、自動的に思考時間に移る)
			this->AddData1(ENEMYSTATE::NEXTSTATE);

			//ダメージを受ける
			this->OnDamage(player->GetAtack(), this->GetDefence());

			//エフェクトを再生する
			m_masicEffectManager->NowEffectPlay(BodyBlow::TECHNIQUE_HEAL_COMMAND, BodyBlow::TECHNIQUE_HEAL_SPECIAL);

			//スコアを追加する
			score->AddScore(this->GetLevel() * BodyBlow::BASE_SCORE);
			score->SetFrameSave(this->GetLevel() * BodyBlow::BASE_SCORE, 60.0f);
		}
	}
}

void HyperEnemy::OnCollision_RightPanel(ITextureBase* obj)
{
	//ステートを変更する
	m_AutoMoveState->SetNowState(DIRECT::RIGHT);

	//描画を反転させる
	this->SetSpriteEffect(SpriteEffects::SpriteEffects_FlipHorizontally);
}

void HyperEnemy::OnCollision_LeftJumpPanel(ITextureBase* obj)
{
	//SoundManagerを取得する
	SoundManager* soundManager = SoundManager::GetInstance();

	if (m_AutoMoveState->GetNowState() == DIRECT::LEFT)
	{
		//実行関数を起こす
		m_JumpState->SetExecute(true);

		//ジャンプ待機中の時のみ、ジャンプする
		if (m_JumpState->GetNowState() == JUMP::JNONE)
		{
			//攻撃を受けていないとき
			if (!this->GetBlink())
			{
				m_JumpState->SetNowState(JUMP::START);
				m_JumpState->SetJumpSpeed(10.0f);
				m_AutoMoveState->SetSpeed(Vector3(3.0f, 0.0f, 0.0f));
				this->SetNowInfo("ハイパー5");
				this->SetLoopInfo("ハイパー5");

				//ジャンプ音を出す
				soundManager->Play(CRI_CUESHEET_0__45_ジャンプ);
			}
		}
	}
}

void HyperEnemy::OnCollision_RightJumpPanel(ITextureBase* obj)
{
	//SoundManagerを取得する
	SoundManager* soundManager = SoundManager::GetInstance();

	if (m_AutoMoveState->GetNowState() == DIRECT::RIGHT)
	{
		//実行関数を起こす
		m_JumpState->SetExecute(true);

		//ジャンプ待機中の時のみ、ジャンプする
		if (m_JumpState->GetNowState() == JUMP::JNONE)
		{
			//攻撃を受けていないとき
			if (!this->GetBlink())
			{
				m_JumpState->SetNowState(JUMP::START);
				m_JumpState->SetJumpSpeed(10.0f);
				m_AutoMoveState->SetSpeed(Vector3(3.0f, 0.0f, 0.0f));
				this->SetNowInfo("ハイパー5");
				this->SetLoopInfo("ハイパー5");

				//ジャンプ音を出す
				soundManager->Play(CRI_CUESHEET_0__45_ジャンプ);
			}
		}
	}
}

void HyperEnemy::OnCollision_Floor(ITextureBase* obj)
{
	//座標の修正
	Vector3 pos = this->GetPosition();
	this->SetPosition(Vector3(pos.x, obj->GetPosition().y - (this->AskOrigin().y * 2), pos.z));

	//落下中の時は、ベクトルを再設定する
	if (m_JumpState->GetNowState() == JUMP::FALL)
	{
		m_JumpState->SetNowState(JUMP::LANDING);
		m_AutoMoveState->SetSpeed(Vector3(2.0f, 0.0f, 0.0f));

		//ダメージを受けていなければ、アニメーションを変更する
		if (this->GetNowInfo() == "ハイパー5")
		{
			this->SetNowInfo("ハイパー2");
			this->SetLoopInfo("ハイパー4");
			this->SetInitTimer(0);
		}
	}
}

void HyperEnemy::OnCollision_Wall(ITextureBase* obj)
{
	//X方向に移動量だけ押し出す
	this->ExtrudeX(obj);

	//移動を反対方向に切り替える
	m_AutoMoveState->ChangeState();

	//描画を反対にする
	this->ChangeSpriteEffect();
}

void HyperEnemy::OnCollision_WhiteBack(ITextureBase* obj)
{
	//this->ExtrudeX(obj);

	//m_AutoMoveState->ChangeState();

	//this->ChangeSpriteEffect();
}

void HyperEnemy::OnCollision_NeedleOverAll(ITextureBase* obj)
{
	//静的データの取得
	StaticData* staticData = StaticData::GetInstance();

	//NeedleOverAllを取得する
	NeedleOverall* needleOver = GameContext::Get<NeedleOverall>();

	//プレイヤーと当たった時の処理
	CoroKun* player = GameContext::Get<CoroKun>();

	//スコアを取得する
	Score* score = GameContext::Get<PlayScene>()->GetScore();

	//ダメージを受けていないとき
	if (!TextureBase::GetBlink())
	{
		//点滅させる
		this->SetBlink(true);
		//シャボン玉を消す
		for (SoapBabble* babble : m_SoapBubbleList)
		{
			babble->SetState(STATE::NONE);
		}
		//自動移動のステートを変更する
		m_AutoMoveState->SetExecute(false);
		//移動方向を変更する
		m_AutoMoveState->ChangeState();
		//向きを返る
		this->ChangeSpriteEffect();
		//アニメーションを切り替える
		this->SetNowInfo("ハイパー1");
		//ダメージステートの情報を送信する
		staticData->Add("ハイパーエネミーダメージ", obj->GetName());
		//ステートを切り替える(ダメージを受けたら、自動的に思考時間に移る)
		this->AddData1(ENEMYSTATE::NEXTSTATE);

		//補正値を設定する
		float correction = 1.0f;

		//針を準備しているタイミングで当たったら、受けるダメージは半分
		if (needleOver->GetNowState() == NEEDLEACTION::MOPE)correction = 0.5f;

		//ダメージを受ける
		this->OnDamage(player->GetAtack(), this->GetDefence(),correction);

		//エフェクトを再生する
		m_masicEffectManager->NowEffectPlay(NeedleOverall::TECHNIQUE_HEAL_COMMAND, NeedleOverall::TECHNIQUE_HEAL_SPECIAL);

		//スコアを追加する
		score->AddScore(this->GetLevel() * NeedleOverall::BASE_SCORE);
		score->SetFrameSave(this->GetLevel() * NeedleOverall::BASE_SCORE, 60.0f);
	}
}

void HyperEnemy::OnCollision_Resurrection(ITextureBase* obj)
{
	//静的データの取得
	StaticData* staticData = StaticData::GetInstance();

	//Resurrectionを取得する
	Resurrection* resurrection = GameContext::Get<Resurrection>();

	//プレイヤーと当たった時の処理
	CoroKun* player = GameContext::Get<CoroKun>();

	//スコアを取得する
	Score* score = GameContext::Get<PlayScene>()->GetScore();

	//ダメージを受けていないとき
	if (!TextureBase::GetBlink())
	{
		//点滅させる
		this->SetBlink(true);
		//シャボン玉を消す
		for (SoapBabble* babble : m_SoapBubbleList)
		{
			babble->SetState(STATE::NONE);
		}
		//自動移動のステートを変更する
		m_AutoMoveState->SetExecute(false);
		//移動方向を変更する
		m_AutoMoveState->ChangeState();
		//向きを返る
		this->ChangeSpriteEffect();
		//アニメーションを切り替える
		this->SetNowInfo("ハイパー1");
		//ダメージステートの情報を送信する
		staticData->Add("ハイパーエネミーダメージ", obj->GetName());
		//ステートを切り替える(ダメージを受けたら、自動的に思考時間に移る)
		this->AddData1(ENEMYSTATE::NEXTSTATE);

		//補正値を設定する
		float correction = 1.0f;

		//針を準備しているタイミングで当たったら、受けるダメージは半分
		if (resurrection->GetNowState() == RENEEDLE::LITTLE)correction = 0.5f;

		//ダメージを受ける
		this->OnDamage(player->GetAtack(), this->GetDefence(), correction);

		//エフェクトを再生する
		m_masicEffectManager->NowEffectPlay(Resurrection::TECHNIQUE_HEAL_COMMAND, Resurrection::TECHNIQUE_HEAL_SPECIAL);

		//スコアを追加する
		score->AddScore(this->GetLevel() * Resurrection::BASE_SCORE);
		score->SetFrameSave(this->GetLevel() * Resurrection::BASE_SCORE, 60.0f);
	}
}

void HyperEnemy::OnCollision_VirusBeam(ITextureBase* obj)
{
	//静的データの取得
	StaticData* staticData = StaticData::GetInstance();

	//プレイヤーと当たった時の処理
	CoroKun* player = GameContext::Get<CoroKun>();

	//スコアを取得する
	Score* score = GameContext::Get<PlayScene>()->GetScore();

	//ダメージを受けていないとき
	if (!TextureBase::GetBlink())
	{
		//点滅させる
		this->SetBlink(true);
		//シャボン玉を消す
		for (SoapBabble* babble : m_SoapBubbleList)
		{
			babble->SetState(STATE::NONE);
		}
		//自動移動のステートを変更する
		m_AutoMoveState->SetExecute(false);
		//アニメーションを切り替える
		this->SetNowInfo("ハイパー1");
		//ダメージステートの情報を送信する
		staticData->Add("ハイパーエネミーダメージ", obj->GetName());
		//ステートを切り替える(ダメージを受けたら、自動的に思考時間に移る)
		this->AddData1(ENEMYSTATE::NEXTSTATE);

		//ダメージを受ける
		this->OnDamage(player->GetAtack(), this->GetDefence());

		//エフェクトを再生する
		m_masicEffectManager->NowEffectPlay(VirusBeam::TECHNIQUE_HEAL_COMMAND, VirusBeam::TECHNIQUE_HEAL_SPECIAL);

		//スコアを追加する
		score->AddScore(this->GetLevel() * VirusBeam::BASE_SCORE);
		score->SetFrameSave(this->GetLevel() * VirusBeam::BASE_SCORE, 60.0f);
	}
}

void HyperEnemy::OnCollision_NormalBullet(ITextureBase* obj)
{
	//静的データの取得
	StaticData* staticData = StaticData::GetInstance();

	//プレイヤーと当たった時の処理
	CoroKun* player = GameContext::Get<CoroKun>();

	//スコアを取得する
	Score* score = GameContext::Get<PlayScene>()->GetScore();

	//ダメージを受けていないとき
	if (!TextureBase::GetBlink())
	{
		//点滅させる
		this->SetBlink(true);
		//自動移動のステートを変更する
		m_AutoMoveState->SetExecute(false);
		//アニメーションを切り替える
		this->SetNowInfo("ハイパー1");
		//ダメージステートの情報を送信する
		staticData->Add("ハイパーエネミーダメージ", obj->GetName());
		//ステートを切り替える(ダメージを受けたら、自動的に思考時間に移る)
		this->AddData1(ENEMYSTATE::NEXTSTATE);

		//補正値を設定する
		float correction = 1.0f;

		correction = obj->GetScale().x / 1.6f;

		//ダメージを受ける
		this->OnDamage(player->GetAtack(), this->GetDefence(),correction);

		int core1 = NormalTechnique::TECHNIQUE_HEAL_COMMAND * correction;
		int core2 = NormalTechnique::TECHNIQUE_HEAL_SPECIAL * correction;

		//エフェクトを再生する
		m_masicEffectManager->NowEffectPlay(core1,core2);

		//スコアを追加する
		score->AddScore(this->GetLevel() * (NormalBullet::BASE_SCORE * correction));
		score->SetFrameSave(this->GetLevel() * (NormalBullet::BASE_SCORE * correction), 60.0f);
	}
}

void HyperEnemy::State_AtackNone()
{
	//プレイヤーのポインタを取得
	CoroKun* player = GameContext::Get<CoroKun>();

	//プレイヤーと一定範囲内にいるときに攻撃する

	Vector3 len = Length(player->GetPosition(), this->GetPosition());

	//ジャンプしていないとき
	if (m_JumpState->GetNowState() == JUMP::JNONE)
	{
		//一定範囲内にいるとき
		if (len.x <= 200.0f && len.y <= 50.0f)
		{
			//移動を止める
			m_AutoMoveState->SetExecute(false);
			//アニメーションを切り替える
			this->SetNowInfo("ハイパー1");
			//攻撃開始
			m_ArrayBabble = NULL;
			//ステートを切り替える
			this->AddData1(ENEMYSTATE::ATACKSTART);
		}
	}
}

void HyperEnemy::State_AtackStart()
{
	//SoundManagerを取得する
	SoundManager* soundManager = SoundManager::GetInstance();

	//アタックタイムを更新する
	m_AtackTime++;

	//攻撃用のSEを再生する
	soundManager->Play(CRI_CUESHEET_0__46_泡攻撃);

	//ステートを切り替える
	this->AddData1(ENEMYSTATE::ATACKING);
}

void HyperEnemy::State_Atacking()
{
	//RandMachineを取得する
	Rand* Ramdom = Rand::GetInstance();

	//アタックタイムを更新する
	m_AtackTime++;

	//0.1秒に1回攻撃する
	if (m_AtackTime % ATACKINTERVAL == 0)
	{
		//参照できないときは、実行しない
		if (m_ArrayBabble != -1)
		{
			//原点を画像の中心として考える
			Vector3 center = this->GetPosition() + this->AskOrigin();

			//敵はもとから左向きが正面なので、向きをとるためには、符号を反転させる
			int dir = this->DirectX() * -1;

			//向きによって、座標が変わる
			center += Vector3(50.0f, 0.0f, 0.0f) * dir;

			//ベクトルを初期化する
			Vector3 vel = Vector3::Zero;

			//Xベクトルの乱数
			double x = Ramdom->RandomMachine(0.5, 2.0);

			//Yベクトルの乱数
			double y = Ramdom->RandomMachine(-1.0, 1.0);

			//拡大倍率の乱数
			double scale = Ramdom->RandomMachine(0.2, 1.0);

			//先頭の泡を取得する
			SoapBabble* babble = m_SoapBubbleList[m_ArrayBabble];

			//座標を設定する
			babble->SetPosition(center);

			//ベクトルを設定する
			babble->SetVelocity(Vector3((float)x * dir, (float)y, 0.0f));

			//拡大倍率を設定する
			babble->SetScale(Vector3((float)scale));

			//描画を起こす
			babble->SetState(STATE::DRAW);

			if (m_ArrayBabble >= SOAPBABBLENUM - 1)m_ArrayBabble = -1;
			else m_ArrayBabble++;
		}
		//配列の要素を参照できないときは、ステートを切り替える
		else this->AddData1(ENEMYSTATE::ATACKRESUTL);
	}
}

void HyperEnemy::State_AtackResult()
{
	//移動を再開
	m_AutoMoveState->SetExecute(true);
	//アニメーションを切り替える
	this->SetNowInfo("ハイパー2");
	//ステートを切り替える
	this->AddData1(ENEMYSTATE::NEXTSTATE);
}

void HyperEnemy::State_AtackNext()
{
	//静的データの取得
	StaticData* staticData = StaticData::GetInstance();

	//SoundManagerを取得する
	SoundManager* soundManager = SoundManager::GetInstance();

	//前のフレームからの経過時間を取得する
	float elapsetTime = staticData->Get_FloatData("elapsedTime");

	//思考時間を加算していく
	m_ThinkingTime += elapsetTime;

	//再生中の音を止める
	soundManager->Stop(CRI_CUESHEET_0__46_泡攻撃);

	//思考が完了したら、ステートを切り替える
	if (m_ThinkingTime > THINKTIME)
	{
		//思考時間は初期化する
		m_ThinkingTime = 0.0f;
		//ステートを切り替える
		this->AddData1(ENEMYSTATE::ENONE);
	}
}

void HyperEnemy::State_DeadStart()
{
	//終了フラグをオンにする
	m_Result = true;

	//アニメーション時間を初期化する
	this->SetInitTimer(NULL);

	//死亡アニメーションを起動する
	this->SetNowInfo("ハイパー6");

	//ループ情報を設定する
	this->SetLoopInfo("ハイパー10");

	//移動の実行関数を落とす
	m_AutoMoveState->SetExecute(false);

	//ステートを切り替える
	this->AddData1(ENEMYSTATE::DEADTING);
}

void HyperEnemy::State_DeadTing()
{
	//アニメーションが終了
	if (this->GetNowInfo() == "ハイパー10")
	{
		//点滅を開始する
		if(!this->GetBlink())this->SetBlink(true);

		//ステートを切り替える
		this->AddData1(ENEMYSTATE::DEAD);
	}
}

void HyperEnemy::State_Dead()
{
	//SoundManagerを取得する
	SoundManager* soundManager = SoundManager::GetInstance();

	//１秒点滅したら、描画を切る
	if (!this->GetBlink())
	{
		//描画を切る
		this->SetState(STATE::NONE);

		//実行関数を落とす
		this->SetActive(false);

		//HPバーの描画を切る
		m_HpGage->SetState(STATE::NONE);
		m_HpBack->SetState(STATE::NONE);

		//経験値の座標を設定する
		m_experience->SetPosition(this->GetPosition());

		//経験値の追尾用のステートを起こす
		m_experience->GetTrackingState()->SetExecute(true);

		//ホーミング用のステートを起こす
		m_experience->GetHomingState()->SetExecute(true);
		m_experience->GetHomingState()->SetStartFlag(true);

		//経験値のステートを切り替える
		m_experience->SetNowState(TARGET_STATE::WAIT);

		//経験値を描画する
		m_experience->SetState(STATE::DRAW);

		//回復アイテムをドロップする
		this->HealItemDrop();

		//SEを再生する
		soundManager->Play(CRI_CUESHEET_0__31_アイテム出現);
	}
}
