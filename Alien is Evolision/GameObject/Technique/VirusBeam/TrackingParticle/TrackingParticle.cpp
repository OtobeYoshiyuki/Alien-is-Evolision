#include "pch.h"
#include "TrackingParticle.h"
#include "GameObject/Technique/VirusBeam/VirusBeam.h"
#include "GameContext.h"
#include "GameObject/Technique/VirusBeam/MainParticle/MainParticle.h"
#include "OtobeLib/StackScene/Scene/Play/PlayScene.h"
#include "OtobeLib/FrameWork/Rand/Rand.h"
#include "OtobeLib/FrameWork/Collision/CollisionManager/CollisionManager.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace OtobeLib;

TrackingParticle::TrackingParticle(ITextureBase* texture):Particle(texture),m_RandNum(0.0)
{
	//乱数マシーンを取得する
	Rand* random = Rand::GetInstance();

	//中心のパーティクルを取得する
	MainParticle* mainParticle = GameContext::Get<VirusBeam>()->GetMainParticle();

	//乱数を設定する(一度だけ)
	m_RandNum = random->RandomMachine(1.0, 3.0);
}

void TrackingParticle::Initialize()
{
}

void TrackingParticle::Update()
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

	//中心のパーティクルを取得する
	MainParticle* mainParticle = virusBeam->GetMainParticle();

	//ターゲットベクトルを初期化する
	Vector3 target = Vector3::Zero;

	//パーティクルが追尾するとき
	if (virusBeam->GetNowState() == ENERGY_CHAGE::TRACKING)
	{
		//中心のパーティクルとの距離を算出する
		target = mainParticle->GetPosition() - this->GetPosition();

		//距離を正規化する
		target.Normalize();

		//ベクトルを補正する
		target *= m_RandNum;
	}

	//ベクトルを設定する
	this->SetVelocity(this->GetVelocity() + target);

	//パーティクルの更新
	Particle::Update();

	//当たり判定を追加する
	GameContext::Get<CollisionManager>()->AddUseOther(this, "中心の粒子");
}

void TrackingParticle::Render()
{
	//パーティクルの描画
	Particle::Render();
}

void TrackingParticle::Finalize()
{
}

void TrackingParticle::OnCollisionEnter(ITextureBase* obj)
{
	Particle::OnCollisionEnter(obj);

	//中心のパーティクルに当たったときの処理
	if (obj->GetName() == "中心の粒子")
	{
		//ウイルスビームを取得する
		VirusBeam* virusBeam = GameContext::Get<VirusBeam>();

		//中心のパーティクルを取得する
		MainParticle* mainParticle = virusBeam->GetMainParticle();

		//周囲のパーティクルが追尾中の時
		if (virusBeam->GetNowState() == ENERGY_CHAGE::TRACKING)
		{
			//パーティクルが描画されているとき
			if (this->GetState() == STATE::DRAW)
			{
				//描画を止める
				this->SetState(STATE::NONE);
				//拡大倍率を取得
				Vector3 scale = mainParticle->GetScale();
				//倍率を上げる
				scale += Vector3(0.08f / (float)mainParticle->GetChageLimit());
				//拡大倍率を設定する
				mainParticle->SetScale(scale);
			}
		}
	}
}

void TrackingParticle::NonCollisionEnter(ITextureBase* obj)
{
	Particle::NonCollisionEnter(obj);
}
