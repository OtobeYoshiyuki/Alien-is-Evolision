#include "pch.h"
#include "NormalEnemy.h"
#include "OtobeLib/FrameWork/ResourceManager/ResourceManager.h"
#include "GameContext.h"
#include "OtobeLib/StackScene/Scene/Play/PlayScene.h"
#include "OtobeLib/FrameWork/StaticData/StaticData.h"
#include "OtobeLib/Status/StatusCalc/StatusCalc.h"
#include "OtobeLib/FrameWork/Rand/Rand.h"
#include "GameObject/Item/Drop/ItemDrop.h"
#include "OtobeLib/FrameWork/SoundManager/SoundManager.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace OtobeLib;

//�G�̋���(�o���l�ɉe������)
const float NormalEnemy::STRENGTH = 100.0f;

NormalEnemy::NormalEnemy(ITextureBase* texture):
	TextureBase(texture),//���N���X�ɏ��𑗐M����
	m_AutoMoveState(nullptr),//�����ړ��̃X�e�[�g
	m_experience(nullptr),//�o���l
	m_HpGage(nullptr),//Hp�Q�[�W
	m_HpBack(nullptr),//Hp�Q�[�W�̗���
	m_masicEffectManager(nullptr),//�G�t�F�N�g�̊Ǘ��N���X
	m_healItem(nullptr),//�񕜃A�C�e��
	m_frameCount(NULL),//�t���[�����̃J�E���g
	m_damageCheck(false)//�_���[�W�̃`�F�b�N�t���O
{
	//ResourceManager���擾����
	ResourceManager* resourceManager = ResourceManager::GetInstance();

	//GameManager���擾����
	TextureManager* gameManager = GameContext::Get<PlayScene>()->GetGameManager().get();

	//�o���l�̉摜���擾����
	ITextureBase* ex = resourceManager->GetTexture("�o���l");

	//�񕜃A�C�e���̉摜���擾����
	ITextureBase* healTex = resourceManager->GetTexture("��1");

	//�`�揇��ύX
	this->SetDrawOrder(5);

	//�_�ł̊Ԋu��ݒ肷��
	TextureBase::SetTarget(0.1f);

	//�_�ł̏I���̐ݒ������
	TextureBase::SetFinal(1);
	TextureBase::SetFinalOption(true);

	//���s�֐����N����
	this->SetActive(true);


	//�A�j���[�V�����̐ݒ�
	for (const std::unique_ptr<AnimationImporter>& importer : Animation::GetAnimeList("NormalEnemy"))
	{
		//�A�j���[�V�����̎g�p����e�N�X�`�����擾����
		ITextureBase* useTexture = resourceManager->GetTexture(importer->GetRefference());

		//�A�j���[�V��������ݒ肷��
		this->SetAnimation(importer->GetAnimation(), useTexture->GetRect());

		//���̃A�j���[�V�����̏���ݒ肷��
		this->SetNextAnimation(importer->GetAnimation(), importer->GetNext());

		//�A�j���[�V�����̏I�����Ԃ�ݒ肷��
		this->SetEndTime(importer->GetAnimation(), importer->GetEnd());
	}

	//�A�j���[�V�����̃t���O��ݒ肷��
	this->SetAniFlag(true);
	this->SetLoopFlag(true);

	//���݂̃A�j���[�V��������ݒ肷��
	this->SetNowInfo("�m�[�}��1");
	this->SetLoopInfo("�m�[�}��6");
	this->SetNowAnimation(this->GetNowAnimation());

	ITextureBase* Gage = resourceManager->GetTexture("WhiteBack");

	//HP�Q�[�W�𐶐�����
	std::unique_ptr<HPGage>pHpGage = std::make_unique<HPGage>(Gage);
	pHpGage->SetColor(Colors::GreenYellow);
	pHpGage->SetRect(RECT{ 0,0,80,10 });
	pHpGage->SetPosition(this->GetPosition() - Vector3(0, 20, 0));
	pHpGage->SetMaxTexLength(80);
	pHpGage->SetDrawOrder(4);
	pHpGage->SetState(STATE::DRAW);
	m_HpGage = pHpGage.get();

	//HP�Q�[�W�̗����𐶐�����
	std::unique_ptr<ITextureBase>pHpBack = std::make_unique<TextureBase>(Gage);
	pHpBack->SetColor(Colors::White);
	pHpBack->SetRect(RECT{ 0,0,80,10 });
	pHpBack->SetPosition(this->GetPosition() - Vector3(0, 20, 0));
	pHpBack->SetDrawOrder(6);
	m_HpBack = pHpBack.get();

	//�o���l�𐶐�����
	std::unique_ptr<Experience> pex = std::make_unique<Experience>(ex);
	pex->SetDrawOrder(4);
	pex->SetState(STATE::NONE);
	m_experience = pex.get();

	//HealManager���擾����
	HealManager* healManager = GameContext::Get<PlayScene>()->GetHealManager();

	//�񕜃A�C�e���𐶐�����
	healManager->CreateItem(healTex, [&](Item* item)
		{	Heal* healItem = dynamic_cast<Heal*>(item);
			m_healItem = healItem; });

	//TextureManager�ɒǉ�����
	gameManager->Add(std::move(pHpGage));
	gameManager->Add(std::move(pHpBack));
	gameManager->Add(std::move(pex));

	//�����ړ��̃X�e�[�g
	m_AutoMoveState = std::make_unique<AutoMoveState>();
	m_AutoMoveState->SetExecute(true);
	m_AutoMoveState->SetNowState(DIRECT::LEFT);
	m_AutoMoveState->SetSpeed(Vector3(1.5f, 0.0f, 0.0f));

	//���X�g�ɒǉ�����
	m_MoveStateList.push_back(m_AutoMoveState.get());
}

