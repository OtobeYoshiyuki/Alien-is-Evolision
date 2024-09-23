#include "pch.h"
#include "HealManager.h"
#include "OtobeLib/StackScene/Scene/Play/PlayScene.h"
#include "GameContext.h"
#include "OtobeLib/FrameWork/StaticData/StaticData.h"
#include "OtobeLib/FrameWork/ResourceManager/ResourceManager.h"
#include "OtobeLib/FrameWork/SoundManager/SoundManager.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace OtobeLib;

//�A�C�e���������Ԋu(1��)
const float HealManager::FIRING_INTERVAL = 60.0f;

HealManager::HealManager():
	m_timer(0.0f)//�o�ߎ���
{
}

void HealManager::Initialize()
{
	//ResourceManager���擾����
	ResourceManager* resourceManager = ResourceManager::GetInstance();

	//�g�p����e�N�X�`�����擾����
	ITextureBase* useTexture = resourceManager->GetTexture("��");

	//PlayScene���擾����
	PlayScene* play = GameContext::Get<PlayScene>();

	//�Y����������������
	this->SetNowState(NULL);

	for (int i = 0; i < HEALITEM_NUM; i++)
	{
		//�񕜃A�C�e���𐶐�����
		this->CreateItem(useTexture, [&](Item* item)
			{
				Heal* healItem = dynamic_cast<Heal*>(item);
				m_healItem.push_back(healItem); });
	}

	//�O��擾�����A�C�e���𔽉f����
	std::vector<std::string> itemData = play->GetSaveDataArray((int)PlayScene::SAVE_DATA::HEAL_INFO);

	//�A�C�e�����l�����Ă��Ȃ��ꍇ�́A�������Ȃ�
	if (itemData[0] == "�Ȃ�")return;

	//�擾�����A�C�e���̐������A�l�����������ɂ���
	for (std::string& itemStr : itemData)
	{
		//�g�p����e�N�X�`�����擾����
		ITextureBase* itemTexture = resourceManager->GetTexture(itemStr);

		//�A�C�e���𐶐�����
		std::unique_ptr<Item> pItem = std::make_unique<Item>(itemTexture);

		//�`���؂�
		pItem->SetState(ITextureBase::STATE::NONE);

		//�ǉ�����
		this->AddItem(std::move(pItem));
	}
}

void HealManager::Update()
{
	//UI�̕\�����I���܂ŉ������Ȃ�
	TimeLag* go = GameContext::Get<PlayScene>()->GetLagGo().get();
	if (!go->GetFinishFlag())return;

	//�ÓI�f�[�^���擾����
	StaticData* staticData = StaticData::GetInstance();

	//�O�̃t���[������̌o�ߎ���
	float elapsedTime = staticData->Get_FloatData("elapsedTime");

	//CameraManager���擾����
	CameraManager* cameraManager = GameContext::Get<CameraManager>();

	//���݉ғ����Ă���J�������擾����
	CameraBase* nowCamera = cameraManager->GetNowCamera();

	//SoundManager���擾����
	SoundManager* soundManager = SoundManager::GetInstance();

	//�J�������X�N���[���J�����̎��́A�X�V���~�߂�
	if (!nowCamera->GetObjectMove())return;

	//�Y�������擾����
	int firing = this->GetNowState();

	//Player���擾����
	CoroKun* player = GameContext::Get<CoroKun>();

	//���g�̍��W���擾����
	Vector3 pos = m_healItem[firing]->GetPosition();

	//���W��ݒ肷��
	pos = Vector3(player->GetPosition().x, pos.y, pos.z);

	//1���o�߂�����A�񕜃A�C�e�����o��
	if (m_timer > FIRING_INTERVAL)
	{
		//�Y�������͈͓����Q�Ƃ���ꍇ�̂�
		if (firing < (int)m_healItem.size())
		{
			//�������������s����
			m_healItem[firing]->OnFall(pos);

			//�����Đ�����
			soundManager->Play(CRI_CUESHEET_0__35_����);

			//�Y�������X�V����
			firing++;

			//�X�e�[�g���X�V����
			this->SetNowState(firing);

			//�o�ߎ��Ԃ�����������
			m_timer = 0.0f;
		}

		//���Ԃ̍X�V�͍s��Ȃ�
		return;
	}

	//���Ԃ��X�V����
	m_timer += elapsedTime;
}

//�񕜃A�C�e���𐶐�����
void HealManager::CreateItem(ITextureBase* refTex, std::function<void(Item*)> func)
{
	//TextureManager���擾����
	TextureManager* gameManager = GameContext::Get<PlayScene>()->GetGameManager().get();

	//Stage���擾����
	Stage* stage = GameContext::Get<Stage>();

	//�}�b�v�����擾����
	std::vector<std::vector<ITextureBase*>>& mapAll = stage->GetMap();

	//�e�N�X�`���𐶐�����
	std::unique_ptr<Item> phealItem = std::make_unique<Heal>(refTex);

	//�`�������
	phealItem->SetState(ITextureBase::STATE::NONE);

	//�����ʒu��ݒ肷��
	phealItem->SetPosition(Heal::INIT_POS);

	//���_��ݒ肷��
	phealItem->SetOrigin(phealItem->AskOrigin());

	//�g��{����ݒ肷��
	phealItem->SetScale(Heal::INIT_SCALE);

	//���|�C���^��o�^����
	func(phealItem.get());

	//���|�C���^��o�^����
	m_allItem.push_back(phealItem.get());

	//�}�b�v����ǉ�����
	mapAll[mapAll.size() - 1].push_back(phealItem.get());

	//TextureManager�ɒǉ�����
	gameManager->Add(std::move(phealItem));
}
