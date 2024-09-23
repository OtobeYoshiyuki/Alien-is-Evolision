#pragma once
#include "GameObject/MoveState/MoveState.h"
class AccelState:public MoveState
{
public:
	//加速する限界
	static const int MAX_ACCEL = 15;
private:
	//加速度
	DirectX::SimpleMath::Vector3 mAccel;
	//加速回数
	int							 mAccelCount;
public:
	//コンストラクタ
	AccelState();

	//デストラクタ
	~AccelState()override {}

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
	//加速度を設定する
	void SetAccel(const DirectX::SimpleMath::Vector3& accel) { mAccel = accel; }

	//加速度を取得する
	const DirectX::SimpleMath::Vector3& GetAccel()const { return mAccel; }

	//AccelStateの初期化をする
	void InitState();
};

