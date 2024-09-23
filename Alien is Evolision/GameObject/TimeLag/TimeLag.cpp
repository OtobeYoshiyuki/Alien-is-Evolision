#include "pch.h"
#include "TimeLag.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

TimeLag::TimeLag():m_Pos(Vector3::Zero),m_Vel(Vector3::Zero),m_Finish(false),
m_FrameCount(NULL),m_FrameFlag(false),m_Target(""),m_AutoMoveState(nullptr),m_NowString(""),
m_JumpState(nullptr),m_AllStringFlag(false),m_customFlag(false)
{
	//AutoMoveState生成
	m_AutoMoveState = std::make_unique<AutoMoveState>();
	m_AutoMoveState->SetExecute(false);

	//JumpState生成
	m_JumpState = std::make_unique<JumpState>();
	m_JumpState->SetExecute(false);

	//MoveStateのリストに追加する
	m_MoveList.push_back(m_AutoMoveState.get());
	m_MoveList.push_back(m_JumpState.get());
}

void TimeLag::Initialize(const bool& flag, const std::string& target,
	const Vector3& pos, const Vector3& vel)
{
	//フラグを設定する
	m_FrameFlag = flag;

	//全体の文字列を設定する
	m_Target = target;

	//座標を設定する
	m_Pos = pos;

	//ベクトルを設定する
	m_Vel = vel;
}

void TimeLag::Update()
{
	//座標の更新をおこなう
	this->UpdatePosition();

	//文字の更新を行う
	this->UpdateString();
}

void TimeLag::UpdatePosition()
{
	//ベクトルを初期化する
	Vector3 vel = m_Vel;

	//実行中のステートからベクトルを算出
	for (MoveState*& move : m_MoveList)
	{
		vel += move->Update();
	}

	//等速直線運動を行う
	m_Pos += vel;
}

void TimeLag::UpdateString()
{
	//文字の更新を行う

	//描画の開始時間を取得する
	int intime = m_InTime[m_Target];

	//開始時刻になったら、文字を設定する
	//次の文字が設定されていなければ、そのまま
	if (m_FrameCount > intime)
	{
		if (this->NextStringRequest(m_Target))
		{
			//現在の文字を設定する
			m_Target = m_NextString[m_Target];
			m_NowString = m_Target;
		}
		else
		{
			//全ての文字が表示された
			m_AllStringFlag = true;
		}
	}

	//文字全体の終了時間になったら
	if (m_FrameCount > m_OutTime)
	{
		//カウンターを初期化する
		m_FrameCount = NULL;
		//カウンターの更新を止める
		m_FrameFlag = false;
		//現在の文字を設定する
		m_NowString = "";
		//終了フラグをtrueにする
		m_Finish = true;
	}

	//フレームカウントを更新する
	if (m_FrameFlag)m_FrameCount++;

}