void NormalEnemy::Initialize()
{
	//PlayScene���擾����
	PlayScene* playScene = GameContext::Get<PlayScene>();

	//TextureManager���擾����
	TextureManager* gameManager = playScene->GetGameManager().get();

	//�ÓI�f�[�^���擾����
	StaticData* staticData = StaticData::GetInstance();

	//Player���擾����
	CoroKun* player = GameContext::Get<CoroKun>();

	//�o���l�̒ǔ��p�̃X�e�[�g�̐ݒ������

	//�x�N�g���̕␳�l��ݒ肷��
	m_experience->GetTrackingState()->SetCorrection(Vector3(10.0f));

	//�ǐՖڕW��ݒ肷��
	m_experience->GetTrackingState()->SetTarget(&player->GetPosition());

	//���g�̍��W��ݒ肷��
	m_experience->GetTrackingState()->SetThis(&m_experience->GetPosition());

	//���L�҂̃|�C���^��ݒ肷��
	m_experience->SetOwner(this);

	//MasicEffectManager�𐶐�����
	m_masicEffectManager = std::make_unique<MasicEffectManager>();
	m_masicEffectManager->Initialize(this);

	//�G�̃��x�������擾����
	int level = staticData->Get_IntData(this->GetName());

	//�X�e�[�^�X��ݒ肷��
	StatusImporter* importer = StatusBase::FindStatus(this->GetName(), level);
	this->SetStatus(importer->GetHp(), importer->GetMp(), importer->GetAtack(), importer->GetDefence(),
		importer->GetSpecial(), importer->GetLevel(), importer->GetNextLevelPoint());
	this->SetMaxStatus(importer->GetHp(), importer->GetMp(), importer->GetAtack(), importer->GetDefence(),
		importer->GetSpecial(), MAX_LEVEL, importer->GetNextLevelPoint());

	//�o���l�̕␳�l��ݒ肷��
	this->SetCompensation(STRENGTH);

	//�X�e�[�g��ݒ肷��
	this->AddData1(ENEMYSTATE::ATACKSTART);

	//��ɓG�̏㑤��HP�Q�[�W������悤�ɂ���
	m_HpGage->SetPosition(this->GetPosition() - Vector3(0, 20, 0));
	m_HpBack->SetPosition(this->GetPosition() - Vector3(0, 20, 0));

	//HP�Q�[�W�̏�����
	m_HpGage->CreateGage(gameManager);
}

