#include "CoroKun.h"
#include "pch.h"
#include "CoroKun.h"
#include "OtobeLib/FrameWork/ResourceManager/ResourceManager.h"
#include "GameContext.h"
#include "OtobeLib/StackScene/Scene/Play/PlayScene.h"
#include "OtobeLib/StackScene/Scene/Select/SelectScene.h"
#include "OtobeLib/StackScene/Scene/DirecTing/DirecTingScene.h"
#include "OtobeLib/FrameWork/Input/Input.h"
#include "OtobeLib/FrameWork/TextureFont/TextureFont.h"
#include "OtobeLib/StackScene/SceneManager/SceneManager.h"
#include "GameObject/Stage/Maptip/MapTip.h"
#include "GameObject/Enemy/Hyper/HyperEnemy.h"
#include "GameObject/Enemy/Super/SuperEnemy.h"
#include "GameObject/Enemy/Normal/NormalEnemy.h"
#include "GameObject/Stage/Stage.h"
#include "GameObject/Stage/Maptip/Wall/Wall.h"
#include "OtobeLib/FrameWork/StaticData/StaticData.h"
#include "OtobeLib/FrameWork/Collision/CollisionManager/CollisionManager.h"
#include "GameObject/MoveState/AccelState/AccelState.h"
#include "GameObject/TimeLag/TimeLag.h"
#include "GameObject/Gage/NormalGage/NormalGage.h"
#include "OtobeLib/Status/StatusCalc/StatusCalc.h"
#include "ExternalLib/Easing/Easing.h"
#include "OtobeLib/FrameWork/SoundManager/SoundManager.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace OtobeLib;
using namespace ExternalLib;

//行ける範囲の最大値
const Vector3 CoroKun::MAX_LIMIT = Vector3(4240.0f, 661.0f,0.0f);

//行ける範囲の最小値
const Vector3 CoroKun::MIN_LIMIT = Vector3(200.0f, 141.0f,0.0f);

//1フレームに加算するスピード
const float CoroKun::SPEED = 5.0f;

//Easing関数に使用するトータルタイム
const float CoroKun::EASING_TOTALTIME = 1.0f;

//Easing関数に使用する最大値
const float CoroKun::EASING_MAX = 1.0f;

//Easing関数に使用する最小値
const float CoroKun::EASING_MIN = 0.0f;

//リバースアビリティの持続時間
const float CoroKun::DEATHBLOW_ABILITY_SUSTAIN_TIME = 10.0f;

//背面の点滅の開始時間
const float CoroKun::BACKTEXTURE_BLINK_START = 7.0f;

CoroKun::CoroKun(ITextureBase* texture):
	TextureBase(texture),//基底クラスに情報を送信する
	m_HPGage(nullptr),//HPゲージ
	m_mpGage(nullptr),//MPゲージ
	m_specialGage(nullptr),//スペシャルゲージ
	m_backTexture(nullptr),//背面の画像
	m_WalkState(nullptr),//WalkState
	m_DamageState(nullptr),//DamageState
	m_BodyBlow(nullptr),//BodyBlow
	m_VirusBeam(nullptr),//VirusBeam
	m_NeedleOverAll(nullptr),//Needleの配列を管理しているクラス
	m_Resurrection(nullptr),//Needleの配列を管理しているクラス
	m_Normal(nullptr),//通常技のクラス
	m_easingTime(0.0f),//Easing関数のTime
	m_timer(0.0f),//経過時間
	m_timerUp(false),//時間の更新
	m_warpPos(Vector3::Zero),//浮遊後の座標
	m_frameCount(NULL),//フレーム数のカウント
	m_damageCheck(false),//ダメージのチェックフラグ
	m_fadeBodyBlow(nullptr),//コマンド欄の体当たりのエフェクト
	m_fadeBeam(nullptr),//コマンド欄のビームのエフェクト
	m_fadeNeedleOverAll(nullptr),//コマンド欄の針の全体攻撃のエフェクト
	m_fadeResurection(nullptr),//コマンド欄の針の範囲攻撃のエフェクト
	m_fadeSpecial(nullptr),//必殺技UIの背景のテクスチャ
	m_sweatTexture(nullptr)//汗のテクスチャ
{
	//描画順を設定する
	this->SetDrawOrder(4);

	//イージングタイムの初期化
	m_easingTime = 0.0f;

	//GameContextに登録する
	GameContext::Register<CoroKun>(this);

	//ResourceManagerを取得する
	ResourceManager* resourceManager = ResourceManager::GetInstance();
	ITextureBase* Gage = resourceManager->GetTexture("WhiteBack");
	ITextureBase* back = resourceManager->GetTexture("SickBack");
	ITextureBase* command = resourceManager->GetTexture("コマンド");
	ITextureBase* sweat = resourceManager->GetTexture("汗");

	//PlaySceneを取得する
	PlayScene* playScene = GameContext::Get<PlayScene>();

	//TextureManagerを取得する
	TextureManager* gameManager = playScene->GetGameManager().get();
	TextureManager* subInfoManager = playScene->GetSubInfoManager().get();

	//背面のテクスチャを生成する
	std::unique_ptr<TextureBase> pbackTexture = std::make_unique<TextureBase>(back);

	//HPゲージを生成する
	std::unique_ptr<HPGage> pHPGage = std::make_unique<HPGage>(Gage);

	//技ゲージを生成する
	std::unique_ptr<TechniqueGage> pTechniqueGage = std::make_unique<TechniqueGage>(Gage);

	//必殺技ゲージを生成する
	std::unique_ptr<DeathblowGage> pDeathblowGage = std::make_unique<DeathblowGage>(Gage);

	//経験値ゲージを生成する
	std::unique_ptr<ExperienceGage> pExperienceGage = std::make_unique<ExperienceGage>(Gage);

	//生ポインタを登録する
	m_backTexture = pbackTexture.get();
	m_HPGage = pHPGage.get();
	m_mpGage = pTechniqueGage.get();
	m_specialGage = pDeathblowGage.get();
	m_expGage = pExperienceGage.get();

	//背面のテクスチャの色を変更する
	m_backTexture->SetColor(Colors::SkyBlue);
	//HPゲージの色を変更する
	m_HPGage->SetColor(Colors::GreenYellow);
	//技ゲージの色を変更する
	m_mpGage->SetColor(Colors::Blue);
	//必殺技ゲージの色を変更する
	m_specialGage->SetColor(Colors::Red);
	//経験値ゲージの色を変更する
	m_expGage->SetColor(Colors::Aqua);

	//背面のテクスチャの描画を切る
	m_backTexture->SetState(STATE::NONE);
	//描画順を設定する
	m_backTexture->SetDrawOrder(5);
	//原点を設定する
	m_backTexture->SetOrigin(m_backTexture->AskOrigin());
	//点滅の設定をする
	m_backTexture->SetBlink(false);
	//点滅の間隔を設定する
	m_backTexture->SetTarget(0.1f);
	//点滅のオプションを設定する
	m_backTexture->SetFinalOption(true);
	//点滅の終了時間を設定する
	m_backTexture->SetFinal(3);

	//切り取り位置を変更する
	//HPゲージ
	m_HPGage->SetRect(RECT{ 0,0,0,50 });
	m_HPGage->SetPosition(Vector3(200.0f, 0.0f, 0.0f));
	m_HPGage->SetMaxTexLength(600);

	//技ゲージ
	m_mpGage->SetRect(RECT{0,0,0,40});
	m_mpGage->SetPosition(Vector3(200.0f, 75.0f, 0.0f));
	m_mpGage->SetDrawOrder(2);
	m_mpGage->SetMaxTexLength(500);

	//必殺技ゲージ
	m_specialGage->SetRect(RECT{ 0,0,0,70 });
	m_specialGage->SetPosition(Vector3(200.0f, 75.0f, 0.0f));
	m_specialGage->SetDrawOrder(4);
	m_specialGage->SetMaxTexLength(600);

	//経験値ゲージ
	m_expGage->SetRect(RECT{ 0,0,0,25 });
	m_expGage->SetPosition(Vector3(200.0f, 50.0f, 0.0f));
	m_expGage->SetMaxTexLength(600);
	m_expGage->SetFrameCalcSpeed((int)SPEED);

	//TextureManagerに追加する
	gameManager->Add(std::move(pbackTexture));
	subInfoManager->Add(std::move(pHPGage));
	subInfoManager->Add(std::move(pTechniqueGage));
	subInfoManager->Add(std::move(pDeathblowGage));
	subInfoManager->Add(std::move(pExperienceGage));

	//画像の中心位置を設定する
	this->SetOrigin(this->AskOrigin());

	//更新関数を起こす
	this->SetActive(true);

	//ステートを設定する
	m_WalkState = std::make_unique<WalkState>();
	m_DamageState = std::make_unique<DamageState>();
	m_AccelState = std::make_unique<AccelState>();
	GameContext::Register<WalkState>(m_WalkState.get());
	GameContext::Register<DamageState>(m_DamageState.get());
	GameContext::Register<AccelState>(m_AccelState.get());
	m_StateList.push_back(m_WalkState.get());
	m_StateList.push_back(m_DamageState.get());
	m_StateList.push_back(m_AccelState.get());

	//WalkStateをonにする
	m_WalkState->SetExecute(true);

	//DamageStateをoffにする
	m_DamageState->SetExecute(false);

	//AccelStateをoffにする
	m_AccelState->SetExecute(false);

	//点滅の間隔を設定する
	TextureBase::SetTarget(0.1f);

	//点滅の終了の設定をする
	TextureBase::SetFinal(3);
	TextureBase::SetFinalOption(true);

	//技を生成する
	this->CreateTechnique();

	//プレイヤーのステータスを取得する
	std::vector<std::string> playerStatus = playScene->GetSaveDataArray((int)PlayScene::SAVE_DATA::PLAYER_STATUS);

	//敵のレベル情報を取得する
	int hp = std::stoi(playerStatus[(int)PlayScene::PLAYER_STATUS::HP]);
	int mp = std::stoi(playerStatus[(int)PlayScene::PLAYER_STATUS::MP]);
	int atack = std::stoi(playerStatus[(int)PlayScene::PLAYER_STATUS::ATACK]);
	int defence = std::stoi(playerStatus[(int)PlayScene::PLAYER_STATUS::DEFENCE]);
	int sp = std::stoi(playerStatus[(int)PlayScene::PLAYER_STATUS::SPECIAL]);
	int level = std::stoi(playerStatus[(int)PlayScene::PLAYER_STATUS::LEVEL]);
	int next = std::stoi(playerStatus[(int)PlayScene::PLAYER_STATUS::NEXT]);

	//ステータスを設定する
	StatusImporter* importer = StatusBase::FindStatus(this->GetName(), level);

	//プレイヤーのステータスを設定する
	this->SetStatus(NULL, NULL, atack, defence, NULL, level, NULL);

	//ステータスの最大値を設定する
	this->SetMaxStatus(importer->GetHp(), importer->GetMp(), importer->GetAtack(), importer->GetDefence(),
		importer->GetSpecial(), MAX_LEVEL, importer->GetNextLevelPoint());

	//ゲージの補正を行う
	this->OnHealHp(hp);
	this->OnHealCommand(mp);
	this->OnHealSpecial(sp);

	//レベルが1の時
	if (this->GetLevel() == MIN_LEVEL)
	{
		//ゲージを回復させる
		this->OnHealExp(next);
	}
	//レベルが２以上の時
	else
	{
		//入れ替え用の経験値を初期化
		int tempNext = NULL;

		for (int i = MIN_LEVEL; i <= level;i++)
		{
			//そのレベルのステータスの上限を取得する
			StatusImporter* ip = StatusBase::FindStatus(this->GetName(), i);

			//経験値の上限を加算していく
			tempNext += ip->GetNextLevelPoint();
		}

		//現在の経験値から引く
		tempNext -= next;
		tempNext = this->GetMaxNextLevelPoint() - tempNext;

		//ゲージを回復させる
		this->OnHealExp(tempNext);

		//経験値を設定する
		this->SetNextLevelPoint(next);
	}

	//画像の大きさを当たり判定に使用する
	this->SetRadius(Vector3(this->AskOrigin()));

	//現在のステートを設定する
	this->AddData2(PLAYERMOVE::INPUT);

	//コマンド欄のエフェクトを生成する

	//コマンド欄の体当たりのエフェクトを生成
	std::unique_ptr<ITextureBase> pfadeBody = std::make_unique<TextureBase>(command);
	pfadeBody->SetPosition(Vector3(0.0f, 0.0f, 0.0f));
	pfadeBody->SetRect(RECT{ 0,0,200,180 });
	pfadeBody->SetColor(Colors::LightGreen);
	pfadeBody->SetState(STATE::DRAW);
	m_fadeBodyBlow = pfadeBody.get();

	//コマンド欄のビームのエフェクトを生成する
	std::unique_ptr<ITextureBase> pfadeBeam = std::make_unique<TextureBase>(command);
	pfadeBeam->SetPosition(Vector3(0.0f, 180.0f, 0.0f));
	pfadeBeam->SetRect(RECT{ 0,180,200,360 });
	pfadeBeam->SetColor(Colors::LightGreen);
	pfadeBeam->SetState(STATE::DRAW);
	m_fadeBeam = pfadeBeam.get();

	//コマンド欄の針の全体攻撃のエフェクトを生成する
	std::unique_ptr<ITextureBase> pfadeNeedle = std::make_unique<TextureBase>(command);
	pfadeNeedle->SetPosition(Vector3(0.0f, 360.0f, 0.0f));
	pfadeNeedle->SetRect(RECT{ 0,360,200,540 });
	pfadeNeedle->SetColor(Colors::LightGreen);
	pfadeNeedle->SetState(STATE::DRAW);
	m_fadeNeedleOverAll = pfadeNeedle.get();

	//コマンド欄の針の範囲攻撃のエフェクトを生成する
	std::unique_ptr<ITextureBase> pfadeResurection = std::make_unique<TextureBase>(command);
	pfadeResurection->SetPosition(Vector3(0.0f, 540.0f, 0.0f));
	pfadeResurection->SetRect(RECT{ 0,540,200,720 });
	pfadeResurection->SetColor(Colors::LightGreen);
	pfadeResurection->SetState(STATE::DRAW);
	m_fadeResurection = pfadeResurection.get();

	//必殺技の背景のUIを生成する
	std::unique_ptr<FadeTexture> pfadeSpecial = std::make_unique<FadeTexture>(Gage);
	pfadeSpecial->SetPosition(Vector3(550.0f, 661.0f, 0.0f));
	pfadeSpecial->SetRect(RECT{ 0,0,250,59 });
	pfadeSpecial->SetColor(Colors::Red);
	pfadeSpecial->SetMaxAlfa(1.0f);
	pfadeSpecial->SetMinAlfa(0.0f);
	pfadeSpecial->SetFadeSpeed(0.02f);
	pfadeSpecial->SetDrawOrder(5);
	m_fadeSpecial = pfadeSpecial.get();

	//汗のテクスチャを生成する
	std::unique_ptr<TextureBase> psweat = std::make_unique<TextureBase>(sweat);
	psweat->SetState(STATE::NONE);
	m_sweatTexture = psweat.get();

	//TextureManagerに追加する
	subInfoManager->Add(std::move(pfadeBody));
	subInfoManager->Add(std::move(pfadeBeam));
	subInfoManager->Add(std::move(pfadeNeedle));
	subInfoManager->Add(std::move(pfadeResurection));
	subInfoManager->Add(std::move(pfadeSpecial));
	gameManager->Add(std::move(psweat));
}

