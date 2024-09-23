#include "pch.h"
#include "HomingState.h"
#include "Utils.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace OtobeLib;

//コンストラクタ
HomingState::HomingState():
	m_length(Vector3::Zero),//距離
	m_angle(0.0f),//角度(デグリー)
	m_start(false),//開始フラグ
	m_finish(false),//終了フラグ
	m_scalar(Vector3(1.0f)),//スカラ値
	m_maxVel(Vector3::Zero),//ベクトルの最大値
	m_minVel(Vector3::Zero),//ベクトルの最小値
	m_dirVel(Vector3::Zero)//ベクトルの向き
{
}

//更新処理
const Vector3 HomingState::Update()
{
	//ベクトルの初期化
	Vector3 vel = Vector3::Zero;

	//実行関数がfalseの時は、何もしない
	if (!this->GetExecute())return vel;

	//開始フラグがオン
	if (m_start)
	{
		//Yベクトルが最大以上の時
		if (m_length.y <= m_maxVel.y)
		{
			//フラグを切る
			m_start = false;

			//フラグを立てる
			m_finish = true;
		}
		else
		{
			//ベクトルを代入する
			vel = m_dirVel;

			//ベクトルを補正する
			vel *= m_scalar;

			//ベクトルを加算する
			m_length += vel;
		}
	}

	//終了フラグがオン
	if (m_finish)
	{
		//360℃回転するたびに、向きを補正する
		if ((int)m_angle % 360 == 0 && (int)m_angle != NULL)
		{
			//向きのYベクトルが0より大きいとき
			if (m_dirVel.y > 0.0f)
			{
				//向きのベクトルを補正する
				m_dirVel.y -= 0.01f;
			}
			else
			{
				//小さいときは、終了フラグを切る
				m_finish = false;
			}
		}

		//角度を更新する
		m_angle += 36.0f;

		//回転行列を生成
		Matrix rot = Matrix::CreateRotationZ(XMConvertToRadians(m_angle));

		//ベクトルを回転させる
		vel = Vector3::Transform(m_dirVel, rot);

		//ベクトルを補正する
		vel *= m_scalar;

		//ベクトルを加算する
		m_length += vel;

	}

	//ベクトルを返す
	return vel;
}

void HomingState::InitState(const DirectX::SimpleMath::Vector3& dir)
{
	//距離を初期化する
	m_length = Vector3::Zero;

	//フラグを初期化する
	m_start = false;
	m_finish = false;

	//角度を初期化する
	m_angle = 0.0f;

	//ベクトルの向きを初期化する
	m_dirVel = dir;
}
