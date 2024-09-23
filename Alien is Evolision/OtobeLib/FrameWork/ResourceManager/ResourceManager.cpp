#include "pch.h"
#include "ResourceManager.h"
#include "OtobeLib/FrameWork/Stream/Stream.h"
#include <cassert>
#include <WICTextureLoader.h>

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace OtobeLib;
using namespace DX;

static const std::vector<std::wstring>TEXTURE_PATH =
{
	L"Resources/Logo.png",
	L"Resources/Title.png",
	L"Resources/Font_Texture2.png",
	L"Resources/nameWindow.png",
	L"Resources/MessageWindow.png",
	L"Resources/Poce.png",
	L"Resources/Menu.png",
	L"Resources/Result.png",
	L"Resources/fade.png",
	L"Resources/PlayerChar.png",
	L"Resources/PlayerComand.png",
	L"Resources/WhiteBack.png",
	L"Resources/MapTip.png",
	L"Resources/EffectAnimation.png",
	L"Resources/StarEffect.png",
	L"Resources/GoalFlag.png",
	L"Resources/Particle.png",
	L"Resources/BeamEffect.png",
	L"Resources/Needle.png",
	L"Resources/Enemies.png",
	L"Resources/shabonEffect.png",
	L"Resources/shabonAnime.png",
	L"Resources/CutIn.png",
	L"Resources/Enemys2.png",
	L"Resources/droplet.png",
	L"Resources/Techunique.png",
	L"Resources/LevelUps.png",
	L"Resources/DeathBlowPlayer.png",
	L"Resources/DeathBlowOura.png",
	L"Resources/DeathBlowEffect.png",
	L"Resources/DeathBlowSelect.png",
	L"Resources/PlayerBack.png",
	L"Resources/Enemies3.png",
	L"Resources/Heal.png",
	L"Resources/Heal1.png",
	L"Resources/Heal2.png",
	L"Resources/Heal3.png",
	L"Resources/StatusWindow.png",
	L"Resources/StatusPlayer.png",
	L"Resources/ItemText.png",
	L"Resources/StageSelectBack.png",
	L"Resources/Arrow.png",
	L"Resources/Empty.png",
	L"Resources/frametape.png",
	L"Resources/NaturalSougen_player1.png",
	L"Resources/NaturalSougen_Enemy.png",
	L"Resources/NaturalSkyOnCloud.png",
	L"Resources/NaturalSkyOnCloudAndEngel.png",
	L"Resources/NaturalSougen_player2.png",
	L"Resources/NaturalSougen_player3.png",
	L"Resources/NaturalSougen_playerVSEngel.png",
	L"Resources/Sweat.png",
	L"Resources/Stage1.png",
	L"Resources/Stage2.png",
	L"Resources/Stage3.png",
	L"Resources/Stage4.png",
	L"Resources/StageSelect1.png",
	L"Resources/StageSelect2.png",
	L"Resources/StageSelect3.png",
	L"Resources/StageSelect4.png",
};

ResourceManager::ResourceManager()
{
}

ResourceManager::~ResourceManager()
{
	//テクスチャの解放
	for (auto& texture : m_TextureList)
	{
		texture.second.reset();
	}
	m_TextureList.clear();
}

bool ResourceManager::LoadResource()
{
	//成功（true）失敗（false）

	//テクスチャの情報を読み込む
	bool check = this->LoadTexture();
	if (!check)return false;

	return true;
}

bool ResourceManager::LoadTexture()
{
	//テクスチャの状態を取得(すでに読み込まれているときは警告を出す)
	assert(this->GetMapTexture().size() == NULL);

	//デバイス情報の取得
	DeviceResources* deviceResources = DeviceResources::GetInstance();
	const auto& device = deviceResources->GetD3DDevice();

	//ファイルの取得
	Stream* File = Stream::GetInstance();

	//テクスチャハンドルの初期化
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>handle = nullptr;
	
	//ファイルオープン(true:成功	false:失敗)
	if (File->Open(L"FileData/TextureInfomation.csv", Stream::FILE::STREAM_IFSTREAM))
	{
		//ファイルからデータをすべて読み込む
		std::vector<std::vector<std::string>>TextureData = File->Load_Data();

		//読み込んだデータを変換する
		std::vector<TextureBase>Textures = TextureBase::Cast(TextureData);

		//必要なテクスチャをすべて読み込む
		for (const std::wstring& path : TEXTURE_PATH)
		{
			//テクスチャの読み込み
			HRESULT hr = CreateWICTextureFromFile(device, path.c_str(), nullptr, handle.GetAddressOf());

			//例外が出たらスローする
			ThrowIfFailed(hr);

			//テクスチャハンドルを設定する
			for (TextureBase& texture:Textures)
			{
				//同じパス情報を設定する
				if (texture.GetPath() == path)
				{
					texture.SetHandle(handle);
					//変換したデータをもとにテクスチャを生成する
					this->AddTex(&texture);
				}
			}
		}

		//ファイルを閉じる
		File->Close();
	}
	else return false;

	return true;
}