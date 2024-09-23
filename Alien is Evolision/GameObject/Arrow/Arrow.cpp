#include "pch.h"
#include "Arrow.h"
#include "Utils.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace OtobeLib;

//コンストラクタ
Arrow::Arrow(ITextureBase* texture):
	TextureBase(texture),//基底クラスに情報を送信する
	m_maxScale(Vector3(1.0f)),//拡大倍率の最大
	m_minScale(Vector3(1.0f)),//拡大倍率の最小
	m_frameVel(Vector3::Zero),//1フレームに加算するベクトル
	m_direction(0)//ベクトルの向き
{
}

//初期化処理
void Arrow::Initialize()
{
}

//更新処理
void Arrow::Update()
{
	//画像の更新処理
	TextureBase::Update();

	//スケーリングの更新
	this->UpdateScale();
}

//描画処理
void Arrow::Render()
{
	//画像の描画処理
	TextureBase::Render();
}

//終了処理
void Arrow::Finalize()
{
}

//Arrowの初期化処理
void Arrow::InitArrow
(
	const Vector3& now,//現在の拡大倍率
	const Vector3& max,//スケーリングする拡大倍率の最大
	const Vector3& min,//スケーリングする拡大倍率の最小
	const float& time,//スケーリングする時間(1秒を60として扱う)
	ARROWSCALE arrowState//現在のステートを設定する
)
{
	//拡大倍率を設定する
	this->SetScale(now);

	//拡大倍率の最大を設定する
	m_maxScale = max;

	//拡大倍率の最小を設定する
	m_minScale = min;

	//ベクトルの移動量を求める
	Vector3 len = max - min;

	//1フレームのベクトルを設定する
	m_frameVel = len / Vector3(time);

	//現在のステートを設定する
	this->SetNowState(arrowState);
}

void Arrow::UpdateScale()
{
	//Switch文で処理を切り替える
	switch (this->GetNowState())
	{
	case ARROWSCALE::UPSCALE:
		this->State_UpScale();
		break;
	case ARROWSCALE::DOWNSCALE:
		this->State_DownScale();
		break;
	}
}

void Arrow::State_UpScale()
{
	//ベクトルの向きを設定する
	m_direction = POSITIVE;

	//現在の拡大倍率を取得する
	Vector3 scale = this->GetScale();

	//向きを補正したベクトルを計算する
	Vector3 dirVel = m_frameVel * m_direction;

	//スケーリングが最大の場合
	if (RargeVector(scale, m_maxScale))
	{
		//ステートを切り替える
		this->SetNowState(ARROWSCALE::DOWNSCALE);

		//スケールを補正する
		scale = m_maxScale;

		//スケーリングは行わない
		return;
	}

	//スケーリングを行う
	scale += dirVel;

	//拡大倍率を再設定する
	this->SetScale(scale);

}

void Arrow::State_DownScale()
{
	//ベクトルの向きを設定する
	m_direction = NEGATIVE;

	//現在の拡大倍率を取得する
	Vector3 scale = this->GetScale();

	//向きを補正したベクトルを計算する
	Vector3 dirVel = m_frameVel * m_direction;

	//スケーリングが最大の場合
	if (SmallVector(scale, m_minScale))
	{
		//ステートを切り替える
		this->SetNowState(ARROWSCALE::UPSCALE);

		//スケールを補正する
		scale = m_minScale;

		//スケーリングは行わない
		return;
	}

	//スケーリングを行う
	scale += dirVel;

	//拡大倍率を再設定する
	this->SetScale(scale);
}
