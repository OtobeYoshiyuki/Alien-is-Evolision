#include "pch.h"
#include "Graphics.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace OtobeLib;

Graphics::Graphics():m_SpriteBatch(nullptr),m_State(nullptr)
{
	//デバイスを生成
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
	//デバイスを取得
	auto device = DX::DeviceResources::GetInstance()->GetD3DDevice();
	//コンテキストを取得
	auto context = DX::DeviceResources::GetInstance()->GetD3DDeviceContext();
	//スプライトバッチを生成
	m_SpriteBatch = std::make_unique<SpriteBatch>(context);
	//コモンステートを生成
	m_State = std::make_unique<CommonStates>(device);
}

