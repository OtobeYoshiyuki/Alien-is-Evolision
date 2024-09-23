#include "pch.h"
#include "GameCamera.h"
#include "GameContext.h"
#include "GameObject/BackGround/BackGround.h"
#include "Game.h"
#include "OtobeLib/FrameWork/Input/Input.h"
#include "GameObject/Player/CoroKun.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace OtobeLib;

GameCamera::GameCamera()
{
}

void GameCamera::Initialize()
{
	//�Q�[���J������Register�ɓo�^����
	GameContext::Register<GameCamera>(this);
}

void GameCamera::Update()
{
	//�L�[�̏�Ԃ��擾����
	Input* input = Input::GetInstance();

	//�L�[�{�[�h�̏�Ԃ��擾����
	const auto& keyState = input->GetKeybord_State();
	const auto& keyTrakker = input->GetKeybord_Trakker();

	//�v���C���[���擾
	CoroKun* player = GameContext::Get<CoroKun>();

	//���W���擾����
	Vector3 pos = this->GetPosition();

	//�s����擾����
	Matrix matrix = this->GetMatrix();

	if (player->GetPosition().x > 500.0f && player->GetPosition().x < 3460)
	{
		pos = Vector3(-player->GetPosition().x + 500, 0.0f, 0.0f);
	}

	matrix = Matrix::CreateTranslation(pos);

	//���W��ݒ肷��
	this->SetPosition(pos);

	//�s���ݒ肷��
	this->SetMatrix(matrix);
}

void GameCamera::Finalize()
{
}
