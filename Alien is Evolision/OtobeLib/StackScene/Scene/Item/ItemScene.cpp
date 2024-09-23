#include "pch.h"
#include "ItemScene.h"
#include "OtobeLib/FrameWork/ResourceManager/ResourceManager.h"
#include "OtobeLib/Texture/TextureBase/TextureBase.h"
#include "OtobeLib/FrameWork/TextureFont/TextureFont.h"
#include "OtobeLib/FrameWork/Input/Input.h"
#include "OtobeLib/StackScene/SceneManager/SceneManager.h"
#include "GameObject/Item/Manager/HealManager.h"
#include "OtobeLib/StackScene/Scene/Play/PlayScene.h"
#include "OtobeLib/FrameWork/StaticData/StaticData.h"
#include "OtobeLib/FrameWork/Stream/Stream.h"
#include "OtobeLib/FrameWork/SoundManager/SoundManager.h"
#include "OtobeLib/StackScene/Scene/Menu/MenuScene.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace OtobeLib;

//�w�i�̏������W
const Vector3 ItemScene::INIT_POS_BACK = Vector3(400.0f,0.0f,0.0f);

//�A�C�e���̏������W
const Vector3 ItemScene::INIT_POS_ITEM = Vector3(500.0f, 100.0f, 0.0f);

//�A�C�e���̗���
const Vector3 ItemScene::INIT_POS_ITEM_BACK = Vector3(500.0f, 100.0f, 0.0f);

//�������̗���
const Vector3 ItemScene::INIT_POS_EXPLANATION_COLUMN_BACK = Vector3(700.0f, 100.0f, 0.0f);

//�w�i�̐؂���ʒu
const RECT ItemScene::INIT_RECT_BACK = RECT{0,0,880,720};

//�A�C�e���̗��ʂ̐؂���ʒu
const RECT ItemScene::INIT_RECT_ITEM_BACK = RECT{ 0,0,200,200 };

//�������̐؂���ʒu
const RECT ItemScene::INIT_RECT_EXPLANATION_COLUMN_BACK = RECT{ 0,0,580,200 };

//�A�C�e�����̏������W
const Vector3 ItemScene::INIT_POS_ITEMTEXT = Vector3(550.0f,350.0f,0.0f);

//�A�C�e���̊Ԋu
const Vector3 ItemScene::ITEM_INTERVAL = Vector3(150.0f, 150.0f, 0.0f);

ItemScene::ItemScene():
	m_TextureManager(nullptr),//�摜�̊Ǘ��N���X
	m_pageNum(NULL),//���݂̃y�[�W��
	m_allPageNum(NULL),//�y�[�W����
	m_selectTexture(nullptr),//�I�𒆂̉摜
	m_zoomItem(nullptr),//�g�債�āA�\������Ă���A�C�e��
	m_rightArrowTexture(nullptr),//�E���̃e�N�X�`��
	m_leftArrowTexture(nullptr)//�����̃e�N�X�`��
{
}

void ItemScene::Initialize()
{
	//�ÓI�f�[�^���擾����
	StaticData* staticData = StaticData::GetInstance();

	//��������
	this->Create();

	//���݂̃y�[�W����ݒ肷��
	m_pageNum = staticData->Get_IntData("�y�[�W��");

	//�y�[�W������ݒ肷��
	m_allPageNum = ((int)m_itemText.size() / (int)m_itemBackText.size()) + 1;

	//���݂̃X�e�[�g��ݒ肷��
	this->SetNowState(NULL);

	//�t�@�C�����擾����
	Stream* File = Stream::GetInstance();

	//�t�@�C�����J������J��
	if (File->Open(L"FileData/ItemFont.csv", Stream::FILE::STREAM_IFSTREAM))
	{
		//�f�[�^��ǂݍ���
		std::vector<std::vector<std::string>> InputData = File->Load_Data();
		//�f�[�^��ϊ�����
		m_fontInfo = this->Cast(InputData);
		//�t�@�C�������
		File->Close();
	}

	//�I�����̏����l��ݒ肷��
	staticData->Add("�I����", (std::string)"�f�t�H���g");

	//�e�L�X�g�f�[�^�̃p�X��ݒ肷��
	staticData->Add("�X�g�[���[�p�X", (std::string)"FileData/ItemRequest.csv");
}

