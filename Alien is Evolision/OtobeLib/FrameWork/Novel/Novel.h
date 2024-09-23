#pragma once
#include "OtobeLib/Texture/Interface/ITextureBase.h"
#include "NameWindow/NameWindow.h"
#include "TextWindow/TextWindow.h"
#include "OtobeLib/FrameWork/Story/Story.h"
#include "OtobeLib/Texture/Manager/TextureManager.h"

namespace OtobeLib
{
	class Novel
	{
	private:
		//�e�L�X�g�E�B���h�E�̃e�N�X�`��
		TextWindow* m_textWindow;	

		//���O�E�B���h�E�̃e�N�X�`��
		NameWindow* m_nameWindow;	

		//�X�g�[���[
		std::unique_ptr<Story>	m_story;

	public:
		//�R���X�g���N�^
		Novel();

		//�f�X�g���N�^
		~Novel();

		//������
		void Initialize(const wchar_t* fileName,TextureManager* texMana);

		//�X�V
		void Update();

		//�I��
		void Finalize();

	public:
		//�e�L�X�g�E�B���h�E�̃|�C���^���擾
		TextWindow* GetTextWindow() { return m_textWindow; }

		//���O�E�B���h�E�̃|�C���^���擾
		NameWindow* GetNameWindow() { return m_nameWindow; }

		//�X�g�[���[�̃|�C���^���擾
		Story* GetStory() { return m_story.get(); }
	};
}

