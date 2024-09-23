#pragma once
#include "OtobeLib/Camera/Base/CameraBase.h"
#include "OtobeLib/Texture/Interface/ITextureBase.h"
#include "TemplateState.h"
#include "TARGET_STATE.h"

class TargetScrollCamera:public OtobeLib::CameraBase,State<TARGET_STATE>
{
public:
	//対象物に移動する移動量
	static const float TARGET_TO_VECTOL;

	//プレイヤーと対象物との距離
	static const float TARGET_TO_LENGTH;
private:
	//現在位置からスクロールさせる位置
	DirectX::SimpleMath::Vector3 m_target;

	//追跡するナンバー
	int							m_targetNumber;
public:
	//コンストラクタ
	TargetScrollCamera();

	//デストラクタ
	~TargetScrollCamera()override {};

	//初期化
	void Initialize()override;

	//更新処理
	void Update()override;

	//終了処理
	void Finalize()override;

	//-------------------------------セッター---------------------------------//
		//座標を設定する
	virtual void SetPosition(const DirectX::SimpleMath::Vector3& pos) { CameraBase::SetPosition(pos); }

	//行列を設定する
	virtual void SetMatrix(const DirectX::SimpleMath::Matrix& matrix) { CameraBase::SetMatrix(matrix); }

	//オブジェクトの移動の有無を設定する
	virtual void SetObjectMove(const bool& move) { CameraBase::SetObjectMove(move); }
	//-------------------------------セッター---------------------------------//

	//-------------------------------ゲッター---------------------------------//
		//座標を設定する
	virtual const DirectX::SimpleMath::Vector3& GetPosition() { return CameraBase::GetPosition(); }

	//行列を設定する
	virtual const DirectX::SimpleMath::Matrix& GetMatrix() { return CameraBase::GetMatrix(); }

	//オブジェクトの移動の有無を設定する
	virtual const bool& GetObjectMove() { return CameraBase::GetObjectMove(); }
	//-------------------------------ゲッター---------------------------------//

public:
	//現在の状態を設定する
	void SetNowState(const TARGET_STATE& now)override { State::SetNowState(now); };

	//現在の状態を取得する
	const TARGET_STATE& GetNowState()const override { return State::GetNowState(); };

	//ひとつ前の状態を取得する
	const TARGET_STATE& GetPreState()const override { return State::GetPreState(); };

public:
	//スクロールさせる位置を設定する
	void SetTarget(const DirectX::SimpleMath::Vector3& target) { m_target = target; }

	//追跡するナンバーを設定する
	void SetTargetNumber(const int& num) { m_targetNumber = num; }

	//スクロールさせる位置を取得する
	const DirectX::SimpleMath::Vector3& GetTarget()const { return m_target; }

	//追跡するナンバーを取得する
	const int& GetTargetNumber()const { return m_targetNumber; }

private:
	//ステートの更新を行う
	void UpdateState();

	//State_Trackingの処理
	void State_Tracking();

	//State_Waitの処理
	void State_Wait();

	//State_FadeOutの処理
	void State_FadeOut();

	//State_FadeInの処理
	void State_FadeIn();

	//State_Changeの処理
	void State_Change();
};

