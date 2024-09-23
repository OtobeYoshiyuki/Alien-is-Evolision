#include "pch.h"
#include "SelectScene.h"
#include "OtobeLib/StackScene/SceneManager/SceneManager.h"
#include "OtobeLib/FrameWork/ResourceManager/ResourceManager.h"
#include "OtobeLib/FrameWork/Input/Input.h"
#include "OtobeLib/FrameWork/TextureFont/TextureFont.h"
#include "OtobeLib/FrameWork/StaticData/StaticData.h"
#include "OtobeLib/StackScene/Scene/Play/PlayScene.h"
#include "OtobeLib/FrameWork/SoundManager/SoundManager.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace OtobeLib;

//�K�E�Z�̖��O(����)
const std::string SelectScene::DEATHBLOW_NAME_EXPROSION_TOP = "�_�|�N";
const std::string SelectScene::DEATHBLOW_NAME_EXPROSION_BOTOM = "�G�N�X�v���|�W����";
const std::string SelectScene::DEATHBLOW_NAME_EXPROSION = "�_�|�N�G�N�X�v���|�W����";

//�K�E�Z�̖��O(�A�r���e�B)
const std::string SelectScene::DEATHBLOW_NAME_ABILITY_TOP = "���o�|�X";
const std::string SelectScene::DEATHBLOW_NAME_ABILITY_BOTOM = "�A�r���e�B";
const std::string SelectScene::DEATHBLOW_NAME_ABILITY = "���o�|�X�A�r���e�B";

//�_�ł̊Ԋu�̍ŏ��l
const float SelectScene::BLINK_INTERVAL_MIN = 0.1f;

SelectScene::SelectScene():
	m_backTexture(nullptr),//�w�i�̉摜
	m_TextureManager(nullptr),//�摜�̊Ǘ��N���X
	m_frameTexture(nullptr)//����̘g�̉摜
{
	//ResourceManager���擾����
	ResourceManager* resourceManager = ResourceManager::GetInstance();

	//�g�p����e�N�X�`�����擾����
	ITextureBase* back = resourceManager->GetTexture("�I��w�i");
	ITextureBase* whiteBack = resourceManager->GetTexture("WhiteBack");
	ITextureBase* frame = resourceManager->GetTexture("����̘g");

	//CoroKun���擾
	CoroKun* player = GameContext::Get<CoroKun>();

	//TextureManager�𐶐�����
	m_TextureManager = std::make_unique<TextureManager>();

	//�w�i�̉摜�𐶐�����
	std::unique_ptr<ITextureBase> pback = std::make_unique<TextureBase>(back);
	//�`�揇��ݒ肷��
	pback->SetDrawOrder((int)DRAWORDER::BACKGROUND);
	//���|�C���^��o�^����
	m_backTexture = pback.get();
	//TextureManager�ɓo�^����
	m_TextureManager->Add(std::move(pback));

	//�������̍���ݒ肷��
	Color blackHalf = Color{ 0.0f,0.0f,0.0f,0.5f };

	for (int i = 0; i < ARRAY_NUM; i++)
	{
		//�l�p�̉摜�𐶐�����
		std::unique_ptr<TextureBase> psquare = std::make_unique<TextureBase>(whiteBack);
		//�`�揇��ݒ肷��
		psquare->SetDrawOrder((int)DRAWORDER::WHITEBACK);
		//�؂���ʒu��ݒ肷��
		psquare->SetRect(RECT{ 0,0,600,200 });
		//�F��ݒ肷��
		psquare->SetColor(Colors::White);
		//�_�ł̐ݒ�
		psquare->SetBlink(false);
		//�_�ł̕b��
		psquare->SetFinal(5);
		//�_�ŃI�v�V�����̉��
		psquare->SetFinalOption(true);
		//�_�ł̊Ԋu�̐ݒ�
		psquare->SetTarget(1.0f);
		//���|�C���^��o�^����
		m_squareTexture.push_back(psquare.get());
		//TextureManager�ɓo�^����
		m_TextureManager->Add(std::move(psquare));
	}

	//�l�p�̉摜�̍��W�̐ݒ���s��
	m_squareTexture[ARRAY_0]->SetPosition(Vector3(300.0f, 50.0f, 0.0f));
	m_squareTexture[ARRAY_1]->SetPosition(Vector3(300.0f, 400.0f, 0.0f));


	for (int i = 0; i < ARRAY_NUM; i++)
	{
		//�S�ʂ̔������̉摜�𐶐�����
		std::unique_ptr<ITextureBase> phalfTex = std::make_unique<TextureBase>(whiteBack);

		//�`�揇��ݒ肷��
		phalfTex->SetDrawOrder((int)DRAWORDER::FRONT_SELECT);

		//�؂���ʒu��ݒ肷��
		phalfTex->SetRect(RECT{ 0,0,1280,330 });

		//���|�C���^��o�^����
		m_frontTexture.push_back(phalfTex.get());

		//TextureManager�ɓo�^����
		m_TextureManager->Add(std::move(phalfTex));
	}

	//�S�ʂ̔������̉摜�̍��W�̐ݒ���s��
	m_frontTexture[ARRAY_0]->SetPosition(Vector3(0.0f, 0.0f, 0.0f));
	m_frontTexture[ARRAY_1]->SetPosition(Vector3(0.0f, 330.0f, 0.0f));

	//�S�ʂ̔������̉摜�̐F�̐ݒ���s��
	if (player->GetSpecial() < GAGE_COST_EXPROSION)
	{
		m_frontTexture[ARRAY_0]->SetColor(Vector4(0.0f,0.0f,0.0f,0.75f));
	}
	else m_frontTexture[ARRAY_0]->SetColor(Vector4::Zero);

	m_frontTexture[ARRAY_1]->SetColor(blackHalf);

	//��ʉ�����UI�𐶐�����
	std::unique_ptr<ITextureBase> pBotomUI = std::make_unique<TextureBase>(whiteBack);
	pBotomUI->SetPosition(Vector3(0.0f, 660.0f, 0.0f));
	pBotomUI->SetRect(RECT{ 0,0,1280,60 });
	m_TextureManager->Add(std::move(pBotomUI));

	//����̘g��UI�𐶐�����
	std::unique_ptr<ITextureBase> pframe = std::make_unique<TextureBase>(frame);
	pframe->SetDrawOrder((int)DRAWORDER::SELECT_FRAME);
	m_frameTexture = pframe.get();
	m_TextureManager->Add(std::move(pframe));

	//�I�𒆂̕K�E�Z��ݒ肷��
	this->AddData2(ARRAY_0);

	//���݂̃X�e�[�g��ݒ肷��
	this->AddData1(SELECT::WAITKEY);
}

