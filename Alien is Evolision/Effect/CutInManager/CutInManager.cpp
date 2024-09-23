#include "pch.h"
#include "CutInManager.h"
#include "GameContext.h"
#include "OtobeLib/StackScene/Scene/CutIn/CutInScene.h"
#include "OtobeLib/Texture/Manager/TextureManager.h"
#include "OtobeLib/FrameWork/ResourceManager/ResourceManager.h"
#include "OtobeLib/FrameWork/StaticData/StaticData.h"
#include "OtobeLib/StackScene/SceneManager/SceneManager.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace OtobeLib;

CutInManager::CutInManager():
	m_cutinTexture(nullptr),//�J�b�g�C���̃e�N�X�`��
	m_backTexture(nullptr),//�J�b�g�C���̔w�i�̃e�N�X�`��
	m_timer(0.0f),//�^�C�}�[
	m_totaltimer(0.0f)//�V�[������������Ă���̌o�ߎ���
{
}

void CutInManager::Initialize()
{
	//ResourceManager���擾����
	ResourceManager* resourceManager = ResourceManager::GetInstance();

	//TextureManager���擾����
	TextureManager* textureManager = GameContext::Get<CutInScene>()->GetTextureManager();

	//�g���e�N�X�`�����擾����
	ITextureBase* cutinTexture = resourceManager->GetTexture("�J�b�g�C��");
	ITextureBase* cutinBack = resourceManager->GetTexture("WhiteBack");

	//�J�b�g�C���𐶐�����
	std::unique_ptr<TextureBase> pCutin = std::make_unique<TextureBase>(cutinTexture);
	//���|�C���^��o�^����
	m_cutinTexture = pCutin.get();

	//�J�b�g�C���̔w�i�𐶐�����
	std::unique_ptr<TextureBase> pCutinBack = std::make_unique<TextureBase>(cutinBack);
	//�w�i�̐F���w�肷��
	pCutinBack->SetColor(Colors::White);
	//���W��ݒ肷��
	pCutinBack->SetPosition(Vector3(0.0f, 200.0f, 0.0f));
	//�؂���ʒu���w�肷��
	pCutinBack->SetRect(RECT{ 0,0,1280,400 });
	//�`�揇��ύX����
	pCutinBack->SetDrawOrder(1);
	//���|�C���^��o�^����
	m_backTexture = pCutinBack.get();

	//TextureManager�ɒǉ�����
	textureManager->Add(std::move(pCutin));
	textureManager->Add(std::move(pCutinBack));
}

void CutInManager::Update()
{
	//SceneManager���擾����
	SceneManager* manager = SceneManager::GetInstance();

	//�ÓI�f�[�^�̎擾
	StaticData* staticData = StaticData::GetInstance();

	//�V�[���̑J�ڂ���1�b��ɁA�ĂёJ�ڂ���
	if (m_totaltimer > 1.0f)
	{
		//1�O��Scene�ɖ߂�
		manager->PopScene();

		//���݂̃t���[���̏����͎��s���Ȃ�
		return;
	}

	//���Ԃɂ���āA�F��ύX����
	if (m_timer > 0.05f)
	{
		//�F���擾����
		Color color = m_backTexture->GetColor();
		//�F�̓����x��100���̎��́A�����x��50���ɂ���
		if (color.A() == 1.0f)color.A(0.5f);
		//�����x��50���̎��́A100���ɂ���
		else color.A(1.0f);
		//�F���Đݒ肷��
		m_backTexture->SetColor(color);
		//�^�C�}�[������������
		m_timer = 0.0f;
	}

	//�^�C�}�[���X�V����
	m_timer += staticData->Get_FloatData("elapsedTime");
	m_totaltimer += staticData->Get_FloatData("elapsedTime");
}
