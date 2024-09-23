#include "pch.h"
#include "Graphics.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace OtobeLib;

Graphics::Graphics():m_SpriteBatch(nullptr),m_State(nullptr)
{
	//�f�o�C�X�𐶐�
	this->Create();
}

Graphics::~Graphics()
{
}

SpriteBatch * Graphics::GetSpriteBatch() const
{
	return m_SpriteBatch.get();
}

CommonStates * Graphics::GetCommonStates() const
{
	return m_State.get();
}

void Graphics::Create()
{
	//�f�o�C�X���擾
	auto device = DX::DeviceResources::GetInstance()->GetD3DDevice();
	//�R���e�L�X�g���擾
	auto context = DX::DeviceResources::GetInstance()->GetD3DDeviceContext();
	//�X�v���C�g�o�b�`�𐶐�
	m_SpriteBatch = std::make_unique<SpriteBatch>(context);
	//�R�����X�e�[�g�𐶐�
	m_State = std::make_unique<CommonStates>(device);
}

