//
// Game.cpp
//

#include "pch.h"
#include "Game.h"
#include "OtobeLib/StackScene/Scene/Logo/LogoScene.h"
#include "OtobeLib/StackScene/Scene/Title/TitleScene.h"
#include "OtobeLib/StackScene/Scene/Play/PlayScene.h"
#include "OtobeLib/StackScene/Scene/Poce/PoceScene.h"
#include "OtobeLib/StackScene/Scene/Menu/MenuScene.h"
#include "OtobeLib/StackScene/Scene/Result/ResultScene.h"
#include "OtobeLib/StackScene/Scene/CutIn/CutInScene.h"
#include "OtobeLib/StackScene/Scene/DirecTing/DirecTingScene.h"
#include "OtobeLib/StackScene/Scene/Select/SelectScene.h"
#include "OtobeLib/StackScene/Scene/Status/StatusScene.h"
#include "OtobeLib/StackScene/Scene/Item/ItemScene.h"
#include "OtobeLib/StackScene/Scene/StageSelect/StageSelectScene.h"
#include "OtobeLib/StackScene/Scene/Ranking/RankingScene.h"
#include "OtobeLib/StackScene/Scene/Choices/ChoicesScene.h"
#include "OtobeLib/StackScene/Scene/Opening/OpeningScene.h"
#include "ExternalLib/Easing/Easing.h"


extern void ExitGame() noexcept;

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace DX;
using namespace OtobeLib;
using namespace ExternalLib;
using Microsoft::WRL::ComPtr;

Game::Game() noexcept(false)
{
    m_deviceResources.reset(DX::DeviceResources::GetInstance());
    m_deviceResources->RegisterDeviceNotify(this);
}

// Initialize the Direct3D resources required to run.
void Game::Initialize(HWND window, int width, int height)
{
    m_deviceResources->SetWindow(window, width, height);

    m_deviceResources->CreateDeviceResources();
    CreateDeviceDependentResources();

    m_deviceResources->CreateWindowSizeDependentResources();
    CreateWindowSizeDependentResources();

    m_Input->GetMouse().SetWindow(window);

    m_ColorList->Initialize();

    m_SceneManager->AddScene("Logo", std::make_unique<LogoScene>);
    m_SceneManager->AddScene("Title", std::make_unique<TitleScene>);
    m_SceneManager->AddScene("Play", std::make_unique<PlayScene>);
    m_SceneManager->AddScene("Poce", std::make_unique<PoceScene>);
    m_SceneManager->AddScene("Menu", std::make_unique<MenuScene>);
    m_SceneManager->AddScene("Result", std::make_unique<ResultScene>);
    m_SceneManager->AddScene("CutIn", std::make_unique<CutInScene>);
    m_SceneManager->AddScene("DirecTing", std::make_unique<DirecTingScene>);
    m_SceneManager->AddScene("Select", std::make_unique<SelectScene>);
    m_SceneManager->AddScene("Status", std::make_unique<StatusScene>);
    m_SceneManager->AddScene("Item", std::make_unique<ItemScene>);
    m_SceneManager->AddScene("StageSelect", std::make_unique<StageSelectScene>);
    m_SceneManager->AddScene("Ranking", std::make_unique<RankingScene>);
    m_SceneManager->AddScene("Choices", std::make_unique<ChoicesScene>);
    m_SceneManager->AddScene("Opening", std::make_unique<OpeningScene>);

    //m_SceneManager->PushScene("Opening");
    m_SceneManager->PushScene("Logo");

    //音声ファイルをロードする
    m_Adx2->Initialize("Audio/Audio.acf", "Audio/CueSheet_0.acb");

    //m_SceneManager->PushScene("Select");

    // TODO: Change the timer settings if you want something other than the default variable timestep mode.
    // e.g. for 60 FPS fixed timestep update logic, call:

    m_timer.SetFixedTimeStep(true);
    m_timer.SetTargetElapsedSeconds(1.0 / 72);
}

#pragma region Frame Update
// Executes the basic game loop.
void Game::Tick()
{
    m_timer.Tick([&]()
    {
        Update(m_timer);
    });

    Render();
}

// Updates the world.
void Game::Update(DX::StepTimer const& timer)
{
    //前回のフレームからの経過時間
    float elapsedTime = float(timer.GetElapsedSeconds());

    // TODO: Add your game logic here.

    //フレームレート
    int FPS = timer.GetFramesPerSecond();

    //プロジェクトの経過時間
    float totalTime = timer.GetTotalSeconds();

    //静的データに登録する
    m_StaticData->Add("elapsedTime", elapsedTime);
    m_StaticData->Add("totalTime", totalTime);
    m_StaticData->Add("FPS", FPS);

    //キー入力の更新
    m_Input->Update();

    //Sceneの更新
    m_SceneManager->Update();

    //音の更新
    m_Adx2->Update();

    //キーのトラッカーを取得
    const auto& trakker = m_Input->GetKeybord_Trakker();

    //Deleteキーが押されたら、終了
    if (trakker.IsKeyPressed(Keyboard::Delete))ExitGame();

}
#pragma endregion

