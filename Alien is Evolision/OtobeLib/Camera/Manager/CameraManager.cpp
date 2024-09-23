#include "pch.h"
#include "CameraManager.h"
#include "Utils.h"
#include "Game.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace OtobeLib;

CameraManager::CameraManager():
	m_popCount(NULL)//�폜��
{
}

void CameraManager::Update()
{
	//�J�����̍폜�y�сA�I������
	for (int i = 0; i < m_popCount; i++)
	{
		//�ғ����Ă���V�[���ɑ΂��āA�s��
		m_cameraList.back()->Finalize();
		m_cameraList.pop_back();
	}

	//�폜�J�E���g������������
	m_popCount = NULL;

	//�J�����̐������s��
	for (std::string& info : m_camereInfo)
	{
		//���̃J�����𐶐�����
		m_cameraList.push_back(m_cameraFactory[info]());
		//�J�����̏��������s��
		m_cameraList.back()->Initialize();
	}

	//�J�����̏����N���A����
	m_camereInfo.clear();

	//�J�����̍X�V���s��
	if (!m_cameraList.empty())
	{
		m_cameraList.back()->Update();
	}
}

void ::CameraManager::PopCamera(int count)
{
	//�ϐ��̏�����
	int Inspection = NULL;

	//�ǉ��������̂������āA�����������炷
	if (!m_camereInfo.empty())
	{
		m_camereInfo.pop_back();
		count--;
	}

	//�J�����̏�������␳����
	Inspection = std::max<int>(count, 0);
	Inspection = std::min<int>(Inspection, (int)m_cameraList.size() - 1);

	//��������ݒ肷��
	this->PopCount(Inspection);
}

//�Ώە�����ʓ��ɂ��邩�m�F����
const bool CameraManager::ScreenCheck(const Vector3& pos)
{
	//�ғ����̃J�������擾����
	CameraBase* nowCamera = this->GetNowCamera();

	//�s�񂩂�A���W�����o��
	Vector3 trans = nowCamera->GetMatrix().Translation();

	//���o�������W�́A�}�C�i�X�Ȃ̂ŁA��Βl�ɒ���
	trans = Length(trans, Vector3::Zero);

	//����́AX���W�݂̂ōs��
	//�Ώە�����ʓ��ɂ��鎞�́Atrue
	if (pos.x >= trans.x && pos.x <= trans.x + Game::MAX_WIDTH)return true;

	//����ȊO�̎��́Afalse��Ԃ�
	return false;
}
