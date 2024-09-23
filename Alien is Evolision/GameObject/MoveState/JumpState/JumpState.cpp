#include "pch.h"
#include "JumpState.h"
#include "OtobeLib/FrameWork/StaticData/StaticData.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace OtobeLib;

JumpState::JumpState():m_JumpLimit(0.0f),m_JumpSpeed(0.0f),m_JumpCount(NULL),
m_JumpCountOption(false),m_JumpCountLimit(NULL),m_JumpDesignation(false),
m_Correction(1.0f),m_OM(nullptr),m_BoundPower(0.0f)
{
	m_OM = std::make_unique<Owner<ITextureBase>>();
}

const Vector3 JumpState::Update()
{
	//ベクトルを初期化する
	Vector3 vel = Vector3::Zero;

	//実行関数がfalseのときは、何もしない
	if (!this->GetExecute())return vel;

	//ジャンプの限界値が設定されていなければ、何もしない
	if (m_JumpLimit == 0.0f)return vel;

	//ステートによって、処理を行う
	switch (this->GetNowState())
	{
	case JUMP::START:
		this->State_Start(vel);
		break;
	case JUMP::FLY:
		this->State_Fly(vel);
		break;
	case JUMP::AIR:
		this->State_Air(vel);
		break;
	case JUMP::FALL:
		this->State_Fall(vel);
		break;
	case JUMP::LANDING:
		this->State_Landing(vel);
		break;
	}

	//ベクトルを返す
	return vel;
}

void JumpState::State_Start(Vector3& vel)
{
	//ジャンプ開始の処理
	if (m_JumpCountOption)
	{
		//設定したジャンプ回数まで届いていない
		if (m_JumpCount <= m_JumpCountLimit)
		{
			//ジャンプ回数を更新する
			m_JumpCount++;
		}
	}

	//ステートをFlyに移行する
	this->SetNowState(JUMP::FLY);
}

void JumpState::State_Fly(Vector3& vel)
{
	//静的データの取得
	StaticData* staticData = StaticData::GetInstance();

	//上昇中の処理

	//変数の初期化
	float jumpSum = 0.0f;

	//リストの中身のジャンプ値の合計を求める
	for (float& jump : m_JumpPowerList)
	{
		jumpSum += jump;
	}

	//ジャンプ値が限界値以上になったら、ステートを切り替え
	//処理を飛ばす
	if (jumpSum >= m_JumpLimit)
	{
		//ステートを上昇から落下に変更
		this->SetNowState(JUMP::AIR);
		return;
	}

	//リストにジャンプパワーを追加する
	m_JumpPowerList.push_back(m_JumpSpeed);

	//リストの一番後ろの速度をベクトルに設定する
	//上方向のベクトルなので、値はマイナスとする
	vel.y = -m_JumpPowerList.back();
}

void JumpState::State_Air(Vector3& vel)
{
	//静的データの取得
	StaticData* staticData = StaticData::GetInstance();

	//上昇から落下に移るステート

	//スピードを徐々に0に近づける
	//スピードはリストの一番後ろを参照する
	float speed = m_JumpPowerList.back() - m_Correction;

	//スピードが0より小さくなったら、FALLに切り替える
	if (speed < 0.0f)
	{
		this->SetNowState(JUMP::FALL);
		return;
	}

	//リストにスピードを追加する
	m_JumpPowerList.push_back(speed);

	//ベクトルを設定する
	vel.y = -m_JumpPowerList.back();
}

void JumpState::State_Fall(Vector3& vel)
{
	//落下中の処理
	
	//リストに追加されているJumpSpeedをベクトルとする
	vel.y = m_JumpPowerList.back();

	//ジャンプオプションが解放されていれば、リストを後ろから消す
	if (m_JumpCountOption)
	{
		//ベクトルを一番後ろから消去していく
		m_JumpPowerList.pop_back();
	}
	//オプションが解放されていなければ、
	else
	{
		if (m_JumpSpeed != vel.y)
		{
			//ベクトルを一番後ろから消去していく
			m_JumpPowerList.pop_back();
		}
	}

	//リストの中身が全てなくなったら、ステートを切り替える
	if (m_JumpPowerList.empty())
	{
		this->SetNowState(JUMP::LANDING);

		//バウンドさせる
		m_JumpLimit -= m_BoundPower;
	}
}

void JumpState::State_Landing(Vector3& vel)
{
	//着地時の処理

	//オプションが解放されているときは、
	//再び、ジャンプさせる
	if (m_JumpCountOption)
	{
		//設定したジャンプ回数まで届いた
		if (m_JumpCount >= m_JumpCountLimit)
		{
			//ステートを初期化する
			this->InitState();
			//指定した回数ジャンプしたら、flagをtrueにする
			m_JumpDesignation = true;
		}
		else
		{
			//再び、ジャンプ
			this->SetNowState(JUMP::START);
		}
	}
	else
	{
		//ステートを初期化する
		this->InitState();
	}
}

void JumpState::InitState()
{
	//ステートの初期化を行う

	//ステートを元に戻す
	this->SetNowState(JUMP::JNONE);

	//ジャンプ回数を初期化する
	m_JumpCount = NULL;

	//リストの中身を開放する
	m_JumpPowerList.clear();

	////限界値を初期化する
	//m_JumpLimit = 0.0f;

	////ジャンプスピードを初期化する
	//m_JumpSpeed = 0.0f;


	////ジャンプオプションを初期化する
	//m_JumpCountOption = false;

	////ジャンプ回数の限界値を初期化する
	//m_JumpCountLimit = NULL;

}

void JumpState::AddFall()
{
	m_JumpPowerList.push_back(m_JumpSpeed);
}

void JumpState::RemoveFall()
{
	m_JumpPowerList.clear();
}
