#include "pch.h"
#include "ExperienceGage.h"
#include "OtobeLib/StackScene/Scene/Play/PlayScene.h"
#include "Effect/LevelUp/LevelUpState.h"
#include "GameObject/Player/CoroKun.h"
#include "GameContext.h"
#include "OtobeLib/Status/StatusCalc/StatusCalc.h"
#include "OtobeLib/FrameWork/SoundManager/SoundManager.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace OtobeLib;

ExperienceGage::ExperienceGage(ITextureBase* texture) :
	TextureBase(texture),//���N���X�ɏ��𑗐M����
	m_Max(NULL),//�摜�̏c�����̍ő�l
	m_calcSpeed(NULL),//1�t���[���P�ʂ̌v�Z�l
	m_ratio(0.0f)//�o���l�Ɖ摜�̔䗦
{
}

void ExperienceGage::Initialize()
{
}

void ExperienceGage::Update()
{
	//�X�e�[�g�̍X�V����
	this->UpdateState();

	//�摜�̍X�V����
	TextureBase::Update();
}

void ExperienceGage::Render()
{
	//�摜�̕`�揈��
	TextureBase::Render();
}

void ExperienceGage::Finalize()
{
}

const bool ExperienceGage::CheckExp() const
{
	//�擾����o���l���Ȃ��ꍇ
	if (m_expList.empty() || m_expList.front() <= 0.0f)
	{
		return false;
	}

	//�o���l�����݂���ꍇ
	return true;
}

void ExperienceGage::UpdateState()
{
	//Switch���ŏ����𕪂���

	switch (this->GetNowState())
	{
	case CALC::CALCING:
		this->State_Calcking();
		break;
	case CALC::STOP:
		this->State_Stop();
		break;
	}
}

void ExperienceGage::State_Calcking()
{
	//SoundManager���擾����
	SoundManager* soundManager = SoundManager::GetInstance();

	//�擾����o���l���Ȃ��ꍇ�́A�������Ȃ�
	if (!this->CheckExp())
	{
		soundManager->Stop(CRI_CUESHEET_0__33_�Q�[�W�㏸);
		return;
	}

	//�v���C���[���擾����
	CoroKun* player = GameContext::Get<CoroKun>();

	//RECT���擾����
	RECT rect = this->GetRect();

	//�ŏ��̌o���l���擾����
	int& exp = m_expList.front();

	//�v���C���[�̃��x�����������E�ɒB���Ă��Ȃ��Ƃ�
	if (player->GetLevel() < CoroKun::MAX_LEVEL)
	{
		//�o���l��~�ς���
		rect.right += m_calcSpeed;

		//�o���l���ő�l�𒴂�����
		if (rect.right > m_Max)
		{
			//�v���C���[�̃��x����19�̎��́A���x���A�b�v�̍ۂɃQ�[�W���ő�ɌŒ肷��
			if (player->GetLevel() < CoroKun::MAX_LEVEL - 1)
			{
				//�o���l������������
				rect.right = NULL;

				//HP�Q�[�W�̃A�j���[�V�������J�n����
				player->SetDamageCheck(true);

				//HP�Q�[�W�̗��ʂ�ݒ肷��
				ITextureBase* backHpTexture = player->GetHpGage()->GetBackGageTexture();

				//HP�Q�[�W�̗��ʂ̐F��ݒ肷��
				backHpTexture->SetColor(Colors::Aqua);

				//���Ԃ�ݒ肷��
				player->GetHpGage()->SetEasingTime(0.0f);
			}
			else {
				//�o���l���Œ肷��
				rect.right = m_Max;
			}

			//�X�e�[�g��؂�ւ���
			this->SetNowState(CALC::STOP);

			//���x���A�b�v�O�̃X�e�[�^�X���擾����
			StatusImporter* importer1 = StatusBase::FindStatus(player->GetName(), player->GetLevel());

			//���x���A�b�v��̃X�e�[�^�X���擾����
			StatusImporter* importer2 = StatusBase::FindStatus(player->GetName(), player->GetLevel() + 1);

			//HP�̍��������߂�
			int difference1 = importer2->GetHp() - importer1->GetHp();

			//MP�̍��������߂�
			int difference2 = importer2->GetMp() - importer1->GetMp();

			//Special�̍��������߂�
			int difference3 = importer2->GetSpecial() - importer1->GetSpecial();

			//�X�e�[�^�X���Đݒ肷��
			player->SetStatus(player->GetHP(), player->GetMP(), importer2->GetAtack(), importer2->GetDefence(),
				player->GetSpecial(), importer2->GetLevel(), player->GetNextLevelPoint());

			player->SetMaxStatus(importer2->GetHp(), importer2->GetMp(), importer2->GetAtack(), importer2->GetDefence(),
				importer2->GetSpecial(), CoroKun::MAX_LEVEL, importer2->GetNextLevelPoint());

			//HP�̐؂���ʒu���擾����
			RECT hpRect = player->GetHpGage()->GetRect();

			//MP�̐؂���ʒu���擾����
			RECT mpRect = player->GetTechniqueGage()->GetRect();

			//Special�̐؂���ʒu���擾����
			RECT spRect = player->GetDeathblowGage()->GetRect();

			//���݂�HP���擾����
			int hp = player->GetHP() + difference1;

			//���݂�MP���擾����
			int mp = player->GetMP() + difference2;

			//���݂̃X�y�V�����|�C���g���擾����
			int special = player->GetSpecial() + difference3;

			//HP����␳����
			StatusCalc::CalcDamage(hp, importer2->GetHp(), importer1->GetHp(),
				player->GetHpGage()->GetMaxTexLength(), player->GetMaxHP(), hpRect.right);

			//MP�����C������
			StatusCalc::CalcDamage(mp, importer2->GetMp(), importer1->GetMp(),
				player->GetTechniqueGage()->GetMaxTexLength(), player->GetMaxMP(), mpRect.right);

			//SP�����C������
			StatusCalc::CalcDamage(special, importer2->GetSpecial(), importer1->GetSpecial(),
				player->GetDeathblowGage()->GetMaxTexLength(), player->GetMaxSpecialPoint(), spRect.right);

			//HP��ݒ肷��
			player->SetHP(hp);

			//MP��ݒ肷��
			player->SetMP(mp);

			//�X�y�V�����|�C���g��ݒ肷��
			player->SetSpecial(special);

			//�摜�̕���ݒ肷��
			player->GetHpGage()->SetRect(hpRect);
			player->GetTechniqueGage()->SetRect(mpRect);
			player->GetDeathblowGage()->SetRect(spRect);

			for (int& e : m_expList)
			{
				//�o���l�̒l�����ɖ߂�
				e /= m_ratio;
			}

			//�䗦���Đݒ肷��
			m_ratio = (float)m_Max / (float)player->GetMaxNextLevelPoint();

			for (int& e : m_expList)
			{
				//�o���l���Đݒ肷��
				e *= m_ratio;
			}

		}
		
		//�摜�̐؂���ʒu��ݒ肷��
		this->SetRect(rect);

		//�o���l�𓾂�
		exp -= m_calcSpeed;

		if (exp <= 0.0f)
		{
			//�o���l�����X�g����폜����
			m_expList.pop_front();
		}
	}
}

void ExperienceGage::State_Stop()
{
	//���x���A�b�v���擾����
	LevelUp* levelUp = GameContext::Get<PlayScene>()->GetLevelUp();

	//�X�e�[�g��؂�ւ���
	if (levelUp->GetNowState() == LEVELUP::NONE)levelUp->SetNowState(LEVELUP::LEVELUP);
}

