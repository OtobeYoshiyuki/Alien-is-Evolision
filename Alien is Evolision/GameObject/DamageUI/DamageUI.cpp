#include "pch.h"
#include "DamageUI.h"
#include "GameContext.h"
#include "GameObject/MoveState/DamageState/DamageState.h"
#include "OtobeLib/FrameWork/StaticData/StaticData.h"
#include "TemplateState.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace OtobeLib;

DamageUI::DamageUI(ITextureBase* texture) :TextureBase(texture)
{
	//���݂̃X�e�[�g��ݒ肷��
	State::SetNowState(FADE_STATE::NONE);

	//�`�揇��ύX����
	this->SetDrawOrder(-1);

	//�`���؂�
	this->SetState(STATE::NONE);
}

void DamageUI::Initialize()
{
}

void DamageUI::Update()
{
	//�e�N�X�`���̍X�V����
	TextureBase::Update();

	//�t�F�[�h�̍X�V����
	this->UpdateFade();
}

void DamageUI::Render()
{
	//�e�N�X�`���̕`�揈��
	TextureBase::Render();
}

void DamageUI::Finalize()
{
}

void DamageUI::UpdateFade()
{
	//�ÓI�f�[�^�̎擾
	StaticData* staticData = StaticData::GetInstance();

	//�_���[�W�X�e�[�g���擾����
	DamageState* damageState = GameContext::Get<DamageState>();

	//�t�F�[�h�̐F���擾����
	Color color = this->GetColor();

	//�f�[�^�������Ă邩�ǂ����m�F����
	//�f�[�^�������ĂȂ�������A�������Ȃ�
	if (!staticData->Request_Data("�v���C���[�_���[�W"))return;

	//�A�ɓ������Ă��炸�A���]��ɓ������Ă�����A�������Ȃ�
	if (staticData->Get_StringData("�v���C���[�_���[�W") == "rebound")return;

	//�f�[�^���󂾂�����A�������Ȃ�
	if (staticData->Get_StringData("�v���C���[�_���[�W") == "")return;
	
	//�t�F�[�h�̍X�V����
	//���s�֐������s���������瓧���x��������
	if (damageState->GetExecute())
	{
		this->SetNowState(FADE_STATE::DOWN);
	}

	//�����x�����Z�̎�
	if (this->GetNowState() == FADE_STATE::DOWN)
	{		
		//�`����I���ɂ���
		this->SetState(STATE::DRAW);
		//���t���[�������x�����Z����
		color = Color(color.R(), color.G(), color.B(), color.A() - 0.01f);
		//�F��ݒ肷��
		this->SetColor(color);
	}

	//�����x��0�ɂȂ�����X�e�[�g�����ɖ߂�
	if (this->GetColor().A() <= 0.0f)
	{
		//�X�e�[�g���f�t�H���g�ɖ߂�
		this->SetNowState(FADE_STATE::NONE);
		//�`���؂�
		this->SetState(STATE::NONE);
		//�F��ݒ肷��
		this->SetColor(Colors::Red);
	}
}
