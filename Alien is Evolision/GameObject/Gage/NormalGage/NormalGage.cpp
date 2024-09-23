#include "pch.h"
#include "NormalGage.h"
#include "GameContext.h"
#include "GameObject/Player/NormalTechnique/NormalTechnique.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace OtobeLib;

NormalGage::NormalGage(ITextureBase* texture):TextureBase(texture),m_Correction(1)
{
}

void NormalGage::Initialize()
{
	//補正値の初期化を行う
	m_Correction = 1;

	//切り取り位置の初期化を行う
	this->SetRect(RECT{ 0,0,0,20 });

	//ステートの初期化を行う
	this->SetNowState(NORMALSTATE::LEFTUP);
}

void NormalGage::Update()
{
	//画像の更新処理
	TextureBase::Update();
}

void NormalGage::Render()
{
	//画像の更新処理
	TextureBase::Render();
}

void NormalGage::Finalize()
{
}

void NormalGage::ChangeGage()
{
	//NormalTechniqueを取得する
	NormalTechnique* Ntch = GameContext::Get<NormalTechnique>();

	//先頭の弾を取得する
	NormalBullet* front = Ntch->GetFrontBullet();

	//拡大倍率を取得する
	Vector3 scale = front->GetScale();

	//画像の切り取り位置を取得する
	RECT rect = this->GetRect();

	//画像の終点Xを取得する
	int rectFx = rect.right;

	if (Ntch->GetAllBulletFront() == nullptr)return;

	//Switch文で処理を分ける
	switch (this->GetNowState())
	{
		//終点Xで比較を行う
	case NORMALSTATE::LEFTUP:
		if (rectFx > MAX_GAGE)
		{
			//補正して、ステートを変更する
			m_Correction = -1;
			this->SetNowState(NORMALSTATE::RIGHTUP);
		}
		break;
	case NORMALSTATE::RIGHTUP:
		if (rectFx < MIN_GAGE)
		{
			//補正して、ステートを変更する
			m_Correction = 1;
			this->SetNowState(NORMALSTATE::LEFTUP);
		}
		break;
	}

	float f = m_Correction;

	//補正値をもとに、終点を変更する
	rectFx += m_Correction;

	//拡大倍率を設定する
	front->SetScale(scale += Vector3(m_Correction / 70.0f));
	
	//現在の切り取り位置を変更する
	this->SetRect(RECT{ 0,0,rectFx,20 });
}