void NormalEnemy::Update()
{
	//UI�̕\�����I���܂ŉ������Ȃ�
	TimeLag* go = GameContext::Get<PlayScene>()->GetLagGo().get();
	if (!go->GetFinishFlag())return;

	//���s�֐���Active�łȂ���΁A�������Ȃ�
	if (!this->GetActive())return;

	//CameraManager���擾����
	CameraManager* cameraManager = GameContext::Get<CameraManager>();

	//���݉ғ����Ă���J�������擾����
	CameraBase* nowCamera = cameraManager->GetNowCamera();

	//�J�������X�N���[���J�����̎��́A�X�V���~�߂�
	if (!nowCamera->GetObjectMove())return;

	//�A�j���[�V�����̍X�V����
	Animation::Update();

	//�摜�̐؂�o���ʒu���X�V����
	this->SetRect(this->GetNowAnimation());

	//�Q�[�W�̍X�V����
	this->UpdateGage();

	//�X�e�[�g�̍X�V����
	this->UpdateState();

	//�摜�̍X�V����
	TextureBase::Update();

	//�_�Œ��̎�
	if (this->GetBlink())
	{
		switch (this->GetNowData2())
		{
		case COLOR_CHANGE::NONE:
			//�F�𔒂ɂ���
			this->SetColor(Colors::White);

			//5�t���[�����ƂɐF��؂�ւ���
			if (m_frameCount % 5 == NULL && m_frameCount != NULL)
			{
				//�X�e�[�g��؂�ւ���
				this->AddData2(COLOR_CHANGE::CHANGE);
			}

			break;
		case COLOR_CHANGE::CHANGE:
			//�F��Ԃɂ���
			this->SetColor(Colors::Red);

			//5�t���[�����ƂɐF��؂�ւ���
			if (m_frameCount % 5 == NULL && m_frameCount != NULL)
			{
				//�X�e�[�g��؂�ւ���
				this->AddData2(COLOR_CHANGE::NONE);
			}

			break;
		}

		//�t���[�������X�V����
		m_frameCount++;
	}
	else
	{
		//�_�ł��Ă��Ȃ��Ƃ�
		this->SetColor(Colors::White);

		//�t���[����������������
		m_frameCount = NULL;
	}

	//�����蔻���ǉ�����
	GameContext::Get<CollisionManager>()->AddUseHero(this);

	//�v���C���[���擾����
	CoroKun* player = GameContext::Get<CoroKun>();

	//���S���ȊO�͓����蔻���ݒ肷��
	if (this->GetHP() > NULL)
	{
		//�v���C���[���������Ă���Ƃ�
		if (player->GetHP() > NULL)
		{
			//�v���C���[�Ƃ̓����蔻���ݒ肷��
			GameContext::Get<CollisionManager>()->AddUseOther(player, this, 100.0f);

			//���͂ɐ�����j�Ƃ̓����蔻���ݒ肷��
			std::vector<Needle*> needleRe = GameContext::Get<Resurrection>()->GetNeedleList();

			for (Needle* needle : needleRe)
			{
				//�����蔻���ݒ肷��
				GameContext::Get<CollisionManager>()->AddUseOther(needle, this, 400.0f);
			}

			//�������ɔ�Ԑj�Ƃ̓����蔻���ݒ肷��
			std::vector<Needle*> needleOver = GameContext::Get<NeedleOverall>()->GetNeedleList();

			for (Needle* needle : needleOver)
			{
				//�����蔻���ݒ肷��
				GameContext::Get<CollisionManager>()->AddUseOther(needle, this, 300.0f);
			}

			//�ʏ�e���擾����
			std::vector<NormalBullet*> normalBulletList = GameContext::Get<NormalTechnique>()->GetBulletsList();

			for (NormalBullet* bullet : normalBulletList)
			{
				//�����蔻���ݒ肷��
				GameContext::Get<CollisionManager>()->AddUseOther(bullet, this, 200.0f);
			}
		}
	}
}

