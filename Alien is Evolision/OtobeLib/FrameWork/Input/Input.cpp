#include "pch.h"
#include "Input.h"

using namespace DirectX;
using namespace OtobeLib;

Input::Input()
{
	m_keybord = std::make_unique<Keyboard>();
	m_mouse = std::make_unique<Mouse>();
}


Input::~Input()
{
}

void Input::Update()
{
	kb = m_keybord->GetState();

	k_trakkar.Update(kb);

	ms = m_mouse->GetState();


	m_trakker.Update(ms);
}

const Keyboard::State & Input::GetKeybord_State() const
{
	return kb;
}

const DirectX::Mouse::State & Input::GetMouse_State() const
{
	return ms;
}

Keyboard & Input::GetKeybord()const
{
	return *m_keybord.get();
}

Mouse & Input::GetMouse()const
{
	return *m_mouse.get();
}

const DirectX::Keyboard::KeyboardStateTracker & Input::GetKeybord_Trakker() const
{
	return k_trakkar;
}

const DirectX::Mouse::ButtonStateTracker& Input::GetMouse_Trakker() const
{
	return m_trakker;
}
