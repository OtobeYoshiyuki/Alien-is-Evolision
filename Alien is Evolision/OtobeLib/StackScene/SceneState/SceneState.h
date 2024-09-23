#pragma once
#include "OtobeLib/FrameWork/Graphics/Graphics.h"
#include "OtobeLib/Texture/Interface/ITextureBase.h"
#include "OtobeLib/Texture/Manager/TextureManager.h"

namespace OtobeLib
{
	class SceneState
	{
	public:
		enum FADESTATE :int
		{
			FADENONE = -1,
			FADEIN,//�t�F�[�h�C��
			WAITKEY,//�Q�[���̏���
			FADEOUT,//�t�F�[�h�A�E�g
			FADERESULT,//�t�F�[�h�I��
		};

	private:
		//�t�F�[�h�̃X�e�[�g
		FADESTATE m_FadeState;

		//�t�F�[�h�̃e�N�X�`��
		ITextureBase* m_Texture;

		//�摜�̊Ǘ��N���X
		std::unique_ptr<TextureManager> m_TextureManager;

		//�t�F�[�h�̉��Z�̑��x
		float	m_fadeSpeed;

	public:
		//���t���[���̃A���t�@�l�����Z���鑬�x
		static const float ALFA_SPEED;

		//�A���t�@�l�̌��E�l
		static const float LIMITTE_ALFA;

	public:
		//�R���X�g���N�^
		SceneState();

		//�f�X�g���N�^
		~SceneState() {};
		
		//������
		void Initialize();

		//�X�V
		void Update();

		//�`��
		void Render();

//-----------------------�Z�b�^�[-----------------------------------------------------//
		void SetFadeState(const FADESTATE& fade) { m_FadeState = fade; };
//-----------------------�Z�b�^�[-----------------------------------------------------//

//-----------------------�Q�b�^�[-----------------------------------------------------//
		const FADESTATE& GetFade()const { return m_FadeState; };
//-----------------------�Q�b�^�[-----------------------------------------------------//

	private:
		//�t�F�[�h�C��
		void Fade_In();

		//�t�F�[�h�A�E�g
		void Fade_Out();

	public:
		//�A���t�@�l��ݒ肷��
		void SetAlfa(const float& alfa);

		//�t�F�[�h�̉��Z�l��ݒ肷��
		void SetFadeSpeed(const float& speed) { m_fadeSpeed = speed; }
	};
}