void CoroKun::Initialize()
{
	//PlaySceneを取得する
	PlayScene* playScene = GameContext::Get<PlayScene>();

	//TextureManagerを取得する
	TextureManager* subInfoManager = playScene->GetSubInfoManager().get();

	//体当たりの初期座標を設定
	m_BodyBlow->SetPosition(this->GetPosition() + Vector3(this->AskOrigin().x * 2, 0.0f, 0.0f));

	//背面のテクスチャの初期座標を設定する
	m_backTexture->SetPosition(this->GetPosition());

	//Stageを取得する
	Stage* stage = GameContext::Get<Stage>();

	//マップ情報を取得する
	std::vector<std::vector<ITextureBase*>>& mapAll = stage->GetMap();

	//マップ情報を追加する
	mapAll[mapAll.size() - 1].push_back(m_BodyBlow);
	mapAll[mapAll.size() - 1].push_back(m_VirusBeam);

	//針の数分、ステージ情報を追加する
	for (Needle* needle : m_NeedleOverAll->GetNeedleList())
	{
		//マップ情報を追加する
		mapAll[mapAll.size() - 1].push_back(needle);
	}

	//針の数分、ステージ情報を追加する
	for (Needle* needle : m_Resurrection->GetNeedleList())
	{
		//マップ情報を追加する
		mapAll[mapAll.size() - 1].push_back(needle);
	}

	//浮遊用の座標を設定する
	m_warpPos = this->GetPosition();

	//HPゲージの初期化
	m_HPGage->CreateGage(subInfoManager);

	//色を反映させるために、更新をかける
	this->UpdateCommandEffect();

	//汗の座標を設定する
	m_sweatTexture->SetPosition(this->GetPosition());
}

void CoroKun::Update()
{
	//静的データの取得
	StaticData* staticData = StaticData::GetInstance();

	//UIの表示が終わるまで何もしない
	TimeLag* go = GameContext::Get<PlayScene>()->GetLagGo().get();
	if (!go->GetFinishFlag())return;

	//CameraManagerを取得する
	CameraManager* cameraManager = GameContext::Get<CameraManager>();

	//現在稼働しているカメラを取得する
	CameraBase* nowCamera = cameraManager->GetNowCamera();

	//カメラがスクロールカメラの時は、更新を止める
	if (!nowCamera->GetObjectMove())return;

	//実行関数が落ちているときは、何もしない
	if (!this->GetActive())return;

	//他のオブジェクトを解放する
	this->GetObjectList().clear();

	//移動の処理
	this->Move();
	
	//プレイヤー関連の処理
	this->PlayerState();

	//HPゲージの更新処理
	this->UpdateHpGage();

	//画像の更新処理
	TextureBase::Update();

	//変数の初期化
	Vector3 sweatPos = Vector3::Zero;

	//右を向いているとき
	if (this->DirectX() == 1)
	{
		sweatPos = m_warpPos + Vector3(50.0f, -50.0f, 0.0f);
	}
	//左を向いているとき
	else
	{
		sweatPos = this->GetPosition() + Vector3(-45.0f, -25.0f, 0.0f);
	}

	m_sweatTexture->SetPosition(sweatPos);

	//汗の表示形式を変更
	m_sweatTexture->SetSpriteEffect(this->GetSpriteEffect());

	//プレイヤーと同じ描画方法
	m_BodyBlow->SetSpriteEffect(this->GetSpriteEffect());

	//AccelStateが実行されているとき
	if (m_AccelState->GetExecute())
	{
		m_BodyBlow->SetState(STATE::DRAW);
	}
	else
	{
		m_BodyBlow->SetState(STATE::NONE);
	}

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

	//コマンドエフェクトの更新処理
	this->UpdateCommandEffect();

	//アニメーションの更新処理
	Animation::Update();

	//アニメーションの情報を設定する
	this->SetRect(this->GetNowAnimation());

	//当たり判定の座標を設定する
	this->SetCenter(this->GetPosition());

	//当たり判定を追加する
	GameContext::Get<CollisionManager>()->AddUseHero(this);

	//HyperEnemyのリストを取得する
	std::vector<HyperEnemy*> hyeneList = GameContext::Get<Stage>()->GetHyperEnemies();

	for (HyperEnemy* hyene : hyeneList)
	{
		//敵も自分も生存しているとき
		if (hyene->GetHP() > NULL && this->GetHP() > NULL)
		{
			//当たり判定を設定する
			GameContext::Get<CollisionManager>()->AddUseOther(hyene, this, 100.0f);
			//泡のインスタンスを取得する
			for (SoapBabble* babble : hyene->GetBabbleList())
			{
				//当たり判定を追加する
				GameContext::Get<CollisionManager>()->AddUseOther(babble, this, 100.0f);
			}
		}
	}

	//SuperEnemyのリストを取得する
	std::vector<SuperEnemy*> superList = GameContext::Get<Stage>()->GetSuperEnemies();

	for (SuperEnemy* super : superList)
	{
		//敵も自分も生存しているとき
		if (super->GetHP() > NULL && this->GetHP() > NULL)
		{
			//当たり判定を設定する
			GameContext::Get<CollisionManager>()->AddUseOther(super, this, 100.0f);
			//泡のインスタンスを取得する
			for (std::unique_ptr<FixedTurret>& turret : super->GetTurretList())
			{
				for (SoapBabble* babble : turret->GetBabbleList())
				{
					//当たり判定を追加する
					GameContext::Get<CollisionManager>()->AddUseOther(babble, this, 300.0f);
				}
			}
		}
	}

	//NormalEnemyのリストを取得する
	std::vector<NormalEnemy*> normalList = GameContext::Get<Stage>()->GetNormalEnemies();

	for (NormalEnemy* normal : normalList)
	{
		//敵も自分も生存しているとき
		if (normal->GetHP() > NULL && this->GetHP() > NULL)
		{
			//当たり判定を設定する
			GameContext::Get<CollisionManager>()->AddUseOther(normal, this, 100.0f);
		}
	}

	//アイテムのリストを取得する
	std::vector<Item*> itemList = GameContext::Get<PlayScene>()->GetHealManager()->GetAllItem();

	for (Item* item : itemList)
	{
		//HPが0より大きいとき
		if (this->GetHP() > NULL)
		{
			//当たり判定を設定する
			GameContext::Get<CollisionManager>()->AddUseOther(this, item, 100.0f);
		}

	}

	//データが存在するかどうか、確認する
	if (!staticData->Request_Data("プレイヤーダメージ"))return;

	//ステートの更新処理
	//ダメージステートが実行中で、7回以上ダメージを与えたらWalkStateを実行する
	if (m_DamageState->GetExecute() && m_DamageState->GetDamageCount() >= DamageState::MAX_DAMAGE)
	{
		if (this->GetNowData2() < PLAYERMOVE::SWEATANIME)
		{
			m_WalkState->SetExecute(true);
		}
		m_DamageState->InitDamageState();
		staticData->Add("プレイヤーダメージ", "");
	}
}

