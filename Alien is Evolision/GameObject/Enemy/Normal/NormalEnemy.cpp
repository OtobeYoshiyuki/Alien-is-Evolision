#include "pch.h"
#include "NormalEnemy.h"
#include "OtobeLib/FrameWork/ResourceManager/ResourceManager.h"
#include "GameContext.h"
#include "OtobeLib/StackScene/Scene/Play/PlayScene.h"
#include "OtobeLib/FrameWork/StaticData/StaticData.h"
#include "OtobeLib/Status/StatusCalc/StatusCalc.h"
#include "OtobeLib/FrameWork/Rand/Rand.h"
#include "GameObject/Item/Drop/ItemDrop.h"
#include "OtobeLib/FrameWork/SoundManager/SoundManager.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace OtobeLib;

//敵の強さ(経験値に影響する)
const float NormalEnemy::STRENGTH = 100.0f;

NormalEnemy::NormalEnemy(ITextureBase* texture):
	TextureBase(texture),//基底クラスに情報を送信する
	m_AutoMoveState(nullptr),//自動移動のステート
	m_experience(nullptr),//経験値
	m_HpGage(nullptr),//Hpゲージ
	m_HpBack(nullptr),//Hpゲージの裏面
	m_masicEffectManager(nullptr),//エフェクトの管理クラス
	m_healItem(nullptr),//回復アイテム
	m_frameCount(NULL),//フレーム数のカウント
	m_damageCheck(false)//ダメージのチェックフラグ
{
	//ResourceManagerを取得する
	ResourceManager* resourceManager = ResourceManager::GetInstance();

	//GameManagerを取得する
	TextureManager* gameManager = GameContext::Get<PlayScene>()->GetGameManager().get();

	//経験値の画像を取得する
	ITextureBase* ex = resourceManager->GetTexture("経験値");

	//回復アイテムの画像を取得する
	ITextureBase* healTex = resourceManager->GetTexture("回復1");

	//描画順を変更
	this->SetDrawOrder(5);

	//点滅の間隔を設定する
	TextureBase::SetTarget(0.1f);

	//点滅の終了の設定をする
	TextureBase::SetFinal(1);
	TextureBase::SetFinalOption(true);

	//実行関数を起こす
	this->SetActive(true);


	//アニメーションの設定
	for (const std::unique_ptr<AnimationImporter>& importer : Animation::GetAnimeList("NormalEnemy"))
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
	this->SetNowInfo("ノーマル1");
	this->SetLoopInfo("ノーマル6");
	this->SetNowAnimation(this->GetNowAnimation());

	ITextureBase* Gage = resourceManager->GetTexture("WhiteBack");

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

	//HealManagerを取得する
	HealManager* healManager = GameContext::Get<PlayScene>()->GetHealManager();

	//回復アイテムを生成する
	healManager->CreateItem(healTex, [&](Item* item)
		{	Heal* healItem = dynamic_cast<Heal*>(item);
			m_healItem = healItem; });

	//TextureManagerに追加する
	gameManager->Add(std::move(pHpGage));
	gameManager->Add(std::move(pHpBack));
	gameManager->Add(std::move(pex));

	//自動移動のステート
	m_AutoMoveState = std::make_unique<AutoMoveState>();
	m_AutoMoveState->SetExecute(true);
	m_AutoMoveState->SetNowState(DIRECT::LEFT);
	m_AutoMoveState->SetSpeed(Vector3(1.5f, 0.0f, 0.0f));

	//リストに追加する
	m_MoveStateList.push_back(m_AutoMoveState.get());
}

void NormalEnemy::Initialize()
{
	//PlaySceneを取得する
	PlayScene* playScene = GameContext::Get<PlayScene>();

	//TextureManagerを取得する
	TextureManager* gameManager = playScene->GetGameManager().get();

	//静的データを取得する
	StaticData* staticData = StaticData::GetInstance();

	//Playerを取得する
	CoroKun* player = GameContext::Get<CoroKun>();

	//経験値の追尾用のステートの設定をする

	//ベクトルの補正値を設定する
	m_experience->GetTrackingState()->SetCorrection(Vector3(10.0f));

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

	//ステートを設定する
	this->AddData1(ENEMYSTATE::ATACKSTART);

	//常に敵の上側にHPゲージが来るようにする
	m_HpGage->SetPosition(this->GetPosition() - Vector3(0, 20, 0));
	m_HpBack->SetPosition(this->GetPosition() - Vector3(0, 20, 0));

	//HPゲージの初期化
	m_HpGage->CreateGage(gameManager);
}