void ItemScene::Update()
{
	//�L�[�{�[�h�̏��
	Input* input = Input::GetInstance();
	const auto& keyState = input->GetKeybord_State();
	const auto& keyTrakker = input->GetKeybord_Trakker();
	const auto& Mouse_Trakker = Input::GetInstance()->GetMouse_Trakker();

	//SceneManager���擾����
	SceneManager* manager = SceneManager::GetInstance();

	//CollisionManager���擾����
	CollisionManager* collisionManager = GameContext::Get<CollisionManager>();

	//SoundManager���擾����
	SoundManager* soundManager = SoundManager::GetInstance();

	//�ÓI�f�[�^�̎擾
	StaticData* staticData = StaticData::GetInstance();

	//MenuScene���擾����
	MenuScene* menu = GameContext::Get<MenuScene>();

	//Player���擾����
	CoroKun* player = GameContext::Get<CoroKun>();

	//�A�C�e�����I������Ă����
	if (keyTrakker.IsKeyPressed(Keyboard::Space))
	{
		//�I�𒆂̃A�C�e�������鎞
		if (this->OnSelectItem())
		{
			//HP���ő�̎�
			if (player->GetHP() >= player->GetMaxHP())
			{
				//SE���Đ�����
				soundManager->Play(CRI_CUESHEET_0__06_�L�����Z����);
			}
			//HP���ő��菬�����Ƃ�
			else
			{
				//�I������Scene��ǉ���������
				manager->PushScene("Choices");

				//SE���Đ�����
				soundManager->Play(CRI_CUESHEET_0__05_���艹);

				//�ȍ~�̏����́A�s��Ȃ�
				return;
			}
		}
		//�A�C�e�����Ȃ��Ƃ�
		else
		{
			//SE���Đ�����
			soundManager->Play(CRI_CUESHEET_0__06_�L�����Z����);
		}
	}

	//�I�����ł͂��������ꂽ��
	if (staticData->Get_StringData("�I����") == "�͂�")
	{
		//SE���Đ�����
		soundManager->Play(CRI_CUESHEET_0__38_��);

		//�A�C�e�����g�p����
		this->UseItem();

		//�I����������������
		staticData->Add("�I����", (std::string)"�f�t�H���g");
	}

	//X�L�[�������ꂽ��A���j���[�ɖ߂�
	if (keyTrakker.IsKeyPressed(Keyboard::X))
	{
		//SE���Đ�����
		soundManager->Play(CRI_CUESHEET_0__06_�L�����Z����);

		manager->PopScene();
		return;
	}

	//ESC�������ꂽ��APlay�ɖ߂�
	if (keyTrakker.IsKeyPressed(Keyboard::Escape))
	{
		//SE���Đ�����
		soundManager->Play(CRI_CUESHEET_0__06_�L�����Z����);

		manager->PopScene(2);
		collisionManager->ResetUseHero();
		return;
	}

	//TextureManager�̍X�V
	m_TextureManager->Update();

	//�A�C�e�����̍X�V
	this->UpdateItemText();

	//�I���摜�̍X�V
	this->UpdateSelectTexture();

	//Menu��TextureManager�̍X�V��������(UI�𓮂�������)
	menu->GetTextureManager()->Update();
}

