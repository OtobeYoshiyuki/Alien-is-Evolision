#include "pch.h"
#include "StatusCalc.h"
#include "OtobeLib/FrameWork/Rand/Rand.h"


using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace OtobeLib;

//�_���[�W���v�Z����֐�
void StatusCalc::CalcDamage(
	int& nowhp,//���݂�HP
	const int& atack,//�U����
	const int& defence,//�h���
	const int& texLength,//�摜�̕�(�ő�l)
	const int& maxhp,//�ő�HP
	LONG& element,//�摜�̕�
	float correction//�_���[�W�̕␳�l
)
{
	//�_���[�W�l���v�Z����
	float damage = (float)atack - (float)defence;

	//���g�̖h��͂̕���������΁A�_���[�W�l���C������
	if (damage <= NULL)damage = 1.0f;

	//�v�Z�����_���[�W�l�ɕ␳�l����Z����
	damage *= correction;

	//HP�Ƀ_���[�W��^����
	nowhp -= (int)damage;

	//HP��0��菬������΁A�C������
	if (nowhp < NULL)nowhp = NULL;

	//HP�o�[�̉摜�Ǝ��ۂ�HP�̒l�̔䗦�����߂�
	float ratio = (float)texLength / (float)maxhp;

	//�_���[�W�̒l��␳����
	damage *= ratio;

	//HP�Q�[�W�Ƀ_���[�W��^����
	element -= damage;

	//�Q�[�W��0��菬�����Ȃ�����A0�ɂ���
	if (element < 0)element = 0;
}

void StatusCalc::CalcExp(
	const int& level,//���x��
	const float& strength,//����
	const int& rand_max,//�����̍ő�l
	const int& texLength,//�e�N�X�`���̕��̍ő�l
	const int& mapep,//�o���l�̍ő�l
	int& Exp,//���ۂ̌o���l
	int& texexp,//�摜��̌o���l
	float& ratio//�o���l�̔䗦
)
{
	//�����N���X���擾����
	Rand* random = Rand::GetInstance();

	//�o���l�̗�����U��
	Exp = random->RandomMachine(0, rand_max);

	//HP�o�[�̉摜�Ǝ��ۂ�HP�̒l�̔䗦�����߂�
	ratio = (float)texLength / (float)mapep;

	//�o���l�́A�����ɓG�̋����ƃ��x�����|�����l�ɁA�U���������̒l�𑫂������̂Ƃ���
	Exp += level * (int)strength;

	//�o���l���摜�̔䗦�ɍ��킹��
	texexp = Exp * ratio;
}

void StatusCalc::CalcHeal
(
	int& status,//�Ώۂ̃X�e�[�^�X
	int& heal,//�񕜒l
	const int& texLength,//�摜�̉���
	const int& maxLength,//�Ώۂ̃X�e�[�^�X�̍ő�l
	LONG& element,//�Ώۂ�RECT
	float corrrection//�␳�l
)
{
	//�񕜒l���X�e�[�^�X�̍ő�l���傫����΁A�␳����
	if (heal > maxLength - status)heal = maxLength - status;

	//�v�Z�����񕜒l�ɕ␳�l����Z����
	heal *= corrrection;

	//�X�e�[�^�X���񕜂�����
	status += heal;

	//�X�e�[�^�X���ő�l�𒴂��Ă�����A�C������
	if (status > maxLength)status = maxLength;

	//HP�o�[�̉摜�Ǝ��ۂ�HP�̒l�̔䗦�����߂�
	float ratio = (float)texLength / (float)maxLength;

	//�񕜒l��␳����
	float grapHeal = (float)heal * ratio;

	//HP�Q�[�W���񕜂�����
	element += grapHeal;

	//�Q�[�W���ő�l���傫����΁A�ő�l�ɏC������
	if (element > texLength)element = texLength;
}
