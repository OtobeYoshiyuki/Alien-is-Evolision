#include "pch.h"
#include "FadeTexture.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace OtobeLib;

//�t�F�[�h�̉��Z���x�̏����l
const float FadeTexture::DEFALT_FADE_SPEED = 0.01f;

//�t�F�[�h�̃A���t�@�l�̍ő�l(�����l)
const float FadeTexture::DEFALT_MAX_ALFA = 1.0f;

//�t�F�[�h�̃A���t�@�l�̍ŏ��l
const float FadeTexture::DEFALT_MIN_ALFA = 0.5f;

FadeTexture::FadeTexture(ITextureBase* texture):
	TextureBase(texture),//���N���X�ɏ��𑗐M����
	m_fadeSpeed(DEFALT_FADE_SPEED),//�t�F�[�h�̉��Z���x
	m_maxAlfa(DEFALT_MAX_ALFA),//�t�F�[�h�̃A���t�@�l�̍ő�l
	m_minAlfa(DEFALT_MIN_ALFA)//�t�F�[�h�̃A���t�@�l�̍ŏ��l
{
	//���݂̃X�e�[�g��ݒ肷��
	this->SetNowState(FADE_STATE::NONE);
}

void FadeTexture::Initialize()
{
}

void FadeTexture::Update()
{
	//Texture�̍X�V����
	TextureBase::Update();

	//�w�i�̐F�����擾����
	Color color = this->GetColor();
	float alfa = color.A();

	//�X�e�[�g�ɂ���āA������؂�ւ���
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

	//�F�����Đݒ肷��
	color.A(alfa);
	this->SetColor(color);
}

void FadeTexture::Render()
{
	//Texture�̕`�揈��
	TextureBase::Render();
}

void FadeTexture::Finalize()
{
}
