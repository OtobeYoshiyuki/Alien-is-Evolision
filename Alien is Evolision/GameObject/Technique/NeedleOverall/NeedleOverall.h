#pragma once
#include "TemplateState.h"
#include "NeedleAction.h"
#include <vector>
#include "GameObject/Technique/Needle/Needle.h"

class NeedleOverall:public State<NEEDLEACTION>
{
public:
	//�������p��Needle�̐�
	static const int NEEDLERANGE_NUM = 13;

	//�Q�[�W�̉񕜗�(�R�}���h�Z)
	static const int TECHNIQUE_HEAL_COMMAND = 45;

	//�Q�[�W�̉񕜗�(�K�E�Z)
	static const int TECHNIQUE_HEAL_SPECIAL = 60;

	//�擾�X�R�A
	static const int BASE_SCORE = 450;

	//�R�}���h�Z�̃Q�[�W�̏����
	static const int GAGE_COST = 100;
private:
	//�L�͈͗p�̐j�̃R���e�i
	std::vector<Needle*> m_NeedleList;

	//�X�V�֐��t���O
	bool				m_Active;
public:
	//�R���X�g���N�^
	NeedleOverall();

	//�f�X�g���N�^
	~NeedleOverall(){};

	//������
	void Initialize();

	//�X�V
	void Update();

private:

	//���ˏ����̃X�e�[�g
	void State_Mope();

	//�Ƃ��L���m�����˂̃X�e�[�g
	void State_Release();

	//�Ƃ��L���m���̃����[�h
	void State_Reload();

public:

	//���݂̏�Ԃ�ݒ肷��
	void SetNowState(const NEEDLEACTION& now)override { State::SetNowState(now); };

	//���݂̏�Ԃ��擾����
	const NEEDLEACTION& GetNowState()const override { return State::GetNowState(); };

	//�ЂƂO�̏�Ԃ��擾����
	const NEEDLEACTION& GetPreState()const override { return State::GetPreState(); };

public:

	//�L�͈͗p�̐j�̃R���e�i���擾����
	std::vector<Needle*>& GetNeedleList() { return m_NeedleList; }

	//�X�V�֐��t���O��ݒ肷��
	void SetActive(const bool& active) { m_Active = active; }

	//�X�V�֐��t���O���擾����
	const bool& GetActive()const { return m_Active; }
};

