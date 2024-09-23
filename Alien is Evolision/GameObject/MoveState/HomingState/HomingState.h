#pragma once
#include "GameObject/MoveState/MoveState.h"
class HomingState:public MoveState
{
private:
	//ベクトルの距離(Z軸を軸とする)
	DirectX::SimpleMath::Vector3	m_length;

	//ベクトルのスカラ値
	DirectX::SimpleMath::Vector3	m_scalar;

	//ベクトルの最大値
	DirectX::SimpleMath::Vector3	m_maxVel;

	//ベクトルの最小値
	DirectX::SimpleMath::Vector3	m_minVel;

	//ベクトルの向き
	DirectX::SimpleMath::Vector3	m_dirVel;

	//回転（デグリー）
	float							m_angle;

	//開始フラグ
	bool							m_start;

	//終了フラグ
	bool							m_finish;

public:
	//コンストラクタ
	HomingState();

	//デストラクタ
	~HomingState()override {}

	//更新処理
	const DirectX::SimpleMath::Vector3 Update()override;

	//---------------------セッター------------------------------------------------------------------//
	 
	//実行関数を設定する
	void SetExecute(const bool& exe) { MoveState::SetExecute(exe); }

	//削除関数を設定する
	void SetIsVal(const bool& val) { MoveState::SetIsVal(val); }

	//距離を設定する
	void SetLength(const DirectX::SimpleMath::Vector3& length) { m_length = length; }

	//角度を設定する
	void SetAngle(const float& angle) { m_angle = angle; }

	//スカラ値を設定する
	void SetScalar(const DirectX::SimpleMath::Vector3& scalar) { m_scalar = scalar; }

	//ベクトルの最大値を設定する
	void SetMaxVel(const DirectX::SimpleMath::Vector3& max) { m_maxVel = max; }

	//ベクトルの最小値を設定する
	void SetMinVel(const DirectX::SimpleMath::Vector3& min) { m_minVel = min; }

	//ベクトルの向きを設定する
	void SetDirVel(const DirectX::SimpleMath::Vector3& dir) { m_dirVel = dir; }

	//開始フラグを設定する
	void SetStartFlag(const bool& start) { m_start = start; }

	//終了フラグを設定する
	void SetFinishFlag(const bool& finish) { m_finish = finish; }

	//---------------------セッター------------------------------------------------------------------//


	//---------------------ゲッター------------------------------------------------------------------//
	 
	//実行関数を取得する
	const bool& GetExecute()const { return MoveState::GetExecute(); }

	//削除関数を取得する
	const bool& GetIsVal()const { return MoveState::GetIsVal(); }

	//距離を取得する
	const DirectX::SimpleMath::Vector3& GetLength()const { return m_length; }

	//角度を取得する
	const float& GetAngle()const { return m_angle; }

	//スカラ値を取得する
	const DirectX::SimpleMath::Vector3& GetScalar()const { return m_scalar; }

	//ベクトルの最大値を取得する
	const DirectX::SimpleMath::Vector3& GetMaxVel()const { return m_maxVel; }

	//ベクトルの最小値を取得する
	const DirectX::SimpleMath::Vector3& GetMinVel()const { return m_minVel; }

	//ベクトルの向きを取得する
	const DirectX::SimpleMath::Vector3& GetDirVel()const { return m_dirVel; }

	//開始フラグを取得する
	const bool& GetStartFlag()const { return m_start; }

	//終了フラグを取得する
	const bool& GetFinishFlag()const { return m_finish; }

	//---------------------ゲッター------------------------------------------------------------------//

public:
	//ステートを初期化する
	void InitState(const DirectX::SimpleMath::Vector3& dir);

};

