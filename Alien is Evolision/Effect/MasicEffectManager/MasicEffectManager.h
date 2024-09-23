#pragma once
#include "TechniqueRecovery/TechniqueRecovery.h"
#include <vector>
#include "TemplateState.h"
class MasicEffectManager:public State<int>
{
public:
	//MP,SPECIALのエフェクトの最大数
	static const int MAX_MASIC_EFFECT = 4;

	//コマンド技用の待機時間
	static const float WAIT_TIME_COMMAND;

	//必殺技用の待機時間
	static const float WAIT_TIME_SPECIAL;

	//座標を設定する際の上下
	static const float POSITION_STANDING_WIDTH;

	//コマンド技エフェクトの名前
	static const std::string EFFECT_NAME_COMMAND;

	//必殺技エフェクトの名前
	static const std::string EFFECT_NAME_SPECIAL;
private:
	//コマンド技用
	std::vector<TechniqueRecovery*>	m_commandRecoverys;

	//必殺技用
	std::vector<TechniqueRecovery*>	m_specialRecoverys;
public:
	//コンストラクタ
	MasicEffectManager();

	//デストラクタ
	~MasicEffectManager() = default;

	//初期化
	void Initialize(OtobeLib::ITextureBase* owner);

	//更新
	void Update();

public:
	//現在の状態を設定する
	void SetNowState(const int& now)override { State::SetNowState(now); };

	//現在の状態を取得する
	const int& GetNowState()const override { return State::GetNowState(); };

	//ひとつ前の状態を取得する
	const int& GetPreState()const override { return State::GetPreState(); };

	//現在のエフェクトを再生する
	void NowEffectPlay(const int& mpheal,const int& specialheal);

private:
	void CreateEffect(OtobeLib::ITextureBase* owner, const std::string& name,const DirectX::XMVECTORF32& color,
		const float& waitTimer,std::vector<TechniqueRecovery*>& recoveryArray,const int& dir);
};