void ItemScene::Render()
{
	const auto& context = DX::DeviceResources::GetInstance()->GetD3DDeviceContext();

	Graphics* Graph = Graphics::GetInstance();

	//�t�H���g���擾����
	TextureFont* font = TextureFont::GetInstance();

	Graph->GetSpriteBatch()->Begin(DirectX::SpriteSortMode::SpriteSortMode_Deferred,
		Graph->GetCommonStates()->NonPremultiplied(), Graph->GetCommonStates()->PointClamp(),
		Graph->GetCommonStates()->DepthDefault(), nullptr,
		[&] {context->OMSetBlendState(Graph->GetCommonStates()->NonPremultiplied(), nullptr, 0xffffffff); });

	//TextureManager�̕`��
	m_TextureManager->Render();

	//for���̐擪
	int first = m_pageNum * (int)m_itemBackText.size() - (int)m_itemBackText.size();

	//�A�C�e��������΁A�\��
	if (this->OnSelectItem())
	{
		//�I�𒆂̃A�C�e�����擾
		ITextureBase* item = this->SelectNowItem();

		for (const std::pair<std::string, Vector2>& exColu : m_fontInfo[item->GetName()])
		{
			//�t�H���g��`�悷��
			font->DrawFormatString(exColu.first, exColu.second, 1.3f, Colors::Black);
		}
	}
	//�A�C�e�����Ȃ���
	else
	{
		for (const std::pair<std::string, Vector2>& exColu : m_fontInfo["�A�C�e����5"])
		{
			//�t�H���g��`�悷��
			font->DrawFormatString(exColu.first, exColu.second, 2.0f, Colors::Black);
		}
	}

	//X�L�[�̃t�H���g�̕`��
	font->DrawFormatString("X�L�|�F���j���|��", Vector2(50.0f, 60.0f), 1.0f, Colors::Black);

	//SPACE�L�[�̃t�H���g�̕`��
	font->DrawFormatString("SPACE�L�|�F����", Vector2(525.0f, 30.0f), 1.0f, Colors::Black);

	//ESC�L�[�̃t�H���g�̕`��
	font->DrawFormatString("ESC�L�|�F�v���C��", Vector2(925.0f, 30.0f), 1.0f, Colors::Black);

	//�y�[�W����`��
	font->DrawFormatString(std::to_string(m_pageNum) + "/" + std::to_string(m_allPageNum),
		Vector2(750.0f, 640.0f), 2.0f, Colors::Black);


	Graph->GetSpriteBatch()->End();
}

void ItemScene::Finalize()
{
}