void CoroKun::Render()
{
	//画像の描画処理
	//削除リクエストがあったら、何もしない
	if (this->GetIsVal())return;

	//テクスチャハンドルが入ってなかったら何もしない
	if (this->GetHandle() == nullptr)return;

	//点滅がoffのときは何もしない
	if (!this->NowBlinking())return;

	//描画がoffのときは何もしない
	if (this->GetState() != STATE::DRAW)return;

	//描画系の情報
	Graphics* Graph = Graphics::GetInstance();
	SpriteBatch* Sprite = Graph->GetSpriteBatch();
	CommonStates* State = Graph->GetCommonStates();

	//描画
	Sprite->Draw(this->GetHandle().Get(), m_warpPos, &this->GetRect(), this->GetColor(),
		XMConvertToRadians(this->GetAngle()), this->GetOrigin(), this->GetScale(), this->GetSpriteEffect());
}

void CoroKun::Finalize()
{
}

void CoroKun::OnCollisionEnter(ITextureBase* obj)
{
	//他のオブジェクトと当たった時に呼ばれる関数
	CollisionBase::OnCollisionEnter(obj);
	
	//床に当たった時
	if (obj->GetName() == "floor")this->OnCollisionFloor(obj);

	//壁に当たった時
	if (obj->GetName() == "wall")this->OnCollisionWall(obj);

	//泡に当たったとき
	if (obj->GetName() == "babul")this->OnCollisionBabul(obj);

	//反転台に当たったとき
	if (obj->GetName() == "rebound")this->OnCollisionRebound(obj);

	//起爆スイッチに当たった時
	if (obj->GetName() == "switch")this->OnCollisionSwitch(obj);

	//落とし穴に当たった時
	if (obj->GetName() == "hole")this->OnCollisionFoleFloor(obj);

	//HyperEnemyに当たった時
	if (obj->GetName() == "HyperEnemy")this->OnCollisionHyperEnemy(obj);

	//SuperEnemyに当たった時
	if (obj->GetName() == "SuperEnemy")this->OnCollision_SuperEnemy(obj);

	//NormalEnemyに当たったとき
	if (obj->GetName() == "NormalEnemy")this->OnCollision_NormalEnemy(obj);

	//UIに当たった時
	if (obj->GetName() == "WhiteBack")this->OnCollisionWhiteBack(obj);

	//HyperEnemyの泡に当たった時
	if (obj->GetName() == "Hypersyabondama")this->OnCollisionHyperBabble(obj);

	//HyperEnemyの泡に当たった時
	if (obj->GetName() == "Supersyabondama")this->OnCollision_SuperBabble(obj);

	//ゴールに当たった時
	if (obj->GetName() == "ゴール")this->OnCollision_Goal(obj);

}

void CoroKun::NonCollisionEnter(ITextureBase* obj)
{
	//他のオブジェクトと当たっていないときに呼ばれる関数
	CollisionBase::NonCollisionEnter(obj);
}

//プレイヤー関連の処理
void CoroKun::PlayerState()
{
	//SoundManagerを取得する
	SoundManager* soundManager = SoundManager::GetInstance();

	//タイマーを取得する
	TimerJudge* timer = GameContext::Get<TimerJudge>();

	//Switch文で処理を分ける
	switch (this->GetNowData2())
	{
	case PLAYERMOVE::INPUT:

		//HPが0になったら、ステートを切り替える
		if (this->GetHP() <= NULL)
		{
			if (!m_DamageState->GetExecute())
			{
				this->AddData2(PLAYERMOVE::SWEATANIME);
				this->SetVelocity(Vector3::Zero);
			}
			m_WalkState->SetExecute(false);
			m_Normal->GetNormalGage()->SetState(STATE::NONE);
			m_Normal->GetBackGage()->SetState(STATE::NONE);
			return;
		}

		//制限時間が尽きたら、ステートを切り替える
		if (timer->GetTimerJudge())
		{
			TimeLag* finish = GameContext::Get<PlayScene>()->GetLagFinish().get();
			finish->SetTarget("1");
			this->Finish(PLAYERMOVE::DEAD,CRI_CUESHEET_0__30_TIMEUP);
			this->SetVelocity(Vector3::Zero);
			return;
		}

		//ステートの更新処理
		this->UpdateState();

		//背面のフェードの更新処理
		this->UpdateBackFade();

		//通常弾の更新関数を実行
		m_Normal->Update();

		//針の更新関数を実行
		m_NeedleOverAll->Update();
		m_Resurrection->Update();

		//技の更新処理
		this->UpdateTechnique();

		//アニメーションが実行中の時
		if (this->GetAniFlag())
		{
			//針を放出するタイミングで、放出する
			if (this->GetNowInfo() == "通常後1" && m_NeedleOverAll->GetNowState() == NEEDLEACTION::MOPE)
			{
				m_NeedleOverAll->SetNowState(NEEDLEACTION::RELEASE);
				m_WalkState->SetExecute(true);

				//SEを鳴らす
				soundManager->Play(CRI_CUESHEET_0__18_針放出);
			}

			//終了時に元に戻す
			if (this->GetNowInfo() == "通常後2")
			{
				m_WalkState->SetExecute(true);
			}
		}

		break;
	case PLAYERMOVE::SWEATANIME:
		this->SweatAnime();
		break;
	case PLAYERMOVE::ROTSCALE:
		this->RotScale();
		break; 
	case PLAYERMOVE::ANIMESTART:
		this->DeadAnimeStart();
		break;
	case PLAYERMOVE::ANIMEFINISH:
		this->DeadAnimeResult();
		break;
	}
}

void CoroKun::Move()
{
	//静的データの取得
	StaticData* staticData = StaticData::GetInstance();

	//ベクトルの初期化
	Vector3 vel = Vector3::Zero;

	//実行中のステートからベクトルを取得する
	for (MoveState* state : m_StateList)
	{
		vel += state->Update();
	}

	//ベクトルを設定する
	this->SetVelocity(vel);

	//イージング関数の更新処理
	this->UpdateEasing();

	//角度を更新する
	this->InputAngle();
}

//HPが0になった時点でのアニメーション
void CoroKun::SweatAnime()
{
	//アニメーション情報を変更
	this->SetNowInfo("死亡1");

	//角度を調節
	if (this->DirectX() == 1)this->SetAngle(15.0f);
	else this->SetAngle(330.0f);

	//汗のテクスチャを表示
	m_sweatTexture->SetState(STATE::DRAW);

	//点滅していないとき
	if (!this->GetBlink())
	{
		//ステートを切り替える
		this->AddData2(PLAYERMOVE::ROTSCALE);

		//汗のテクスチャを非表示
		m_sweatTexture->SetState(STATE::NONE);
	}
}

//回転と縮小の処理
void CoroKun::RotScale()
{
	//1フレームずつ加算する
	float angle = this->GetAngle();
	angle += 1.0f;
	this->SetAngle(angle);

	//少しずつ、縮小する
	Vector3 scale = this->GetScale();
	scale -= Vector3(0.01f);
	this->SetScale(scale);

	//大きさが10分の1になったら
	if (scale.x <= 0.1f)
	{
		//倍率を元に戻す
		this->SetScale(Vector3(1.0f));

		//角度を元に戻す
		this->SetAngle(0.0f);

		//アニメーション情報を変更
		this->SetNowInfo("死亡2");
		this->SetLoopFlag("死亡10");

		//アニメーションを起動
		this->SetAniFlag(true);
		this->SetLoopFlag(false);

		//点滅開始
		this->SetBlink(true);

		//オプションを切る
		this->SetFinalOption(false);

		//ステートを切り替える
		this->AddData2(PLAYERMOVE::ANIMESTART);
	}
}

void CoroKun::DeadAnimeStart()
{
	//アニメーションが終了
	if (this->GetNowInfo() == "死亡10")
	{
		//ステートを切り替える
		this->AddData2(PLAYERMOVE::ANIMEFINISH);

		//描画を切る
		this->SetState(STATE::NONE);
	}
}

void CoroKun::DeadAnimeResult()
{
	//静的データを取得する
	StaticData* staticData = StaticData::GetInstance();

	//実行関数を落とす
	this->SetActive(false);

	//SceneManagerを取得する
	SceneManager* Manager = SceneManager::GetInstance();
	SceneState* state = Manager->GetState();
	
	//死んだら、遷移する
	state->SetFadeState(SceneState::FADEOUT);

	//フェードの加算速度を設定する
	state->SetFadeSpeed(SceneState::ALFA_SPEED);

	//SoundManagerを取得する
	SoundManager* soundManager = SoundManager::GetInstance();

	//SEを再生する
	soundManager->Play(CRI_CUESHEET_0__23_プレイフェードアウト);
}

void CoroKun::UpdateState()
{
	//SceneManagerを取得する
	SceneManager* Manager = SceneManager::GetInstance();

	//キーの状態を取得する
	Input* input = Input::GetInstance();

	//キーボードの状態を取得する
	const auto& keyState = input->GetKeybord_State();
	const auto& keyTrakker = input->GetKeybord_Trakker();

	//静的データの取得
	StaticData* staticData = StaticData::GetInstance();

	//SoundManagerを取得する
	SoundManager* soundManager = SoundManager::GetInstance();

	//ベクトルを初期化する
	Vector3 vel = Vector3::Zero;

	//カットインの演出後、技を実行する
	if (staticData->Get_StringData("カットイン") == "体当たり")
	{
		//実行関数をtrueにする
		m_BodyBlow->SetActive(true);
		//WalkStateを止める
		m_WalkState->SetExecute(false);
		//AccelStateを初期化する
		m_AccelState->InitState();
		//実行関数をtrueにする
		m_AccelState->SetExecute(true);
		//向いているとき(向きによって、ベクトルを切り替える)
		vel = Vector3(SPEED * this->DirectX(), 0.0f, 0.0f);
		//ベクトルを設定する
		m_AccelState->SetAccel(vel);
		//カットインの情報をデフォルトに戻す
		staticData->Add("カットイン", (std::string)"デフォルト");
		//SEを再生する
		soundManager->Play(CRI_CUESHEET_0__26_体当たり);
	}

	//AccelStateを実行する
	if (keyState.Z && keyState.A)
	{
		//AccelStateが実行されていないとき
		if (!m_AccelState->GetExecute() && !this->ALLTechniqueExe() && !this->GetBlink())
		{
			//MPが50以上の時
			if (this->GetMP() >= BodyBlow::GAGE_COST)
			{
				//ゲージを消費する
				this->OnCommand(BodyBlow::GAGE_COST);
				//CutInSceneを追加生成する
				Manager->PushScene("CutIn");
				//カットインの情報を追加する
				staticData->Add("カットイン", (std::string)"体当たり");
			}
			//技が使えないとき
			else
			{
				if (keyTrakker.IsKeyPressed(Keyboard::Z) || keyTrakker.IsKeyPressed(Keyboard::A))
				{
					//SEを再生する
					soundManager->Play(CRI_CUESHEET_0__06_キャンセル音);
				}
			}
		}
	}
}

