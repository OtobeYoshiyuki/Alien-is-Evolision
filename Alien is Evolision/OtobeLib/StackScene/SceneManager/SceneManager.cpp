#include "pch.h"
#include "SceneManager.h"
#include "OtobeLib/StackScene/Scene/Logo/LogoScene.h"
#include "OtobeLib/StackScene/Scene/Title/TitleScene.h"
#include "OtobeLib/StackScene/Scene/Play/PlayScene.h"
#include "OtobeLib/StackScene/Scene/Poce/PoceScene.h"
#include "OtobeLib/StackScene/Scene/Menu/MenuScene.h"
#include "OtobeLib/StackScene/Scene/Result/ResultScene.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace OtobeLib;

SceneManager::SceneManager():m_State(nullptr),m_PopCount(NULL)
{
	//Fade���Ǘ�����X�e�[�g�𐶐�����
	m_State = std::make_unique<SceneState>();
	m_State->Initialize();
}

void SceneManager::Update()
{
	//Fade�̍X�V��������
	m_State->Update();

	//�V�[���̍폜�y�сA�I������
	for (int i = 0;i < m_PopCount;i++)
	{ 
		//�ғ����Ă���V�[���ɑ΂��āA�s��
		m_SceneList.back()->Finalize();
		m_SceneList.pop_back();
	}

	//�폜�J�E���g������������
	m_PopCount = NULL;

	//�V�[���̐������s��
	for(std::string& info:m_SceneInfo)
	{
		//���̃V�[���𐶐�����
		m_SceneList.push_back(m_SceneFactory[info]());
		//�V�[���̏��������s��
		m_SceneList.back()->Initialize();
	}

	//�V�[���̏����N���A����
	m_SceneInfo.clear();

	//�V�[���̍X�V���s��
	if (!m_SceneList.empty())
	{
		m_SceneList.back()->Update();
	}
}

void SceneManager::Render()
{
	//�ғ����Ă���V�[�����ׂĂ�`�悷��
	for (std::unique_ptr<ISceneBase>& scene:m_SceneList)
	{
		//�V�[����`��
		scene->Render();
	}

	//�t�F�[�h�̕`��
	m_State->Render();
}

void SceneManager::PopScene(int count)
{
	//�ϐ��̏�����
	int Inspection = NULL;

	//�ǉ��������̂������āA�����������炷
	if (!m_SceneInfo.empty())
	{
		m_SceneInfo.pop_back();
		count--;
	}

	//�V�[���̏�������␳����
	Inspection = std::max<int>(count, 0);
	Inspection = std::min<int>(Inspection, m_SceneList.size() - 1);

	//��������ݒ肷��
	this->PopCount(Inspection);
}
