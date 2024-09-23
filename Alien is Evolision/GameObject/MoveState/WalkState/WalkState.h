#pragma once
#include "GameObject/MoveState/MoveState.h"
class WalkState:public MoveState
{
private:
	//1フレームに進むベクトル
	DirectX::SimpleMath::Vector3 mWalk;
public:
	//コンストラクタ
	WalkState();

	//デストラクタ
	~WalkState()override{}

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
	//ベクトルを設定する
	void SetWalk(const DirectX::SimpleMath::Vector3& walk) { mWalk = walk; }

	//ベクトルを取得する
	const DirectX::SimpleMath::Vector3& GetWalk()const { return mWalk; }
};