void SelectScene::Initialize()
{
	//SceneManager����SceneState���擾����
	SceneManager* Manager = SceneManager::GetInstance();
	SceneState* state = Manager->GetState();

	state->SetFadeState(SceneState::WAITKEY);
	state->SetAlfa(0.0f);
}

void SelectScene::Update()
{
	//SceneManager����SceneState���擾����
	SceneManager* Manager = SceneManager::GetInstance();
	SceneState* state = Manager->GetState();

	//�X�e�[�g�̍X�V����
	this->UpdateState();

	//TextureManager�̍X�V
	m_TextureManager->Update();
}

void SelectScene::Render()
{
	//�`��n�̏�Ԃ��擾����
	Graphics* Graph = Graphics::GetInstance();
	const auto& context = DX::DeviceResources::GetInstance()->GetD3DDeviceContext();

	//�t�H���g�̏�Ԃ��擾����
	TextureFont* Font = TextureFont::GetInstance();

	//�`��̊J�n
	Graph->GetSpriteBatch()->Begin(DirectX::SpriteSortMode::SpriteSortMode_Deferred,
		Graph->GetCommonStates()->NonPremultiplied(), Graph->GetCommonStates()->PointClamp(),
		Graph->GetCommonStates()->DepthDefault(), nullptr,
		[&] {context->OMSetBlendState(Graph->GetCommonStates()->NonPremultiplied(), nullptr, 0xffffffff); });

	//�摜�̕`�揈��
	m_TextureManager->Render();

	//�Z���̕`��
	if (m_squareTexture[ARRAY_0]->NowBlinking())
	{
		Font->DrawFormatString(DEATHBLOW_NAME_EXPROSION_TOP, Vector2(550.0f, 70.0f), 1.5f, Colors::Black);
		Font->DrawFormatString(DEATHBLOW_NAME_EXPROSION_BOTOM, Vector2(400.0f, 150.0f), 1.5f, Colors::Black);
	}

	if (m_squareTexture[ARRAY_1]->NowBlinking())
	{
		Font->DrawFormatString(DEATHBLOW_NAME_ABILITY_TOP, Vector2(500.0f, 450.0f), 1.5f, Colors::Black);
		Font->DrawFormatString(DEATHBLOW_NAME_ABILITY_BOTOM, Vector2(550.0f, 520.0f), 1.5f, Colors::Black);
	}

	//�t�H���g�̕`��
	//�㉺�L�[�̃t�H���g�̕`��
	Font->DrawFormatString("�㉺�L�|�F�ړ�", Vector2(10.0f, 670.0f), 1.0f, Colors::Black);

	//SPACE�L�[�̃t�H���g�̕`��
	Font->DrawFormatString("SPACE�L�|�F����", Vector2(500.0f, 670.0f), 1.0f, Colors::Black);

	//ESC�L�[�̃t�H���g�̕`��
	Font->DrawFormatString("ESC�L�|�F�߂�", Vector2(1000.0f, 670.0f), 1.0f, Colors::Black);

	//�`��̏I��
	Graph->GetSpriteBatch()->End();
}