void CoroKun::UpdateTechnique()
{
	//SceneManagerを取得する
	SceneManager* Manager = SceneManager::GetInstance();

	//静的データを取得する
	StaticData* staticData = StaticData::GetInstance();

	//キーの状態を取得する
	Input* input = Input::GetInstance();

	//キーボードの状態を取得する
	const auto& keyState = input->GetKeybord_State();
	const auto& keyTrakker = input->GetKeybord_Trakker();

	//SoundManagerを取得する
	SoundManager* soundManager = SoundManager::GetInstance();

	//カットインの演出後、技を実行する
	if (staticData->Get_StringData("カットイン") == "ビーム")
	{
		m_VirusBeam->SetPosition(this->GetPosition() + Vector3(this->AskOrigin().x * this->DirectX(), -this->AskOrigin().y / 2, 0.0f));
		//ビームの座標を設定する
		m_VirusBeam->SetParticlesPos(this->GetPosition() + Vector3(this->AskOrigin().x * 2, 0.0f, 0.0f) * this->DirectX());
		//SpriteEffectを設定する
		m_VirusBeam->SetSpriteEffect(this->GetSpriteEffect());
		//移動を止める
		m_WalkState->SetExecute(false);
		//実行関数をtrueにする
		m_VirusBeam->SetActive(true);
		//全てのパーティクルの描画をオンにする
		for (Particle*& particle : m_VirusBeam->GetAllParticleList())
		{
			//パーティクルが描画されていないとき
			if (particle->GetState() == STATE::NONE)
			{
				//描画をオンにする
				particle->SetState(STATE::DRAW);
			}
		}
		//ステートを切り替える
		m_VirusBeam->SetNowState(ENERGY_CHAGE::TRACKING);
		//カットインの情報をデフォルトに戻す
		staticData->Add("カットイン", (std::string)"デフォルト");
		//SEを鳴らす
		soundManager->Play(CRI_CUESHEET_0__14_チャージ);
	}

	//カットインの演出後、技を実行する
	if (staticData->Get_StringData("カットイン") == "オーバーオール")
	{
		//アニメーションのフラグを立てる
		this->SetAniFlag(true);
		//アニメーションの現在の情報を設定する
		this->SetNowInfo("通常1");
		//アニメーションのループ情報を設定する
		this->SetLoopInfo("通常後1");
		//ステートを変更する
		m_NeedleOverAll->SetNowState(NEEDLEACTION::MOPE);
		m_WalkState->SetExecute(false);
		//更新関数をtrueにする
		m_NeedleOverAll->SetActive(true);
		//カットインの情報をデフォルトに戻す
		staticData->Add("カットイン", (std::string)"デフォルト");
		//SEを鳴らす
		soundManager->Play(CRI_CUESHEET_0__17_針用意);
	}

	//カットインの演出後、技を実行する
	if (staticData->Get_StringData("カットイン") == "リザレクション")
	{
		//アニメーションのフラグを立てる
		this->SetAniFlag(true);
		//アニメーションの現在の情報を設定する
		this->SetNowInfo("通常2");
		//アニメーションのループ情報を設定する
		this->SetLoopInfo("通常後2");
		//ステートを変更する
		m_Resurrection->SetNowState(RENEEDLE::LITTLE);
		m_WalkState->SetExecute(false);
		//更新関数をtrueにする
		m_Resurrection->SetActive(true);
		//カットインの情報をデフォルトに戻す
		staticData->Add("カットイン", (std::string)"デフォルト");
		//SEを鳴らす
		soundManager->Play(CRI_CUESHEET_0__17_針用意);
	}

	//キーを押して、ビームを出す
	if (keyState.Z && keyState.C)
	{
		//ビームのチャージステートがNONEのときにだけ、処理を通す
		if (m_VirusBeam->GetNowState() == ENERGY_CHAGE::ENERGYNONE && !this->ALLTechniqueExe() && !this->GetBlink())
		{
			//プレイヤーが右か、左を向いているとき
			if (this->DirectX() != 0)
			{
				//MPが150以上の時
				if (this->GetMP() >= VirusBeam::GAGE_COST)
				{
					//ゲージを消費する
					this->OnCommand(VirusBeam::GAGE_COST);
					//CutInSceneを追加生成する
					Manager->PushScene("CutIn");
					//カットインの情報を追加する
					staticData->Add("カットイン", (std::string)"ビーム");
				}
				//技が使えないとき
				else
				{
					if (keyTrakker.IsKeyPressed(Keyboard::Z) || keyTrakker.IsKeyPressed(Keyboard::C))
					{
						//SEを再生する
						soundManager->Play(CRI_CUESHEET_0__06_キャンセル音);
					}
				}
			}
		}
	}

	//キーを押して、Needleを全方位に出す
	if (keyState.Z && keyState.D)
	{
		//ダメージを受けていない　技を使っていない　アニメーションのフラグを落ちているとき
		if (!this->GetAniFlag() && !this->ALLTechniqueExe() && !this->GetBlink())
		{
			//MPが100以上の時
			if (this->GetMP() >= NeedleOverall::GAGE_COST)
			{
				//ゲージを消費する
				this->OnCommand(NeedleOverall::GAGE_COST);
				//CutInSceneを追加生成する
				Manager->PushScene("CutIn");
				//カットインの情報を追加する
				staticData->Add("カットイン", (std::string)"オーバーオール");
			}
			//技が使えないとき
			else
			{
				if (keyTrakker.IsKeyPressed(Keyboard::Z) || keyTrakker.IsKeyPressed(Keyboard::D))
				{
					//SEを再生する
					soundManager->Play(CRI_CUESHEET_0__06_キャンセル音);
				}
			}
		}
	}

	//キーを押して、Needleを周囲に出す
	if (keyState.Z && keyState.S)
	{
		//ダメージを受けていない　技を使っていない　アニメーションのフラグを落ちているとき
		if (!this->GetAniFlag() && !this->ALLTechniqueExe() && !this->GetBlink())
		{
			//MPが90以上の時
			if (this->GetMP() >= Resurrection::GAGE_COST)
			{
				//ゲージを消費する
				this->OnCommand(Resurrection::GAGE_COST);
				//CutInSceneを追加生成する
				Manager->PushScene("CutIn");
				//カットインの情報を追加する
				staticData->Add("カットイン", (std::string)"リザレクション");
			}
			//技が使えないとき
			else
			{
				if (keyTrakker.IsKeyPressed(Keyboard::Z) || keyTrakker.IsKeyPressed(Keyboard::S))
				{
					//SEを再生する
					soundManager->Play(CRI_CUESHEET_0__06_キャンセル音);
				}
			}
		}
	}

	//通常攻撃
	//エネルギーをチャージする
	//キーが押されているとき
	//左右どちらかを向いているときのみ実行
	if (this->DirectX() != 0)
	{
		if (keyState.Q && !this->ALLTechniqueExe() 
			&& !this->GetBlink())
		{
			if (keyTrakker.IsKeyPressed(Keyboard::Q))
			{
				if (m_Normal->GetCheckPush())
				{
					m_Normal->SetCheckPush(false);
				}

				m_Normal->SetCheckRelease(false);

				//先頭の弾を設定する
				m_Normal->SetFrontBullet(m_Normal->GetAllBulletFront());

				if (m_Normal->GetFrontBullet() != nullptr)
				{
					//先頭の弾を描画する
					m_Normal->GetFrontBullet()->SetState(STATE::DRAW);

					//先頭の弾の座標を設定する
					m_Normal->GetFrontBullet()->SetPosition(this->GetPosition() + Vector3(100.0f, 0.0f, 0.0f) * this->DirectX());

					//先頭の弾の拡大倍率を設定する
					m_Normal->GetFrontBullet()->SetScale(Vector3(0.2f));

					//SEを鳴らす
					soundManager->Play(CRI_CUESHEET_0__14_チャージ);
				}
			}

			if (m_Normal->GetFrontBullet() != nullptr && !m_Normal->GetCheckPush())
			{
				//NormalTechniqueからNomalGageを取得する
				NormalGage* nGage = m_Normal->GetNormalGage();

				//エネルギーチャージを行う
				nGage->ChangeGage();

				//先頭の弾の座標を設定する
				m_Normal->GetFrontBullet()->SetPosition(this->GetPosition() + Vector3(100.0f, 0.0f, 0.0f) * this->DirectX());
			}

		}

		//キーが離されたとき
		if (keyTrakker.IsKeyReleased(Keyboard::Q))
		{
			if (m_Normal->GetFrontBullet() != nullptr &&
				!this->GetBlink())
			{
				//NormalTechniqueからNomalGageを取得する
				NormalGage* nGage = m_Normal->GetNormalGage();

				//ゲージの初期化を行う
				nGage->Initialize();

				//先頭の弾のベクトルを設定する
				m_Normal->GetFrontBullet()->SetVelocity(Vector3(7.5f, 0.0f, 0.0f) * this->DirectX());

				//先頭の弾のステートを変更する
				m_Normal->GetFrontBullet()->SetNowState(ENERGY_CHAGE::ENERGYATACK);

				//PlayerTargetを設定する
				m_Normal->GetFrontBullet()->SetPlayerTarget(this->GetPosition() + Vector3(700.0f, 0.0f, 0.0f) * this->DirectX());

				if (m_Normal->GetCheckRelease())
				{
					//SEを止める
					soundManager->Stop(CRI_CUESHEET_0__14_チャージ);
				}
				else 
				{
					//SEを止める
					soundManager->Stop(CRI_CUESHEET_0__14_チャージ);

					//SEを鳴らす
					soundManager->Play(CRI_CUESHEET_0__16_通常弾);
				}
			}
		}
	}

	//必殺技の演出が終わったら、ダメージを与える(ダメージが発生するのは、ダークエクスプロージョンのときのみ)
	if (staticData->Get_StringData("必殺技") == SelectScene::DEATHBLOW_NAME_EXPROSION)
	{
		//HyperEnemyにダメージを与える
		this->OnExeDeathBlowHyperEnemy();

		//SuperEnemeyにダメージを与える
		this->OnExeDeathBlowSuperEnemy();

		//NormalEnemyにダメージを与える
		this->OnExeDeathBlowNormalEnemy();

		//必殺技情報をデフォルトに戻す
		staticData->Add("必殺技", (std::string)"デフォルト");
	}

	//必殺技の演出が終わったら、10秒間だけ、オーラを出す
	if (staticData->Get_StringData("必殺技") == SelectScene::DEATHBLOW_NAME_ABILITY)
	{
		//タイマーのフラグを起動する
		m_timerUp = true;

		//背面のテクスチャを描画する
		m_backTexture->SetState(STATE::DRAW);

		//ステータスを設定する(攻撃力と防御力を入れ替える)
		int atack = this->GetAtack();
		int defence = this->GetDefence();
		std::swap(atack, defence);
		this->SetAtack(atack);
		this->SetDefence(defence);

		//ステータスの最大値を設定する(攻撃力と防御力を入れ替える)
		int maxAtack = this->GetMaxAtack();
		int maxDefence = this->GetMaxDefence();
		std::swap(maxAtack, maxDefence);
		this->SetMaxAtack(maxAtack);
		this->SetMaxDefence(maxDefence);

		//必殺技情報をデフォルトに戻す
		staticData->Add("必殺技", (std::string)"デフォルト");
	}

	//必殺技(別のSceneで演出を行う)
	//左右どちらかを向いているときのみ実行
	if (this->DirectX() != 0 && !this->GetBlink())
	{
		//キーが押されているとき
		if (keyState.Z && keyState.V)
		{
			if (this->GetSpecial() >= SelectScene::GAGE_COST_REVASEAVIRITY)
			{
				//SEを再生する
				soundManager->Play(CRI_CUESHEET_0__39_メニューを開く);

				//SelectSceneを追加生成する
				Manager->PushScene("Select");
			}
			//技が使えないとき
			else
			{
				//SEを再生する
				soundManager->Play(CRI_CUESHEET_0__06_キャンセル音);
			}
		}
	}

	//体当たりの座標を更新する
	m_BodyBlow->SetPosition(this->GetPosition() + Vector3(this->AskOrigin().x * 2, 0.0f, 0.0f) * this->DirectX());
	m_VirusBeam->SetPosition(this->GetPosition() + Vector3(this->AskOrigin().x * this->DirectX(), -this->AskOrigin().y / 2, 0.0f));
}

