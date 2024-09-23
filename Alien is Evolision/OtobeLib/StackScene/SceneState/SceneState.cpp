#include "pch.h"
#include "SceneState.h"
#include "OtobeLib/FrameWork/Stream/Stream.h"
#include "OtobeLib/FrameWork/Input/Input.h"
#include "OtobeLib/FrameWork/ResourceManager/ResourceManager.h"

using namespace OtobeLib;
using namespace DirectX;
using namespace DirectX::SimpleMath;

const float SceneState::ALFA_SPEED = 0.01f;//���t���[���̃A���t�@�l�����Z���鑬�x
const float SceneState::LIMITTE_ALFA = 1.0f;//�A���t�@�l�̌��E�l

SceneState::SceneState():m_Texture(nullptr),m_TextureManager(nullptr),m_FadeState(FADESTATE::FADENONE)
{
}

void SceneState::Initialize()
{
	//�K�v�ȃ��\�[�X���Q�Ƃ���
	ResourceManager* resourceManager = ResourceManager::GetInstance();
	ITextureBase* FadeTexture = resourceManager->GetTexture("WhiteBack");
	std::unique_ptr<ITextureBase>pTexture = std::make_unique<TextureBase>(FadeTexture);
	m_Texture = pTexture.get();

	//�F��ݒ肷��
	m_Texture->SetColor(Colors::Black);

	//Manager�N���X�ɓo�^����
	m_TextureManager = std::make_unique<TextureManager>();
	m_TextureManager->Add(std::move(pTexture));

	//�t�F�[�h�̏����l��ݒ肷��
	m_fadeSpeed = SceneState::ALFA_SPEED;
}

void SceneState::Update()
{
	m_TextureManager->Update();

	switch (m_FadeState)
	{
	case SceneState::FADEIN:
		Fade_In();
		break;
	case SceneState::FADEOUT:
		Fade_Out();
		break;
	case SceneState::FADERESULT:
		m_FadeState = FADENONE;
		break;
	}
}

void SceneState::Render()
{
	const auto& context = DX::DeviceResources::GetInstance()->GetD3DDeviceContext();

	Graphics* Graph = Graphics::GetInstance();

	Graph->GetSpriteBatch()->Begin(DirectX::SpriteSortMode::SpriteSortMode_Deferred,
		Graph->GetCommonStates()->NonPremultiplied(), Graph->GetCommonStates()->PointClamp(),
		Graph->GetCommonStates()->DepthDefault(), nullptr,
		[&] {context->OMSetBlendState(Graph->GetCommonStates()->NonPremultiplied(), nullptr, 0xffffffff); });

	m_TextureManager->Render();

	Graph->GetSpriteBatch()->End();

}

void SceneState::Fade_In()
{
	Color color = m_Texture->GetColor();
	float alfa = m_Texture->GetColor().A();
	alfa -= m_fadeSpeed;//���t���[���A���t�@�l�����Z����
	m_Texture->SetColor(Color(color.R(),color.G(),color.B(), alfa));

	if (m_Texture->GetColor().A() <= NULL)//�A���t�@�l�����E�l�𒴂�����A�A���t�@�l�̉��Z���~�߂āA���̏�Ԃֈڍs����
	{
		m_Texture->SetColor(Color(color.R(), color.G(), color.B(), NULL));
		m_FadeState = FADESTATE::WAITKEY;
	}
}

void SceneState::Fade_Out()
{
	Color color = m_Texture->GetColor();
	float alfa = m_Texture->GetColor().A();
	alfa += m_fadeSpeed;//���t���[���A���t�@�l�����Z����
	m_Texture->SetColor(Color(color.R(), color.G(), color.B(), alfa));

	if (m_Texture->GetColor().A() >= LIMITTE_ALFA)//�A���t�@�l�����E�l�𒴂�����A�A���t�@�l�̉��Z���~�߂āA���̏�Ԃֈڍs����
	{
		m_Texture->SetColor(Color(color.R(), color.G(), color.B(), LIMITTE_ALFA));
		m_FadeState = FADESTATE::FADERESULT;
	}
}

void SceneState::SetAlfa(const float& alfa)
{
	Color color = m_Texture->GetColor();
	color.A(alfa);
	m_Texture->SetColor(color);
}
