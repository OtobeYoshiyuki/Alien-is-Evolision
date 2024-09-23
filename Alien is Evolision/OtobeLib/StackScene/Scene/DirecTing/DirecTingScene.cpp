#include "pch.h"
#include "DirecTingScene.h"
#include "OtobeLib/FrameWork/StaticData/StaticData.h"
#include "OtobeLib/FrameWork/Graphics/Graphics.h"
#include "OtobeLib/FrameWork/ResourceManager/ResourceManager.h"
#include "OtobeLib/StackScene/SceneManager/SceneManager.h"
#include "OtobeLib/StackScene/Scene/Select/SelectScene.h"
#include "OtobeLib/FrameWork/SoundManager/SoundManager.h"
#include "Game.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace OtobeLib;

//�X�e�[�g��؂�ւ��鎞��
const float DirecTingScene::CHANGE_TIME_PLAYER = 2.0f;

//�X�e�[�g��؂�ւ��鎞��
const float DirecTingScene::CHANGE_TIME_EXPROSION = 1.0f;

//�v���C���[�̃A�j���[�V�����̑҂�����
const float DirecTingScene::WAIT_TIME_FACE = 1.0f;

//�v���C���[�̔w�i�̃A�j���[�V�����̑҂�����
const float DirecTingScene::WAIT_TIME_BACKFADE = 3.0f;

DirecTingScene::DirecTingScene():
	m_TextureManager(nullptr),//�摜�̊Ǘ��N���X
	m_backPlayer(nullptr),//�v���C���[�̔w�ʂ̃e�N�X�`��
	m_backTexture(nullptr),//�w�i�̃e�N�X�`��
	m_playerAnimeTex(nullptr),//�v���C���[�̃A�j���[�V�����p�̃e�N�X�`��
	m_waveAnimeTex(nullptr),//�I�[���̃A�j���[�V�����p�̃e�N�X�`��
	m_timer(0.0f)//�o�ߎ���
{
}

