#include "pch.h"
#include "VirusBeam.h"
#include "GameContext.h"
#include "OtobeLib/FrameWork/ResourceManager/ResourceManager.h"
#include "OtobeLib/StackScene/Scene/Play/PlayScene.h"
#include "GameObject/Player/CoroKun.h"
#include "GameObject/Stage/Stage.h"
#include "GameObject/Enemy/Hyper/HyperEnemy.h"
#include "GameObject/Enemy/Super/SuperEnemy.h"
#include "GameObject/Enemy/Normal/NormalEnemy.h"
#include "OtobeLib/FrameWork/SoundManager/SoundManager.h"
#include "Game.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace OtobeLib;

VirusBeam::VirusBeam(ITextureBase* texture):TextureBase(texture),m_MainParticle(nullptr),m_DrawFinal(NULL),
m_DrawLimit(NULL),m_DrawTimeLimit(NULL),m_FrameCount(NULL),m_BeamSpeed(NULL)
{
	//描画を切る
	this->SetState(STATE::NONE);

	//描画順を変更する
	this->SetDrawOrder(2);

	//ResourceManagerを取得する
	ResourceManager* resourceManager = ResourceManager::GetInstance();

	//ゲーム用のTextureManagerを取得する
	TextureManager* GameManager = GameContext::Get<PlayScene>()->GetGameManager().get();

	//粒子のテクスチャを取得する
	ITextureBase* particle = resourceManager->GetTexture("粒子");

	//プレイヤーのポインタを取得する
	CoroKun* player = GameContext::Get<CoroKun>();

	//GameContextに登録する
	GameContext::Register<VirusBeam>(this);

	//中心のパーティクルを生成
	std::unique_ptr<MainParticle> pParticle = std::make_unique<MainParticle>(particle);
	pParticle->SetPosition(player->GetPosition() + Vector2(player->AskOrigin().x * 2));
	pParticle->SetScale(Vector3(0.2f));
	pParticle->SetName("中心の粒子");
	pParticle->SetOrigin(pParticle->AskOrigin());
	pParticle->SetState(STATE::NONE);
	pParticle->SetActive(false);
	pParticle->SetChageLimit(2);
	pParticle->SetScaleLimitCount(3);
	pParticle->SetScaleLimit(1.0f);
	//生ポインタを登録する
	m_MainParticle = pParticle.get();
	m_AllParticleList.push_back(m_MainParticle);

	//ゲーム用のTextureManagerに追加する
	GameManager->Add(std::move(pParticle));

	//角度を初期化する
	float angle = 0.0f;

	//周囲のパーティクルを作成する
	for (int i = 0; i < OTHER_PARTICLE_NUM; i++)
	{
		//指定した数だけ、パーティクルを生成する
		std::unique_ptr<TrackingParticle> pParticle2 = std::make_unique<TrackingParticle>(particle);

		//初期座標は中心のパーティクルの座標とする
		pParticle2->SetPosition(m_MainParticle->GetPosition());

		//ベクトルを初期化する
		Vector3 vel = Vector3(0.0f, -50.0f, 0.0f);

		//ベクトルを更新
		this->AskCircleVectol(vel, angle);

		//更新したベクトルを座標に加算する
		pParticle2->SetPosition(pParticle2->GetPosition() + vel);
		 
		//拡大倍率を変更する
		pParticle2->SetScale(Vector3(0.1f));

		//名前を変更する
		pParticle2->SetName("周りの粒子");

		//原点を変更する
		pParticle2->SetOrigin(pParticle2->AskOrigin());

		//描画を変更する
		pParticle2->SetState(STATE::NONE);

		//更新関数の設定をする
		pParticle2->SetActive(false);

		//リストに生ポインタを追加する
		m_OtherParticleList.push_back(pParticle2.get());
		m_AllParticleList.push_back(pParticle2.get());

		//ゲーム用のTextureManagerに追加する
		GameManager->Add(std::move(pParticle2));
	}

	//切り出し位置を再設定する
	this->SetRect(RECT{ 0,0,0,50 });
}

void VirusBeam::Initialize()
{
	//Stageを取得する
	Stage* stage = GameContext::Get<Stage>();

	//マップ情報を取得する
	std::vector<std::vector<ITextureBase*>>& mapAll = stage->GetMap();

	//全てのパーティクルの数分マップ情報を追加する
	for (Particle* particle : m_AllParticleList)
	{
		//マップ情報を追加する
		mapAll[mapAll.size() - 1].push_back(particle);
	}
}

