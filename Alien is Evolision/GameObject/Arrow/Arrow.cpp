#include "pch.h"
#include "Arrow.h"
#include "Utils.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace OtobeLib;

//�R���X�g���N�^
Arrow::Arrow(ITextureBase* texture):
	TextureBase(texture),//���N���X�ɏ��𑗐M����
	m_maxScale(Vector3(1.0f)),//�g��{���̍ő�
	m_minScale(Vector3(1.0f)),//�g��{���̍ŏ�
	m_frameVel(Vector3::Zero),//1�t���[���ɉ��Z����x�N�g��
	m_direction(0)//�x�N�g���̌���
{
}

//����������
void Arrow::Initialize()
{
}

//�X�V����
void Arrow::Update()
{
	//�摜�̍X�V����
	TextureBase::Update();

	//�X�P�[�����O�̍X�V
	this->UpdateScale();
}

//�`�揈��
void Arrow::Render()
{
	//�摜�̕`�揈��
	TextureBase::Render();
}

//�I������
void Arrow::Finalize()
{
}

//Arrow�̏���������
void Arrow::InitArrow
(
	const Vector3& now,//���݂̊g��{��
	const Vector3& max,//�X�P�[�����O����g��{���̍ő�
	const Vector3& min,//�X�P�[�����O����g��{���̍ŏ�
	const float& time,//�X�P�[�����O���鎞��(1�b��60�Ƃ��Ĉ���)
	ARROWSCALE arrowState//���݂̃X�e�[�g��ݒ肷��
)
{
	//�g��{����ݒ肷��
	this->SetScale(now);

	//�g��{���̍ő��ݒ肷��
	m_maxScale = max;

	//�g��{���̍ŏ���ݒ肷��
	m_minScale = min;

	//�x�N�g���̈ړ��ʂ����߂�
	Vector3 len = max - min;

	//1�t���[���̃x�N�g����ݒ肷��
	m_frameVel = len / Vector3(time);

	//���݂̃X�e�[�g��ݒ肷��
	this->SetNowState(arrowState);
}

void Arrow::UpdateScale()
{
	//Switch���ŏ�����؂�ւ���
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
	//�x�N�g���̌�����ݒ肷��
	m_direction = POSITIVE;

	//���݂̊g��{�����擾����
	Vector3 scale = this->GetScale();

	//������␳�����x�N�g�����v�Z����
	Vector3 dirVel = m_frameVel * m_direction;

	//�X�P�[�����O���ő�̏ꍇ
	if (RargeVector(scale, m_maxScale))
	{
		//�X�e�[�g��؂�ւ���
		this->SetNowState(ARROWSCALE::DOWNSCALE);

		//�X�P�[����␳����
		scale = m_maxScale;

		//�X�P�[�����O�͍s��Ȃ�
		return;
	}

	//�X�P�[�����O���s��
	scale += dirVel;

	//�g��{�����Đݒ肷��
	this->SetScale(scale);

}

void Arrow::State_DownScale()
{
	//�x�N�g���̌�����ݒ肷��
	m_direction = NEGATIVE;

	//���݂̊g��{�����擾����
	Vector3 scale = this->GetScale();

	//������␳�����x�N�g�����v�Z����
	Vector3 dirVel = m_frameVel * m_direction;

	//�X�P�[�����O���ő�̏ꍇ
	if (SmallVector(scale, m_minScale))
	{
		//�X�e�[�g��؂�ւ���
		this->SetNowState(ARROWSCALE::UPSCALE);

		//�X�P�[����␳����
		scale = m_minScale;

		//�X�P�[�����O�͍s��Ȃ�
		return;
	}

	//�X�P�[�����O���s��
	scale += dirVel;

	//�g��{�����Đݒ肷��
	this->SetScale(scale);
}