void DirecTingScene::Initialize()
{
	//SoundManager���擾����
	SoundManager* soundManager = SoundManager::GetInstance();

	//SE���Đ�����
	soundManager->Play(CRI_CUESHEET_0__41_�I�[��);

	//�����̏���
	this->Create();
}

void DirecTingScene::Update()
{
	//TextureManager�̍X�V
	m_TextureManager->Update();

	//�X�e�[�g�̍X�V����
	this->UpdateState();
}

void DirecTingScene::Render()
{
	//�`��n�̏�Ԃ��擾����
	Graphics* Graph = Graphics::GetInstance();
	const auto& context = DX::DeviceResources::GetInstance()->GetD3DDeviceContext();

	//�`��̊J�n
	Graph->GetSpriteBatch()->Begin(DirectX::SpriteSortMode::SpriteSortMode_Deferred,
		Graph->GetCommonStates()->NonPremultiplied(), Graph->GetCommonStates()->PointClamp(),
		Graph->GetCommonStates()->DepthDefault(), nullptr,
		[&] {context->OMSetBlendState(Graph->GetCommonStates()->NonPremultiplied(), nullptr, 0xffffffff); });

	//�摜�̕`�揈��
	m_TextureManager->Render();

	//�`��̏I��
	Graph->GetSpriteBatch()->End();
}

void DirecTingScene::Finalize()
{
}

void DirecTingScene::Create()
{
	//��������

	//�Y����������������
	this->AddData2(NULL);

	//�ÓI�f�[�^���擾����
	StaticData* staticData = StaticData::GetInstance();

	//ResourceManager���擾����
	ResourceManager* resourceManager = ResourceManager::GetInstance();

	//TextureManager�𐶐�����
	m_TextureManager = std::make_unique<TextureManager>();

	//�g�p����摜���擾����
	ITextureBase* bg = resourceManager->GetTexture("�X�e�[�W�Z���N�g" + staticData->Get_StringData("�X�e�[�W"));
	ITextureBase* playerBack = resourceManager->GetTexture("�K�E�Z�v���C���[�w�i");
	ITextureBase* playerTex = resourceManager->GetTexture("�K�E�Z�v���C���[1");
	ITextureBase* wave = resourceManager->GetTexture("�K�E�Z�I�[��1");
	ITextureBase* expro = resourceManager->GetTexture("�K�E�Z�G�t�F�N�g1");

	//�w�i�̃e�N�X�`���𐶐�����
	std::unique_ptr<ITextureBase> pbg = std::make_unique<TextureBase>(bg);
	//�`�揇��ݒ肷��
	pbg->SetDrawOrder((int)DRAWORDER::BACKGROUND);
	//�`���؂�
	pbg->SetState(ITextureBase::STATE::NONE);
	//���|�C���^��o�^����
	m_backTexture = pbg.get();
	//TextureManager�ɓo�^����
	m_TextureManager->Add(std::move(pbg));

	//�v���C���[�̃A�j���[�V�����p�̃e�N�X�`��
	std::unique_ptr<AnimeTexture> panimePlayer = std::make_unique<AnimeTexture>(playerTex);
	//������
	panimePlayer->InitAnimation("DeathBlowPlayer", false, false, "Face1", "Face4");
	//�`�揇��ݒ肷��
	panimePlayer->SetDrawOrder((int)DRAWORDER::PLAYER);
	//�`���؂�
	panimePlayer->SetState(ITextureBase::STATE::NONE);
	//���W��ݒ肷��
	panimePlayer->SetPosition(Vector3(Game::MAX_WIDTH / 4, 0.0f, 0.0f));
	//���|�C���^��o�^����
	m_playerAnimeTex = panimePlayer.get();
	//TextureManager�ɓo�^����
	m_TextureManager->Add(std::move(panimePlayer));

	//�v���C���[�̔w�ʂ̃e�N�X�`��
	std::unique_ptr<ITextureBase> pbgPlayer = std::make_unique<TextureBase>(playerBack);
	//�`�揇��ݒ肷��
	pbgPlayer->SetDrawOrder((int)DRAWORDER::PLAYER_BACK);
	//�`���؂�
	pbgPlayer->SetState(ITextureBase::STATE::NONE);
	//���W��ݒ肷��
	pbgPlayer->SetPosition(Vector3(Game::MAX_WIDTH / 4, 0.0f, 0.0f));
	//�F��ݒ肷��
	pbgPlayer->SetColor(Colors::SkyBlue);
	//���|�C���^��o�^����
	m_backPlayer = pbgPlayer.get();
	//TextureManager�ɓo�^����
	m_TextureManager->Add(std::move(pbgPlayer));

	//�I�[���̃A�j���[�V�����p�̃e�N�X�`��
	std::unique_ptr<AnimeTexture> panimeWave = std::make_unique<AnimeTexture>(wave);
	//������
	panimeWave->InitAnimation("Wave", true, true, "Wave1", "Wave14");
	//�`�揇��ݒ肷��
	panimeWave->SetDrawOrder((int)DRAWORDER::WAVE);
	//�`���؂�
	panimeWave->SetState(ITextureBase::STATE::NONE);
	//���W��ݒ肷��
	panimeWave->SetPosition(Vector3(200.0f,-300.0f, 0.0f));
	//�g��{����ݒ肷��
	panimeWave->SetScale(Vector3(1.5f, 1.5f, 1.0f));
	//���|�C���^��o�^����
	m_waveAnimeTex = panimeWave.get();
	//TextureManager�ɓo�^����
	m_TextureManager->Add(std::move(panimeWave));

	//�����̃A�j���[�V�����p�e�N�X�`��
	//3��������
	for (int i = 0; i < EXPROSION_ARRAY_NUM; i++)
	{
		//�����p�̃e�N�X�`��
		std::unique_ptr<AnimeTexture> pexpro = std::make_unique<AnimeTexture>(expro);
		//������
		pexpro->InitAnimation("Exprosion", false, false, "Expro1", "Expro9");
		//�`�揇��ݒ肷��
		pexpro->SetDrawOrder((int)DRAWORDER::EXPROSION);
		//�`���؂�
		pexpro->SetState(ITextureBase::STATE::NONE);
		//���|�C���^��o�^����
		m_exproAnimeTex.push_back(pexpro.get());
		//TextureManager�ɓo�^����
		m_TextureManager->Add(std::move(pexpro));
	}

	//�����̍��W��ݒ肷��
	m_exproAnimeTex[ARRAY_0]->SetPosition(Vector3(400.0f, 0.0f, 0.0f));
	m_exproAnimeTex[ARRAY_1]->SetPosition(Vector3(0.0f, 200.0f, 0.0f));
	m_exproAnimeTex[ARRAY_2]->SetPosition(Vector3(800.0f, 200.0f, 0.0f));
}

void DirecTingScene::UpdateState()
{
	//�X�e�[�g�̍X�V����

	//Switch���ŏ����𕪂���
	switch (this->GetNowData1())
	{
	case DIRECTING::NONE:
		this->State_None();
		break;
	case DIRECTING::WAVE_ANIME:
		this->State_WaveAnime();
		break;
	case DIRECTING::TIMER_UP:
		this->State_TimerUp();
		break;
	case DIRECTING::PLAYER_ANIME:
		this->State_PlayerAnime();
		break;
	case DIRECTING::BACK_FADE:
		this->State_BackFade();
		break;
	case DIRECTING::EXPROSION_ANIME:
		this->State_ExproAnime();
		break;
	case DIRECTING::END:
		this->State_End();
		break;
	}
}

void DirecTingScene::State_None()
{
	//State_None�̏���

	//�`����N����
	m_playerAnimeTex->SetState(ITextureBase::STATE::DRAW);
	m_backTexture->SetState(ITextureBase::STATE::DRAW);
	m_waveAnimeTex->SetState(ITextureBase::STATE::DRAW);

	//�X�e�[�g��؂�ւ���
	this->AddData1(DIRECTING::WAVE_ANIME);
}

void DirecTingScene::State_WaveAnime()
{
	//State_WaveAnime�̏���

	//�I�[�����S���o�Ă�����A�X�e�[�g��؂�ւ���
	if (m_waveAnimeTex->GetNowInfo() == "Wave9")
	{
		this->AddData1(DIRECTING::TIMER_UP);
	}
}

void DirecTingScene::State_TimerUp()
{
	//State_TimerUp�̏���

	//�ÓI�f�[�^���擾����
	StaticData* staticData = StaticData::GetInstance();

	//2�b�o�߂�����A�X�e�[�g��؂�ւ���
	if (m_timer > CHANGE_TIME_PLAYER)
	{
		//�o�ߎ��Ԃ�����������
		m_timer = 0.0f;

		//�v���C���[�̃A�j���[�V�������N��������
		m_playerAnimeTex->SetAniFlag(true);

		//�X�e�[�g��؂�ւ���
		this->AddData1(DIRECTING::PLAYER_ANIME);

		//���Ԃ̍X�V�͍s��Ȃ�
		return;
	}

	//�o�ߎ��Ԃ��X�V����
	m_timer += staticData->Get_FloatData("elapsedTime");
}

void DirecTingScene::State_PlayerAnime()
{
	//State_PlayerAnime�̏���

	//SceneManager���擾����
	SceneManager* manager = SceneManager::GetInstance();

	//�ÓI�f�[�^���擾����
	StaticData* staticData = StaticData::GetInstance();

	//SoundManager���擾����
	SoundManager* soundManager = SoundManager::GetInstance();

	//�v���C���[�̃A�j���[�V�������I������Ƃ�
	if (!m_playerAnimeTex->GetAniFlag())
	{
		//�K�E�Z���_�[�N�G�N�X�v���[�W�����̎�
		if (staticData->Get_StringData("�K�E�Z") == SelectScene::DEATHBLOW_NAME_EXPROSION)
		{
			//�ڂ��J���I����Ă���A���Ԃ��o�߂����Ƃ�
			if (m_timer > WAIT_TIME_FACE)
			{
				//�o�ߎ��Ԃ�����������
				m_timer = 0.0f;

				//������x�J�b�g�C�����o��
				manager->PushScene("CutIn");

				//�X�e�[�g��؂�ւ���
				this->AddData1(DIRECTING::EXPROSION_ANIME);

				//SE���~������
				soundManager->Stop(CRI_CUESHEET_0__41_�I�[��);

				//���Ԃ̍X�V�͍s��Ȃ�
				return;
			}
		}

		//�K�E�Z�����o�[�X�A�r���e�B�̎�
		if (staticData->Get_StringData("�K�E�Z") == SelectScene::DEATHBLOW_NAME_ABILITY)
		{
			//�ڂ��J���I����Ă���A���Ԃ��o�߂����Ƃ�
			if (m_timer > WAIT_TIME_FACE)
			{
				//�o�ߎ��Ԃ�����������
				m_timer = 0.0f;

				//������x�J�b�g�C�����o��
				manager->PushScene("CutIn");

				//�X�e�[�g��؂�ւ���
				this->AddData1(DIRECTING::BACK_FADE);
				this->AddData3(FADE_STATE::NONE);

				//SE���~������
				soundManager->Stop(CRI_CUESHEET_0__41_�I�[��);

				//���Ԃ̍X�V�͍s��Ȃ�
				return;
			}
		}


		//�o�ߎ��Ԃ��X�V����
		m_timer += staticData->Get_FloatData("elapsedTime");
	}
}

void DirecTingScene::State_BackFade()
{
	//State_BackFade�̏���

	//SoundManager���擾����
	SoundManager* soundManager = SoundManager::GetInstance();

	//SceneManager���擾����
	SceneManager* manager = SceneManager::GetInstance();

	//�ÓI�f�[�^���擾����
	StaticData* staticData = StaticData::GetInstance();

	//�v���C���[�̔w�i��`�悷��
	m_backPlayer->SetState(ITextureBase::STATE::DRAW);

	//�I�[���̕`���؂�
	m_waveAnimeTex->SetState(ITextureBase::STATE::NONE);

	//�w�i�̐F�����擾����
	Color color = m_backPlayer->GetColor();
	float alfa = color.A();

	//�X�e�[�g�ɂ���āA������؂�ւ���
	switch (this->GetNowData3())
	{
	case FADE_STATE::NONE:
		//SE���Đ�����
		soundManager->Play(CRI_CUESHEET_0__10_����);
		this->AddData3(FADE_STATE::DOWN);
		break;
	case FADE_STATE::DOWN:
		if (color.A() <= 0.5f)this->AddData3(FADE_STATE::FUP);
		else alfa -= 0.01f;
		break;
	case FADE_STATE::FUP:
		if (color.A() >= 1.0f)this->AddData3(FADE_STATE::DOWN);
		else alfa += 0.01f;
		break;
	}

	//�F�����Đݒ肷��
	color.A(alfa);
	m_backPlayer->SetColor(color);

	//���Ԃ��o�߂�����A�Ă�Scene��ǉ���������
	if (m_timer > WAIT_TIME_BACKFADE)
	{
		//������x�J�b�g�C�����o��
		manager->PushScene("CutIn");

		//�X�e�[�g��؂�ւ���
		this->AddData1(DIRECTING::END);

		//�o�ߎ��Ԃ�����������
		m_timer = 0.0f;

		//SE���~������
		soundManager->Stop(CRI_CUESHEET_0__10_����);

		//���Ԃ̍X�V���s��Ȃ�
		return;
	}

	//�o�ߎ��Ԃ��X�V����
	m_timer += staticData->Get_FloatData("elapsedTime");
}

void DirecTingScene::State_ExproAnime()
{
	//State_ExproAnime�̏���

	//�ÓI�f�[�^���擾����
	StaticData* staticData = StaticData::GetInstance();

	//SoundManager���擾����
	SoundManager* soundManager = SoundManager::GetInstance();

	//�`�������
	m_playerAnimeTex->SetState(ITextureBase::STATE::NONE);
	m_backTexture->SetState(ITextureBase::STATE::NONE);
	m_waveAnimeTex->SetState(ITextureBase::STATE::NONE);

	//1�b�����Ɏ��s
	if (m_timer > CHANGE_TIME_EXPROSION)
	{
		//���݂̓Y�������擾����
		int suffix = this->GetNowData2();

		//�o�ߎ��Ԃ�����������
		m_timer = 0.0f;

		//�A�j���[�V�������N��������
		m_exproAnimeTex[this->GetNowData2()]->SetAniFlag(true);

		//�`����N����
		m_exproAnimeTex[this->GetNowData2()]->SetState(ITextureBase::STATE::DRAW);

		//�Y�������X�V����
		suffix++;

		//�Y������ݒ肷��
		this->AddData2(suffix);

		//SE���Đ�����
		soundManager->Play(CRI_CUESHEET_0__09_������);
	}

	//�Ō�̃A�j���[�V�������N��
	if (this->GetNowData2() >= EXPROSION_ARRAY_NUM)
	{
		//�X�e�[�g��؂�ւ���
		this->AddData1(DIRECTING::END);

		//�o�ߎ��Ԃ̍X�V�͂��Ȃ�
		return;
	}

	//�o�ߎ��Ԃ��X�V����
	m_timer += staticData->Get_FloatData("elapsedTime");
}


void DirecTingScene::State_End()
{
	//State_End�̏���

	//SceneManager���擾����
	SceneManager* manager = SceneManager::GetInstance();

	for (AnimeTexture* animeTex : m_exproAnimeTex)
	{
		//�A�j���[�V�������I��
		if (!animeTex->GetAniFlag())animeTex->SetState(ITextureBase::STATE::NONE);
	}

	//�A�j���[�V�������I��
	if (!m_exproAnimeTex[ARRAY_2]->GetAniFlag() ||
		this->GetPreData1() == DIRECTING::BACK_FADE)
	{
		//���݂�Scene������
		manager->PopScene();
	}

}