void ItemScene::Create()
{
	//�ÓI�f�[�^���擾����
	StaticData* staticData = StaticData::GetInstance();

	//ResourceManager���擾����
	ResourceManager* resourceManager = ResourceManager::GetInstance();

	//TextureManager�𐶐�����
	m_TextureManager = std::make_unique<TextureManager>();

	//�g�p����e�N�X�`�����擾����
	ITextureBase* back = resourceManager->GetTexture("WhiteBack");
	ITextureBase* item = resourceManager->GetTexture("��");
	ITextureBase* arrow = resourceManager->GetTexture("���");

	//�w�i�̉摜�𐶐�����
	std::unique_ptr<ITextureBase> pback = std::make_unique<TextureBase>(back);

	//���W��ݒ肷��
	pback->SetPosition(INIT_POS_BACK);

	//�؂���ʒu��ݒ肷��
	pback->SetRect(INIT_RECT_BACK);

	//�F��ݒ肷��
	pback->SetColor(Colors::Gray);

	//�A�C�e���̗��ʂ𐶐�����
	std::unique_ptr<ITextureBase> pitemBack = std::make_unique<TextureBase>(back);

	//���W��ݒ肷��
	pitemBack->SetPosition(INIT_POS_ITEM_BACK);

	//�؂���ʒu��ݒ肷��
	pitemBack->SetRect(INIT_RECT_ITEM_BACK);

	//�F��ݒ肷��
	pitemBack->SetColor(Colors::Aqua);

	//�A�C�e���𐶐�����
	std::unique_ptr<ITextureBase> pitem = std::make_unique<TextureBase>(item);

	//���W��ݒ肷��
	pitem->SetPosition(INIT_POS_ITEM);

	//�g��{����ݒ肷��
	pitem->SetScale(Vector3(0.5f));

	//���|�C���^��o�^����
	m_zoomItem = pitem.get();

	//�������̗��ʂ𐶐�����
	std::unique_ptr<ITextureBase> pexCol = std::make_unique<TextureBase>(back);

	//���W��ݒ肷��
	pexCol->SetPosition(INIT_POS_EXPLANATION_COLUMN_BACK);

	//�؂���ʒu��ݒ肷��
	pexCol->SetRect(INIT_RECT_EXPLANATION_COLUMN_BACK);

	//TextureManager�ɒǉ�����
	m_TextureManager->Add(std::move(pback));
	m_TextureManager->Add(std::move(pitemBack));
	m_TextureManager->Add(std::move(pitem));
	m_TextureManager->Add(std::move(pexCol));

	for (int i = 0; i < COLITEM; i++)
	{
		for (int j = 0; j < ROWITEM; j++)
		{
			//�A�C�e�����̔w�ʂ𐶐�����
			std::unique_ptr<ITextureBase> pitemTextBack = std::make_unique<TextureBase>(back);

			//���W��ݒ肷��
			pitemTextBack->SetPosition(INIT_POS_ITEMTEXT + Vector3(ITEM_INTERVAL.x * j, ITEM_INTERVAL.y * i, 0.0f));

			//�؂���ʒu��ݒ肷��
			pitemTextBack->SetRect(RECT{ 0,0,120,120 });

			//�F��ݒ肷��
			pitemTextBack->SetColor(Colors::Aquamarine);
			
			//���|�C���^��o�^����
			m_itemBackText.push_back(pitemTextBack.get());

			//TextureManager�ɒǉ�����
			m_TextureManager->Add(std::move(pitemTextBack));
		}
	}

	//ItemManager���擾����
	HealManager* itemManager = GameContext::Get<PlayScene>()->GetHealManager();

	for (int i = 0;i < (int)itemManager->GetItem().size(); i++)
	{
		//�A�C�e���̏����擾
		std::pair<std::string, float>itemInfo = this->ChangeItemTag(itemManager->GetItem()[i]->GetName());

		//�g�p����e�N�X�`�����擾����
		ITextureBase* useTex = resourceManager->GetTexture(itemInfo.first);

		//�Y���������킹��
		int t = i % (int)m_itemBackText.size();

		//�摜�𐶐�����
		std::unique_ptr<Heal> pItemTex = std::make_unique<Heal>(useTex);

		//�񕜗ʂ�ݒ肷��
		pItemTex->SetHealPower(itemInfo.second);

		//�`��͑S�ď���
		pItemTex->SetState(ITextureBase::STATE::NONE);

		//���W�́A���ꂼ�ꍇ�킹��
		pItemTex->SetPosition(m_itemBackText[t]->GetPosition());

		//���|�C���^��o�^����
		m_itemText.push_back(pItemTex.get());

		//TextureManager�ɒǉ�����
		m_TextureManager->Add(std::move(pItemTex));
	}

	//�I�𒆂̉摜�𐶐�����
	std::unique_ptr<ITextureBase> pselect = std::make_unique<TextureBase>(back);

	//���W��ݒ肷��
	pselect->SetPosition(INIT_POS_ITEMTEXT);

	//�F��ݒ肷��
	pselect->SetColor(Color{ 0.0f,0.0f,1.0f,0.5f });

	//�؂���ʒu��ݒ肷��
	pselect->SetRect(RECT{ 0,0,120,120 });

	//���|�C���^��o�^����
	m_selectTexture = pselect.get();

	//TextureManager�ɒǉ�����
	m_TextureManager->Add(std::move(pselect));

	//X�L�[�̗��ʂ̃e�N�X�`���𐶐�����
	std::unique_ptr<ITextureBase> pbackXkey = std::make_unique<TextureBase>(back);
	pbackXkey->SetPosition(Vector3(30.0f, 50.0f, 0.0f));
	pbackXkey->SetRect(RECT{ 0,0,330,50 });

	//ESC�L�[�̗��ʂ̃e�N�X�`���𐶐�����
	std::unique_ptr<ITextureBase> pbackEsckey = std::make_unique<TextureBase>(back);
	pbackEsckey->SetPosition(Vector3(900.0f, 25.0f, 0.0f));
	pbackEsckey->SetRect(RECT{ 0,0,370,50 });

	//SPACE�L�[�̗��ʂ̃e�N�X�`���𐶐�����
	std::unique_ptr<ITextureBase> pbackSpace = std::make_unique<TextureBase>(back);
	pbackSpace->SetPosition(Vector3(500.0f, 25.0f, 0.0f));
	pbackSpace->SetRect(RECT{ 0,0,370,50 });

	//���̖��𐶐�����
	std::unique_ptr<Arrow> pArrowLeft = std::make_unique<Arrow>(arrow);
	pArrowLeft->InitArrow(Vector3(0.4f), Vector3(0.7f), Vector3(0.4f), 120.0f, ARROWSCALE::UPSCALE);
	pArrowLeft->SetPosition(Vector3(475.0f, 475.0f, 0.0f));
	pArrowLeft->SetOrigin(pArrowLeft->AskOrigin());
	pArrowLeft->SetAngle(180.0f);
	pArrowLeft->SetColor(Colors::Red);
	pArrowLeft->SetState(ITextureBase::STATE::NONE);
	m_leftArrowTexture = pArrowLeft.get();

	//�E�̖��𐶐�����
	std::unique_ptr<Arrow> pArrowRight = std::make_unique<Arrow>(arrow);
	pArrowRight->InitArrow(Vector3(0.4f), Vector3(0.7f), Vector3(0.4f), 120.0f, ARROWSCALE::UPSCALE);
	pArrowRight->SetPosition(Vector3(1200.0f, 475.0f, 0.0f));
	pArrowRight->SetOrigin(pArrowRight->AskOrigin());
	pArrowRight->SetColor(Colors::Red);
	pArrowRight->SetState(ITextureBase::STATE::NONE);
	m_rightArrowTexture = pArrowRight.get();

	//TextureManager�ɒǉ�����
	m_TextureManager->Add(std::move(pbackXkey));
	m_TextureManager->Add(std::move(pbackEsckey));
	m_TextureManager->Add(std::move(pbackSpace));
	m_TextureManager->Add(std::move(pArrowLeft));
	m_TextureManager->Add(std::move(pArrowRight));
}