void VirusBeam::Update()
{
	//UIの表示が終わるまで何もしない
	TimeLag* go = GameContext::Get<PlayScene>()->GetLagGo().get();
	if (!go->GetFinishFlag())return;

	//CameraManagerを取得する
	CameraManager* cameraManager = GameContext::Get<CameraManager>();

	//現在稼働しているカメラを取得する
	CameraBase* nowCamera = cameraManager->GetNowCamera();

	//カメラがスクロールカメラの時は、更新を止める
	if (!nowCamera->GetObjectMove())return;

	//SoundManagerを取得する
	SoundManager* soundManager = SoundManager::GetInstance();

	//プレイヤーのポインタを取得する
	CoroKun* player = GameContext::Get<CoroKun>();

	//強い敵のvectorを取得する
	std::vector<HyperEnemy*> hyenemies = GameContext::Get<Stage>()->GetHyperEnemies();

	//ちょっと強い敵のvectorを取得する
	std::vector<SuperEnemy*> superenemies = GameContext::Get<Stage>()->GetSuperEnemies();

	//普通の敵のvectorを取得する
	std::vector<NormalEnemy*> normalenemies = GameContext::Get<Stage>()->GetNormalEnemies();

	//ビーム攻撃中の処理
	if (this->GetNowState() == ENERGY_CHAGE::ENERGYATACK)
	{
		//描画する
		this->SetState(STATE::DRAW);

		for (HyperEnemy* hyene : hyenemies)
		{
			//敵のシャボン玉攻撃を取得する
			std::vector<SoapBabble*> highbabble = hyene->GetBabbleList();
			//敵のHPが0より大きいときに当たり判定を追加する
			if (hyene->GetHP() > NULL)
			{
				//当たり判定を追加する
				GameContext::Get<CollisionManager>()->AddUseOther(hyene, this, (float)m_DrawFinal / 2.0f + 100.0f);
				for (SoapBabble* babble : highbabble)
				{
					//当たり判定を追加する
					GameContext::Get<CollisionManager>()->AddUseOther(babble, this, (float)m_DrawFinal / 2.0f + 100.0f);
				}
			}
		}

		for (SuperEnemy* super : superenemies)
		{
			//固定砲台を取得する
			for (std::unique_ptr<FixedTurret>& turret : super->GetTurretList())
			{
				//敵のHPが0より大きいときに当たり判定を追加する
				if (super->GetHP() > NULL)
				{
					//当たり判定を追加する
					GameContext::Get<CollisionManager>()->AddUseOther(super, this, (float)m_DrawFinal / 2.0f + 100.0f);
					for (SoapBabble* babble : turret->GetBabbleList())
					{
						//当たり判定を追加する
						GameContext::Get<CollisionManager>()->AddUseOther(babble, this, (float)m_DrawFinal / 2.0f + 100.0f);
					}
				}
			}
		}

		for (NormalEnemy* normal : normalenemies)
		{
			//敵のHPが0より大きいときに当たり判定を追加する
			if (normal->GetHP() > NULL)
			{
				//当たり判定を追加する
				GameContext::Get<CollisionManager>()->AddUseOther(normal, this, (float)m_DrawFinal / 2.0f + 100.0f);
			}
		}

		//制限時間を超えたら、ビームを止める
		if (m_FrameCount > m_DrawTimeLimit)
		{
			//描画を止める
			this->SetState(STATE::NONE);

			//ビームを初期化する
			this->InitBeam();

			//ステートを切り替える
			this->SetNowState(ENERGY_CHAGE::ENERGYNONE);

			//移動停止しているときに、移動を再開させる
			if (!player->GetWalkState()->GetExecute())
			{
				player->GetWalkState()->SetExecute(true);
			}
			//更新関数を落とす
			this->SetActive(false);

			//SEを止める
			soundManager->Stop(CRI_CUESHEET_0__15_ビーム);
		}

		//現在の終点が限界値未満の時は終点を更新する
		if (m_DrawFinal < m_DrawLimit)
		{
			m_DrawFinal += m_BeamSpeed;
		}
		m_FrameCount++;

		//左向きにビームを打っているときは、画像の原点を変更する
		if (this->GetSpriteEffect() == SpriteEffects::SpriteEffects_FlipHorizontally)
		{
			//原点を設定する
			this->SetOrigin(Vector2(m_DrawFinal, 0.0f));
		}
		//右向きにビームを打っているときは、変更しない
		else
		{
			//原点を初期化する
			this->SetOrigin(Vector2::Zero);
		}

	}

	//画像の切り出し位置を更新する
	this->SetRect(RECT{ 0,0,m_DrawFinal,50 });

	//画像の原点を設定する
	this->SetOrigin(this->GetOrigin());

	//パーティクルの更新処理
	this->UpdateParticle();
	
	//画像の更新処理
	TextureBase::Update();

	//当たり判定の座標を設定する
	this->SetCenter(this->GetPosition());

	//当たり判定の半径を設定する
	this->SetRadius(Vector3(this->AskOrigin()));

	//当たり判定の追加
	GameContext::Get<CollisionManager>()->AddUseHero(this);
}

void VirusBeam::Render()
{
	//画像の描画処理
	TextureBase::Render();
}

void VirusBeam::Finalize()
{
}

void VirusBeam::OnCollisionEnter(ITextureBase* obj)
{
	CollisionBase::OnCollisionEnter(obj);
}

void VirusBeam::NonCollisionEnter(ITextureBase* obj)
{
	CollisionBase::NonCollisionEnter(obj);
}

void VirusBeam::WithPlayer(ITextureBase* texture)
{
	//プレイヤーと同じ処理を行う

	//画像を押し出す
	TextureBase::Extrude(texture);

	//中心のパーティクルの処理
	m_MainParticle->Extrude(texture);

	//中心に集まってくるパーティクルの処理
	for (TrackingParticle*& particle : m_OtherParticleList)
	{
		particle->Extrude(texture);
	}

}

void VirusBeam::InitBeam()
{
	//ビームの初期化をする

	//フレーム数を初期化する
	m_FrameCount = NULL;

	//終点を初期化する
	m_DrawFinal = NULL;

	//切り出し位置を初期化する
	this->SetRect(RECT{ 0,0,0,0 });

	
}

void VirusBeam::UpdateParticle()
{
	//パーティクルの更新処理
	
	//ベクトルを設定する
	for (Particle*& particle : m_AllParticleList)
	{
		particle->SetVelocity(this->GetVelocity());
	}
}

void VirusBeam::AskCircleVectol(Vector3& vel,float& angle)
{
	//回転行列を作成する（スクリーン座標では、Z軸のみの回転とする）
	Matrix rot = Matrix::CreateRotationZ(XMConvertToRadians(angle));

	//ベクトルを回転させる
	vel = Vector3::Transform(vel, rot);

	//角度を更新する
	angle += 36.0f;
}

