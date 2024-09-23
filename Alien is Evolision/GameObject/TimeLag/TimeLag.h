#pragma once
#include <map>
#include <string>
#include "GameObject/MoveState/AutoMoveState/AutoMoveState.h"
#include "GameObject/MoveState/JumpState/JumpState.h"
class TimeLag
{
private:
	//最初の文字を設定する
	std::string					m_Target;
	//現在の文字を設定する
	std::string					m_NowString;
	//描画の開始時間
	std::map<std::string, int>	m_InTime;
	//描画の終了時間
	int							m_OutTime;
	//次に表示する文字列
	std::map<std::string, std::string> m_NextString;
	//ターゲットの座標
	DirectX::SimpleMath::Vector3 m_Pos;
	//ターゲットのベクトル
	DirectX::SimpleMath::Vector3 m_Vel;
	//フレーム数のカウント
	int							m_FrameCount;
	//フレーム数をカウントするフラグ
	bool						m_FrameFlag;
	//全ての文字が表示されたフラグ
	bool						m_AllStringFlag;
	//演出が終了したか
	bool						m_Finish;
	//カスタムフラグ
	bool						m_customFlag;
	//AutoMoveState
	std::unique_ptr<AutoMoveState>	m_AutoMoveState;
	//JumpState
	std::unique_ptr<JumpState>		m_JumpState;
	//MoveStateのリスト
	std::list<MoveState*>			m_MoveList;
public:
	//コンストラクタ
	TimeLag();

	//デストラクタ
	~TimeLag() {};

	//初期化
	void Initialize(const bool& flag, const std::string& target,
		const DirectX::SimpleMath::Vector3& pos,const DirectX::SimpleMath::Vector3& vel);

	//更新
	void Update();

public:
	//対象の文字を設定する
	void SetTarget(const std::string& target) { m_Target = target; }

	//指定したキーの描画の開始時間を設定する
	void SetInTime(const std::string& name, const int& time) { m_InTime[name] = time; }

	//指定したキーの描画の開始時間を設定する
	void SetOutTime(const int& time) { m_OutTime = time; }

	//指定したキーの次の表示する文字列を設定する
	void SetNextString(const std::string& now, const std::string& next) { m_NextString[now] = next; }

	//更新フラグを設定する
	void SetFrameFlag(const bool& flag) { m_FrameFlag = flag; }

	//ベクトルを設定する
	void SetVelocity(const DirectX::SimpleMath::Vector3& vel) { m_Vel = vel; }

	//演出のフラグを設定する
	void SetFinishFlag(const bool& flag) { m_Finish = flag; }

	//カスタムフラグを設定する
	void SetCustomFlag(const bool& flag) { m_customFlag = flag; }

public:
	//現在の描画する文字列を取得する
	const std::string GetNowString()const { return m_NowString; };

	//演出のフラグを取得する
	const bool& GetFinishFlag()const { return m_Finish; }

	//全体の文字の判定用のフラグを取得する
	const bool& GetAllStringFlag()const { return m_AllStringFlag; }

	//次の表示する文字列があるかどうか、確認する
	const bool NextStringRequest(const std::string& now)const
	{
		//イテレーターを宣言する
		std::map<std::string, std::string>::const_iterator iter;

		//目的物を検索する
		iter = m_NextString.find(now);

		//目的物が検索出来たら、trueを返す
		if (iter != m_NextString.end())return true;

		//目的物が検索できなかった
		return false;
	}

	//座標を取得する
	const DirectX::SimpleMath::Vector3& GetPosition()const { return m_Pos; }

	//AutoMoveStateを取得する
	std::unique_ptr<AutoMoveState>& GetAutoMoveState() { return m_AutoMoveState; }

	//JumpStateを取得する
	std::unique_ptr<JumpState>& GetJumpState() { return m_JumpState; }

	//FrameFragを取得する
	const bool& GetFrameFrag()const { return m_FrameFlag; }

	//カスタムフラグを取得する
	const bool& GetCustomFlag()const { return m_customFlag; }

	//フレームカウントを取得する
	const int& GetFrameCount()const { return m_FrameCount; }

private:
	//座標の更新
	void UpdatePosition();

	//文字の更新
	void UpdateString();
};
