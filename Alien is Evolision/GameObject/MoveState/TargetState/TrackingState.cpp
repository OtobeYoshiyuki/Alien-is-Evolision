#include "pch.h"
#include "TrackingState.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace OtobeLib;

TrackingState::TrackingState():
	m_target(nullptr),//追跡対象
	m_this(nullptr),//自分自身の座標
	m_correction(Vector3::Zero)//ベクトルの補正値
{
}

const Vector3 TrackingState::Update()
{
	//ベクトルの初期化
	Vector3 vel = Vector3::Zero;

	//実行関数がfalseのときは何もしない
	if (!this->GetExecute())return vel;

	//追跡対象が設定されていなければ、何もしない
	if (!m_target || !m_this)return vel;

	//補正値が設定されていなければ、何もしない
	if (m_correction == Vector3::Zero)return vel;

	//自身と対象の距離を求める
	Vector3 len = *m_target - *m_this;

	//距離を正規化する
	len.Normalize();

	//ベクトルを補正する
	len *= m_correction;

	//ベクトルを渡す
	vel = len;

	//ベクトルを返す
	return vel;
}