void SelectScene::Finalize()
{
}

void SelectScene::Create()
{
}

void SelectScene::UpdateState()
{
	//�X�e�[�g�̍X�V����

	//Switch���ŏ����𕪂���
	switch (this->GetNowData1())
	{
	case SELECT::WAITKEY:
		this->State_WaitKey();
		break;
	case SELECT::CALCING:
		this->State_Calcing();
		break; 
	case SELECT::CHANGE_CUTIN:
		this->State_Change_CutIn();
		break;
	case SELECT::CHANGE_AND_DELETE:
		this->State_Change_Delete();
		break;
	}
}

void SelectScene::State_WaitKey()
{
	//WAITKEY�̏���

	//CoroKun���擾
	CoroKun* player = GameContext::Get<CoroKun>();

	//SceneManager����SceneState���擾����
	SceneManager* Manager = SceneManager::GetInstance();
	SceneState* state = Manager->GetState();

	//�ÓI�f�[�^���擾����
	StaticData* staticData = StaticData::GetInstance();

	//�L�[�̏�Ԃ̎擾
	Input* input = Input::GetInstance();
	const auto& keyState = input->GetKeybord_State();
	const auto& keyTrakker = input->GetKeybord_Trakker();

	//SoundManager���擾����
	SoundManager* soundManager = SoundManager::GetInstance();

	//Esc�L�[�������ꂽ��APlayScene�ɖ߂�
	if (keyTrakker.IsKeyPressed(Keyboard::Escape))
	{
		//SE���Đ�����
		soundManager->Play(CRI_CUESHEET_0__06_�L�����Z����);

		Manager->PopScene();
	}

	//�㉺�̃L�[�������ꂽ��A�I����ύX����
	if (keyTrakker.IsKeyPressed(Keyboard::Up) ||
		keyTrakker.IsKeyPressed(Keyboard::Down))
	{
		//�I��Ώۂ��擾����
		int target = this->GetNowData2();

		//�I��Ώۂ��f�N�������g����
		target--;

		//�I��Ώۂ��Βl�ɒ���
		target = abs(target);

		//�F���擾����
		Color preColor = m_frontTexture[ARRAY_0]->GetColor();
		Color nowColor = m_frontTexture[ARRAY_1]->GetColor();

		//����ւ���
		std::swap(preColor, nowColor);

		//�F���Đݒ肷��
		m_frontTexture[ARRAY_0]->SetColor(preColor);
		m_frontTexture[ARRAY_1]->SetColor(nowColor);

		//�I��Ώۂ�ݒ肷��
		this->AddData2(target);

		//�_�[�N�G�N�X�v���[�W�����̎�
		if (target == ARRAY_0)
		{
			//SP��300�ȏ゠��Ƃ�
			if (player->GetSpecial() >= SelectScene::GAGE_COST_EXPROSION)
			{
				m_frontTexture[target]->SetColor(Color(0.0f, 0.0f, 0.0f, 0.0f));
			}
			//SP��300�����̎�
			else m_frontTexture[target]->SetColor(Color(0.0f, 0.0f, 0.0f, 0.75f));
		}
		//�^�[�Q�b�g����Ȃ��Ƃ�
		else m_frontTexture[ARRAY_0]->SetColor(Color(0.0f, 0.0f, 0.0f, 0.5f));

		//���o�[�X�A�r���e�B�̎�
		if (target == ARRAY_1)
		{
			//SP��100�ȏ゠��Ƃ�
			if (player->GetSpecial() >= SelectScene::GAGE_COST_REVASEAVIRITY)
			{
				m_frontTexture[target]->SetColor(Color(0.0f, 0.0f, 0.0f, 0.0f));
			}
			//SP��300�����̎�
			else m_frontTexture[target]->SetColor(Color(0.0f, 0.0f, 0.0f, 0.75f));
		}
		//�^�[�Q�b�g����Ȃ��Ƃ�
		else m_frontTexture[ARRAY_1]->SetColor(Color(0.0f, 0.0f, 0.0f, 0.5f));

		//�I�𒆂̉摜�̍��W��ݒ肷��
		m_frameTexture->SetPosition(m_frontTexture[target]->GetPosition());

		//SE���Đ�����
		soundManager->Play(CRI_CUESHEET_0__07_�㉺��);
	}

	//�X�y�[�X�L�[�������ꂽ��A�I��Ώۂ�_�ł�����
	if (keyTrakker.IsKeyPressed(Keyboard::Space))
	{
		//�_�[�N�G�N�X�v���[�W������I��
		if (this->GetNowData2() == ARRAY_0)
		{
			//�Q�[�W��300�ȏ�̎�
			if (player->GetSpecial() >= GAGE_COST_EXPROSION)
			{
				//�ÓI�f�[�^�ɏ���ǉ�����
				staticData->Add("�K�E�Z", DEATHBLOW_NAME_EXPROSION);

				//�K�E�Z���g�p����
				player->OnSpesial(GAGE_COST_EXPROSION);

				//�X�e�[�g��؂�ւ���
				this->AddData1(SELECT::CALCING);

				//�_�ł�����
				m_squareTexture[this->GetNowData2()]->SetBlink(true);

				//SE���Đ�����
				soundManager->Play(CRI_CUESHEET_0__05_���艹);
			}
			else
			{
				//SE���Đ�����
				soundManager->Play(CRI_CUESHEET_0__06_�L�����Z����);
			}
		}

		//���o�[�X�A�r���e�B��I��
		if (this->GetNowData2() == ARRAY_1)
		{
			//�Q�[�W��100�ȏ�̎�
			if (player->GetSpecial() >= GAGE_COST_REVASEAVIRITY)
			{
				//�ÓI�f�[�^�ɏ���ǉ�����
				staticData->Add("�K�E�Z", DEATHBLOW_NAME_ABILITY);

				//�K�E�Z���g�p����
				player->OnSpesial(GAGE_COST_REVASEAVIRITY);

				//�X�e�[�g��؂�ւ���
				this->AddData1(SELECT::CALCING);

				//�_�ł�����
				m_squareTexture[this->GetNowData2()]->SetBlink(true);

				//SE���Đ�����
				soundManager->Play(CRI_CUESHEET_0__05_���艹);
			}
			else
			{
				//SE���Đ�����
				soundManager->Play(CRI_CUESHEET_0__06_�L�����Z����);
			}
		}
	}
}

