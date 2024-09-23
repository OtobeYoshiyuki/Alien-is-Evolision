#pragma once
#include "OtobeLib/Camera/Base/CameraBase.h"
#include <list>
#include <map>
#include <string>
#include <functional>
namespace OtobeLib
{
	class CameraManager
	{
	private:
		//�J�����̃��X�g
		std::list<std::unique_ptr<CameraBase>> m_cameraList;

		//�J�����̂̍H��
		std::map<std::string, std::function<std::unique_ptr<CameraBase>()>> m_cameraFactory;

		//�J�����̏�����
		int m_popCount;

		//��������J�����̖��O
		std::list<std::string> m_camereInfo;
	public:
		//�R���X�g���N�^
		CameraManager();

		//�f�X�g���N�^
		~CameraManager() = default;

		//�X�V����
		void Update();

		//�ғ����̃J�������擾����
		CameraBase* GetNowCamera() { return m_cameraList.back().get(); }

		//�J�����̃C���X�^���X��ǉ�����
		void AddCamera(const std::string& name,std::function<std::unique_ptr<CameraBase>()> func)
		{
			//�J�����̃|�C���^��o�^����
			m_cameraFactory[name] = func;
		}

		//���̃J�����ɐ؂�ւ���(���݉ғ����Ă���J������S�ď���)
		void ChangeCamera(const std::string& next)
		{
			//�J������ǉ���������
			this->PushCamera(next);
			//�폜�����ғ����̃J�����̐��ɂ���
			this->PopCount((int)m_cameraList.size());
		}

		//�V�[����ǉ���������
		void PushCamera(const std::string& next)
		{
			//���̐�������V�[����ǉ�����
			m_camereInfo.push_back(next);
		}

		//�J�������폜����
		void PopCamera(int count = 1);

	private:
		//�J�����̏�������ݒ肷��
		void PopCount(const int& count)
		{
			//�J�����̍폜����ݒ肷��
			m_popCount = count;
		}

	public:
		//�Ώە�����ʓ��ɂ��邩�`�F�b�N����
		const bool ScreenCheck(const DirectX::SimpleMath::Vector3& pos);
	};
}