void CoroKun::UpdateEasing()
{
	//Easing関数の更新処理

	//静的データの取得
	StaticData* staticData = StaticData::GetInstance();

	//前回のフレームからの経過時間を取得する
	float elapsedTime = staticData->Get_FloatData("elapsedTime");

	//現在のベクトルを取得する
	Vector3 vel = this->GetVelocity();

	//プレイヤーのHPが0より大きい時
	if (this->GetHP() > NULL)
	{
		//サインカーブで計算する
		vel.y += sinf(m_easingTime) * 0.1f;

		//経過時間の更新
		m_easingTime += elapsedTime * 4.0f;
	}

	//浮遊後の座標を更新する
	m_warpPos += vel;
}

void CoroKun::UpdateBackFade()
{
	//フェードの更新処理

	//静的データの取得
	StaticData* staticData = StaticData::GetInstance();

	//SoundManagerを取得する
	SoundManager* soundManager = SoundManager::GetInstance();

	//elapsedTimeの取得
	float elapsedTime = staticData->Get_FloatData("elapsedTime");

	//ベクトルを設定する
	m_backTexture->SetVelocity(this->GetVelocity());

	//タイマーのフラグが起動しているとき
	if (m_timerUp)
	{
		//背景の色情報を取得する
		Color color = m_backTexture->GetColor();
		float alfa = color.A();

		//ステートによって、処理を切り替える
		switch (this->GetNowData1())
		{
		case FADE_STATE::DOWN:
			if (color.A() <= 0.5f)this->AddData1(FADE_STATE::FUP);
			else alfa -= 0.01f;
			break;
		case FADE_STATE::FUP:
			if (color.A() >= 1.0f)this->AddData1(FADE_STATE::DOWN);
			else alfa += 0.01f;
			break;
		}

		//色情報を再設定する
		color.A(alfa);
		m_backTexture->SetColor(color);

		//10秒経過
		if (m_timer > DEATHBLOW_ABILITY_SUSTAIN_TIME)
		{
			//点滅していない
			if (!m_backTexture->GetBlink())
			{
				//描画を切る
				m_backTexture->SetState(STATE::NONE);

				//経過時間を初期化する
				m_timer = 0.0f;

				//タイマーのフラグを切る
				m_timerUp = false;

				//ステータスを設定する(攻撃力と防御力を入れ替える)
				int atack = this->GetAtack();
				int defence = this->GetDefence();
				std::swap(atack, defence);
				this->SetAtack(atack);
				this->SetDefence(defence);

				//ステータスの最大値を設定する(攻撃力と防御力を入れ替える)
				int maxAtack = this->GetMaxAtack();
				int maxDefence = this->GetMaxDefence();
				std::swap(maxAtack, maxDefence);
				this->SetMaxAtack(maxAtack);
				this->SetMaxDefence(maxDefence);

				//アルファ値を元に戻す
				m_backTexture->SetColor(Colors::SkyBlue);

				//ステートを切り替える
				this->AddData1(FADE_STATE::NONE);

				//経過時間の更新は行わない
				return;
			}
		}
		//7秒経過
		else if (m_timer > BACKTEXTURE_BLINK_START)
		{
			//点滅していない
			if (!m_backTexture->GetBlink())
			{
				//点滅させる
				m_backTexture->SetBlink(true);
			}

			//点滅するとき
			if (m_backTexture->GetTargetCount() <= 0.0f)
			{
				soundManager->Play(CRI_CUESHEET_0__40_点滅);
			}
		}

		//経過時間を更新する
		m_timer += elapsedTime;
	}
}

void CoroKun::UpdateHpGage()
{
	//静的データを取得する
	StaticData* staticData = StaticData::GetInstance();

	//前回のフレームからの経過時間を取得する
	float elapsedTime = staticData->Get_FloatData("elapsedTime");

	//ダメージを受けているとき
	if (m_damageCheck)
	{
		//背面のテクスチャを取得する
		ITextureBase* backTexture = m_HPGage->GetBackGageTexture();

		//HPゲージの裏面の切り出し位置を取得する
		RECT back = m_HPGage->GetBackGageTexture()->GetRect();

		//HPゲージの切り出し位置を取得する
		RECT front = m_HPGage->GetRect();

		//ダメージの差分を求める
		int diffrent = (int)(back.right - front.right);

		//現在の時間を取得する
		float time = m_HPGage->GetEasingTime();

		//時間を更新する
		time += elapsedTime;

		//ダメージをスカラ補正する
		diffrent *= time;

		//ゲージに動きをつける
		back.right -= diffrent;

		//切り出し位置を再設定する
		m_HPGage->GetBackGageTexture()->SetRect(back);

		//時間を再設定する
		m_HPGage->SetEasingTime(time);

		if (m_HPGage->GetEasingTime() >= 1.0f)
		{
			//1秒経過したら、フラグを切る
			m_damageCheck = false;

			//切り出し位置を設定する
			m_HPGage->GetBackGageTexture()->SetRect(m_HPGage->GetRect());
		}
	}
}

void CoroKun::UpdateCommandEffect()
{
	//体当たりが使用可能な時
	this->PlayCommandEffect(m_fadeBodyBlow, BodyBlow::GAGE_COST);

	//ビームが使用可能な時
	this->PlayCommandEffect(m_fadeBeam, VirusBeam::GAGE_COST);

	//ニードルインクジェクションが使用可能な時
	this->PlayCommandEffect(m_fadeNeedleOverAll, NeedleOverall::GAGE_COST);

	//リザレクションが使用可能な時
	this->PlayCommandEffect(m_fadeResurection, Resurrection::GAGE_COST);

	//必殺技が使用可能な時
	this->PlaySpecialEffect(m_fadeSpecial, SelectScene::GAGE_COST_REVASEAVIRITY,Colors::Red);
}

void CoroKun::PlayCommandEffect(ITextureBase* command, const int& cost)
{
	//コマンドが使用可能な時
	if (this->GetMP() >= cost)
	{
		//色を変更する
		command->SetColor(Colors::LightGreen);
	}
	//コマンドが使用不可能な時
	else
	{
		//色を変更する
		command->SetColor(Colors::Gray);
	}
}

void CoroKun::PlaySpecialEffect(FadeTexture* special, const int& cost, const XMVECTORF32 color)
{
	//必殺技が使用可能な時
	if (this->GetSpecial() >= cost)
	{
		//アニメーションが起動していないとき
		if (special->GetNowState() == FADE_STATE::NONE)
		{
			//アニメーションを起動する
			special->SetNowState(FADE_STATE::DOWN);
		}
	}
	//必殺技が使用不可な時
	else
	{
		//色を固定する
		special->SetColor(color);

		//アニメーションを起動しない
		special->SetNowState(FADE_STATE::NONE);
	}
}

void CoroKun::OnCollisionFloor(ITextureBase* floor)
{
	//画像を押し出す
	TextureBase::Extrude(this);

	//浮遊後の座標も合わせる
	m_warpPos += -this->GetVelocity();

	//体当たりの当たり判定
	m_BodyBlow->WithPlayer(this);

	//ウイルスビームの当たり判定
	m_VirusBeam->WithPlayer(this);

	//背面のテクスチャを押し出す
	m_backTexture->Extrude(this);

	//他のオブジェクトに当たったら、WalkStateをtrueにして、AccelStateを落とす
	if (!m_VirusBeam->GetActive())
	{
		m_WalkState->SetExecute(true);
		m_AccelState->SetExecute(false);
	}

	//体当たり中の時は、実行関数を落とす
	if (m_BodyBlow->GetActive())
	{
		m_BodyBlow->SetActive(false);
	}
}

void CoroKun::OnCollisionWall(ITextureBase* wall)
{
	bool flag = true;

	//画像を押し出す
	TextureBase::Extrude(this);

	//浮遊後の座標も合わせる
	m_warpPos += -this->GetVelocity();

	//体当たりの当たり判定
	m_BodyBlow->WithPlayer(this);

	//ウイルスビームの当たり判定
	m_VirusBeam->WithPlayer(this);

	//背面のテクスチャを押し出す
	m_backTexture->Extrude(this);

	//他のオブジェクトに当たったら、WalkStateをtrueにして、AccelStateを落とす
	if (!m_VirusBeam->GetActive())
	{
		m_WalkState->SetExecute(true);
		m_AccelState->SetExecute(false);
	}

	//体当たり中の時は、実行関数を落とす
	if (m_BodyBlow->GetActive())
	{
		m_BodyBlow->SetActive(false);
	}

	//当たったオブジェクトを追加する
	CollisionBase::Add(wall);
}


