#include "pch.h"
#include "MainParticle.h"
#include "GameContext.h"
#include "OtobeLib/StackScene/Scene/Play/PlayScene.h"
#include "GameObject/Technique/VirusBeam/VirusBeam.h"
#include "GameObject/Technique/VirusBeam/TrackingParticle/TrackingParticle.h"
#include "OtobeLib/FrameWork/Collision/CollisionManager/CollisionManager.h"
#include "OtobeLib/FrameWork/SoundManager/SoundManager.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace OtobeLib;

//1フレームにスケールに加算する速度
const Vector3 MainParticle::SCALESPEED = Vector3(0.1f);

MainParticle::MainParticle(ITextureBase* texture):Particle(texture),m_ChageCount(NULL),m_LimitChage(NULL),
m_ChageFinish(false),m_ScaleCount(NULL),m_ScaleLimitCount(NULL),m_ScaleLimit(0.0f),m_ScaleVel(Vector3::Zero)
{
}

void MainParticle::Initialize()
{
	//終了フラグを初期化する
	m_ChageFinish = false;

	//チャージカウントを初期化する
	m_ChageCount = NULL;

	//拡大倍率を初期化する
	this->SetScale(Vector3(0.2f));
}

void MainParticle::Update()
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

	//ウイルスビームを取得する
	VirusBeam* virusBeam = GameContext::Get<VirusBeam>();

	//追尾用のパーティクルのリストを取得する
	std::vector<TrackingParticle*> TrackingList = virusBeam->GetParticleList();

	//パーティクルの更新
	Particle::Update();

	//変数を初期化する
	int NoDrawNum = NULL;

	//描画が切られている数を数える
	for (TrackingParticle*& particle : TrackingList)
	{
		if (particle->GetState() == STATE::NONE)
		{
			NoDrawNum++;
		}
	}

	//ステートによって、処理を切り替える
	switch (virusBeam->GetNowState())
	{
	case ENERGY_CHAGE::TRACKING:
		this->State_Tracking(NoDrawNum);
		break;
	case ENERGY_CHAGE::ENERGYUP:
		this->State_PowerUp(NoDrawNum);
		break;
	case ENERGY_CHAGE::ENERGYMAX:
		this->State_Max();
		break;
	}

	//当たり判定を追加する
	GameContext::Get<CollisionManager>()->AddUseHero(this);
}

void MainParticle::Render()
{
	//パーティクルの描画
	Particle::Render();
}

void MainParticle::Finalize()
{
}

void MainParticle::OnCollisionEnter(ITextureBase* obj)
{
	Particle::OnCollisionEnter(obj);
}

void MainParticle::NonCollisionEnter(ITextureBase* obj)
{
	Particle::NonCollisionEnter(obj);
}

void MainParticle::State_Tracking(const int& NoDrawNum)
{
	//周囲のパーティクルが追尾しているときの処理

	//ウイルスビームを取得する
	VirusBeam* virusBeam = GameContext::Get<VirusBeam>();

	//追尾用のパーティクルのリストを取得する
	std::vector<TrackingParticle*> TrackingList = virusBeam->GetParticleList();

	//全ての描画が切られている
	if (NoDrawNum >= (int)TrackingList.size())
	{
		//ステートを変更する
		virusBeam->SetNowState(ENERGY_CHAGE::ENERGYUP);

		//限界値に達していなければ、チャージ数を更新する
		if (m_ChageCount < m_LimitChage)
		{
			m_ChageCount++;
		}
	}
}

void MainParticle::State_PowerUp(const int& NoDrawNum)
{
	//中心のパーティクルが大きくなる処理

	//ウイルスビームを取得する
	VirusBeam* virusBeam = GameContext::Get<VirusBeam>();

	//追尾用のパーティクルのリストを取得する
	std::vector<TrackingParticle*> TrackingList = virusBeam->GetParticleList();

	//全ての描画が切られていたら、チャージ数を更新する
	if (NoDrawNum >= (int)TrackingList.size())
	{
		//すでに限界値に達していたら、何もしない
		if (m_ChageCount >= m_LimitChage)
		{
			//チャージ完了フラグをオンにする
			m_ChageFinish = true;
			//カウントを初期化する
			m_ChageCount = NULL;
			//ステートを切り替える
			virusBeam->SetNowState(ENERGY_CHAGE::ENERGYMAX);
			//描画を消す
			for (TrackingParticle* particle : TrackingList)
			{
				//描画をオフにする
				particle->SetState(STATE::NONE);
			}
		}
		//限界値に達していなければ、更新する
		else
		{
			//再び、描画する
			for (TrackingParticle* particle : TrackingList)
			{
				//描画をオンにする
				particle->SetState(STATE::DRAW);
			}
			//ステートを切り替える
			virusBeam->SetNowState(ENERGY_CHAGE::TRACKING);
			//座標を再設定する
			virusBeam->SetParticlesPos(this->GetPosition());
		}
	}
}

void MainParticle::State_Max()
{
	//エネルギーがMAXの時の処理

	//SoundManagerを取得する
	SoundManager* soundManager = SoundManager::GetInstance();

	//ウイルスビームを取得する
	VirusBeam* virusBeam = GameContext::Get<VirusBeam>();

	//現在の倍率を取得する
	Vector3 scale = this->GetScale();

	//さらにSwitch文で処理を分ける
	switch (this->GetNowState())
	{
	case SCALE::SNONE:
		//カウントを更新する
		m_ScaleCount++;
		//ステートを切り替える
		this->SetNowState(SCALE::EXPANSION);
		break;
	case SCALE::EXPANSION:
		//拡大の処理

		//拡大パラメーターの更新
		if (m_ScaleVel.x < m_ScaleLimit)
		{
			scale += SCALESPEED;
			m_ScaleVel += SCALESPEED;
		}
		//ステートを切り替える
		else
		{
			//スケールベクトルを初期化する
			m_ScaleVel = Vector3::Zero;
			//ステートを切り替える
			this->SetNowState(SCALE::SHRINK);
		}
		break;
	case SCALE::SHRINK:
		//縮小の処理

		//縮小パラメーターの更新
		if (m_ScaleVel.x < m_ScaleLimit)
		{
			scale -= SCALESPEED;
			m_ScaleVel += SCALESPEED;
		}
		else
		{
			//指定回数以上拡大・縮小を行ったら、次のステートに移行する
			if (m_ScaleCount >= m_ScaleLimitCount)
			{
				//ステートの切り替え
				virusBeam->SetNowState(ENERGY_CHAGE::ENERGYATACK);
				//ステートの切り替え
				this->SetNowState(SCALE::SNONE);
				//描画を消す
				this->SetState(STATE::NONE);
				//スケールベクトルを初期化する
				m_ScaleVel = Vector3::Zero;
				//拡大倍率を初期化する
				scale = Vector3(0.2f);
				//スケールカウントを初期化する
				m_ScaleCount = NULL;
				//SEを再生する
				soundManager->Play(CRI_CUESHEET_0__15_ビーム);
			}
			//ループ
			else
			{
				//ステートの切り替え
				this->SetNowState(SCALE::SNONE);
				//スケールベクトルを初期化する
				m_ScaleVel = Vector3::Zero;
			}
		}
		break;
	}

	//倍率を再設定する
	this->SetScale(scale);
}
