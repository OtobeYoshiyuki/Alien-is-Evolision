#pragma once
#include <vector>
#include "Effect/SoapBabble/SoapBabble.h"
#include "TemplateState.h"
#include "OtobeLib/FrameWork/Owner/Owner.h"
#include "GameObject/MoveState/MoveState.h"
#include "GameObject/MoveState/JumpState/JumpState.h"
#include "GameObject/MoveState/AutoMoveState/AutoMoveState.h"

//�O���錾�i���݃C���N���[�h���邽�߁j
class SuperEnemy;

//�Œ�C��N���X
class FixedTurret:public OtobeLib::Owner<SuperEnemy>
{
public:
	//�q���̐�
	static const int CHILD_NUM = 10;
	//��������
	static const float SURVIVAL_TIME;
private:
	//�S�Ă̖A�̃R���e�i
	std::vector<SoapBabble*>	m_allBabble;

	//�e�̖A
	SoapBabble*					m_parentBabble;

	//�q���̖A
	std::vector<SoapBabble*>	m_childBabbles;

	//MoveState�̃R���e�i
	std::vector<MoveState*>		m_StateList;

	//JumpState
	std::unique_ptr<JumpState>	m_JumpState;

	//AutoMoveState
	std::unique_ptr<AutoMoveState> m_AutoMoveState;

	//�A�̐�������
	float							m_SurvivalTime;

	//�e�����˂��ꂽ�t���O
	bool							m_bulletFiring;
public:
	//�R���X�g���N�^
	FixedTurret();

	//�f�X�g���N�^
	~FixedTurret() = default;

	//������
	void Initialize();

	//�X�V����
	void Update();

public:
	//���L�҂̃|�C���^��ݒ肷��
	void SetOwner(SuperEnemy* enemy)override { Owner::SetOwner(enemy); }

	//���L�҂̃|�C���^���擾����
	SuperEnemy* GetOwner()override { return Owner::GetOwner(); }

public:
	//�S�Ă̖A���擾����
	std::vector<SoapBabble*>& GetBabbleList() { return m_allBabble; }

	//�e�̖A���擾����
	SoapBabble* GetParentBabble() { return m_parentBabble; }

	//�q���̖A���擾����
	std::vector<SoapBabble*>& GetChildBabble() { return m_childBabbles; }

	//JumpState���擾����
	JumpState* GetJumpState() { return m_JumpState.get(); }

	//AutoMoveState���擾����
	AutoMoveState* GetAutoMoveState() { return m_AutoMoveState.get(); }

public:
	//������U��
	void RandShake(SoapBabble* child);

	//���ˍς݂̃t���O��ݒ肷��
	void SetFring(const bool& flag) { m_bulletFiring = flag; }

	//��������������
	void InitTurret();

private:
	//�X�e�[�g�̍X�V����
	void UpdateState();

	//�A�̍X�V����
	void UpdateBabble();
};

