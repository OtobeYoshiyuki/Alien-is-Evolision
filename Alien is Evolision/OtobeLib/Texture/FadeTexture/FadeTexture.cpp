#include "pch.h"
#include "FadeTexture.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace OtobeLib;

//フェードの加算速度の初期値
const float FadeTexture::DEFALT_FADE_SPEED = 0.01f;

//フェードのアルファ値の最大値(初期値)
const float FadeTexture::DEFALT_MAX_ALFA = 1.0f;

//フェードのアルファ値の最小値
const float FadeTexture::DEFALT_MIN_ALFA = 0.5f;

FadeTexture::FadeTexture(ITextureBase* texture):
	TextureBase(texture),//基底クラスに情報を送信する
	m_fadeSpeed(DEFALT_FADE_SPEED),//フェードの加算速度
	m_maxAlfa(DEFALT_MAX_ALFA),//フェードのアルファ値の最大値
	m_minAlfa(DEFALT_MIN_ALFA)//フェードのアルファ値の最小値
{
	//現在のステートを設定する
	this->SetNowState(FADE_STATE::NONE);
}

void FadeTexture::Initialize()
{
}

void FadeTexture::Update()
{
	//Textureの更新処理
	TextureBase::Update();

	//背景の色情報を取得する
	Color color = this->GetColor();
	float alfa = color.A();

	//ステートによって、処理を切り替える
	switch (this->GetNowState())
	{
	case FADE_STATE::DOWN:
		if (color.A() <= m_minAlfa)this->SetNowState(FADE_STATE::FUP);
		else alfa -= m_fadeSpeed;
		break;
	case FADE_STATE::FUP:
		if (color.A() >= m_maxAlfa)this->SetNowState(FADE_STATE::DOWN);
		else alfa += m_fadeSpeed;
		break;
	}

	//色情報を再設定する
	color.A(alfa);
	this->SetColor(color);
}

void FadeTexture::Render()
{
	//Textureの描画処理
	TextureBase::Render();
}

void FadeTexture::Finalize()
{
}