const std::pair<std::string, float> ItemScene::ChangeItemTag(const std::string& name)
{
	//�ϐ���錾����
	std::pair<std::string, float> itemInfo;

	//�Ή�������̂ɕϊ�����
	if (name == "��")itemInfo = { "�A�C�e����1",0.5f };
	if (name == "��1")itemInfo = { "�A�C�e����2",0.1f };
	if (name == "��2")itemInfo = { "�A�C�e����3",0.2f };
	if (name == "��3")itemInfo = { "�A�C�e����4",0.3f };

	//�ϊ��������̂�Ԃ�
	return itemInfo;
}

//�I�����Ă���A�C�e������A���̃^�O�ƁA���W���擾����
const std::pair<std::string, Vector3> ItemScene::ReverseItem(const std::string& name)
{
	//�ϐ���錾����
	std::pair<std::string, Vector3> itemInfo;

	//�Ή�������̂ɕϊ�����
	if (name == "�A�C�e����1")itemInfo = { "��",INIT_POS_ITEM };
	if (name == "�A�C�e����2")itemInfo = { "��1",INIT_POS_ITEM + Vector3(75.0f,0.0f,0.0f) };
	if (name == "�A�C�e����3")itemInfo = { "��2",INIT_POS_ITEM + Vector3(40.0f,0.0f,0.0f) };
	if (name == "�A�C�e����4")itemInfo = { "��3",INIT_POS_ITEM + Vector3(40.0f,0.0f,0.0f) };
	if (name == "�A�C�e����5")itemInfo = { "�����",INIT_POS_ITEM };

	//�ϊ��������̂�Ԃ�
	return itemInfo;
}

