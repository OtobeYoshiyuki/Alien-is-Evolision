#include "pch.h"
#include "SuperEnemy.h"
#include "OtobeLib/FrameWork/ResourceManager/ResourceManager.h"
#include "OtobeLib/StackScene/Scene/Play/PlayScene.h"
#include "OtobeLib/FrameWork/StaticData/StaticData.h"
#include "GameObject/Player/CoroKun.h"
#include "OtobeLib/Status/StatusCalc/StatusCalc.h"
#include "OtobeLib/FrameWork/Rand/Rand.h"
#include "GameObject/Item/Drop/ItemDrop.h"
#include "OtobeLib/FrameWork/SoundManager/SoundManager.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace OtobeLib;

//思考時間
const float SuperEnemy::THINK_TIME = 5.0f;

//敵の強さ(経験値に影響する)
const float SuperEnemy::STRENGTH = 200.0f;


SuperEnemy::SuperEnemy(ITextureBase* texture):
	TextureBase(texture),//基底クラスに情報を送信する
	m_HpGage(nullptr),//Hpゲージ
	m_HpBack(nullptr),//Hpゲージの裏面
	m_ThinkTime(0.0f),//思考時間
	m_experience(nullptr),//経験値
	m_masicEffectManager(nullptr),//技のエフェクトの管理クラス
	m_frameCount(NULL),//フレーム数のカウント
	m_damageCheck(false)//ダメージのチェックフラグ
{
	//ResourceManagerを取得する
	ResourceManager* resourceManager = ResourceManager::GetInstance();

	//GameManagerを取得する
	TextureManager* gameManager = GameContext::Get<PlayScene>()->GetGameManager().get();

	//経験値の画像を取得する
	ITextureBase* ex = resourceManager->GetTexture("経験値");

	//描画順を変更
	this->SetDrawOrder(5);

	//点滅の間隔を設定する
	TextureBase::SetTarget(0.1f);

	//点滅の終了の設定をする
	TextureBase::SetFinal(1);
	TextureBase::SetFinalOption(true);

	//固定砲台のインデックスを初期化する
	this->AddData2(0);

	//実行関数を起こす
	this->SetActive(true);


	//アニメーションの設定
	for (const std::unique_ptr<AnimationImporter>& importer : Animation::GetAnimeList("SuperEnemy"))
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
	this->SetAniFlag(false);
	this->SetLoopFlag(false);

	//現在のアニメーション情報を設定する
	this->SetNowInfo("スーパー1");
	this->SetLoopInfo("スーパー5");
	this->SetNowAnimation(this->GetNowAnimation());

	ITextureBase* Gage = resourceManager->GetTexture("WhiteBack");

	//回復アイテムの画像を取得する
	ITextureBase* healTex = resourceManager->GetTexture("回復2");

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
	this->SetRadius(Vector3(this->AskOrigin()*2));

	//HealManagerを取得する
	HealManager* healManager = GameContext::Get<PlayScene>()->GetHealManager();

	//回復アイテムを生成する
	healManager->CreateItem(healTex, [&](Item* item)
		{	Heal* healItem = dynamic_cast<Heal*>(item);
	m_healItem = healItem; });
}