void NormalEnemy::Update()
{
	//UIの表示が終わるまで何もしない
	TimeLag* go = GameContext::Get<PlayScene>()->GetLagGo().get();
	if (!go->GetFinishFlag())return;

	//実行関数がActiveでなければ、何もしない
	if (!this->GetActive())return;

	//CameraManagerを取得する
	CameraManager* cameraManager = GameContext::Get<CameraManager>();

	//現在稼働しているカメラを取得する
	CameraBase* nowCamera = cameraManager->GetNowCamera();

	//カメラがスクロールカメラの時は、更新を止める
	if (!nowCamera->GetObjectMove())return;

	//アニメーションの更新処理
	Animation::Update();

	//画像の切り出し位置を更新する
	this->SetRect(this->GetNowAnimation());

	//ゲージの更新処理
	this->UpdateGage();

	//ステートの更新処理
	this->UpdateState();

	//画像の更新処理
	TextureBase::Update();

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
				GameContext::Get<CollisionManager>()->AddUseOther(needle, this, 400.0f);
			}

			//長距離に飛ぶ針との当たり判定を設定する
			std::vector<Needle*> needleOver = GameContext::Get<NeedleOverall>()->GetNeedleList();

			for (Needle* needle : needleOver)
			{
				//当たり判定を設定する
				GameContext::Get<CollisionManager>()->AddUseOther(needle, this, 300.0f);
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

void NormalEnemy::Render()
{
	//画像の描画処理
	TextureBase::Render();
}

void NormalEnemy::Finalize()
{
}

void NormalEnemy::OnCollisionEnter(ITextureBase* obj)
{
	//当たり判定の処理
	CollisionBase::OnCollisionEnter(obj);

	//プレイヤーに当たったときの処理
	if (obj->GetName() == "SickChar")this->OnCollision_Player(obj);

	//広範囲攻撃に当たったときの処理
	if (obj->GetName() == "shortNeedle")this->OnCollision_NeedleOverAll(obj);

	//狭範囲攻撃に当たったときの処理
	if (obj->GetName() == "longNeedle")this->OnCollision_Resurrection(obj);

	//ウイルスビームに当たったときの処理
	if (obj->GetName() == "ウイルスビーム")this->OnCollision_VirusBeam(obj);

	//通常弾に当たったときの処理
	if (obj->GetName() == "通常弾")this->OnCollision_NormalBullet(obj);

	//左向きのパネルに当たったときの処理
	if (obj->GetName() == "LeftPanel")this->OnCollision_LeftPanel(obj);

	//右向きのパネルに当たったときの処理
	if (obj->GetName() == "RightPanel")this->OnCollision_RightPanel(obj);
}

void NormalEnemy::NonCollisionEnter(ITextureBase* obj)
{
	//当たり判定の処理
	CollisionBase::NonCollisionEnter(obj);
}

void NormalEnemy::OnDamage(const int& atack, const int& defence, const float correction)
{
	//NormalTechniqueを取得する
	NormalTechnique* Ntch = GameContext::Get<NormalTechnique>();

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

void NormalEnemy::UpdateGage()
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

void NormalEnemy::UpdateState()
{
	//ベクトルの初期化
	Vector3 vel = Vector3::Zero;

	//Stageを取得
	Stage* stage = GameContext::Get<Stage>();

	//静的データの取得
	StaticData* staticData = StaticData::GetInstance();

	//実行中のステートからベクトルを取得する
	for (MoveState*& state : m_MoveStateList)
	{
		vel += state->Update();
	}

	//ベクトルを設定する
	this->SetVelocity(vel);

	//HPが0以下になったら、死亡ステートに移行する
	if (this->GetHP() <= NULL && this->GetNowData1() < ENEMYSTATE::DEADSTART)
	{
		this->AddData1(ENEMYSTATE::DEADSTART);
	}

	//ステートによって、処理を切り替える
	switch (this->GetNowData1())
	{
	case ENEMYSTATE::ATACKSTART:
		this->State_AtackStart();
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

void NormalEnemy::State_AtackStart()
{
	//State_Startの処理

	//現在のアニメーション情報を設定する
	this->SetNowInfo("ノーマル1");
	this->SetLoopInfo("ノーマル6");

	//アニメーションの更新を止める
	this->SetAniFlag(true);

	//自動移動のステートを設定する
	m_AutoMoveState->SetExecute(true);

	//ステートを切り替える
	this->AddData1(ENEMYSTATE::ATACKING);
}

void NormalEnemy::State_AtackNext()
{
	//State_Nextの処理

	//アニメーションの更新を止める
	this->SetAniFlag(false);

	//現在のアニメーション情報を設定する
	this->SetNowInfo("ノーマル4");

	//自動移動のステートを設定する
	m_AutoMoveState->SetExecute(false);

	//点滅が終了
	if (!this->GetBlink())
	{
		//ステートを切り替える
		this->AddData1(ENEMYSTATE::ATACKSTART);
	}
}

void NormalEnemy::State_DeadStart()
{
	//State_DeadStartの処理

	//自動移動のステートを設定する
	m_AutoMoveState->SetExecute(false);

	//アニメーションを止める
	this->SetAniFlag(false);

	//点滅が終了
	if (!this->GetBlink())
	{
		//現在のアニメーション情報を設定する
		this->SetNowInfo("ノーマル7");
		this->SetLoopInfo("ノーマル9");

		//ループを切る
		this->SetLoopFlag(false);

		//アニメーションを起動
		this->SetAniFlag(true);

		//ステートを切り替える
		this->AddData1(ENEMYSTATE::DEADTING);
	}


}

void NormalEnemy::State_DeadTing()
{
	//State_Deadtingの処理

	//アニメーションが終了したら、点滅させる

	if (!this->GetAniFlag())
	{
		//点滅を開始
		this->SetBlink(true);

		//ステートを切り替える
		this->AddData1(ENEMYSTATE::DEAD);
	}
}

void NormalEnemy::State_Dead()
{
	//State_Deadの処理

	//SoundManagerを取得する
	SoundManager* soundManager = SoundManager::GetInstance();

	//点滅が終わった
	if (!this->GetBlink())
	{
		//描画を切る
		this->SetState(STATE::NONE);

		//実行関数を落とす
		this->SetActive(false);

		//HPバーの描画を切る
		m_HpGage->SetState(STATE::NONE);
		m_HpBack->SetState(STATE::NONE);

		//エフェクトの座標を設定する
		m_experience->SetPosition(this->GetPosition());

		//経験値の追尾用のステートを起こす
		m_experience->GetTrackingState()->SetExecute(true);

		//ホーミング用のステートを起こす
		m_experience->GetHomingState()->SetExecute(true);
		m_experience->GetHomingState()->SetStartFlag(true);

		//経験値のステートを切り替える
		m_experience->SetNowState(TARGET_STATE::WAIT);

		//SEを再生する
		soundManager->Play(CRI_CUESHEET_0__31_アイテム出現);

		//経験値を描画する
		m_experience->SetState(STATE::DRAW);

		//回復アイテムをドロップする
		this->HealItemDrop();
	}
}

void NormalEnemy::HealItemDrop()
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

void NormalEnemy::OnCollision_Player(ITextureBase* obj)
{
	//プレイヤーと当たった時の処理
	CoroKun* player = dynamic_cast<CoroKun*>(obj);

	//スコアを取得する
	Score* score = GameContext::Get<PlayScene>()->GetScore();

	//SoundManagerを取得する
	SoundManager* soundManager = SoundManager::GetInstance();

	//ダメージを受けていないとき
	if (!TextureBase::GetBlink())
	{
		//プレイヤーが体当たり中のとき
		if (player->GetBodyBlow()->GetActive())
		{
			//点滅させる
			this->SetBlink(true);

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

void NormalEnemy::OnCollision_NeedleOverAll(ITextureBase* obj)
{
	//NeedleOverAllを取得する
	NeedleOverall* needleOver = GameContext::Get<NeedleOverall>();

	//プレイヤーと当たった時の処理
	CoroKun* player = GameContext::Get<CoroKun>();

	//スコアを取得する
	Score* score = GameContext::Get<PlayScene>()->GetScore();

	//SoundManagerを取得する
	SoundManager* soundManager = SoundManager::GetInstance();

	//ダメージを受けていないとき
	if (!TextureBase::GetBlink())
	{
		//点滅させる
		this->SetBlink(true);

		//ステートを切り替える(ダメージを受けたら、自動的に思考時間に移る)
		this->AddData1(ENEMYSTATE::NEXTSTATE);

		//補正値を設定する
		float correction = 1.0f;

		//針を準備しているタイミングで当たったら、受けるダメージは半分
		if (needleOver->GetNowState() == NEEDLEACTION::MOPE)correction = 0.5f;

		//ダメージを受ける
		this->OnDamage(player->GetAtack(), this->GetDefence(), correction);

		//エフェクトを再生する
		m_masicEffectManager->NowEffectPlay(NeedleOverall::TECHNIQUE_HEAL_COMMAND, NeedleOverall::TECHNIQUE_HEAL_SPECIAL);

		//スコアを追加する
		score->AddScore(this->GetLevel() * NeedleOverall::BASE_SCORE);
		score->SetFrameSave(this->GetLevel() * NeedleOverall::BASE_SCORE, 60.0f);
	}
}

void NormalEnemy::OnCollision_Resurrection(ITextureBase* obj)
{
	//Resurrectionを取得する
	Resurrection* resurrection = GameContext::Get<Resurrection>();

	//プレイヤーと当たった時の処理
	CoroKun* player = GameContext::Get<CoroKun>();

	//スコアを取得する
	Score* score = GameContext::Get<PlayScene>()->GetScore();

	//SoundManagerを取得する
	SoundManager* soundManager = SoundManager::GetInstance();

	//ダメージを受けていないとき
	if (!TextureBase::GetBlink())
	{
		//点滅させる
		this->SetBlink(true);

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

void NormalEnemy::OnCollision_VirusBeam(ITextureBase* obj)
{
	//プレイヤーと当たった時の処理
	CoroKun* player = GameContext::Get<CoroKun>();

	//スコアを取得する
	Score* score = GameContext::Get<PlayScene>()->GetScore();

	//SoundManagerを取得する
	SoundManager* soundManager = SoundManager::GetInstance();

	//ダメージを受けていないとき
	if (!TextureBase::GetBlink())
	{
		//点滅させる
		this->SetBlink(true);

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

void NormalEnemy::OnCollision_NormalBullet(ITextureBase* obj)
{
	//プレイヤーと当たった時の処理
	CoroKun* player = GameContext::Get<CoroKun>();

	//スコアを取得する
	Score* score = GameContext::Get<PlayScene>()->GetScore();

	//SoundManagerを取得する
	SoundManager* soundManager = SoundManager::GetInstance();

	//ダメージを受けていないとき
	if (!TextureBase::GetBlink())
	{
		//点滅させる
		this->SetBlink(true);

		//ステートを切り替える(ダメージを受けたら、自動的に思考時間に移る)
		this->AddData1(ENEMYSTATE::NEXTSTATE);

		//補正値を設定する
		float correction = 1.0f;

		correction = obj->GetScale().x / 1.6f;

		//ダメージを受ける
		this->OnDamage(player->GetAtack(), this->GetDefence(), correction);

		int core1 = NormalTechnique::TECHNIQUE_HEAL_COMMAND * correction;
		int core2 = NormalTechnique::TECHNIQUE_HEAL_SPECIAL * correction;

		//エフェクトを再生する
		m_masicEffectManager->NowEffectPlay(core1, core2);

		//スコアを追加する
		score->AddScore(this->GetLevel() * (NormalBullet::BASE_SCORE * correction));
		score->SetFrameSave(this->GetLevel() * (NormalBullet::BASE_SCORE * correction), 60.0f);
	}
}

void NormalEnemy::OnCollision_LeftPanel(ITextureBase* obj)
{
	//ステートを変更する
	m_AutoMoveState->SetNowState(DIRECT::LEFT);

	//描画を反転させる
	this->SetSpriteEffect(SpriteEffects::SpriteEffects_None);
}

void NormalEnemy::OnCollision_RightPanel(ITextureBase* obj)
{
	//ステートを変更する
	m_AutoMoveState->SetNowState(DIRECT::RIGHT);

	//描画を反転させる
	this->SetSpriteEffect(SpriteEffects::SpriteEffects_FlipHorizontally);
}
