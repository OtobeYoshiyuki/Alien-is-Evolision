#pragma once
#include "GameObject/MoveState/MoveState.h"
#include "Direct.h"
#include "TemplateState.h"
class AutoMoveState :public MoveState,State<DIRECT>
{
private:
	//1フレームに加算するスピード
	DirectX::SimpleMath::Vector3	m_Speed;
public:
	//コンストラクタ
	AutoMoveState();

	//デストラクタ
	~AutoMoveState()override {}

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
	void SetNowState(const DIRECT& now)override { State::SetNowState(now); };

	//現在の状態を取得する
	const DIRECT& GetNowState()const override { return State::GetNowState(); };

	//ひとつ前の状態を取得する
	const DIRECT& GetPreState()const override { return State::GetPreState(); };

public:
	//1フレームに加算するスピードを設定する
	void SetSpeed(const DirectX::SimpleMath::Vector3& speed) { m_Speed = speed; }

	//1フレームに加算するスピードを取得する
	const DirectX::SimpleMath::Vector3& GetSpeed()const { return m_Speed; }

	//ステートを反転させる
	void ChangeState();
};