//泡に当たったときの処理
void CoroKun::OnCollisionBabul(ITextureBase* babul)
{
	//静的データの取得
	StaticData* staticData = StaticData::GetInstance();

	//画像を押し出す
	TextureBase::Extrude(this);

	//浮遊後の座標も合わせる
	m_warpPos += -this->GetVelocity();

	//体当たりの当たり判定
	m_BodyBlow->WithPlayer(this);

	//ウイルスビームの当たり判定
	m_VirusBeam->WithPlayer(this);

	//背面のテクスチャを押し出す
	m_backTexture->Extrude(this);

	//他のオブジェクトに当たったら、WalkStateをtrueにして、AccelStateを落とす
	m_WalkState->SetExecute(true);
	m_AccelState->SetExecute(false);

	//体当たり中の時は、実行関数を落とす
	if (m_BodyBlow->GetActive())
	{
		m_BodyBlow->SetActive(false);
	}

	//WalkStateの実行関数が実行されていて、ダメージを受けていないとき
	if (!TextureBase::GetBlink())
	{
		//ダメージを受ける
		this->OnDamage(this->GetMaxHP() * 0.1f, NULL);

		//跳ね返って当たったとき
		if (m_DamageState->GetExecute())
		{
			//与えるダメージを設定する
			m_DamageState->SetDamage(m_DamageState->GetFirstDamage());
			//ベクトルを設定する
			this->SetVelocity(m_DamageState->GetFirstDamage());
			//ステートを初期化する
			m_DamageState->InitDamageState();
		}
		//自分で当たったとき
		else if(m_WalkState->GetExecute())
		{
			//与えるダメージを設定する
			m_DamageState->SetDamage(-this->GetVelocity());
		}
		//実行関数の設定
		m_DamageState->SetExecute(true);
		m_WalkState->SetExecute(false);
		//点滅の設定
		TextureBase::SetBlink(true);
		//ダメージ情報を設定する
		staticData->Add("プレイヤーダメージ", babul->GetName());
	}
}

//反転台に当たったときの処理
void CoroKun::OnCollisionRebound(ITextureBase* rebound)
{
	//静的データの取得
	StaticData* staticData = StaticData::GetInstance();

	//SoundManagerを取得する
	SoundManager* soundManager = SoundManager::GetInstance();

	//画像を押し出す
	TextureBase::Extrude(this);

	//浮遊後の座標も合わせる
	m_warpPos += -this->GetVelocity();

	//体当たりの当たり判定
	m_BodyBlow->WithPlayer(this);

	//ウイルスビームの当たり判定
	m_VirusBeam->WithPlayer(this);

	//背面のテクスチャを押し出す
	m_backTexture->Extrude(this);

	//体当たり中の時は、実行関数を落とす
	if (m_BodyBlow->GetActive())
	{
		m_BodyBlow->SetActive(false);
	}

	//他のオブジェクトに当たったら、WalkStateをtrueにして、AccelStateを落とす
	m_WalkState->SetExecute(true);
	m_AccelState->SetExecute(false);

	//自身のベクトルを取得する
	Vector3 vel = this->GetVelocity();
	vel.Normalize();

	//点滅していないとき
	if (!TextureBase::GetBlink())
	{
		//与えるダメージを設定する
		m_DamageState->SetDamage(Vector3(SPEED) * -vel);
		//実行関数の設定
		m_DamageState->SetExecute(true);
		m_WalkState->SetExecute(false);
		//ダメージ情報を設定する
		staticData->Add("プレイヤーダメージ", rebound->GetName());
		//SEを再生する
		soundManager->Play(CRI_CUESHEET_0__37_反転台);
	}
}

void CoroKun::OnCollisionSwitch(ITextureBase* switchi)
{
	//画像を押し出す
	TextureBase::Extrude(this);

	//浮遊後の座標も合わせる
	m_warpPos += -this->GetVelocity();

	//体当たりの当たり判定
	m_BodyBlow->WithPlayer(this);

	//ウイルスビームの当たり判定
	m_VirusBeam->WithPlayer(this);

	//背面のテクスチャを押し出す
	m_backTexture->Extrude(this);

	//体当たり中の時は、実行関数を落とす
	if (m_BodyBlow->GetActive())
	{
		m_BodyBlow->SetActive(false);
	}
}

void CoroKun::OnCollisionFoleFloor(ITextureBase* fole)
{
	//画像を押し出す
	TextureBase::Extrude(this);

	//浮遊後の座標も合わせる
	m_warpPos += -this->GetVelocity();

	//体当たりの当たり判定
	m_BodyBlow->WithPlayer(this);

	//ウイルスビームの当たり判定
	m_VirusBeam->WithPlayer(this);

	//背面のテクスチャを押し出す
	m_backTexture->Extrude(this);

	//他のオブジェクトに当たったら、WalkStateをtrueにして、AccelStateを落とす
	m_WalkState->SetExecute(true);
	m_AccelState->SetExecute(false);

	//体当たり中の時は、実行関数を落とす
	if (m_BodyBlow->GetActive())
	{
		m_BodyBlow->SetActive(false);
	}
}

void CoroKun::OnCollisionHyperEnemy(ITextureBase* hyene)
{	
	//静的データの取得
	StaticData* staticData = StaticData::GetInstance();

	//敵のインスタンスを取得する
	HyperEnemy* hyperenemy = dynamic_cast<HyperEnemy*>(hyene);

	//敵に当たったとき
	if(!this->GetBlink() && !hyperenemy->GetBlink())
	{
		//ビームを実行中
		if (m_VirusBeam->GetActive())
		{
			//ステートを切り替える
			m_VirusBeam->SetNowState(ENERGY_CHAGE::ENERGYNONE);

			//実行関数を落とす
			m_VirusBeam->SetActive(false);

			//ビームの初期化
			m_VirusBeam->InitBeam();
			m_VirusBeam->GetMainParticle()->Initialize();

			//ビームの描画を切る
			for (Particle* particle : m_VirusBeam->GetAllParticleList())
			{
				particle->SetState(STATE::NONE);
			}
		}

		//技を使っていないとき
		if (!m_BodyBlow->GetActive())
		{
			//画像を押し出す
			TextureBase::Extrude(this);

			//浮遊後の座標も合わせる
			m_warpPos += -this->GetVelocity();

			//体当たりの当たり判定
			m_BodyBlow->WithPlayer(this);

			//ウイルスビームの当たり判定
			m_VirusBeam->WithPlayer(this);

			//背面のテクスチャを押し出す
			m_backTexture->Extrude(this);

			//ダメージベクトルを設定する
			Vector3 vel = -this->GetVelocity();

			//ベクトルがゼロベクトルの場合は、ベクトルを補正する
			if (vel.x == 0.0f)vel = Vector3(SPEED * -hyene->DirectX(), 0.0f, 0.0f);

			//ダメージを受ける
			m_DamageState->SetDamage(vel);

			//実行関数の設定
			m_DamageState->SetExecute(true);
			m_WalkState->SetExecute(false);

			//点滅させる
			this->SetBlink(true);

			//ダメージ情報を設定する
			staticData->Add("プレイヤーダメージ", hyene->GetName());

			//ダメージを受ける
			this->OnDamage(hyperenemy->GetAtack(), this->GetDefence());
		}
	}
}

void CoroKun::OnCollision_SuperEnemy(ITextureBase* super)
{
	//静的データの取得
	StaticData* staticData = StaticData::GetInstance();

	//敵のインスタンスを取得する
	SuperEnemy* superenemy = dynamic_cast<SuperEnemy*>(super);

	//敵に当たったとき
	if (!this->GetBlink() && !superenemy->GetBlink())
	{
		//ビームを実行中
		if (m_VirusBeam->GetActive())
		{
			//ステートを切り替える
			m_VirusBeam->SetNowState(ENERGY_CHAGE::ENERGYNONE);

			//実行関数を落とす
			m_VirusBeam->SetActive(false);

			//ビームの初期化
			m_VirusBeam->InitBeam();
			m_VirusBeam->GetMainParticle()->Initialize();

			//ビームの描画を切る
			for (Particle* particle : m_VirusBeam->GetAllParticleList())
			{
				particle->SetState(STATE::NONE);
			}
		}

		//技を使っていないとき
		if (!m_BodyBlow->GetActive())
		{
			//画像を押し出す
			TextureBase::Extrude(this);

			//浮遊後の座標も合わせる
			m_warpPos += -this->GetVelocity();

			//体当たりの当たり判定
			m_BodyBlow->WithPlayer(this);

			//ウイルスビームの当たり判定
			m_VirusBeam->WithPlayer(this);

			//背面のテクスチャを押し出す
			m_backTexture->Extrude(this);

			//ダメージベクトルを設定する
			Vector3 vel = -this->GetVelocity();

			//ベクトルがゼロベクトルの場合は、ベクトルを補正する
			if (vel.x == 0.0f)vel = Vector3(SPEED * -super->DirectX(), 0.0f, 0.0f);

			//ダメージを受ける
			m_DamageState->SetDamage(vel);

			//実行関数の設定
			m_DamageState->SetExecute(true);
			m_WalkState->SetExecute(false);

			//点滅させる
			this->SetBlink(true);

			//ダメージ情報を設定する
			staticData->Add("プレイヤーダメージ", super->GetName());

			//ダメージを受ける
			this->OnDamage(superenemy->GetAtack(), this->GetDefence());
		}
	}
}

void CoroKun::OnCollision_NormalEnemy(ITextureBase* normal)
{
	//静的データの取得
	StaticData* staticData = StaticData::GetInstance();

	//敵のインスタンスを取得する
	NormalEnemy* normalEnemy = dynamic_cast<NormalEnemy*>(normal);

	//敵に当たったとき
	if (!this->GetBlink() && !normalEnemy->GetBlink())
	{
		//ビームを実行中
		if (m_VirusBeam->GetActive())
		{
			//ステートを切り替える
			m_VirusBeam->SetNowState(ENERGY_CHAGE::ENERGYNONE);

			//実行関数を落とす
			m_VirusBeam->SetActive(false);

			//ビームの初期化
			m_VirusBeam->InitBeam();
			m_VirusBeam->GetMainParticle()->Initialize();

			//ビームの描画を切る
			for (Particle* particle : m_VirusBeam->GetAllParticleList())
			{
				particle->SetState(STATE::NONE);
			}
		}

		//技を使っていないとき
		if (!m_BodyBlow->GetActive())
		{
			//画像を押し出す
			TextureBase::Extrude(this);

			//浮遊後の座標も合わせる
			m_warpPos += -this->GetVelocity();

			//体当たりの当たり判定
			m_BodyBlow->WithPlayer(this);

			//ウイルスビームの当たり判定
			m_VirusBeam->WithPlayer(this);

			//背面のテクスチャを押し出す
			m_backTexture->Extrude(this);

			//ダメージベクトルを設定する
			Vector3 vel = -this->GetVelocity();

			//ベクトルがゼロベクトルの場合は、ベクトルを補正する
			if (vel.x == 0.0f)vel = Vector3(SPEED * -normalEnemy->DirectX(), 0.0f, 0.0f);

			//ダメージを受ける
			m_DamageState->SetDamage(vel);

			//実行関数の設定
			m_DamageState->SetExecute(true);
			m_WalkState->SetExecute(false);

			//点滅させる
			this->SetBlink(true);

			//ダメージ情報を設定する
			staticData->Add("プレイヤーダメージ", normalEnemy->GetName());

			//ダメージを受ける
			this->OnDamage(normalEnemy->GetAtack(), this->GetDefence());
		}
	}
}

