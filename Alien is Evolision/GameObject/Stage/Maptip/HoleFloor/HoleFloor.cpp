#include "pch.h"
#include "HoleFloor.h"
#include "OtobeLib/FrameWork/StaticData/StaticData.h"
#include "GameContext.h"
#include "GameObject/Player/CoroKun.h"
#include "OtobeLib/FrameWork/Collision/CollisionManager/CollisionManager.h"
#include "OtobeLib/Camera/Manager/CameraManager.h"
#include "GameObject/TargetScrollCamera/TargetScrollCamera.h"
#include "OtobeLib/FrameWork/SoundManager/SoundManager.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace OtobeLib;

//���Ƃ����̃O���[�v������������
HoleFloor::HoleMap HoleFloor::s_holeGrop = HoleMap();

HoleFloor::HoleFloor(ITextureBase* texture):MapTip(texture),m_ExprosionCount(0.0f),m_ExprosionFlag(false),
m_SelealNumber(NULL)
{
}

void HoleFloor::Initialize()
{
	//�O���[�v��ݒ肷��
	s_holeGrop[m_SelealNumber].push_back(this);
}

void HoleFloor::Update()
{
	//CameraManager���擾����
	CameraManager* cameraManager = GameContext::Get<CameraManager>();

	//���݉ғ����Ă���J�������擾����
	CameraBase* nowCamera = cameraManager->GetNowCamera();	

	//�X�N���[���J�����ɃL���X�g����
	TargetScrollCamera* scrollCamera = dynamic_cast<TargetScrollCamera*>(nowCamera);

	//�J�������X�N���[���J�����̎��́A�X�V���~�߂�
	if (!nowCamera->GetObjectMove() && scrollCamera &&
		scrollCamera->GetNowState() < TARGET_STATE::WAIT)return;

	//�}�b�v�`�b�v�̍X�V����
	MapTip::Update();

	//��������^�C�~���O���X�V����
	this->UpdateExprosion();

	//�v���C���[���擾����
	CoroKun* player = GameContext::Get<CoroKun>();

	//�����蔻���ǉ�����
	GameContext::Get<CollisionManager>()->AddUseOther(this, player, 200.0f);

	//�j�[�h���̒������͈͍U�����擾����
	std::vector<Needle*> needleList = GameContext::Get<NeedleOverall>()->GetNeedleList();

	for (Needle* needle : needleList)
	{
		//�����蔻���ǉ�����
		GameContext::Get<CollisionManager>()->AddUseOther(this, needle, 100.0f);
	}
}

void HoleFloor::Render()
{
	//�}�b�v�`�b�v�̕`�揈��
	MapTip::Render();
}

void HoleFloor::Finalize()
{
}

void HoleFloor::OnCollisionEnter(ITextureBase* obj)
{
	MapTip::OnCollisionEnter(obj);
}

void HoleFloor::NonCollisionEnter(ITextureBase* obj)
{
	MapTip::NonCollisionEnter(obj);
}

const std::vector<float> HoleFloor::FileDataConvert(const std::vector<std::vector<std::string>>& Input)
{
	//�f�[�^���i�[����z���錾����
	std::vector<float>ConvertData;

	for (size_t i = 0; i < Input.size(); i++)
	{
		for (size_t j = 0; j < Input[i].size(); j++)
		{
			//���������Ă��Ȃ���΁A�������Ȃ�
			if (Input[i][j] == "")continue;

			//�f�[�^��String����int�ɕϊ�����
			ConvertData.push_back(std::stoi(Input[i][j]));
		}
	}

	//�ϊ������f�[�^��Ԃ�
	return ConvertData;
}

void HoleFloor::UpdateExprosion()
{
	//�ÓI�f�[�^�̎擾
	StaticData* staticData = StaticData::GetInstance();

	//SoundManager���擾����
	SoundManager* soundManager = SoundManager::GetInstance();

	//��������^�C�~���O���X�V����
	//��������^�C�~���O���ݒ肳��Ă��Ȃ�������A�������Ȃ�
	if (m_ExprosionCount <= 0.0f)return;

	//���g�̘A�ԂƓ����N���X�C�b�`���o�^����Ă��邩�m�F����
	if (!staticData->Request_Data("switch" + std::to_string(this->GetSerealNumber())))return;

	//���g�̘A�ԂƓ����N���X�C�b�`�̘A�Ԃ��擾����
	float anotherCount = staticData->Get_FloatData("switch" + std::to_string(this->GetSerealNumber()) + "Count");

	//��������^�C�~���O�ɂȂ�����A�`���؂�
	if (anotherCount > m_ExprosionCount)
	{
		if (this->GetState() == STATE::DRAW)
		{
			//SE���Đ�����
			soundManager->Play(CRI_CUESHEET_0__36_�u���b�N����);
		}
		this->SetState(STATE::NONE);
	}
	
}