void SelectScene::State_Calcing()
{
	//CALCING�̏���

	//SoundManager���擾����
	SoundManager* soundManager = SoundManager::GetInstance();

	//�I��Ώۂ��擾����
	TextureBase* target = m_squareTexture[this->GetNowData2()];

	//�_�ŊԊu���擾����
	float interval = target->GetTarget();

	//�_�ł̊Ԋu��0.1f�܂Ō��炷
	if (interval > BLINK_INTERVAL_MIN)
	{
		interval -= 0.005f;
	}

	//�_�ŊԊu��ݒ肷��
	target->SetTarget(interval);

	//���ݕ\������Ă���΁ASE���Đ�����
	if (target->GetTargetCount() <= 0.0f)
	{
		soundManager->Play(CRI_CUESHEET_0__40_�_��);
	}

	//�_�ł��I��
	if (!target->GetBlink())
	{
		//�X�e�[�g��؂�ւ���
		this->AddData1(SELECT::CHANGE_CUTIN);
	}
}

void SelectScene::State_Change_CutIn()
{
	//CHANGE_CUTIN�̏���

	//SceneManager����SceneState���擾����
	SceneManager* Manager = SceneManager::GetInstance();

	//CutInScene��ǉ���������
	Manager->PushScene("CutIn");

	//�X�e�[�g��؂�ւ���
	this->AddData1(SELECT::CHANGE_AND_DELETE);
}

void SelectScene::State_Change_Delete()
{
	//CHANGE_AND_DELETE�̏���

	//SceneManager����SceneState���擾����
	SceneManager* Manager = SceneManager::GetInstance();

	//���݂�Scene������
	Manager->PopScene();

	//DirecTingScene��ǉ���������
	Manager->PushScene("DirecTing");
}