void ItemScene::UpdateItemText()
{
	//ResourceManager���擾����
	ResourceManager* resourceManager = ResourceManager::GetInstance();

	//�y�[�W������ݒ肷��
	m_allPageNum = ((int)m_itemText.size() / (int)m_itemBackText.size()) + 1;

	//�A�C�e����8�̎��́A�y�[�W������1�ɕύX����
	if ((int)m_itemText.size() == (int)m_itemBackText.size())m_allPageNum = 1;

	//�y�[�W������1�̎�
	if (m_allPageNum <= 1)
	{
		//�y�[�W����1�ɂ���
		m_pageNum = 1;

		//�E���̕\��������
		m_rightArrowTexture->SetState(ITextureBase::STATE::NONE);

		//�����̕\��������
		m_leftArrowTexture->SetState(ITextureBase::STATE::NONE);
	}
	//���݂̃y�[�W����2�y�[�W�ڂ��瑍�y�[�W�������̏ꍇ
	else if (m_pageNum > 1 && m_pageNum < m_allPageNum)
	{
		//�E����\��
		m_rightArrowTexture->SetState(ITextureBase::STATE::DRAW);

		//������\��
		m_leftArrowTexture->SetState(ITextureBase::STATE::DRAW);
	}
	//�y�[�W�����P�y�[�W�ڂ̎�
	else if (m_pageNum == 1)
	{
		//�E����\��
		m_rightArrowTexture->SetState(ITextureBase::STATE::DRAW);

		//�����̕\��������
		m_leftArrowTexture->SetState(ITextureBase::STATE::NONE);
	}
	//�y�[�W�����ŏI�y�[�W�̎�
	else if (m_pageNum == m_allPageNum)
	{
		//�E���̕\��������
		m_rightArrowTexture->SetState(ITextureBase::STATE::NONE);

		//������\��
		m_leftArrowTexture->SetState(ITextureBase::STATE::DRAW);
	}
		
	//for���̐擪
	int first = m_pageNum * (int)m_itemBackText.size() - (int)m_itemBackText.size();

	//for���̏I���
	int finish = first + (int)m_itemBackText.size();

	//�A�C�e���̊l������8�����̏ꍇ��
	if (finish > (int)m_itemText.size())
	{
		//�I�����l�������A�C�e���̐��ɂ���
		finish = (int)m_itemText.size();
	}

	for (ITextureBase* item : m_itemText)
	{
		//�S�ẴA�C�e���̕`���؂�
		item->SetState(ITextureBase::STATE::NONE);
	}

	//���݁A�J����Ă���y�[�W�ɑ΂��āA�X�V��������
	for (int i = first; i < finish; i++)
	{
		//�Y���������킹��
		int t = i % (int)m_itemBackText.size();

		//�I�����Ă���y�[�W�̃A�C�e���̂݁A�\������
		m_itemText[i]->SetState(ITextureBase::STATE::DRAW);

		//���W�́A���ꂼ�ꍇ�킹��
		m_itemText[i]->SetPosition(m_itemBackText[t]->GetPosition());
	}

	//�A�C�e�����Ȃ���΁A�������Ȃ�
	if (!this->OnSelectItem())
	{
		//���݁A�I�����Ă���A�C�e����傫���\������
		std::pair<std::string, Vector3> zoomInfo = this->ReverseItem("�A�C�e����5");

		//���ݎg�p���Ă���e�N�X�`�����擾����
		ITextureBase* usingTexture = resourceManager->GetTexture(zoomInfo.first);

		//�e�N�X�`�������ւ���
		m_zoomItem->SetTexture(usingTexture);

		//�g��{����ݒ肷��
		m_zoomItem->SetScale(Vector3(0.5f));

		//���W���X�V����
		m_zoomItem->SetPosition(zoomInfo.second);
	}
	else
	{
		//���݁A�I�����Ă���A�C�e����傫���\������
		std::pair<std::string, Vector3> zoomInfo = this->ReverseItem(this->SelectNowItem()->GetName());

		//���ݎg�p���Ă���e�N�X�`�����擾����
		ITextureBase* usingTexture = resourceManager->GetTexture(zoomInfo.first);

		//�e�N�X�`�������ւ���
		m_zoomItem->SetTexture(usingTexture);

		//�g��{����ݒ肷��
		m_zoomItem->SetScale(Vector3(0.5f));

		//���W���X�V����
		m_zoomItem->SetPosition(zoomInfo.second);
	}
}

