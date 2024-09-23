#pragma once
#include "GameObject/MoveState/MoveState.h"
class DamageState:public MoveState
{
private:
	//与えるダメージの値
	DirectX::SimpleMath::Vector3	mDamage;

	//ダメージを与える回数
	int								mDamageCount;

	//初回のダメージ
	DirectX::SimpleMath::Vector3	mFirstDamage;
public:
	//ダメージを与える最大値
	static const int				MAX_DAMAGE = 7;
public:
	//コンストラクタ
	DamageState();

	//デストラクタ
	~DamageState()override {}

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

	//ダメージの値を設定する
	void SetDamage(const DirectX::SimpleMath::Vector3& damage) { mDamage = damage; }

	//初回ダメージの値を設定する
	void SetFirstDamage(const DirectX::SimpleMath::Vector3& first) { mFirstDamage = first; }

	//ダメージの値を取得する
	const DirectX::SimpleMath::Vector3& GetDamage()const { return mDamage; }

	//初回ダメージの値を設定する
	const DirectX::SimpleMath::Vector3& GetFirstDamage()const { return mFirstDamage; }

	//ダメージステートの初期化
	void InitDamageState();

	//ダメージカウントを取得
	const int& GetDamageCount()const { return mDamageCount; }
};

