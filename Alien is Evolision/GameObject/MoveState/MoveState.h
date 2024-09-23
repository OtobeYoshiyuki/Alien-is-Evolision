#pragma once
class MoveState
{
private:
	//実行するフラグ
	bool						mExecute;
	//削除するフラグ
	bool						mIsVal;
public:
	//コンストラクタ
	MoveState();

	//デストラクタ
	virtual ~MoveState(){}

	//更新処理
	virtual const DirectX::SimpleMath::Vector3 Update() { return DirectX::SimpleMath::Vector3::Zero; }

	//---------------------セッター------------------------------------------------------------------//
	//実行関数を設定する
	void SetExecute(const bool& exe) { mExecute = exe; }

	//削除関数を設定する
	void SetIsVal(const bool& val) { mIsVal = val; }
	//---------------------セッター------------------------------------------------------------------//


	//---------------------ゲッター------------------------------------------------------------------//
	//実行関数を取得する
	const bool& GetExecute()const { return mExecute; }

	//削除関数を取得する
	const bool& GetIsVal()const { return mIsVal; }
	//---------------------ゲッター------------------------------------------------------------------//
};