void SuperEnemy::Initialize()
{
	//PlaySceneを取得する
	PlayScene* playScene = GameContext::Get<PlayScene>();

	//TextureManagerを取得する
	TextureManager* gameManager = playScene->GetGameManager().get();

	//静的データを取得する
	StaticData* staticData = StaticData::GetInstance();

	//Playerを取得する
	CoroKun* player = GameContext::Get<CoroKun>();

	//固定砲台の生成を行う
	for (int i = 0; i < TURRET_NUM; i++)
	{
		//インスタンスを生成する
		std::unique_ptr<FixedTurret> pturret = std::make_unique<FixedTurret>();
		//所有者のポインタを設定する
		pturret->SetOwner(this);
		//初期化する
		pturret->Initialize();
		//描画を切る
		for (SoapBabble* babble : pturret->GetBabbleList())
		{
			babble->SetState(STATE::NONE);
		}
		//所有権をリストに追加する
		m_TurrentList.push_back(std::move(pturret));
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

void SuperEnemy::Update()
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

	//固定砲台の更新
	for (std::unique_ptr<FixedTurret>& turret : m_TurrentList)
	{
		turret->Update();
	}

	//画像の更新処理
	TextureBase::Update();

	//点滅中の時
	if (this->GetBlink())
	{
		switch (this->GetNowData3())
		{
		case COLOR_CHANGE::NONE:
			//色を白にする
			this->SetColor(Colors::White);

			//5フレームごとに色を切り替える
			if (m_frameCount % 5 == NULL && m_frameCount != NULL)
			{
				//ステートを切り替える
				this->AddData3(COLOR_CHANGE::CHANGE);
			}

			break;
		case COLOR_CHANGE::CHANGE:
			//色を赤にする
			this->SetColor(Colors::Red);

			//5フレームごとに色を切り替える
			if (m_frameCount % 5 == NULL && m_frameCount != NULL)
			{
				//ステートを切り替える
				this->AddData3(COLOR_CHANGE::NONE);
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

	//当たり判定の座標を設定する
	this->SetCenter(this->GetPosition());

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

void SuperEnemy::Render()
{
	//画像の描画処理
	TextureBase::Render();
}

void SuperEnemy::Finalize()
{
}

void SuperEnemy::OnCollisionEnter(ITextureBase* obj)
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

}

void SuperEnemy::NonCollisionEnter(ITextureBase* obj)
{
	//当たり判定の処理
	CollisionBase::NonCollisionEnter(obj);
}

void SuperEnemy::OnDamage(const int& atack, const int& defence, const float correction)
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

void SuperEnemy::UpdateGage()
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

void SuperEnemy::UpdateState()
{
	//ステートの更新処理
	//ステートによって、処理を分ける

	//ダメージを受けていないときで、HPが0になった時
	if (!this->GetBlink() && this->GetHP() <= NULL && this->GetNowData1() < ENEMYSTATE::DEADSTART)
	{
		//ステートを切り替える
		this->AddData1(ENEMYSTATE::DEADSTART);
	}

	switch (this->GetNowData1())
	{
	case ENEMYSTATE::ENONE:
		this->State_None();
		break;
	case ENEMYSTATE::ATACKSTART:
		this->State_Start();
		break;
	case ENEMYSTATE::ATACKING:
		this->State_Atacking();
		break;
	case ENEMYSTATE::ATACKRESUTL:
		this->State_AtackResult();
		break;
	case ENEMYSTATE::NEXTSTATE:
		this->State_Next();
		break;
	case ENEMYSTATE::DEADSTART:
		this->State_DeadStart();
		break;
	case ENEMYSTATE::DEADTING:
		this->State_Deadting();
		break;
	case ENEMYSTATE::DEAD:
		this->State_Dead();
		break;
	}
}

void SuperEnemy::State_None()
{
	//State_Noneの処理

	//SoundManagerを取得する
	SoundManager* soundManager = SoundManager::GetInstance();

	//アニメーションを起動させる
	this->SetAniFlag(true);

	//座標を設定する
	for (SoapBabble* babble : m_TurrentList[this->GetNowData2()]->GetBabbleList())
	{
		babble->SetPosition(this->GetPosition());
	}

	//発射用の警告を出す
	soundManager->Play(CRI_CUESHEET_0__43_攻撃前SE);

	//次のステートへ移行する
	this->AddData1(ENEMYSTATE::ATACKSTART);
}

void SuperEnemy::State_Start()
{
	//State_Startの処理

	//SoundManagerを取得する
	SoundManager* soundManager = SoundManager::GetInstance();

	//アニメーションが終わるまで待つ
	if (!this->GetAniFlag())
	{
		//発射用の音を出す
		soundManager->Play(CRI_CUESHEET_0__44_発射SE);

		//自動移動とジャンプステートを起動させる
		m_TurrentList[this->GetNowData2()]->GetAutoMoveState()->SetExecute(true);
		m_TurrentList[this->GetNowData2()]->GetJumpState()->SetExecute(true);

		m_TurrentList[this->GetNowData2()]->GetJumpState()->SetNowState(JUMP::START);

		//全ての泡を描画する
		for (SoapBabble* babble : m_TurrentList[this->GetNowData2()]->GetBabbleList())
		{
			babble->SetState(STATE::DRAW);
		}

		//ステートを切り替える
		this->AddData1(ENEMYSTATE::ATACKING);
	}
}

void SuperEnemy::State_Atacking()
{
	//State_Atackingの処理

	//SoundManagerを取得する
	SoundManager* soundManager = SoundManager::GetInstance();

	//ジャンプが終了したら、描画を消す
	if (m_TurrentList[this->GetNowData2()]->GetJumpState()->GetJumpmDesignation())
	{
		//着弾用の音を出す
		soundManager->Play(CRI_CUESHEET_0__42_着弾SE);

		m_TurrentList[this->GetNowData2()]->GetParentBabble()->SetState(ITextureBase::STATE::NONE);

		//自動移動とジャンプステートを停止させる
		m_TurrentList[this->GetNowData2()]->GetAutoMoveState()->SetExecute(false);
		m_TurrentList[this->GetNowData2()]->GetJumpState()->SetExecute(false);

		//終了フラグを再設定する
		m_TurrentList[this->GetNowData2()]->GetJumpState()->SetJumpmDesignation(false);

		//乱数を振る
		for (SoapBabble* child : m_TurrentList[this->GetNowData2()]->GetChildBabble())
		{
			m_TurrentList[this->GetNowData2()]->RandShake(child);
		}

		//ステートを切り替える
		this->AddData1(ENEMYSTATE::ATACKRESUTL);
	}
}

void SuperEnemy::State_AtackResult()
{
	//State_AtackResultの処理

	//静的データの取得
	StaticData* staticData = StaticData::GetInstance();

	//思考時間が経過したら、元に戻す
	if (m_ThinkTime > THINK_TIME)
	{
		m_ThinkTime = 0.0f;

		//現在の弾を取得する
		int bullet = this->GetNowData2();

		//最後の弾の場合は、最初の球に戻る
		if (bullet == (int)m_TurrentList.size() - 1)bullet = NULL;
		
		else bullet++;

		//弾を装填する
		this->AddData2(bullet);

		//ステートを切り替える
		this->AddData1(ENEMYSTATE::ENONE);
	}

	//思考時間を更新する
	m_ThinkTime += staticData->Get_FloatData("elapsedTime");
}

void SuperEnemy::State_Next()
{
	//State_Nextの処理

	//点滅が終了するまで待つ
	if (!this->GetBlink())
	{
		//点滅が終了したら、ステートを切り替える
		this->AddData1(ENEMYSTATE::ENONE);

		//思考時間を初期化する
		m_ThinkTime = 0.0f;

		//自動移動とジャンプステートを停止させる
		m_TurrentList[this->GetNowData2()]->GetAutoMoveState()->SetExecute(false);
		m_TurrentList[this->GetNowData2()]->GetJumpState()->SetExecute(false);

		//終了フラグを再設定する
		m_TurrentList[this->GetNowData2()]->GetJumpState()->SetJumpmDesignation(false);
	}
}

void SuperEnemy::State_DeadStart()
{
	//State_DeadStartの処理

	//TurretListの処理
	for (std::unique_ptr<FixedTurret>& turret : m_TurrentList)
	{
		//実行関数を落とす
		turret->GetAutoMoveState()->SetExecute(false);
		turret->GetJumpState()->SetExecute(false);
		//終了フラグを再設定する
		turret->GetJumpState()->SetJumpmDesignation(false);
		for (SoapBabble* babble : turret->GetBabbleList())
		{
			//泡の描画を切る
			babble->SetState(STATE::NONE);
		}
	}

	//配列の参照インデックスを初期化する
	this->AddData2(NULL);

	//アニメーションを切り替える
	this->SetNowInfo("スーパー6");

	//アニメーションのループ情報を設定する
	this->SetLoopInfo("スーパー10");

	//アニメーションを起動させる
	this->SetAniFlag(true);

	//ステートを切り替える
	this->AddData1(ENEMYSTATE::DEADTING);
}

void SuperEnemy::State_Deadting()
{
	//State_Deadtingの処理

	//アニメーションが終了するまで待つ
	if (this->GetNowInfo() == "スーパー10")
	{
		//アニメーションが終了したら、１秒間点滅させる
		this->SetBlink(true);

		//ステートを切り替える
		this->AddData1(ENEMYSTATE::DEAD);
	}
}

void SuperEnemy::State_Dead()
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

void SuperEnemy::HealItemDrop()
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

void SuperEnemy::OnCollision_Player(ITextureBase* obj)
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

			//アニメーションを切り替える
			this->SetNowInfo("スーパー1");

			//アニメーションの更新を止める
			this->SetAniFlag(false);

			//ステートを切り替える(ダメージを受けたら、自動的に思考時間に移る)
			this->AddData1(ENEMYSTATE::NEXTSTATE);

			//フラグを強制的に発射済みにする
			m_TurrentList[this->GetNowData2()]->InitTurret();

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

void SuperEnemy::OnCollision_NeedleOverAll(ITextureBase* obj)
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

		//アニメーションを切り替える
		this->SetNowInfo("スーパー1");

		//アニメーションの更新を止める
		this->SetAniFlag(false);

		//ステートを切り替える(ダメージを受けたら、自動的に思考時間に移る)
		this->AddData1(ENEMYSTATE::NEXTSTATE);

		//フラグを強制的に発射済みにする
		m_TurrentList[this->GetNowData2()]->InitTurret();

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

void SuperEnemy::OnCollision_Resurrection(ITextureBase* obj)
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

		//アニメーションを切り替える
		this->SetNowInfo("スーパー1");

		//アニメーションの更新を止める
		this->SetAniFlag(false);

		//ステートを切り替える(ダメージを受けたら、自動的に思考時間に移る)
		this->AddData1(ENEMYSTATE::NEXTSTATE);

		//フラグを強制的に発射済みにする
		m_TurrentList[this->GetNowData2()]->InitTurret();

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

void SuperEnemy::OnCollision_VirusBeam(ITextureBase* obj)
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

		//アニメーションを切り替える
		this->SetNowInfo("スーパー1");

		//アニメーションの更新を止める
		this->SetAniFlag(false);

		//ステートを切り替える(ダメージを受けたら、自動的に思考時間に移る)
		this->AddData1(ENEMYSTATE::NEXTSTATE);

		//フラグを強制的に発射済みにする
		m_TurrentList[this->GetNowData2()]->InitTurret();

		//ダメージを受ける
		this->OnDamage(player->GetAtack(), this->GetDefence());

		//エフェクトを再生する
		m_masicEffectManager->NowEffectPlay(VirusBeam::TECHNIQUE_HEAL_COMMAND, VirusBeam::TECHNIQUE_HEAL_SPECIAL);

		//スコアを追加する
		score->AddScore(this->GetLevel() * VirusBeam::BASE_SCORE);
		score->SetFrameSave(this->GetLevel() * VirusBeam::BASE_SCORE, 60.0f);
	}
}

void SuperEnemy::OnCollision_NormalBullet(ITextureBase* obj)
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

		//アニメーションを切り替える
		this->SetNowInfo("スーパー1");

		//アニメーションの更新を止める
		this->SetAniFlag(false);

		//ステートを切り替える(ダメージを受けたら、自動的に思考時間に移る)
		this->AddData1(ENEMYSTATE::NEXTSTATE);

		//フラグを強制的に発射済みにする
		m_TurrentList[this->GetNowData2()]->InitTurret();

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
