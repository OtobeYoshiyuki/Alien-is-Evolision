#pragma once
#include "GameObject/MoveState/MoveState.h"

class TrackingState:public MoveState
{
private:
	//追跡対象
	const DirectX::SimpleMath::Vector3* m_target;

	//自身
	const DirectX::SimpleMath::Vector3* m_this;

	//ベクトルを計算する補正値
	DirectX::SimpleMath::Vector3 m_correction;
public:
	//コンストラクタ
	TrackingState();

	//デストラクタ
	~TrackingState()override {}

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
	//追跡対象を設定する
	void SetTarget(const DirectX::SimpleMath::Vector3* target) { m_target = target; }

	//自身の座標を設定する
	void SetThis(const DirectX::SimpleMath::Vector3* myself) { m_this = myself; }

	//ベクトルの補正値を設定する
	void SetCorrection(const DirectX::SimpleMath::Vector3& correction) { m_correction = correction; }

	//追跡対象を取得する
	const DirectX::SimpleMath::Vector3& GetTarget()const { return *m_target; }

	//ベクトルの補正値を取得する
	const DirectX::SimpleMath::Vector3& GetCorrection()const { return m_correction; }
};

