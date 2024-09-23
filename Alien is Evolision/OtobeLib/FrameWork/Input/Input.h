#pragma once
#include <Keyboard.h>
#include <Mouse.h>
#include "OtobeLib/FrameWork/Singleton/Singleton.h"

namespace OtobeLib
{
	class Input:public Singleton<Input>
	{
		friend class Singleton<Input>;//��{�I�Ƀ����o�ɃA�N�Z�X���Ȃ�
	private:
		std::unique_ptr<DirectX::Keyboard> m_keybord;//�L�[�{�[�h
		std::unique_ptr<DirectX::Mouse> m_mouse;//�}�E�X
		DirectX::Keyboard::State kb;//�L�[���͂̏��
		DirectX::Mouse::State ms;//�}�E�X�̓��͏��
		DirectX::Keyboard::KeyboardStateTracker k_trakkar;//�L�[�{�[�h�g���b�J�[
		DirectX::Mouse::ButtonStateTracker m_trakker;//�}�E�X�g���b�J�[
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