void ItemScene::UpdateSelectTexture()
{
	//�L�[�{�[�h�̏��
	Input* input = Input::GetInstance();
	const auto& keyState = input->GetKeybord_State();
	const auto& keyTrakker = input->GetKeybord_Trakker();
	const auto& Mouse_Trakker = Input::GetInstance()->GetMouse_Trakker();

	//�E�L�[�������ꂽ��A�E�֑I���摜���ړ�
	if (keyTrakker.IsKeyPressed(Keyboard::Right))
	{
		//�I�𒆂̏���
		this->InputSelect(1, [&](int& state, Vector3& pos)
			{
				//����𒴂�����A�y�[�W���X�V
				if (state % ROWITEM == NULL)
				{
					//�y�[�W�����ɒB���Ă��Ȃ����
					if (m_pageNum < m_allPageNum)
					{
						//�y�[�W�����X�V
						m_pageNum++;
					}
					//�I��Ώۂ������ʒu�ɖ߂�
					pos.x = INIT_POS_ITEMTEXT.x;

					//�X�e�[�g�����ɖ߂�
					state -= ROWITEM;
				}
				//����𒴂��Ă��Ȃ���΁A����ւ���
				else
				{
					//�ׂ̍��W���擾����
					Vector3 nextPos = m_itemBackText[state]->GetPosition();

					//����ւ����s��
					std::swap(pos, nextPos);
				}
				;}
		);
	}

	//���L�[�������ꂽ��A���֑I���摜���ړ�
	if (keyTrakker.IsKeyPressed(Keyboard::Left))
	{
		//�I�𒆂̏���
		this->InputSelect(-1, [&](int& state, Vector3& pos)
			{
				//����𒴂�����A�y�[�W���X�V
				if ((state + 1) % ROWITEM == NULL)
				{
					//1�y�[�W�ڈȍ~�̏ꍇ
					if (m_pageNum > 1)
					{
						//�y�[�W�����X�V
						m_pageNum--;
					}
					//�X�e�[�g�����ɖ߂�
					state += ROWITEM;

					//�I��Ώۂ������ʒu�ɖ߂�
					pos.x = m_itemBackText[state]->GetPosition().x;
				}
				//����𒴂��Ă��Ȃ���΁A����ւ���
				else
				{
					//�ׂ̍��W���擾����
					Vector3 nextPos = m_itemBackText[state]->GetPosition();

					//����ւ����s��
					std::swap(pos, nextPos);
				}
				; }
		);
	}

	//��L�[�������ꂽ��A��֑I���摜���ړ�
	if (keyTrakker.IsKeyPressed(Keyboard::Up))
	{
		//�I�𒆂̏���
		this->InputSelect(-4, [&](int& state, Vector3& pos)
			{
				//����𒴂�����A�y�[�W���X�V
				if (state < NULL)
				{
					//�X�e�[�g�����ɖ߂�
					state += ROWITEM * 2;

					//�I��Ώۂ������ʒu�ɖ߂�
					pos.y = m_itemBackText[state]->GetPosition().y;
				}
				//����𒴂��Ă��Ȃ���΁A����ւ���
				else
				{
					//�ׂ̍��W���擾����
					Vector3 nextPos = m_itemBackText[state]->GetPosition();

					//����ւ����s��
					std::swap(pos, nextPos);
				}
			});
	}

	//���L�[�������ꂽ��A���֑I���摜���ړ�
	if (keyTrakker.IsKeyPressed(Keyboard::Down))
	{
		//�I�𒆂̏���
		this->InputSelect(4, [&](int& state, Vector3& pos)
			{
				//����𒴂�����A�y�[�W���X�V
				if (state >= (int)m_itemBackText.size())
				{
					//�X�e�[�g�����ɖ߂�
					state -= ROWITEM * 2;

					//�I��Ώۂ������ʒu�ɖ߂�
					pos.y = m_itemBackText[state]->GetPosition().y;
				}
				//����𒴂��Ă��Ȃ���΁A����ւ���
				else
				{
					//�ׂ̍��W���擾����
					Vector3 nextPos = m_itemBackText[state]->GetPosition();

					//����ւ����s��
					std::swap(pos, nextPos);
				}
			});
	}
}

