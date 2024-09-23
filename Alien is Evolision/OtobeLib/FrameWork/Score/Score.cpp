#include "pch.h"
#include "Score.h"
#include "OtobeLib/FrameWork/SoundManager/SoundManager.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace OtobeLib;

Score::Score():
	m_score(NULL),//�X�R�A
	m_saving(NULL),//���~
	m_add(1),//���Z��(�f�t�H���g�ł�1)
	m_soundCheck(false)//���̒�~�𖳎�����t���O
{
}

void Score::Initialize()
{
}

void Score::Update()
{
	//SoundManager���擾����
	SoundManager* soundManager = SoundManager::GetInstance();

	//���~����X�R�A�����Z���Ă���
	if (m_saving > 0.0f)
	{
		//�X�R�A���C���N�������g
		m_score += m_frameSave * m_add;

		//���~���g��
		m_saving -= m_frameSave;

		if (m_saving < 0.0f)
		{
			float sav = fabsf(m_saving);
			m_score -= sav;
			m_score = (int)m_score;
			m_saving = 0.0f;
			m_frameSave = 0.0f;
		}
	}
	else
	{
		if (!m_soundCheck)
		{
			//�����~�߂�
			soundManager->Stop(CRI_CUESHEET_0__19_�X�R�A���Z);
		}
	}

	//�X�R�A��␳����
	if (m_score < NULL)
	{
		m_score = NULL;
	}
}
