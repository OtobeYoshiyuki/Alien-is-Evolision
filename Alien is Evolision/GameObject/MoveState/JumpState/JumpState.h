#pragma once
#include "GameObject/MoveState/MoveState.h"
#include "TemplateState.h"
#include "JUMP.h"
#include "OtobeLib/FrameWork/Owner/Owner.h"
#include "OtobeLib/Texture/Interface/ITextureBase.h"
#include <list>
class JumpState :public MoveState,State<JUMP>
{
private:
	//ステートの所有者のポインタ
	std::unique_ptr<OtobeLib::Owner<OtobeLib::ITextureBase>> m_OM;
	//更新されるジャンプの値を追加するリスト
	std::list<float>	m_JumpPowerList;
	//ジャンプの限界値
	float				m_JumpLimit;
	//ジャンプするたびに、バウンドさせる値
	float				m_BoundPower;
	//ジャンプのスピードを設定する
	float				m_JumpSpeed;
	//ジャンプする回数
	int					m_JumpCount;
	//ジャンプする回数の限界値
	int					m_JumpCountLimit;
	//ジャンプ回数のオプション開放
	bool				m_JumpCountOption;
	//指定した回数ジャンプしたフラグ
	bool				m_JumpDesignation;
	//上昇から下落まで補正するベクトル
	float				m_Correction;
public:
	//コンストラクタ
	JumpState();

	//デストラクタ
	~JumpState()override {}

	//更新処理
	const DirectX::SimpleMath::Vector3 Update()override;

	//---------------------セッター------------------------------------------------------------------//
	//実行関数を設定する
	void SetExecute(const bool& exe) { MoveState::SetExecute(exe); }

	//削除関数を設定する
	void SetIsVal(const bool& val) { MoveState::SetIsVal(val); }
	//---------------------セッター------------------------------------------------------------------//


	//---------------------ゲッター------------------------------------------------------------------//
	//実行関数を取得する
	const bool& GetExecute()const { return MoveState::GetExecute(); }

	//削除関数を取得する
	const bool& GetIsVal()const { return MoveState::GetIsVal(); }
	//---------------------ゲッター------------------------------------------------------------------//

public:
	//現在の状態を設定する
	void SetNowState(const JUMP& now)override { State::SetNowState(now); };

	//現在の状態を取得する
	const JUMP& GetNowState()const override { return State::GetNowState(); };

	//ひとつ前の状態を取得する
	const JUMP& GetPreState()const override { return State::GetPreState(); };

public:
	//ジャンプの限界値を設定する
	void SetJumpLimit(const float& limit) { m_JumpLimit = limit; }

	//ジャンプのスピードを設定する
	void SetJumpSpeed(const float& speed) { m_JumpSpeed = speed; }

	//ジャンプする回数を設定する
	void SetJumpLimitCount(const int& limit) { m_JumpCountLimit = limit; }

	//ジャンプする回数のオプションを設定する
	void SetJumpOption(const bool& option) { m_JumpCountOption = option; }

	//上昇から下落までの補正するベクトルを設定する
	void SetCorrection(const float& cor) { m_Correction = cor; }

	//バウンドさせる値
	void SetBoundPower(const float& bound) { m_BoundPower = bound; }

	//指定した回数ジャンプしたフラグを設定する
	void SetJumpmDesignation(const bool& designation) { m_JumpDesignation = designation; }

	//所有者のポインタを設定する
	void SetOM(OtobeLib::ITextureBase* texture) { m_OM->SetOwner(texture); }

	//ジャンプの限界値を取得する
	const float& GetJumpLimit()const { return m_JumpLimit; }

	//ジャンプのスピードを設定する
	const float& GetJumpSpeed()const { return m_JumpSpeed; }

	//ジャンプする回数を取得する
	const int& GetJumpLimitCount()const { return m_JumpCountLimit; }

	//ジャンプする回数のオプションを設定する
	const bool& GetJumpOption()const { return m_JumpCountOption; }

	//現在のジャンプ数を取得する
	const int& GetJumpCount()const { return m_JumpCount; }

	//指定した回数ジャンプしたフラグを取得する
	const bool& GetJumpmDesignation()const { return m_JumpDesignation; }

	//バウンドさせる値を取得する
	const float& GetBoundPower()const { return m_BoundPower; }

	//ジャンプベクトルの補正値を取得する
	const float& GetCorrection()const { return m_Correction; }

	//所有者を管理しているポインタを取得する
	OtobeLib::Owner<OtobeLib::ITextureBase>* GetOM() { return m_OM.get(); }


private:
	//ジャンプ開始のステート
	void State_Start(DirectX::SimpleMath::Vector3& vel);

	//ジャンプ中のステート
	void State_Fly(DirectX::SimpleMath::Vector3& vel);

	//ジャンプから落下に移るステート
	void State_Air(DirectX::SimpleMath::Vector3& vel);

	//落下中のステート
	void State_Fall(DirectX::SimpleMath::Vector3& vel);

	//着地時のステート
	void State_Landing(DirectX::SimpleMath::Vector3& vel);

public:
	//ステートの初期化
	void InitState();

	//落下するベクトルを一定数追加する
	void AddFall();

	//落下するベクトルを開放する
	void RemoveFall();
};