void ItemScene::InputSelect(const int& move, std::function<void(int&, Vector3&)> func)
{
	//SoundManager���擾����
	SoundManager* soundManager = SoundManager::GetInstance();

	//SE���Đ�
	soundManager->Play(CRI_CUESHEET_0__07_�㉺��);

	//���݂̑I��Ώۂ��擾����
	int state = this->GetNowState();

	//�I�𒆂̍��W���擾����
	Vector3 nowPos = m_selectTexture->GetPosition();

	//�X�e�[�g���w�肵�����������炷
	state += move;

	//if���̏���
	func(state, nowPos);

	//�I��Ώۂ̉摜�̍X�V���s��
	m_selectTexture->SetPosition(nowPos);

	//�X�e�[�g�̍X�V���s��
	this->SetNowState(state);
}

const bool ItemScene::OnSelectItem()
{
	//for���̐擪
	int first = m_pageNum * (int)m_itemBackText.size() - (int)m_itemBackText.size();

	//�A�C�e��������΁A�\��
	if (first + this->GetNowState() < (int)m_itemText.size())return true;

	return false;
}

Item* ItemScene::SelectNowItem()
{
	//for���̐擪
	int first = m_pageNum * (int)m_itemBackText.size() - (int)m_itemBackText.size();

	//���݂̃A�C�e����Ԃ�
	return m_itemText[first + this->GetNowState()];
}

void ItemScene::UseItem()
{
	//Player���擾����
	CoroKun* player = GameContext::Get<CoroKun>();

	//ItemManager���擾����
	HealManager* itemManager = GameContext::Get<PlayScene>()->GetHealManager();

	//�A�C�e�����擾����
	Item* item = this->SelectNowItem();

	//�L���X�g����
	Heal* healItem = dynamic_cast<Heal*>(item);

	//�񕜗ʂ�ݒ肷��
	int heal = player->GetMaxHP() * healItem->GetHealPower();

	//�A�C�e���ɉ�����HP���񕜂���
	player->OnHealHp(heal);

	//�g�p�����A�C�e��������
	item->SetIsVal(true);
	m_itemText.erase(m_itemText.begin() + this->OnSelectPlace());
	itemManager->EraseItem(this->OnSelectPlace());
}

const int ItemScene::OnSelectPlace()
{
	//for���̐擪
	int first = m_pageNum * (int)m_itemBackText.size() - (int)m_itemBackText.size();

	//�Y������Ԃ�
	return first + this->GetNowState();
}

const FontInfo ItemScene::Cast(const std::vector<std::vector<std::string>>& input)
{
	//�f�[�^���i�[����R���e�i��錾����
	FontInfo itemFont;

	//�f�[�^���i�[����R���e�i��錾����
	std::vector<std::vector<std::string>> strContena;

	//�f�[�^�̐���錾����
	strContena.resize(input.size() - 1);

	//�w�肵���O���[�v�̏����擾����
	for (size_t i = 0; i < input.size() - 1; i++)
	{
		for (size_t j = 0; j < input[i].size(); j++)
		{
			//1�s���f�[�^���i�[����
			strContena[i].push_back(input[i + 1][j]);
		}
	}

	//�f�[�^�̌^��ϊ�����
	for (int i = 0; i < (int)strContena.size(); i++)
	{
		itemFont[strContena[i][0]].push_back({ strContena[i][1],Vector2(std::stof(strContena[i][2]),std::stof(strContena[i][3])) });
	}

	//�ϊ������f�[�^��Ԃ�
	return itemFont;
}
