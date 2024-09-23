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
	//Fadeを管理するステートを生成する
	m_State = std::make_unique<SceneState>();
	m_State->Initialize();
}

void SceneManager::Update()
{
	//Fadeの更新をかける
	m_State->Update();

	//シーンの削除及び、終了処理
	for (int i = 0;i < m_PopCount;i++)
	{ 
		//稼働しているシーンに対して、行う
		m_SceneList.back()->Finalize();
		m_SceneList.pop_back();
	}

	//削除カウントを初期化する
	m_PopCount = NULL;

	//シーンの生成を行う
	for(std::string& info:m_SceneInfo)
	{
		//次のシーンを生成する
		m_SceneList.push_back(m_SceneFactory[info]());
		//シーンの初期化を行う
		m_SceneList.back()->Initialize();
	}

	//シーンの情報をクリアする
	m_SceneInfo.clear();

	//シーンの更新を行う
	if (!m_SceneList.empty())
	{
		m_SceneList.back()->Update();
	}
}

void SceneManager::Render()
{
	//稼働しているシーンすべてを描画する
	for (std::unique_ptr<ISceneBase>& scene:m_SceneList)
	{
		//シーンを描画
		scene->Render();
	}

	//フェードの描画
	m_State->Render();
}

void SceneManager::PopScene(int count)
{
	//変数の初期化
	int Inspection = NULL;

	//追加したものを消して、消去数を減らす
	if (!m_SceneInfo.empty())
	{
		m_SceneInfo.pop_back();
		count--;
	}

	//シーンの消す数を補正する
	Inspection = std::max<int>(count, 0);
	Inspection = std::min<int>(Inspection, m_SceneList.size() - 1);

	//消去数を設定する
	this->PopCount(Inspection);
}