void CoroKun::OnCollisionWhiteBack(ITextureBase* whiteback)
{
	bool flag = true;

	//画像を押し出す
	TextureBase::Extrude(this);

	//浮遊後の座標も合わせる
	m_warpPos += -this->GetVelocity();

	//体当たりの当たり判定
	m_BodyBlow->WithPlayer(this);

	//ウイルスビームの当たり判定
	m_VirusBeam->WithPlayer(this);

	//背面のテクスチャを押し出す
	m_backTexture->Extrude(this);

	//他のオブジェクトに当たったら、
	//WalkStateをtrueにして、AccelStateを落とす
	m_WalkState->SetExecute(true);
	m_AccelState->SetExecute(false);

	//体当たり中の時は、実行関数を落とす
	if (m_BodyBlow->GetActive())
	{
		m_BodyBlow->SetActive(false);
	}
}

void CoroKun::OnCollisionHyperBabble(ITextureBase* highbabble)
{
	//静的データの取得
	StaticData* staticData = StaticData::GetInstance();

	//敵のインスタンスを取得する
	SoapBabble* babble = dynamic_cast<SoapBabble*>(highbabble);
	HyperEnemy* enemy = dynamic_cast<HyperEnemy*>(babble->GetOwner());

	//移動中かビームを打っているとき(ダメージを受けていないとき)
	if (m_WalkState->GetExecute() && !this->GetBlink() ||
		m_VirusBeam->GetActive() && !this->GetBlink())
	{
		//画像を押し出す
		TextureBase::Extrude(this);

		//浮遊後の座標も合わせる
		m_warpPos += -this->GetVelocity();

		//体当たりの当たり判定
		m_BodyBlow->WithPlayer(this);

		//ウイルスビームの当たり判定
		m_VirusBeam->WithPlayer(this);

		//背面のテクスチャを押し出す
		m_backTexture->Extrude(this);

		//ダメージベクトルを設定する
		Vector3 vel = -this->GetVelocity();

		//敵のダメージベクトルを設定する
		Vector3 babblevel = highbabble->GetVelocity();

		//ベクトルを正規化する
		babblevel.Normalize();

		//ベクトルがゼロベクトルの場合は、ベクトルを補正する
		if (vel.x == 0.0f)vel = Vector3(SPEED * babblevel.x, 0.0f, 0.0f);

		//ダメージを受ける
		m_DamageState->SetDamage(vel);

		//点滅させる
		this->SetBlink(true);

		//実行関数の設定
		m_DamageState->SetExecute(true);
		m_WalkState->SetExecute(false);

		//ビームを実行中(チャージ中の時)
		if (m_VirusBeam->GetActive())
		{
			//ステートを切り替える
			m_VirusBeam->SetNowState(ENERGY_CHAGE::ENERGYNONE);

			//実行関数を落とす
			m_VirusBeam->SetActive(false);

			//ビームの初期化
			m_VirusBeam->InitBeam();
			m_VirusBeam->GetMainParticle()->Initialize();

			//ビームの描画を切る
			for (Particle* particle : m_VirusBeam->GetAllParticleList())
			{
				particle->SetState(STATE::NONE);
			}
		}

		//ダメージ情報を設定する
		staticData->Add("プレイヤーダメージ", highbabble->GetName());

		//ダメージを受ける
		this->OnDamage(enemy->GetAtack(), this->GetDefence());
	}
}

void CoroKun::OnCollision_SuperBabble(ITextureBase* superbabble)
{
	//静的データの取得
	StaticData* staticData = StaticData::GetInstance();

	//敵のインスタンスを取得する
	SoapBabble* babble = dynamic_cast<SoapBabble*>(superbabble);
	SuperEnemy* enemy = dynamic_cast<SuperEnemy*>(babble->GetOwner());

	//移動中かビームを打っているとき(ダメージを受けていないとき)
	if (m_WalkState->GetExecute() && !this->GetBlink() ||
		m_VirusBeam->GetActive() && !this->GetBlink())
	{
		//画像を押し出す
		TextureBase::Extrude(this);

		//浮遊後の座標も合わせる
		m_warpPos += -this->GetVelocity();

		//体当たりの当たり判定
		m_BodyBlow->WithPlayer(this);

		//ウイルスビームの当たり判定
		m_VirusBeam->WithPlayer(this);

		//背面のテクスチャを押し出す
		m_backTexture->Extrude(this);

		//ダメージベクトルを設定する
		Vector3 vel = -this->GetVelocity();

		//敵のダメージベクトルを設定する
		Vector3 babblevel = superbabble->GetVelocity();

		//ベクトルを正規化する
		babblevel.Normalize();

		//ベクトルがゼロベクトルの場合は、ベクトルを補正する
		if (vel.x == 0.0f)vel = Vector3(SPEED * babblevel.x, 0.0f, 0.0f);

		//ダメージを受ける
		m_DamageState->SetDamage(vel);

		//点滅させる
		this->SetBlink(true);

		//実行関数の設定
		m_DamageState->SetExecute(true);
		m_WalkState->SetExecute(false);

		//ビームを実行中(チャージ中の時)
		if (m_VirusBeam->GetActive())
		{
			//ステートを切り替える
			m_VirusBeam->SetNowState(ENERGY_CHAGE::ENERGYNONE);

			//実行関数を落とす
			m_VirusBeam->SetActive(false);

			//ビームの初期化
			m_VirusBeam->InitBeam();
			m_VirusBeam->GetMainParticle()->Initialize();

			//ビームの描画を切る
			for (Particle* particle : m_VirusBeam->GetAllParticleList())
			{
				particle->SetState(STATE::NONE);
			}
		}

		//ダメージ情報を設定する
		staticData->Add("プレイヤーダメージ", superbabble->GetName());

		//補正値
		float correction = 1.0f;

		//当たったのが親の泡の時はダメージが2倍になる
		if (babble == enemy->GetTurretList()[enemy->GetNowData2()]->GetParentBabble())
		{
			correction = 2.0f;
		}

		//ダメージを受ける
		this->OnDamage(enemy->GetAtack(), this->GetDefence(), correction);
	}
}

void CoroKun::OnCollision_Goal(ITextureBase* goal)
{
	//終了の処理
	this->Finish(PLAYERMOVE::CLEAR,CRI_CUESHEET_0__29_GOAL);
}


void CoroKun::InputAngle()
{
	//キーの状態を取得する
	Input* input = Input::GetInstance();

	//キーボードの状態を取得する
	const auto& keyState = input->GetKeybord_State();
	const auto& keyTrakker = input->GetKeybord_Trakker();

	//体当たり中は何もしない
	if (m_AccelState->GetExecute())return;

	//移動していないときは、何もしない
	if (!m_WalkState->GetExecute())return;

	//右方向を向く
	if (keyState.Right && !this->ALLTechniqueExe())
	{
		this->SetAngle(0.0f);
		this->SetSpriteEffect(SpriteEffects::SpriteEffects_None);
		return;
	}

	//左を向く
	if (keyState.Left && !this->ALLTechniqueExe())
	{
		this->SetAngle(0.0f);
		this->SetSpriteEffect(SpriteEffects::SpriteEffects_FlipHorizontally);
		return;
	}
}

void CoroKun::CreateTechnique()
{
	//ResourceManagerを取得する
	ResourceManager* resourceManager = ResourceManager::GetInstance();

	//TextureManagerを取得する
	TextureManager* texMana = GameContext::Get<PlayScene>()->GetGameManager().get();

	//CollisionManagerを取得する
	CollisionManager* colMana = GameContext::Get<CollisionManager>();

	//使用するテクスチャを取得する
	ITextureBase* blow = resourceManager->GetTexture("衝突1");
	ITextureBase* beam1 = resourceManager->GetTexture("ビーム1");
	ITextureBase* player1 = resourceManager->GetTexture("SickChar");
	ITextureBase* player2 = resourceManager->GetTexture("SickChar2"); 
	ITextureBase* circle = resourceManager->GetTexture("粒子");

	//技の設定をする

	//衝突エフェクトを生成する
	std::unique_ptr<BodyBlow>pBody = std::make_unique<BodyBlow>(blow);
	m_BodyBlow = pBody.get();
	m_BodyBlow->SetName("衝突");

	//アニメーションの設定
	for (const std::unique_ptr<AnimationImporter>& importer : Animation::GetAnimeList("体当たり"))
	{
		//アニメーションの使用するテクスチャを取得する
		ITextureBase* useTexture = resourceManager->GetTexture(importer->GetRefference());

		//アニメーション情報を設定する
		m_BodyBlow->SetAnimation(importer->GetAnimation(), useTexture->GetRect());

		//次のアニメーションの情報を設定する
		m_BodyBlow->SetNextAnimation(importer->GetAnimation(), importer->GetNext());

		//アニメーションの終了時間を設定する
		m_BodyBlow->SetEndTime(importer->GetAnimation(), importer->GetEnd());
	}

	//アニメーションの初期化
	m_BodyBlow->SetAniFlag(true);
	m_BodyBlow->SetLoopFlag(true);

	m_BodyBlow->SetNowInfo("衝突1");
	m_BodyBlow->SetLoopInfo("衝突2");
	m_BodyBlow->SetNowAnimation(m_BodyBlow->GetRect());

	//GameContextに追加する
	GameContext::Register<BodyBlow>(m_BodyBlow);

	//TextureManagerに追加する
	GameContext::Get<PlayScene>()->GetGameManager()->Add(std::move(pBody));

	//ビームエフェクトを生成する
	std::unique_ptr<VirusBeam> pVirus = std::make_unique<VirusBeam>(beam1);
	m_VirusBeam = pVirus.get();
	m_VirusBeam->SetName("ウイルスビーム");
	m_VirusBeam->SetDrawTimeLimit(180);
	m_VirusBeam->SetBeamSpeed(20);
	m_VirusBeam->SetDrawLimit(700);
	m_VirusBeam->SetColor(Color(1.0f, 1.0f, 1.0f, 0.5f));

	//CollisionManagerに追加する
	GameContext::Get<CollisionManager>()->Add(m_VirusBeam);

	GameContext::Get<PlayScene>()->GetGameManager()->Add(std::move(pVirus));

	//Needleの長距離範囲攻撃を生成する
	m_NeedleOverAll = std::make_unique<NeedleOverall>();

	//Needleの短距離範囲攻撃を生成する
	m_Resurrection = std::make_unique<Resurrection>();

	//アニメーションの初期化をする

//--------------------------広範囲のアニメーションの設定--------------------------//

	//アニメーションの設定
	for (const std::unique_ptr<AnimationImporter>& importer : Animation::GetAnimeList("Needle1"))
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

//--------------------------広範囲のアニメーションの設定----------------------//

//--------------------------狭範囲用のアニメーションの設定----------------------//

	//アニメーションの設定
	for (const std::unique_ptr<AnimationImporter>& importer : Animation::GetAnimeList("Needle2"))
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

//--------------------------狭範囲用のアニメーションの設定----------------------//

//--------------------------死亡用のアニメーションの設定----------------------//

		//アニメーションの設定
	for (const std::unique_ptr<AnimationImporter>& importer : Animation::GetAnimeList("Dead"))
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

//--------------------------死亡用のアニメーションの設定----------------------//
	
	//プレイヤーの現在のアニメーションの設定
	this->SetAniFlag(false);
	this->SetLoopFlag(false);

	this->SetNowInfo("通常1");
	this->SetLoopInfo("通常後1");
	this->SetNowAnimation(player1->GetRect());

	//通常の技を生成する
	m_Normal = std::make_unique<NormalTechnique>();
}

