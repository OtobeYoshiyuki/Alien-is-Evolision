#pragma once
#include "TemplateState.h"
#include "ReNeedle.h"
#include <list>
#include "GameObject/Technique/Needle/Needle.h"

class Resurrection :public State<RENEEDLE>
{
public:
	//�������p��Needle�̐�
	static const int NEEDLESHORT_NUM = 13;

	//�ҋ@����U���Ɉڂ�܂ł̎���
	static const float MOVETIME;

	//�Q�[�W�̉񕜗�(�R�}���h�Z)
	static const int TECHNIQUE_HEAL_COMMAND = 40;

	//�Q�[�W�̉񕜗�(�K�E�Z)
	static const int TECHNIQUE_HEAL_SPECIAL = 50;

	//�擾�X�R�A
	static const int BASE_SCORE = 500;

	//�R�}���h�Z�̃Q�[�W�̏����
	static const int GAGE_COST = 90;
private:
	//�L�͈͗p�̐j�̃R���e�i
	std::vector<Needle*> m_NeedleList;

	//�X�V�֐��̃t���O
	bool				m_Active;

	//�^�C�}�[
	float				m_Timer;
public:
	//�R���X�g���N�^
	Resurrection();

	//�f�X�g���N�^
	~Resurrection()override {};

	//������
	void Initialize();

	//�X�V
	void Update();

public:
	//���݂̏�Ԃ�ݒ肷��
	void SetNowState(const RENEEDLE& now)override { State::SetNowState(now); };

	//���݂̏�Ԃ��擾����
	const RENEEDLE& GetNowState()const override { return State::GetNowState(); };

	//�ЂƂO�̏�Ԃ��擾����
	const RENEEDLE& GetPreState()const override { return State::GetPreState(); };

public:

	//�X�V�֐��̃t���O��ݒ肷��
	void SetActive(const bool& active) { m_Active = active; }

	//�X�V�֐��̃t���O���擾����
	const bool& GetActive()const { return m_Active; }

	//�j�[�h���̃��X�g���擾����
	std::vector<Needle*>& GetNeedleList() { return m_NeedleList; }

private:

	//�ҋ@���̃X�e�[�g�̏���
	void State_Little();

	//�U�����̃X�e�[�g�̏���
	void State_Atack();

	//�I���̃X�e�[�g�̏���
	void State_End();
};

