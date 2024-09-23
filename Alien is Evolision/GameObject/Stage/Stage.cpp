#include "pch.h"
#include "Stage.h"
#include "OtobeLib/FrameWork/Stream/Stream.h"
#include "OtobeLib/FrameWork/ResourceManager/ResourceManager.h"
#include "OtobeLib/StackScene/Scene/Play/PlayScene.h"
#include "GameContext.h"
#include "GameObject/Player/CoroKun.h"
#include "OtobeLib/FrameWork/Collision/CollisionManager/CollisionManager.h"
#include "OtobeLib/FrameWork/StaticData/StaticData.h"
#include "GameObject/Stage/Maptip/Floor/Floor.h"
#include "GameObject/Stage/Maptip/Wall/Wall.h"
#include "GameObject/Stage/Maptip/Babul/Babul.h"
#include "GameObject/Stage/Maptip/Rebound/Rebound.h"
#include "GameObject/Stage/Maptip/HoleSwitch/HoleSwitch.h"
#include "GameObject/Stage/Maptip/HoleFloor/HoleFloor.h"
#include "GameObject/Enemy/Normal/NormalEnemy.h"
#include "GameObject/Enemy/Super/SuperEnemy.h"
#include "GameObject/Enemy/Hyper/HyperEnemy.h"
#include "GameObject/Technique/BodyBlow/BodyBlow.h"
#include "GameObject/CheckPoint/CheckPoint.h"


using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace OtobeLib;

const float Stage::SCROLL_SPEED = 5.0f;

Stage::Stage():m_StageFile(FIRST_STAGE),m_CoroKun(nullptr)
{
}

void Stage::Initialize(const std::wstring& filenamemain, const std::wstring& filenamesub)
{
	//�t�@�C���̏�Ԃ��擾����
	Stream* File = Stream::GetInstance();

	//�~�j�}�b�v���擾����
	MiniMap* miniMap = GameContext::Get<MiniMap>();

	//�t�@�C�����J��

	if (File->Open(filenamesub.c_str(), Stream::FILE::STREAM_IFSTREAM))
	{
		//�f�[�^��ǂݍ���
		std::vector<std::vector<std::string>>LoadData = File->Load_Data();
		//�ϊ��O�̃f�[�^��ۑ�����
		m_subBeforeData = LoadData;
		//String����int��Convert����
		std::vector<std::vector<int>>ConvertData = this->FileDataConvert(LoadData);
		//�ϊ���̃f�[�^��ۑ�����
		m_subAfterData = ConvertData;
		//�z��̏c�̗v�f���m�ۂ���
		m_MapTip.resize(ConvertData.size() + 1);
		//�}�b�v�`�b�v�𐶐�����
		this->CreateMapTip(ConvertData);
		//�t�@�C�������
		File->Close();
	}

	//�t�@�C�����J��
	if (File->Open(filenamemain.c_str(), Stream::FILE::STREAM_IFSTREAM))
	{
		//�f�[�^��ǂݍ���
		std::vector<std::vector<std::string>>LoadData = File->Load_Data();
		//�ϊ��O�̃f�[�^��ۑ�����
		m_mainBeforeData = LoadData;
		//String����int��Convert����
		std::vector<std::vector<int>>ConvertData = this->FileDataConvert(LoadData);
		//�ϊ���̃f�[�^��ۑ�����
		m_mainAfterData = ConvertData;
		//�}�b�v�`�b�v�𐶐�����
		this->CreateMapTip(ConvertData);
		//�t�@�C�������
		File->Close();
	}

	//�������ƂɁA���X�g�Ƀ|�C���^��o�^����
	this->AddVector();

	//���ꂼ��̏��������Ă�
	for (std::vector<ITextureBase*>mapVec : m_MapTip)
	{
		for (ITextureBase* map : mapVec)
		{
			map->Initialize();
		}
	}
}

void Stage::Update()
{
}

const std::vector<std::vector<int>> Stage::FileDataConvert(const std::vector<std::vector<std::string>>& Input)
{
	//�ϐ���錾����
	std::vector<std::vector<int>>ConvertData;

	//�c�̗v�f�������m�ۂ���
	ConvertData.resize(Input.size());

	for (size_t i = 0; i < Input.size(); i++)
	{
		for (size_t j = 0; j < Input[i].size(); j++)
		{
			//int�ɕϊ��������̂�ǉ�����
			ConvertData[i].push_back(std::stoi(Input[i][j]));
		}
	}

	//Vector��Ԃ�
	return ConvertData;
}