void CoroKun::OnDamage(const int& atack, const int& defence,const float correction)
{
	//HPを取得する
	int hp = this->GetHP();

	//rectを取得する
	RECT rect = m_HPGage->GetRect();

	//ダメージを計算する
	StatusCalc::CalcDamage(hp, atack, defence,
		m_HPGage->GetMaxTexLength(), this->GetMaxHP(), rect.right,correction);

	//HPを設定する
	this->SetHP(hp);

	//RECTを設定する
	m_HPGage->SetRect(rect);

	//SoundManagerを取得する
	SoundManager* soundManager = SoundManager::GetInstance();

	//SEを鳴らす
	soundManager->Play(CRI_CUESHEET_0__11_ダメージ);

	//ダメージ用のフラグを立てる
	m_damageCheck = true;

	//HPゲージの裏面のテクスチャを取得する
	ITextureBase* backHpTexture = m_HPGage->GetBackGageTexture();

	//HPゲージの裏面の色を設定する
	backHpTexture->SetColor(Colors::Red);

	//ダメージのカウントを初期化する
	m_HPGage->SetEasingTime(0.0f);

	//通常攻撃を初期化する
	if (m_Normal->GetFrontBullet() != nullptr)
	{
		m_Normal->GetFrontBullet()->Initialize();
	}
	m_Normal->GetNormalGage()->Initialize();

	//SEを止める
	soundManager->Stop(CRI_CUESHEET_0__14_チャージ);
}

void CoroKun::OnCommand(const int& cost)
{
	//MPを取得する
	int mp = this->GetMP();

	//rectを取得する
	RECT rect = m_mpGage->GetRect();

	//ダメージを計算する
	StatusCalc::CalcDamage(mp, cost, NULL,
		m_mpGage->GetMaxTexLength(), this->GetMaxMP(), rect.right, 1.0f);

	//MPを設定する
	this->SetMP(mp);

	//RECTを設定する
	m_mpGage->SetRect(rect);
}

void CoroKun::OnSpesial(const int& cost)
{
	//SPを取得する
	int sp = this->GetSpecial();

	//rectを取得する
	RECT rect = m_specialGage->GetRect();

	//ダメージを計算する
	StatusCalc::CalcDamage(sp, cost, NULL,
		m_specialGage->GetMaxTexLength(), this->GetMaxSpecialPoint(), rect.right, 1.0f);

	//SPを設定する
	this->SetSpecial(sp);

	//RECTを設定する
	m_specialGage->SetRect(rect);
}

void CoroKun::OnHealCommand(int& heal)
{
	//MPを取得する
	int mp = this->GetMP();

	//rectを取得する
	RECT rect = m_mpGage->GetRect();

	//回復量を計算する
	StatusCalc::CalcHeal(mp, heal, m_mpGage->GetMaxTexLength(), this->GetMaxMP(), rect.right);

	//MPを設定する
	this->SetMP(mp);

	//RECTを設定する
	m_mpGage->SetRect(rect);
}

void CoroKun::OnHealSpecial(int& heal)
{
	//SpecialPointを取得する
	int sp = this->GetSpecial();

	//rectを取得する
	RECT rect = m_specialGage->GetRect();

	//回復量を計算する
	StatusCalc::CalcHeal(sp, heal, m_specialGage->GetMaxTexLength(), this->GetMaxSpecialPoint(), rect.right);

	//SPを設定する
	this->SetSpecial(sp);

	//RECTを設定する
	m_specialGage->SetRect(rect);
}

void CoroKun::OnHealHp(int& heal)
{
	//HPを取得する
	int hp = this->GetHP();

	//rectを取得する
	RECT rect = m_HPGage->GetRect();

	//回復量を計算する
	StatusCalc::CalcHeal(hp, heal, m_HPGage->GetMaxTexLength(), this->GetMaxHP(), rect.right);

	//HPを設定する
	this->SetHP(hp);

	//RECTを設定する
	m_HPGage->SetRect(rect);
}

void CoroKun::OnHealExp(int& heal)
{
	//取得経験値を取得する
	int next = this->GetNextLevelPoint();

	//rectを取得する
	RECT rect = m_expGage->GetRect();

	//回復量を計算する
	StatusCalc::CalcHeal(next, heal, m_expGage->GetMaxTexLength(), this->GetMaxNextLevelPoint(), rect.right);

	//HPを設定する
	this->SetNextLevelPoint(next);

	//RECTを設定する
	m_expGage->SetRect(rect);
}

void CoroKun::Finish(const PLAYERMOVE& state, const int& sound)
{
	//PlaySceneを取得する
	PlayScene* play = GameContext::Get<PlayScene>();

	//タイマーを取得する
	TimerJudge* timer = GameContext::Get<TimerJudge>();

	//ゴール演出を取得する
	TimeLag* finish = play->GetLagFinish().get();

	//SoundManagerを取得する
	SoundManager* soundManager = SoundManager::GetInstance();

	//体当たり停止
	m_WalkState->SetExecute(false);

	//ゲージを非表示にする
	m_Normal->GetNormalGage()->SetState(STATE::NONE);
	m_Normal->GetBackGage()->SetState(STATE::NONE);

	//処理を通すのは、一度だけ
	if (!finish->GetFrameFrag())
	{
		finish->SetFrameFlag(true);
		timer->SetTimerUp(false);
		play->SetNowState(BACKOUT::ADMISSON);
		this->AddData2(state);

		//SEを再生する
		soundManager->Play(sound);
	}
}

void CoroKun::OnExeDeathBlowHyperEnemy()
{
	//CameraManagerを取得する
	CameraManager* cameraManager = GameContext::Get<CameraManager>();

	//ステージを取得する
	Stage* stage = GameContext::Get<Stage>();

	//スコアを取得する
	Score* score = GameContext::Get<PlayScene>()->GetScore();

	//HyperEnemyを取得する
	std::vector<HyperEnemy*> hyperEnemies = stage->GetHyperEnemies();

	for (HyperEnemy* enemy : hyperEnemies)
	{
		//すでにやられているときは、カウントしない
		if (enemy->GetState() == STATE::NONE)continue;

		//画面内にいる敵にのみ、ダメージを与える
		if (cameraManager->ScreenCheck(enemy->GetPosition()))
		{
			//点滅させる
			enemy->SetBlink(true);

			//泡を消す
			for (SoapBabble* babble : enemy->GetBabbleList())
			{
				babble->SetState(STATE::NONE);
			}

			//敵にダメージを与える(必殺技は、倍のダメージを与える)
			enemy->OnDamage(this->GetAtack(), enemy->GetDefence(), 2.0f);

			//スコアを追加する
			score->AddScore(enemy->GetLevel() * DirecTingScene::BASE_SCORE);
			score->SetFrameSave(enemy->GetLevel() * DirecTingScene::BASE_SCORE, 60.0f);

			//エフェクトを再生する
			enemy->GetMasicEffectManager()->NowEffectPlay
			(DirecTingScene::TECHNIQUE_HEAL_COMMAND, DirecTingScene::TECHNIQUE_HEAL_SPECIAL);
		}
	}
}

void CoroKun::OnExeDeathBlowSuperEnemy()
{
	//CameraManagerを取得する
	CameraManager* cameraManager = GameContext::Get<CameraManager>();

	//ステージを取得する
	Stage* stage = GameContext::Get<Stage>();

	//スコアを取得する
	Score* score = GameContext::Get<PlayScene>()->GetScore();

	//SuperEnemyを取得する
	std::vector<SuperEnemy*> superEnemies = stage->GetSuperEnemies();

	for (SuperEnemy* enemy : superEnemies)
	{
		//すでにやられているときは、カウントしない
		if (enemy->GetState() == STATE::NONE)continue;

		//画面内にいる敵にのみ、ダメージを与える
		if (cameraManager->ScreenCheck(enemy->GetPosition()))
		{
			//点滅させる
			enemy->SetBlink(true);

			//泡を消す
			for (std::unique_ptr<FixedTurret>& turret : enemy->GetTurretList())
			{
				for (SoapBabble* babble : turret->GetBabbleList())
				{
					babble->SetState(STATE::NONE);
				}
			}

			//敵にダメージを与える(必殺技は、倍のダメージを与える)
			enemy->OnDamage(this->GetAtack(), enemy->GetDefence(), 2.0f);

			//スコアを追加する
			score->AddScore(enemy->GetLevel() * DirecTingScene::BASE_SCORE);
			score->SetFrameSave(enemy->GetLevel() * DirecTingScene::BASE_SCORE, 60.0f);

			//エフェクトを再生する
			enemy->GetMasicEffectManager()->NowEffectPlay
			(DirecTingScene::TECHNIQUE_HEAL_COMMAND, DirecTingScene::TECHNIQUE_HEAL_SPECIAL);
		}

	}
}

void CoroKun::OnExeDeathBlowNormalEnemy()
{
	//CameraManagerを取得する
	CameraManager* cameraManager = GameContext::Get<CameraManager>();

	//ステージを取得する
	Stage* stage = GameContext::Get<Stage>();

	//スコアを取得する
	Score* score = GameContext::Get<PlayScene>()->GetScore();

	//NormalEnemyを取得する
	std::vector<NormalEnemy*> normalEnemies = stage->GetNormalEnemies();

	for (NormalEnemy* enemy : normalEnemies)
	{
		//すでにやられているときは、カウントしない
		if (enemy->GetState() == STATE::NONE)continue;

		//画面内にいる敵にのみ、ダメージを与える
		if (cameraManager->ScreenCheck(enemy->GetPosition()))
		{
			//点滅させる
			enemy->SetBlink(true);

			//敵にダメージを与える(必殺技は、倍のダメージを与える)
			enemy->OnDamage(this->GetAtack(), enemy->GetDefence(), 2.0f);

			//スコアを追加する
			score->AddScore(enemy->GetLevel() * DirecTingScene::BASE_SCORE);
			score->SetFrameSave(enemy->GetLevel() * DirecTingScene::BASE_SCORE, 60.0f);

			//エフェクトを再生する
			enemy->GetMasicEffectManager()->NowEffectPlay
			(DirecTingScene::TECHNIQUE_HEAL_COMMAND, DirecTingScene::TECHNIQUE_HEAL_SPECIAL);
		}

	}
}

const bool CoroKun::ALLTechniqueExe()
{
	if (m_BodyBlow->GetActive())return true;

	if (m_VirusBeam->GetActive())return true;

	if (m_NeedleOverAll->GetActive())return true;

	if (m_Resurrection->GetActive())return true;

	return false;
}
