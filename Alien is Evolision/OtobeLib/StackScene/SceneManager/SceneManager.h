#pragma once
#include "OtobeLib/StackScene/Interface/ISceneBase.h"
#include "OtobeLib/FrameWork/Singleton/Singleton.h"
#include "OtobeLib/StackScene/SceneState/SceneState.h"
#include <functional>
namespace OtobeLib
{
	class SceneManager :public Singleton<SceneManager>
	{
		friend Singleton<SceneManager>;
	private:
		//�V�[���̃��X�g
		std::list<std::unique_ptr<ISceneBase>>m_SceneList;


		//�V�[���̐������s���t�@�N�g���[
		std::map<std::string, std::function<std::unique_ptr<ISceneBase>()>> m_SceneFactory;

		//�V�[���̏��
		std::list<std::string> m_SceneInfo;

		//�V�[�����폜���鐔
		int			m_PopCount;

		//�t�F�[�h�̊Ǘ�
		std::unique_ptr<SceneState>m_State;
	private:
		//�R���X�g���N�^
		SceneManager();
	public:
		//�f�X�g���N�^
		~SceneManager() {};

		//�X�V
		void Update();

		//�`��
		void Render();

		//�ғ����̃V�[�����擾����
		ISceneBase* GetNowScene() { return m_SceneList.back().get(); }

		//�t�F�[�h�̃X�e�[�g���擾����
		SceneState* GetState() { return m_State.get(); }

		//�V�[���̏���ǉ�����
		void AddScene(const std::string& name, std::function<std::unique_ptr<ISceneBase>()> func)
		{
			//std::make_unique�֐���ݒ肷��
			m_SceneFactory[name] = func;
		}

		//���̃V�[���ɐ؂�ւ���(���݉ғ����Ă���V�[����S�ď���)
		void ChangeScene(const std::string& next) 
		{ 
			//�V�[����ǉ���������
			this->PushScene(next);
			//�폜�����ғ����̃V�[���̐��ɂ���
			this->PopCount((int)m_SceneList.size());
		}

		//�V�[����ǉ���������
		void PushScene(const std::string& next)
		{
			//���̐�������V�[����ǉ�����
			m_SceneInfo.push_back(next);
		}

		//�V�[�����폜����
		void PopScene(int count = 1);

	private:
		//�V�[���̏�������ݒ肷��
		void PopCount(const int& count)
		{
			//�V�[���̍폜����ݒ肷��
			m_PopCount = count;
		}
	};
}