void Stage::CreateMapTip(const std::vector<std::vector<int>>& Input)
{
	//ResourceManager���擾����
	ResourceManager* resourceManager = ResourceManager::GetInstance();
	ITextureBase* mapTip = resourceManager->GetTexture("�}�b�v�`�b�v");

	//TextureManager���擾����
	TextureManager* gameManager = GameContext::Get<PlayScene>()->GetGameManager().get();

	for (size_t i = 0; i < Input.size(); i++)
	{
		for (size_t j = 0; j < Input[i].size(); j++)
		{
			//std::unique_ptr��錾����
			std::unique_ptr<ITextureBase>pMapTip;
			//�f�[�^����ɓ������Ƃ��́A�������΂�
			if ((MapTip::MAP_TYPE)Input[i][j] == MapTip::MAP_TYPE::NONE)continue;
			//�}�b�v�`�b�v�𐶐����āA�K�v�ȏ���ϊ�����
			MapTip::CreateMapTip(Input[i][j],pMapTip);
			//���W��ݒ肷��
			float x = j * /*(pMapTip->GetRect().right - pMapTip->GetRect().left)*/40 + CoroKun::MIN_LIMIT.x;
			float y = i * /*(pMapTip->GetRect().bottom - pMapTip->GetRect().top)*/40 + CoroKun::MIN_LIMIT.y;
			Vector3 pos = Vector3(x, y, 0.0f);
			//�}�b�v�`�b�v�̏���ݒ肷��
			pMapTip->SetPosition(pos);
			//Vector�z��ɒǉ�����
			m_MapTip[i].push_back(pMapTip.get());
			//TextureManager�ɒǉ�����
			gameManager->Add(std::move(pMapTip));
		}
	}
}

void Stage::AddVector()
{
	//�ÓI�f�[�^���擾����
	StaticData* staticData = StaticData::GetInstance();

	//�t�@�C���̏�Ԃ��擾����
	Stream* File = Stream::GetInstance();

	for (size_t i = 0; i < m_MapTip.size(); i++)
	{
		for (size_t j = 0; j < m_MapTip[i].size(); j++)
		{
			//�}�b�v�`�b�v�̃|�C���^���擾
			ITextureBase* mapTip = m_MapTip[i][j];
			//��񂪓o�^����Ă��Ȃ������珈�����΂�
			if (!mapTip)continue;
			//if���ŏ����𕪂���
			//�v���C���[�̎�
			if (mapTip->GetName() == "SickChar") {
				m_CoroKun = dynamic_cast<CoroKun*>(mapTip);
				GameContext::Register<CoroKun>(m_CoroKun);
			}
			//���̎�
			if (mapTip->GetName() == "floor")m_Floors.push_back(dynamic_cast<Floor*>(mapTip));
			//�ǂ̎�
			if (mapTip->GetName() == "wall")m_Walls.push_back(dynamic_cast<Wall*>(mapTip));
			//�A�̎�
			if (mapTip->GetName() == "babul")m_Babuls.push_back(dynamic_cast<Babul*>(mapTip));
			//���]��̎�
			if (mapTip->GetName() == "rebound")m_Rebounds.push_back(dynamic_cast<Rebound*>(mapTip));
			//�N���X�C�b�`�̎�
			if (mapTip->GetName().substr(0, 6) == "switch")m_Switches.push_back(dynamic_cast<HoleSwitch*>(mapTip));
			//���Ƃ����̎�
			if (mapTip->GetName().substr(0, 4) == "hole")m_HoleFloors.push_back(dynamic_cast<HoleFloor*>(mapTip));
			//���ʂ̓G�̎�
			if (mapTip->GetName() == "NormalEnemy")m_NormalEnemies.push_back(dynamic_cast<NormalEnemy*>(mapTip));
			//���������G�̎�
			if (mapTip->GetName() == "SuperEnemy")m_SuperEnemies.push_back(dynamic_cast<SuperEnemy*>(mapTip));
			//�ƂĂ������G�̎�
			if (mapTip->GetName() == "HyperEnemy")m_HyperEnemies.push_back(dynamic_cast<HyperEnemy*>(mapTip));
			//�p�l���̎�
			if (mapTip->GetName() == "LeftJumpPanel" || mapTip->GetName() == "RightJumpPanel" ||
				mapTip->GetName() == "LeftPanel" || mapTip->GetName() == "RightPanel")
			{
				m_CheckPoints.push_back(dynamic_cast<CheckPoint*>(mapTip));
			}
		}
	}

	//�t�@�C���p�X��ݒ肷��
	std::wstring filePath = L"FileData/Stage" + 
		std::to_wstring(staticData->Get_IntData("�X�e�[�W")) + L"ExproInfo.csv";

	//�t�@�C�����J��
	if (File->Open(filePath.c_str(),Stream::FILE::STREAM_IFSTREAM))
	{
		std::vector<std::vector<std::string>>LoadData = File->Load_Data();
		std::vector<float>ConvertData = HoleFloor::FileDataConvert(LoadData);
		//���Ƃ����̔�������^�C�~���O��ݒ肷��
		for (int i = 0; i < (int)m_HoleFloors.size();i++)
		{
			//HoleFloor�̃|�C���^���擾����
			HoleFloor* hole = m_HoleFloors[i];
			//��������^�C�~���O���擾����
			float timing = ConvertData[i];
			//��������^�C�~���O��ݒ肷��
			hole->SetExproTiming(timing);
		}
		File->Close();
	}
}