void NormalEnemy::Render()
{
	//�摜�̕`�揈��
	TextureBase::Render();
}

void NormalEnemy::Finalize()
{
}

void NormalEnemy::OnCollisionEnter(ITextureBase* obj)
{
	//�����蔻��̏���
	CollisionBase::OnCollisionEnter(obj);

	//�v���C���[�ɓ��������Ƃ��̏���
	if (obj->GetName() == "SickChar")this->OnCollision_Player(obj);

	//�L�͈͍U���ɓ��������Ƃ��̏���
	if (obj->GetName() == "shortNeedle")this->OnCollision_NeedleOverAll(obj);

	//���͈͍U���ɓ��������Ƃ��̏���
	if (obj->GetName() == "longNeedle")this->OnCollision_Resurrection(obj);

	//�E�C���X�r�[���ɓ��������Ƃ��̏���
	if (obj->GetName() == "�E�C���X�r�[��")this->OnCollision_VirusBeam(obj);

	//�ʏ�e�ɓ��������Ƃ��̏���
	if (obj->GetName() == "�ʏ�e")this->OnCollision_NormalBullet(obj);

	//�������̃p�l���ɓ��������Ƃ��̏���
	if (obj->GetName() == "LeftPanel")this->OnCollision_LeftPanel(obj);

	//�E�����̃p�l���ɓ��������Ƃ��̏���
	if (obj->GetName() == "RightPanel")this->OnCollision_RightPanel(obj);
}

void NormalEnemy::NonCollisionEnter(ITextureBase* obj)
{
	//�����蔻��̏���
	CollisionBase::NonCollisionEnter(obj);
}

