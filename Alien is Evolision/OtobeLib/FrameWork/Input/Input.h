#pragma once
#include <Keyboard.h>
#include <Mouse.h>
#include "OtobeLib/FrameWork/Singleton/Singleton.h"

namespace OtobeLib
{
	class Input:public Singleton<Input>
	{
		friend class Singleton<Input>;//基本的にメンバにアクセスしない
	private:
		std::unique_ptr<DirectX::Keyboard> m_keybord;//キーボード
		std::unique_ptr<DirectX::Mouse> m_mouse;//マウス
		DirectX::Keyboard::State kb;//キー入力の情報
		DirectX::Mouse::State ms;//マウスの入力情報
		DirectX::Keyboard::KeyboardStateTracker k_trakkar;//キーボードトラッカー
		DirectX::Mouse::ButtonStateTracker m_trakker;//マウストラッカー
	private:
		Input();
	public:
		~Input();
	public:
		void Update();
		const DirectX::Keyboard::State& GetKeybord_State()const;
		const DirectX::Mouse::State& GetMouse_State()const;
		DirectX::Keyboard& GetKeybord()const;
		DirectX::Mouse& GetMouse()const;
		const DirectX::Keyboard::KeyboardStateTracker& GetKeybord_Trakker()const;
		const DirectX::Mouse::ButtonStateTracker& GetMouse_Trakker()const;
	};
}