#pragma region Frame Render
// Draws the scene.
void Game::Render()
{
    // Don't try to render anything before the first Update.
    if (m_timer.GetFrameCount() == 0)
    {
        return;
    }

    Clear();

    m_deviceResources->PIXBeginEvent(L"Render");
    auto context = m_deviceResources->GetD3DDeviceContext();

    // TODO: Add your rendering code here.
    context;

    m_SceneManager->Render();

    m_deviceResources->PIXEndEvent();

    // Show the new frame.
    m_deviceResources->Present();
}

// Helper method to clear the back buffers.
void Game::Clear()
{
    m_deviceResources->PIXBeginEvent(L"Clear");

    // Clear the views.
    auto context = m_deviceResources->GetD3DDeviceContext();
    auto renderTarget = m_deviceResources->GetRenderTargetView();
    auto depthStencil = m_deviceResources->GetDepthStencilView();

    context->ClearRenderTargetView(renderTarget, Colors::CornflowerBlue);
    context->ClearDepthStencilView(depthStencil, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    context->OMSetRenderTargets(1, &renderTarget, depthStencil);

    // Set the viewport.
    auto viewport = m_deviceResources->GetScreenViewport();
    context->RSSetViewports(1, &viewport);

    m_deviceResources->PIXEndEvent();
}
#pragma endregion

#pragma region Message Handlers
// Message handlers
void Game::OnActivated()
{
    // TODO: Game is becoming active window.
}

void Game::OnDeactivated()
{
    // TODO: Game is becoming background window.
}

void Game::OnSuspending()
{
    // TODO: Game is being power-suspended (or minimized).
}

void Game::OnResuming()
{
    m_timer.ResetElapsedTime();

    // TODO: Game is being power-resumed (or returning from minimize).
}

void Game::OnWindowMoved()
{
    auto r = m_deviceResources->GetOutputSize();
    m_deviceResources->WindowSizeChanged(r.right, r.bottom);
}

void Game::OnWindowSizeChanged(int width, int height)
{
    if (!m_deviceResources->WindowSizeChanged(width, height))
        return;

    CreateWindowSizeDependentResources();

    // TODO: Game window is being resized.
}

// Properties
void Game::GetDefaultSize(int& width, int& height) const noexcept
{
    // TODO: Change to desired default window size (note minimum size is 320x200).
    width = MAX_WIDTH;
    height = MAX_HEIGHT;
}
#pragma endregion

#pragma region Direct3D Resources
// These are the resources that depend on the device.
void Game::CreateDeviceDependentResources()
{
    auto device = m_deviceResources->GetD3DDevice();

    // TODO: Initialize device dependent objects here (independent of window size).
    device;

    //ResourceManagerクラス生成
    m_ResourceManager.reset(ResourceManager::GetInstance());

    bool check = m_ResourceManager->LoadResource();
    assert(check);

    //Randクラス生成
    m_Rand.reset(Rand::GetInstance());

    //Inputクラス生成
    m_Input.reset(Input::GetInstance());

    //Streamクラス生成
    m_Stream.reset(Stream::GetInstance());

    //Graphicsクラス生成
    m_Graphics.reset(Graphics::GetInstance());

    //Collisionクラス生成
    m_Collision.reset(Collision::GetInstance());

    //ColorListクラス生成
    m_ColorList.reset(ColorList::GetInstance());

    //SceneManagerクラス生成
    m_SceneManager.reset(SceneManager::GetInstance());

    //Texture_Fontクラス生成
    m_TextureFont.reset(TextureFont::GetInstance());

    //StaticDataクラス生成
    m_StaticData.reset(StaticData::GetInstance());

    //Arrayクラス生成
    m_Array.reset(Array::GetInstance());

    //ADX2クラス生成
    m_Adx2.reset(ADX2::GetInstance());

    //SoundManagerクラス生成
    m_SoundManager.reset(SoundManager::GetInstance());
}

// Allocate all memory resources that change on a window SizeChanged event.
void Game::CreateWindowSizeDependentResources()
{
    // TODO: Initialize windows-size dependent objects here.
}

void Game::OnDeviceLost()
{
    // TODO: Add Direct3D resource cleanup here.
}

void Game::OnDeviceRestored()
{
    CreateDeviceDependentResources();

    CreateWindowSizeDependentResources();
}
#pragma endregion