void NormalEnemy::OnDamage(const int& atack, const int& defence, const float correction)
{
	//NormalTechnique���擾����
	NormalTechnique* Ntch = GameContext::Get<NormalTechnique>();

	//�X�R�A���擾����
	Score* score = GameContext::Get<PlayScene>()->GetScore();

	//HP���擾����
	int hp = this->GetHP();

	//rect���擾����
	RECT rect = m_HpGage->GetRect();

	//�_���[�W���v�Z����
	StatusCalc::CalcDamage(hp, atack, defence,
		m_HpGage->GetMaxTexLength(), this->GetMaxHP(), rect.right, correction);

	//HP��ݒ肷��
	this->SetHP(hp);

	//RECT��ݒ肷��
	m_HpGage->SetRect(rect);

	//SoundManager���擾����
	SoundManager* soundManager = SoundManager::GetInstance();

	//SE��炷
	soundManager->Play(CRI_CUESHEET_0__11_�_���[�W);

	if (score->GetSaving() == NULL)
	{
		//SE��炷
		soundManager->Play(CRI_CUESHEET_0__19_�X�R�A���Z);
	}

	//�_���[�W�p�̃t���O�𗧂Ă�
	m_damageCheck = true;

	//HP�Q�[�W�̗��ʂ̃e�N�X�`�����擾����
	ITextureBase* backHpTexture = m_HpGage->GetBackGageTexture();

	//HP�Q�[�W�̗��ʂ̐F��ݒ肷��
	backHpTexture->SetColor(Colors::Red);

	//�_���[�W�̃J�E���g������������
	m_HpGage->SetEasingTime(0.0f);

}

void NormalEnemy::UpdateGage()
{
	//��ɓG�̏㑤��HP�Q�[�W������悤�ɂ���
	m_HpGage->SetPosition(this->GetPosition() - Vector3(0, 20, 0));
	m_HpBack->SetPosition(this->GetPosition() - Vector3(0, 20, 0));
	m_HpGage->GetBackGageTexture()->SetPosition(m_HpGage->GetPosition());

	//�ÓI�f�[�^���擾����
	StaticData* staticData = StaticData::GetInstance();

	//�O��̃t���[������̌o�ߎ��Ԃ��擾����
	float elapsedTime = staticData->Get_FloatData("elapsedTime");

	//�_���[�W���󂯂Ă���Ƃ�
	if (m_damageCheck)
	{
		//�w�ʂ̃e�N�X�`�����擾����
		ITextureBase* backTexture = m_HpGage->GetBackGageTexture();

		//HP�Q�[�W�̗��ʂ̐؂�o���ʒu���擾����
		RECT back = m_HpGage->GetBackGageTexture()->GetRect();

		//HP�Q�[�W�̐؂�o���ʒu���擾����
		RECT front = m_HpGage->GetRect();

		//�_���[�W�̍��������߂�
		int diffrent = (int)(back.right - front.right);

		//���݂̎��Ԃ��擾����
		float time = m_HpGage->GetEasingTime();

		//���Ԃ��X�V����
		time += elapsedTime;

		//�_���[�W���X�J���␳����
		diffrent *= time;

		//�Q�[�W�ɓ���������
		back.right -= diffrent;

		//�؂�o���ʒu���Đݒ肷��
		m_HpGage->GetBackGageTexture()->SetRect(back);

		//���Ԃ��Đݒ肷��
		m_HpGage->SetEasingTime(time);

		if (m_HpGage->GetEasingTime() >= 1.0f)
		{
			//1�b�o�߂�����A�t���O��؂�
			m_damageCheck = false;

			//�؂�o���ʒu��ݒ肷��
			m_HpGage->GetBackGageTexture()->SetRect(m_HpGage->GetRect());
		}
	}

}

void NormalEnemy::UpdateState()
{
	//�x�N�g���̏�����
	Vector3 vel = Vector3::Zero;

	//Stage���擾
	Stage* stage = GameContext::Get<Stage>();

	//�ÓI�f�[�^�̎擾
	StaticData* staticData = StaticData::GetInstance();

	//���s���̃X�e�[�g����x�N�g�����擾����
	for (MoveState*& state : m_MoveStateList)
	{
		vel += state->Update();
	}

	//�x�N�g����ݒ肷��
	this->SetVelocity(vel);

	//HP��0�ȉ��ɂȂ�����A���S�X�e�[�g�Ɉڍs����
	if (this->GetHP() <= NULL && this->GetNowData1() < ENEMYSTATE::DEADSTART)
	{
		this->AddData1(ENEMYSTATE::DEADSTART);
	}

	//�X�e�[�g�ɂ���āA������؂�ւ���
	switch (this->GetNowData1())
	{
	case ENEMYSTATE::ATACKSTART:
		this->State_AtackStart();
		break;
	case ENEMYSTATE::NEXTSTATE:
		this->State_AtackNext();
		break;
	case ENEMYSTATE::DEADSTART:
		this->State_DeadStart();
		break;
	case ENEMYSTATE::DEADTING:
		this->State_DeadTing();
		break;
	case ENEMYSTATE::DEAD:
		this->State_Dead();
		break;
	}
}

void NormalEnemy::State_AtackStart()
{
	//State_Start�̏���

	//���݂̃A�j���[�V��������ݒ肷��
	this->SetNowInfo("�m�[�}��1");
	this->SetLoopInfo("�m�[�}��6");

	//�A�j���[�V�����̍X�V���~�߂�
	this->SetAniFlag(true);

	//�����ړ��̃X�e�[�g��ݒ肷��
	m_AutoMoveState->SetExecute(true);

	//�X�e�[�g��؂�ւ���
	this->AddData1(ENEMYSTATE::ATACKING);
}

void NormalEnemy::State_AtackNext()
{
	//State_Next�̏���

	//�A�j���[�V�����̍X�V���~�߂�
	this->SetAniFlag(false);

	//���݂̃A�j���[�V��������ݒ肷��
	this->SetNowInfo("�m�[�}��4");

	//�����ړ��̃X�e�[�g��ݒ肷��
	m_AutoMoveState->SetExecute(false);

	//�_�ł��I��
	if (!this->GetBlink())
	{
		//�X�e�[�g��؂�ւ���
		this->AddData1(ENEMYSTATE::ATACKSTART);
	}
}

void NormalEnemy::State_DeadStart()
{
	//State_DeadStart�̏���

	//�����ړ��̃X�e�[�g��ݒ肷��
	m_AutoMoveState->SetExecute(false);

	//�A�j���[�V�������~�߂�
	this->SetAniFlag(false);

	//�_�ł��I��
	if (!this->GetBlink())
	{
		//���݂̃A�j���[�V��������ݒ肷��
		this->SetNowInfo("�m�[�}��7");
		this->SetLoopInfo("�m�[�}��9");

		//���[�v��؂�
		this->SetLoopFlag(false);

		//�A�j���[�V�������N��
		this->SetAniFlag(true);

		//�X�e�[�g��؂�ւ���
		this->AddData1(ENEMYSTATE::DEADTING);
	}


}

void NormalEnemy::State_DeadTing()
{
	//State_Deadting�̏���

	//�A�j���[�V�������I��������A�_�ł�����

	if (!this->GetAniFlag())
	{
		//�_�ł��J�n
		this->SetBlink(true);

		//�X�e�[�g��؂�ւ���
		this->AddData1(ENEMYSTATE::DEAD);
	}
}

void NormalEnemy::State_Dead()
{
	//State_Dead�̏���

	//SoundManager���擾����
	SoundManager* soundManager = SoundManager::GetInstance();

	//�_�ł��I�����
	if (!this->GetBlink())
	{
		//�`���؂�
		this->SetState(STATE::NONE);

		//���s�֐��𗎂Ƃ�
		this->SetActive(false);

		//HP�o�[�̕`���؂�
		m_HpGage->SetState(STATE::NONE);
		m_HpBack->SetState(STATE::NONE);

		//�G�t�F�N�g�̍��W��ݒ肷��
		m_experience->SetPosition(this->GetPosition());

		//�o���l�̒ǔ��p�̃X�e�[�g���N����
		m_experience->GetTrackingState()->SetExecute(true);

		//�z�[�~���O�p�̃X�e�[�g���N����
		m_experience->GetHomingState()->SetExecute(true);
		m_experience->GetHomingState()->SetStartFlag(true);

		//�o���l�̃X�e�[�g��؂�ւ���
		m_experience->SetNowState(TARGET_STATE::WAIT);

		//SE���Đ�����
		soundManager->Play(CRI_CUESHEET_0__31_�A�C�e���o��);

		//�o���l��`�悷��
		m_experience->SetState(STATE::DRAW);

		//�񕜃A�C�e�����h���b�v����
		this->HealItemDrop();
	}
}

void NormalEnemy::HealItemDrop()
{
	//�������擾����
	Rand* rand = Rand::GetInstance();

	//�h���b�v����m�������߂�
	int pro = ItemDrop::GetProbability(this->GetName(), this->GetLevel());

	//0����100�ŗ�����U��
	int result = rand->RandomMachine(0, 100);

	//�����̌��ʂ��h���b�v����m���ȉ��̏ꍇ�ɁA�h���b�v������
	if (pro >= result)
	{
		m_healItem->OnFall(this->GetPosition());
	}
}

void NormalEnemy::OnCollision_Player(ITextureBase* obj)
{
	//�v���C���[�Ɠ����������̏���
	CoroKun* player = dynamic_cast<CoroKun*>(obj);

	//�X�R�A���擾����
	Score* score = GameContext::Get<PlayScene>()->GetScore();

	//SoundManager���擾����
	SoundManager* soundManager = SoundManager::GetInstance();

	//�_���[�W���󂯂Ă��Ȃ��Ƃ�
	if (!TextureBase::GetBlink())
	{
		//�v���C���[���̓����蒆�̂Ƃ�
		if (player->GetBodyBlow()->GetActive())
		{
			//�_�ł�����
			this->SetBlink(true);

			//�X�e�[�g��؂�ւ���(�_���[�W���󂯂���A�����I�Ɏv�l���ԂɈڂ�)
			this->AddData1(ENEMYSTATE::NEXTSTATE);

			//�_���[�W���󂯂�
			this->OnDamage(player->GetAtack(), this->GetDefence());

			//�G�t�F�N�g���Đ�����
			m_masicEffectManager->NowEffectPlay(BodyBlow::TECHNIQUE_HEAL_COMMAND, BodyBlow::TECHNIQUE_HEAL_SPECIAL);

			//�X�R�A��ǉ�����
			score->AddScore(this->GetLevel() * BodyBlow::BASE_SCORE);
			score->SetFrameSave(this->GetLevel() * BodyBlow::BASE_SCORE, 60.0f);
		}
	}
}

void NormalEnemy::OnCollision_NeedleOverAll(ITextureBase* obj)
{
	//NeedleOverAll���擾����
	NeedleOverall* needleOver = GameContext::Get<NeedleOverall>();

	//�v���C���[�Ɠ����������̏���
	CoroKun* player = GameContext::Get<CoroKun>();

	//�X�R�A���擾����
	Score* score = GameContext::Get<PlayScene>()->GetScore();

	//SoundManager���擾����
	SoundManager* soundManager = SoundManager::GetInstance();

	//�_���[�W���󂯂Ă��Ȃ��Ƃ�
	if (!TextureBase::GetBlink())
	{
		//�_�ł�����
		this->SetBlink(true);

		//�X�e�[�g��؂�ւ���(�_���[�W���󂯂���A�����I�Ɏv�l���ԂɈڂ�)
		this->AddData1(ENEMYSTATE::NEXTSTATE);

		//�␳�l��ݒ肷��
		float correction = 1.0f;

		//�j���������Ă���^�C�~���O�œ���������A�󂯂�_���[�W�͔���
		if (needleOver->GetNowState() == NEEDLEACTION::MOPE)correction = 0.5f;

		//�_���[�W���󂯂�
		this->OnDamage(player->GetAtack(), this->GetDefence(), correction);

		//�G�t�F�N�g���Đ�����
		m_masicEffectManager->NowEffectPlay(NeedleOverall::TECHNIQUE_HEAL_COMMAND, NeedleOverall::TECHNIQUE_HEAL_SPECIAL);

		//�X�R�A��ǉ�����
		score->AddScore(this->GetLevel() * NeedleOverall::BASE_SCORE);
		score->SetFrameSave(this->GetLevel() * NeedleOverall::BASE_SCORE, 60.0f);
	}
}

void NormalEnemy::OnCollision_Resurrection(ITextureBase* obj)
{
	//Resurrection���擾����
	Resurrection* resurrection = GameContext::Get<Resurrection>();

	//�v���C���[�Ɠ����������̏���
	CoroKun* player = GameContext::Get<CoroKun>();

	//�X�R�A���擾����
	Score* score = GameContext::Get<PlayScene>()->GetScore();

	//SoundManager���擾����
	SoundManager* soundManager = SoundManager::GetInstance();

	//�_���[�W���󂯂Ă��Ȃ��Ƃ�
	if (!TextureBase::GetBlink())
	{
		//�_�ł�����
		this->SetBlink(true);

		//�X�e�[�g��؂�ւ���(�_���[�W���󂯂���A�����I�Ɏv�l���ԂɈڂ�)
		this->AddData1(ENEMYSTATE::NEXTSTATE);

		//�␳�l��ݒ肷��
		float correction = 1.0f;

		//�j���������Ă���^�C�~���O�œ���������A�󂯂�_���[�W�͔���
		if (resurrection->GetNowState() == RENEEDLE::LITTLE)correction = 0.5f;

		//�_���[�W���󂯂�
		this->OnDamage(player->GetAtack(), this->GetDefence(), correction);

		//�G�t�F�N�g���Đ�����
		m_masicEffectManager->NowEffectPlay(Resurrection::TECHNIQUE_HEAL_COMMAND, Resurrection::TECHNIQUE_HEAL_SPECIAL);

		//�X�R�A��ǉ�����
		score->AddScore(this->GetLevel() * Resurrection::BASE_SCORE);
		score->SetFrameSave(this->GetLevel() * Resurrection::BASE_SCORE, 60.0f);
	}
}

void NormalEnemy::OnCollision_VirusBeam(ITextureBase* obj)
{
	//�v���C���[�Ɠ����������̏���
	CoroKun* player = GameContext::Get<CoroKun>();

	//�X�R�A���擾����
	Score* score = GameContext::Get<PlayScene>()->GetScore();

	//SoundManager���擾����
	SoundManager* soundManager = SoundManager::GetInstance();

	//�_���[�W���󂯂Ă��Ȃ��Ƃ�
	if (!TextureBase::GetBlink())
	{
		//�_�ł�����
		this->SetBlink(true);

		//�X�e�[�g��؂�ւ���(�_���[�W���󂯂���A�����I�Ɏv�l���ԂɈڂ�)
		this->AddData1(ENEMYSTATE::NEXTSTATE);

		//�_���[�W���󂯂�
		this->OnDamage(player->GetAtack(), this->GetDefence());

		//�G�t�F�N�g���Đ�����
		m_masicEffectManager->NowEffectPlay(VirusBeam::TECHNIQUE_HEAL_COMMAND, VirusBeam::TECHNIQUE_HEAL_SPECIAL);

		//�X�R�A��ǉ�����
		score->AddScore(this->GetLevel() * VirusBeam::BASE_SCORE);
		score->SetFrameSave(this->GetLevel() * VirusBeam::BASE_SCORE, 60.0f);
	}
}

void NormalEnemy::OnCollision_NormalBullet(ITextureBase* obj)
{
	//�v���C���[�Ɠ����������̏���
	CoroKun* player = GameContext::Get<CoroKun>();

	//�X�R�A���擾����
	Score* score = GameContext::Get<PlayScene>()->GetScore();

	//SoundManager���擾����
	SoundManager* soundManager = SoundManager::GetInstance();

	//�_���[�W���󂯂Ă��Ȃ��Ƃ�
	if (!TextureBase::GetBlink())
	{
		//�_�ł�����
		this->SetBlink(true);

		//�X�e�[�g��؂�ւ���(�_���[�W���󂯂���A�����I�Ɏv�l���ԂɈڂ�)
		this->AddData1(ENEMYSTATE::NEXTSTATE);

		//�␳�l��ݒ肷��
		float correction = 1.0f;

		correction = obj->GetScale().x / 1.6f;

		//�_���[�W���󂯂�
		this->OnDamage(player->GetAtack(), this->GetDefence(), correction);

		int core1 = NormalTechnique::TECHNIQUE_HEAL_COMMAND * correction;
		int core2 = NormalTechnique::TECHNIQUE_HEAL_SPECIAL * correction;

		//�G�t�F�N�g���Đ�����
		m_masicEffectManager->NowEffectPlay(core1, core2);

		//�X�R�A��ǉ�����
		score->AddScore(this->GetLevel() * (NormalBullet::BASE_SCORE * correction));
		score->SetFrameSave(this->GetLevel() * (NormalBullet::BASE_SCORE * correction), 60.0f);
	}
}

void NormalEnemy::OnCollision_LeftPanel(ITextureBase* obj)
{
	//�X�e�[�g��ύX����
	m_AutoMoveState->SetNowState(DIRECT::LEFT);

	//�`��𔽓]������
	this->SetSpriteEffect(SpriteEffects::SpriteEffects_None);
}

void NormalEnemy::OnCollision_RightPanel(ITextureBase* obj)
{
	//�X�e�[�g��ύX����
	m_AutoMoveState->SetNowState(DIRECT::RIGHT);

	//�`��𔽓]������
	this->SetSpriteEffect(SpriteEffects::SpriteEffects_FlipHorizontally);
}