ITextureBase* Stage::DirectlyBlowMap(ITextureBase* target,MapTip* mapType)
{
	//���ׂ���W���擾����
	Vector3 enemyPos = target->GetPosition() + target->AskOrigin();

	//�T�C�Y���擾����
	Vector3 size = target->AskOrigin() * 2;

	//�㉺�̍��W�����߂�
	float left = target->GetPosition().x;
	float right = left + size.x;
	float top = enemyPos.y;
	float botom = top + size.y;

	//�������擾����
	int direct = target->DirectX();

	//���������W��錾����
	Vector3 pos = Vector3::Zero;

	//���ړ��̎�(�E����)
	if (direct == 1)
	{
		//���W�����_(0,0)�ɍ��킹��
		pos = mapType->GetPosition();
	}
	//�E�ړ��̎�(������)
	else if (direct == -1)
	{
		//���W�����_(MAPSIZE_X,0)�ɍ��킹��
		pos = mapType->GetPosition() + Vector2(MapTip::MAPTIP_SIZEX, 0.0f);
	}

	//AABB�Ŕ�������
	if (pos.x >= left && pos.x <= right)
	{
		if (pos.y >= top && pos.y <= botom)
		{
			return mapType;
		}
	}

	return nullptr;
}

void Stage::MoveMapActive(const bool& active)
{
	//�v���C���[�̎��s�֐���ݒ肷��
	m_CoroKun->SetActive(active);

	//�̓�������擾����
	BodyBlow* body = GameContext::Get<BodyBlow>();

	//�̓�����̎��s�֐���ݒ肷��
	body->SetActive(active);

	//�E�C���X�r�[�����擾����
	VirusBeam* beam = GameContext::Get<VirusBeam>();

	//�E�C���X�r�[���̎��s�֐���ݒ肷��
	beam->SetActive(active);

	//�r�[���̃p�[�e�B�N���̎��s�֐���ݒ肷��
	for (Particle* patticle : beam->GetAllParticleList())
	{
		//���s�֐���ݒ肷��
		patticle->SetActive(active);
	}

	//�j�[�h���C���N�W�F�N�V�������擾����
	NeedleOverall* needleOver = GameContext::Get<NeedleOverall>();

	//�j���擾����
	for (Needle* needle : needleOver->GetNeedleList())
	{
		//���s�֐���ݒ肷��
		needle->SetActive(active);
	}

	//���U���N�V�������擾����
	Resurrection* resurrection = GameContext::Get<Resurrection>();

	//�j���擾����
	for (Needle* needle : resurrection->GetNeedleList())
	{
		//���s�֐���ݒ肷��
		needle->SetActive(active);
	}

	//������Ƌ����G�̎��s�֐���ݒ肷��
	for (SuperEnemy* enemy : m_SuperEnemies)
	{
		//���s�֐���ݒ肷��
		enemy->SetActive(active);

		//�Œ�C����擾����
		for (std::unique_ptr<FixedTurret>& turret : enemy->GetTurretList())
		{
			//�A���擾����
			for (SoapBabble* babble : turret->GetBabbleList())
			{
				//���s�֐���ݒ肷��
				babble->SetActive(active);
			}
		}
	}

	//�����G�̎��s�֐���ݒ肷��
	for (HyperEnemy* enemy : m_HyperEnemies)
	{
		//���s�֐���ݒ肷��
		enemy->SetActive(active);

		//�A���擾����
		for (SoapBabble* babble : enemy->GetBabbleList())
		{
			//���s�֐���ݒ肷��
			babble->SetActive(active);
		}
	}
}
