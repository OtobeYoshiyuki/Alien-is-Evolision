#include "pch.h"
#include "CutInScene.h"
#include "GameContext.h"
#include "OtobeLib/FrameWork/Graphics/Graphics.h"
#include "OtobeLib/FrameWork/SoundManager/SoundManager.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace OtobeLib;

CutInScene::CutInScene():
	m_CutInManager(nullptr),//�J�b�g�C���̊Ǘ��N���X
	m_TextureManager(nullptr)//�e�N�X�`���̊Ǘ��N���X
{
}

void CutInScene::Initialize()
{
	//GameContext�ɓo�^����
	GameContext::Register<CutInScene>(this);

	//SoundManager���擾����
	SoundManager* soundManager = SoundManager::GetInstance();

	//TextureManager�𐶐�����
	m_TextureManager = std::make_unique<TextureManager>();

	//CutInManager�𐶐�����
	m_CutInManager = std::make_unique<CutInManager>();
	//���������s��
	m_CutInManager->Initialize();
	//SE��炷
	soundManager->Play(CRI_CUESHEET_0__20_�J�b�g�C��);
}

void CutInScene::Update()
{
	//TextureManager�̍X�V����
	m_TextureManager->Update();

	//CutInManager�̍X�V����
	m_CutInManager->Update();
}

void CutInScene::Render()
{
	//�`��n�̏�Ԃ��擾����
	Graphics* Graph = Graphics::GetInstance();
	const auto& context = DX::DeviceResources::GetInstance()->GetD3DDeviceContext();

	//�`��̊J�n
	Graph->GetSpriteBatch()->Begin(DirectX::SpriteSortMode::SpriteSortMode_Deferred,
		Graph->GetCommonStates()->NonPremultiplied(), Graph->GetCommonStates()->PointClamp(),
		Graph->GetCommonStates()->DepthDefault(), nullptr,
		[&] {context->OMSetBlendState(Graph->GetCommonStates()->NonPremultiplied(), nullptr, 0xffffffff); });

	//�摜�̕`�揈��
	m_TextureManager->Render();

	//�`��̏I��
	Graph->GetSpriteBatch()->End();
}

void CutInScene::Finalize()
{
}

void CutInScene::Create()
{
}
