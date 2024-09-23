//
// Game.h
//

#pragma once

#include "DeviceResources.h"
#include "StepTimer.h"
#include "OtobeLib\FrameWork\Singleton\Singleton.h"
#include "OtobeLib\StackScene\SceneManager\SceneManager.h"
#include "OtobeLib\FrameWork\Stream\Stream.h"
#include "OtobeLib\FrameWork\ColorList\ColorList.h"
#include "OtobeLib\FrameWork\Collision\Collision.h"
#include "OtobeLib\FrameWork\Graphics\Graphics.h"
#include "OtobeLib\FrameWork\Input\Input.h"
#include "OtobeLib\FrameWork\TextureFont\TextureFont.h"
#include "OtobeLib\FrameWork\Story\Story.h"
#include "OtobeLib/FrameWork/StaticData/StaticData.h"
#include "OtobeLib/FrameWork/ResourceManager/ResourceManager.h"
#include "OtobeLib/FrameWork/Rand/Rand.h"
#include "OtobeLib/FrameWork/Array/Array.h"
#include "ADX2/Adx2.h"
#include "OtobeLib/FrameWork/SoundManager/SoundManager.h"

// A basic game implementation that creates a D3D11 device and
// provides a game loop.
class Game final : public DX::IDeviceNotify
{
public:
    static const int MAX_WIDTH = 1280;          //âÊñ ÇÃâ°ÇÃí∑Ç≥
    static const int MAX_HEIGHT = 720;          //âÊñ ÇÃècÇÃí∑Ç≥
public:

    Game() noexcept(false);
    ~Game() = default;

    Game(Game&&) = default;
    Game& operator= (Game&&) = default;

    Game(Game const&) = delete;
    Game& operator= (Game const&) = delete;

    // Initialization and management
    void Initialize(HWND window, int width, int height);

    // Basic game loop
    void Tick();

    // IDeviceNotify
    void OnDeviceLost() override;
    void OnDeviceRestored() override;

    // Messages
    void OnActivated();
    void OnDeactivated();
    void OnSuspending();
    void OnResuming();
    void OnWindowMoved();
    void OnWindowSizeChanged(int width, int height);

    // Properties
    void GetDefaultSize( int& width, int& height ) const noexcept;

private:

    void Update(DX::StepTimer const& timer);
    void Render();

    void Clear();

    void CreateDeviceDependentResources();
    void CreateWindowSizeDependentResources();

private:
    // Device resources.
    std::unique_ptr<DX::DeviceResources>    m_deviceResources;

    // Rendering loop timer.
    DX::StepTimer                           m_timer;

	//Input<Singleton>
	std::unique_ptr<OtobeLib::Input>					m_Input;

	//Stream<Singleton>
	std::unique_ptr<OtobeLib::Stream>					m_Stream;

	//Graphics<Singleton>
	std::unique_ptr<OtobeLib::Graphics>				m_Graphics;

	//ColorList<Singleton>
	std::unique_ptr<OtobeLib::ColorList>				m_ColorList;

	//Collision<Singleton>
	std::unique_ptr<OtobeLib::Collision>				m_Collision;

	//SceneManager<Singleton>
	std::unique_ptr<OtobeLib::SceneManager>			m_SceneManager;

	//TextureFont<Singleton>
	std::unique_ptr<OtobeLib::TextureFont>			m_TextureFont;

	//StaticData<Singleton>								
	std::unique_ptr<OtobeLib::StaticData>				m_StaticData;

	//ResourceManager<Singleton>
	std::unique_ptr<OtobeLib::ResourceManager>			m_ResourceManager;

    //Rand<Singleton>
    std::unique_ptr<OtobeLib::Rand>                    m_Rand;

    //Array<Singleton>
    std::unique_ptr<OtobeLib::Array>                    m_Array;

    //ADX2<Singleton>
    std::unique_ptr<ADX2>                               m_Adx2;

    //SoundManager<Singleton>
    std::unique_ptr<OtobeLib::SoundManager>             m_SoundManager;
};
