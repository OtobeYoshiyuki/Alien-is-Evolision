#include "pch.h"
#include "MasicEffectManager.h"
#include "OtobeLib/FrameWork/ResourceManager/ResourceManager.h"
#include "OtobeLib/StackScene/Scene/Play/PlayScene.h"
#include "OtobeLib/FrameWork/SoundManager/SoundManager.h"
#include "GameContext.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace OtobeLib;

//�R�}���h�Z�p�̑ҋ@����
const float MasicEffectManager::WAIT_TIME_COMMAND = 1.0f;

//�K�E�Z�p�̑ҋ@����
const float MasicEffectManager::WAIT_TIME_SPECIAL = 2.0f;

//���W��ݒ肷��ۂ̏㉺
const float MasicEffectManager::POSITION_STANDING_WIDTH = 20.0f;

//�R�}���h�Z�G�t�F�N�g�̖��O
const std::string MasicEffectManager::EFFECT_NAME_COMMAND = "�R�}���h�G�t�F�N�g";

//�K�E�Z�G�t�F�N�g�̖��O
const std::string MasicEffectManager::EFFECT_NAME_SPECIAL = "�K�E�Z�G�t�F�N�g";

MasicEffectManager::MasicEffectManager()
{
}

void MasicEffectManager::Initialize(ITextureBase* owner)
{
	//Player���擾����
	CoroKun* player = GameContext::Get<CoroKun>();

	//�z��̓Y������ݒ肷��
	this->SetNowState(NULL);

	for (int i = 0; i < MAX_MASIC_EFFECT; i++)
	{
		//�R�}���h�Z�p�̃e�N�X�`���𐶐�����
		this->CreateEffect(owner, EFFECT_NAME_COMMAND,
			Colors::Blue, WAIT_TIME_COMMAND, m_commandRecoverys,-1);

		//�K�E�Z�p�̃e�N�X�`���𐶐�����
		this->CreateEffect(owner, EFFECT_NAME_SPECIAL,
			Colors::Red, WAIT_TIME_SPECIAL, m_specialRecoverys,1);
	}
}

void MasicEffectManager::Update()
{
}

void MasicEffectManager::NowEffectPlay(const int& mpheal, const int& specialheal)
{
	//���݂̃G�t�F�N�g���Đ�����

	//SoundManager���擾����
	SoundManager* soundManager = SoundManager::GetInstance();

	//SE��炷
	soundManager->Play(CRI_CUESHEET_0__32_�A�C�e���ǐ�);

	//�Y�������擾����
	int effect = this->GetNowState();

	//�Ώۂ̃G�t�F�N�g���擾����
	TechniqueRecovery* techRecovery = m_commandRecoverys[effect];
	TechniqueRecovery* deathRecovery = m_specialRecoverys[effect];

	//�񕜗ʂ�ݒ肷��
	techRecovery->SetHeal(mpheal);
	deathRecovery->SetHeal(specialheal);

	//���W��ݒ肷��
	techRecovery->SetPosition(techRecovery->GetOwner()->GetPosition() + Vector3(0.0f, -POSITION_STANDING_WIDTH, 0.0f));
	deathRecovery->SetPosition(techRecovery->GetOwner()->GetPosition() + Vector3(0.0f, POSITION_STANDING_WIDTH, 0.0f));

	//�G�t�F�N�g�̃X�e�[�g��ݒ肷��
	techRecovery->SetNowState(TARGET_STATE::TRACKING);
	deathRecovery->SetNowState(TARGET_STATE::TRACKING);

	//�o���l�̒ǔ��p�̃X�e�[�g���N����
	techRecovery->GetTrackingState()->SetExecute(true);
	deathRecovery->GetTrackingState()->SetExecute(true);

	//�o���l�̃z�[�~���O�p�̃X�e�[�g���N����
	techRecovery->GetHomingState()->SetExecute(true);
	techRecovery->GetHomingState()->SetStartFlag(true);
	deathRecovery->GetHomingState()->SetExecute(true);
	deathRecovery->GetHomingState()->SetStartFlag(true);

	//�z�[�~���O�p�̃X�e�[�g������������
	techRecovery->GetHomingState()->InitState(Vector3(0.0f, -5.0f, 0.0f));
	deathRecovery->GetHomingState()->InitState(Vector3(0.0f, 5.0f, 0.0f));

	//�`����N����
	techRecovery->SetState(ITextureBase::STATE::DRAW);
	deathRecovery->SetState(ITextureBase::STATE::DRAW);

	//�z��̖����܂ŗ�����A�擪�ɖ߂�
	//����ȊO�̏ꍇ�́A�z����X�V����
	if (effect >= MAX_MASIC_EFFECT - 1)effect = 0;
	else effect++;

	//�Y������ݒ肷��
	this->SetNowState(effect);
}

void MasicEffectManager::CreateEffect
(
	ITextureBase* owner,//���L��
	const std::string& name,//���O
	const DirectX::XMVECTORF32& color,//�摜�̐F
	const float& waitTimer,//�҂�����
	std::vector<TechniqueRecovery*>& recoveryArray,//�Ώۂ̔z��(std::vector<TechniqueRecovery*>)
	const int& dir//�x�N�g���̌���(1���|1���w��)
)
{
	//ResourceManager���擾����
	ResourceManager* resourceManager = ResourceManager::GetInstance();

	//TextureManager���擾����
	TextureManager* gameManager = GameContext::Get<PlayScene>()->GetGameManager().get();

	//Player���擾����
	CoroKun* player = GameContext::Get<CoroKun>();

	//�g�p����摜���擾����
	ITextureBase* useTexture = resourceManager->GetTexture("�Q�[�W�G�t�F�N�g");

	//�e�N�X�`���𐶐�����
	std::unique_ptr<TechniqueRecovery> pcommondEffect = std::make_unique<TechniqueRecovery>(useTexture);

	//���O��ݒ肷��
	pcommondEffect->SetName(name);

	//�F��ݒ肷��
	pcommondEffect->SetColor(color);

	//�`��̗L����ݒ肷��
	pcommondEffect->SetState(ITextureBase::STATE::NONE);

	//�҂����Ԃ�ݒ肷��
	pcommondEffect->SetWaitTimer(waitTimer);

	//���L�҂̃|�C���^��ݒ肷��
	pcommondEffect->SetOwner(owner);

	//�x�N�g���̕␳�l��ݒ肷��
	pcommondEffect->GetTrackingState()->SetCorrection(Vector3(30.0f));

	//�ǐՖڕW��ݒ肷��
	pcommondEffect->GetTrackingState()->SetTarget(&player->GetPosition());

	//���g�̍��W��ݒ肷��
	pcommondEffect->GetTrackingState()->SetThis(&pcommondEffect->GetPosition());

	//�x�N�g���̌�����ݒ肷��
	pcommondEffect->GetHomingState()->SetDirVel(Vector3(0.0f, 5.0f, 0.0f) * dir);

	//�z��ɐ��|�C���^��ǉ�����
	recoveryArray.push_back(pcommondEffect.get());

	//TextureManager�ɒǉ�����
	gameManager->Add(std::move(pcommondEffect));
}
