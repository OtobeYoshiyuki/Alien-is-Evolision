#include "pch.h"
#include "Ranking.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace OtobeLib;

Ranking::Ranking() :
	m_level(""),//���x��
	m_score(nullptr),//�X�R�A
	m_timer(nullptr),//�^�C�}�[
	m_blink(nullptr),//�_��
	m_color(Colors::White)//�F
{
	//�X�R�A�̐���
	m_score = std::make_unique<Score>();
	m_score->SetSoundCheck(true);
	
	//�^�C�}�[�̐���
	m_timer = std::make_unique<TimerJudge>();

	//�_�ł̐���
	m_blink = std::make_unique<Blinking>();
	m_blink->SetTarget(0.5f);
	
	//�F�̐ݒ�
	m_color = Colors::Black;
}

void Ranking::Update()
{
	//�X�R�A�̍X�V
	m_score->Update();

	//�^�C�}�[�̍X�V
	m_timer->Update();

	//�_�ł̍X�V
	m_blink->Update();
}
