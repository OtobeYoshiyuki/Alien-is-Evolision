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
	//�␳�l�̏��������s��
	m_Correction = 1;

	//�؂���ʒu�̏��������s��
	this->SetRect(RECT{ 0,0,0,20 });

	//�X�e�[�g�̏��������s��
	this->SetNowState(NORMALSTATE::LEFTUP);
}

void NormalGage::Update()
{
	//�摜�̍X�V����
	TextureBase::Update();
}

void NormalGage::Render()
{
	//�摜�̍X�V����
	TextureBase::Render();
}

void NormalGage::Finalize()
{
}

void NormalGage::ChangeGage()
{
	//NormalTechnique���擾����
	NormalTechnique* Ntch = GameContext::Get<NormalTechnique>();

	//�擪�̒e���擾����
	NormalBullet* front = Ntch->GetFrontBullet();

	//�g��{�����擾����
	Vector3 scale = front->GetScale();

	//�摜�̐؂���ʒu���擾����
	RECT rect = this->GetRect();

	//�摜�̏I�_X���擾����
	int rectFx = rect.right;

	if (Ntch->GetAllBulletFront() == nullptr)return;

	//Switch���ŏ����𕪂���
	switch (this->GetNowState())
	{
		//�I�_X�Ŕ�r���s��
	case NORMALSTATE::LEFTUP:
		if (rectFx > MAX_GAGE)
		{
			//�␳���āA�X�e�[�g��ύX����
			m_Correction = -1;
			this->SetNowState(NORMALSTATE::RIGHTUP);
		}
		break;
	case NORMALSTATE::RIGHTUP:
		if (rectFx < MIN_GAGE)
		{
			//�␳���āA�X�e�[�g��ύX����
			m_Correction = 1;
			this->SetNowState(NORMALSTATE::LEFTUP);
		}
		break;
	}

	float f = m_Correction;

	//�␳�l�����ƂɁA�I�_��ύX����
	rectFx += m_Correction;

	//�g��{����ݒ肷��
	front->SetScale(scale += Vector3(m_Correction / 70.0f));
	
	//���݂̐؂���ʒu��ύX����
	this->SetRect(RECT{ 0,0,rectFx,20 });
}
