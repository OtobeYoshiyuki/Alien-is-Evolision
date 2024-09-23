#pragma once
#include "TechniqueRecovery/TechniqueRecovery.h"
#include <vector>
#include "TemplateState.h"
class MasicEffectManager:public State<int>
{
public:
	//MP,SPECIAL�̃G�t�F�N�g�̍ő吔
	static const int MAX_MASIC_EFFECT = 4;

	//�R�}���h�Z�p�̑ҋ@����
	static const float WAIT_TIME_COMMAND;

	//�K�E�Z�p�̑ҋ@����
	static const float WAIT_TIME_SPECIAL;

	//���W��ݒ肷��ۂ̏㉺
	static const float POSITION_STANDING_WIDTH;

	//�R�}���h�Z�G�t�F�N�g�̖��O
	static const std::string EFFECT_NAME_COMMAND;

	//�K�E�Z�G�t�F�N�g�̖��O
	static const std::string EFFECT_NAME_SPECIAL;
private:
	//�R�}���h�Z�p
	std::vector<TechniqueRecovery*>	m_commandRecoverys;

	//�K�E�Z�p
	std::vector<TechniqueRecovery*>	m_specialRecoverys;
public:
	//�R���X�g���N�^
	MasicEffectManager();

	//�f�X�g���N�^
	~MasicEffectManager() = default;

	//������
	void Initialize(OtobeLib::ITextureBase* owner);

	//�X�V
	void Update();

public:
	//���݂̏�Ԃ�ݒ肷��
	void SetNowState(const int& now)override { State::SetNowState(now); };

	//���݂̏�Ԃ��擾����
	const int& GetNowState()const override { return State::GetNowState(); };

	//�ЂƂO�̏�Ԃ��擾����
	const int& GetPreState()const override { return State::GetPreState(); };

	//���݂̃G�t�F�N�g���Đ�����
	void NowEffectPlay(const int& mpheal,const int& specialheal);

private:
	void CreateEffect(OtobeLib::ITextureBase* owner, const std::string& name,const DirectX::XMVECTORF32& color,
		const float& waitTimer,std::vector<TechniqueRecovery*>& recoveryArray,const int& dir);
};

