#pragma once
#include "OtobeLib/StackScene/Interface/ISceneBase.h"
#include "OtobeLib/Texture/Interface/ITextureBase.h"
#include "OtobeLib/Texture/Manager/TextureManager.h"
#include "OtobeLib/Texture/AnimeTexture/AnimeTexture.h"
#include "GameObject/DamageUI/FadeState.h"
#include "TemplateState.h"
#include "DIRECTING.h"
#include <vector>

namespace OtobeLib
{
	//�K�E�Z�̉��o���s���V�[��
	class DirecTingScene:public ISceneBase,StateArray<DIRECTING,int,FADE_STATE>
	{
		//�`�揇(���������̂��擪�ɕ`�悳���)
		enum class DRAWORDER :int
		{
			EXPROSION,//����
			PLAYER,//�v���C���[
			PLAYER_BACK,//�v���C���[�̗���
			WAVE,//�I�[��
			BACKGROUND,//�w�i
		};
	public:
		//�z��̑傫��
		static const int EXPROSION_ARRAY_NUM = 3;
		
		//�X�e�[�g��؂�ւ��鎞��
		static const float CHANGE_TIME_PLAYER;

		//�X�e�[�g��؂�ւ��鎞��
		static const float CHANGE_TIME_EXPROSION;

		//�v���C���[�̃A�j���[�V�����̑҂�����
		static const float WAIT_TIME_FACE;

		//�v���C���[�̔w�i�̃A�j���[�V�����̑҂�����
		static const float WAIT_TIME_BACKFADE;

		//0
		static const int ARRAY_0 = 0;

		//1
		static const int ARRAY_1 = 1;

		//2
		static const int ARRAY_2 = 2;

		//�Q�[�W�̉񕜗�(�R�}���h�Z)
		static const int TECHNIQUE_HEAL_COMMAND = 50;

		//�Q�[�W�̉񕜗�(�K�E�Z)
		static const int TECHNIQUE_HEAL_SPECIAL = 100;

		//�擾�X�R�A
		static const int BASE_SCORE = 1000;
	private:
		//�摜�̊Ǘ��N���X
		std::unique_ptr<TextureManager>	m_TextureManager;

		//�w�i
		ITextureBase*			m_backTexture;

		//�v���C���[�̃A�j���[�V�����p�N���X
		AnimeTexture*			m_playerAnimeTex;

		//�I�[���̃A�j���[�V�����p�N���X
		AnimeTexture*			m_waveAnimeTex;

		//�����G�t�F�N�g�̃A�j���[�V�����p�N���X(��������̂ŁAstd::vector�ɂ���)
		std::vector<AnimeTexture*>	m_exproAnimeTex;

		//���ʂ̃e�N�X�`��
		ITextureBase*			m_backPlayer;

		//�o�ߎ���
		float					m_timer;

	public:
		//�R���X�g���N�^
		DirecTingScene();

		//�f�X�g���N�^
		~DirecTingScene()override {};

		//������
		void Initialize()override;

		//�X�V����
		void Update()override;

		//�`�揈��
		void Render()override;

		//�I������
		void Finalize()override;

		//����
		void Create()override;

	public:
		//���݂̏�Ԃ�ݒ肷��
		void AddData1(const DIRECTING& state)override { StateArray::AddData1(state); };
		void AddData2(const int& state)override { StateArray::AddData2(state); };
		void AddData3(const FADE_STATE& state)override { StateArray::AddData3(state); };

		//���݂̏�Ԃ��擾����
		const DIRECTING& GetNowData1()const override { return StateArray::GetNowData1(); };
		const int& GetNowData2()const override { return StateArray::GetNowData2(); };
		const FADE_STATE& GetNowData3()const override { return StateArray::GetNowData3(); };

		//�ЂƂO�̏�Ԃ��擾����
		const DIRECTING& GetPreData1()const override { return StateArray::GetPreData1(); };
		const int& GetPreData2()const override { return StateArray::GetPreData2(); };
		const FADE_STATE& GetPreData3()const override { return StateArray::GetPreData3(); };

	private:
		//�X�e�[�g�̍X�V����
		void UpdateState();

		//State_None�̏���
		void State_None();

		//State_WaveAnime�̏���
		void State_WaveAnime();

		//State_TimerUp�̏���
		void State_TimerUp();

		//State_PlayerAnime�̏���
		void State_PlayerAnime();

		//State_BackFade�̏���
		void State_BackFade();

		//State_ExproAnime�̏���
		void State_ExproAnime();

		//State_End�̏���
		void State_End();
	};
}