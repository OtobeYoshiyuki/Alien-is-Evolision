#include "pch.h"
#include "MasicEffectManager.h"
#include "OtobeLib/FrameWork/ResourceManager/ResourceManager.h"
#include "OtobeLib/StackScene/Scene/Play/PlayScene.h"
#include "OtobeLib/FrameWork/SoundManager/SoundManager.h"
#include "GameContext.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace OtobeLib;

//コマンド技用の待機時間
const float MasicEffectManager::WAIT_TIME_COMMAND = 1.0f;

//必殺技用の待機時間
const float MasicEffectManager::WAIT_TIME_SPECIAL = 2.0f;

//座標を設定する際の上下
const float MasicEffectManager::POSITION_STANDING_WIDTH = 20.0f;

//コマンド技エフェクトの名前
const std::string MasicEffectManager::EFFECT_NAME_COMMAND = "コマンドエフェクト";

//必殺技エフェクトの名前
const std::string MasicEffectManager::EFFECT_NAME_SPECIAL = "必殺技エフェクト";

MasicEffectManager::MasicEffectManager()
{
}

void MasicEffectManager::Initialize(ITextureBase* owner)
{
	//Playerを取得する
	CoroKun* player = GameContext::Get<CoroKun>();

	//配列の添え字を設定する
	this->SetNowState(NULL);

	for (int i = 0; i < MAX_MASIC_EFFECT; i++)
	{
		//コマンド技用のテクスチャを生成する
		this->CreateEffect(owner, EFFECT_NAME_COMMAND,
			Colors::Blue, WAIT_TIME_COMMAND, m_commandRecoverys,-1);

		//必殺技用のテクスチャを生成する
		this->CreateEffect(owner, EFFECT_NAME_SPECIAL,
			Colors::Red, WAIT_TIME_SPECIAL, m_specialRecoverys,1);
	}
}

void MasicEffectManager::Update()
{
}

void MasicEffectManager::NowEffectPlay(const int& mpheal, const int& specialheal)
{
	//現在のエフェクトを再生する

	//SoundManagerを取得する
	SoundManager* soundManager = SoundManager::GetInstance();

	//SEを鳴らす
	soundManager->Play(CRI_CUESHEET_0__32_アイテム追跡);

	//添え字を取得する
	int effect = this->GetNowState();

	//対象のエフェクトを取得する
	TechniqueRecovery* techRecovery = m_commandRecoverys[effect];
	TechniqueRecovery* deathRecovery = m_specialRecoverys[effect];

	//回復量を設定する
	techRecovery->SetHeal(mpheal);
	deathRecovery->SetHeal(specialheal);

	//座標を設定する
	techRecovery->SetPosition(techRecovery->GetOwner()->GetPosition() + Vector3(0.0f, -POSITION_STANDING_WIDTH, 0.0f));
	deathRecovery->SetPosition(techRecovery->GetOwner()->GetPosition() + Vector3(0.0f, POSITION_STANDING_WIDTH, 0.0f));

	//エフェクトのステートを設定する
	techRecovery->SetNowState(TARGET_STATE::TRACKING);
	deathRecovery->SetNowState(TARGET_STATE::TRACKING);

	//経験値の追尾用のステートを起こす
	techRecovery->GetTrackingState()->SetExecute(true);
	deathRecovery->GetTrackingState()->SetExecute(true);

	//経験値のホーミング用のステートを起こす
	techRecovery->GetHomingState()->SetExecute(true);
	techRecovery->GetHomingState()->SetStartFlag(true);
	deathRecovery->GetHomingState()->SetExecute(true);
	deathRecovery->GetHomingState()->SetStartFlag(true);

	//ホーミング用のステートを初期化する
	techRecovery->GetHomingState()->InitState(Vector3(0.0f, -5.0f, 0.0f));
	deathRecovery->GetHomingState()->InitState(Vector3(0.0f, 5.0f, 0.0f));

	//描画を起こす
	techRecovery->SetState(ITextureBase::STATE::DRAW);
	deathRecovery->SetState(ITextureBase::STATE::DRAW);

	//配列の末尾まで来たら、先頭に戻る
	//それ以外の場合は、配列を更新する
	if (effect >= MAX_MASIC_EFFECT - 1)effect = 0;
	else effect++;

	//添え字を設定する
	this->SetNowState(effect);
}

void MasicEffectManager::CreateEffect
(
	ITextureBase* owner,//所有者
	const std::string& name,//名前
	const DirectX::XMVECTORF32& color,//画像の色
	const float& waitTimer,//待ち時間
	std::vector<TechniqueRecovery*>& recoveryArray,//対象の配列(std::vector<TechniqueRecovery*>)
	const int& dir//ベクトルの向き(1か－1を指定)
)
{
	//ResourceManagerを取得する
	ResourceManager* resourceManager = ResourceManager::GetInstance();

	//TextureManagerを取得する
	TextureManager* gameManager = GameContext::Get<PlayScene>()->GetGameManager().get();

	//Playerを取得する
	CoroKun* player = GameContext::Get<CoroKun>();

	//使用する画像を取得する
	ITextureBase* useTexture = resourceManager->GetTexture("ゲージエフェクト");

	//テクスチャを生成する
	std::unique_ptr<TechniqueRecovery> pcommondEffect = std::make_unique<TechniqueRecovery>(useTexture);

	//名前を設定する
	pcommondEffect->SetName(name);

	//色を設定する
	pcommondEffect->SetColor(color);

	//描画の有無を設定する
	pcommondEffect->SetState(ITextureBase::STATE::NONE);

	//待ち時間を設定する
	pcommondEffect->SetWaitTimer(waitTimer);

	//所有者のポインタを設定する
	pcommondEffect->SetOwner(owner);

	//ベクトルの補正値を設定する
	pcommondEffect->GetTrackingState()->SetCorrection(Vector3(30.0f));

	//追跡目標を設定する
	pcommondEffect->GetTrackingState()->SetTarget(&player->GetPosition());

	//自身の座標を設定する
	pcommondEffect->GetTrackingState()->SetThis(&pcommondEffect->GetPosition());

	//ベクトルの向きを設定する
	pcommondEffect->GetHomingState()->SetDirVel(Vector3(0.0f, 5.0f, 0.0f) * dir);

	//配列に生ポインタを追加する
	recoveryArray.push_back(pcommondEffect.get());

	//TextureManagerに追加する
	gameManager->Add(std::move(pcommondEffect));
}
