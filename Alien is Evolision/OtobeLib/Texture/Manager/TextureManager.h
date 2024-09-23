#pragma once
#include "OtobeLib/Texture/Interface/ITextureBase.h"
namespace OtobeLib
{
	class TextureManager
	{	
	private:
		std::list<std::unique_ptr<ITextureBase>>	m_TextureList;//�e�N�X�`���̃��X�g
	public:
		//�R���X�g���N�^
		TextureManager();
		//�f�X�g���N�^
		~TextureManager();
		//�X�V����
		void Update();	
		//�`�揈��
		void Render();

	public:
		//�ǉ�
		void Add(std::unique_ptr<ITextureBase>&& tex) { m_TextureList.push_back(std::move(tex)); }
		//�Ō�ɒǉ��������̂��擾����
		ITextureBase* GetBack() { return m_TextureList.back().get(); }
		//���X�g���擾����
		std::list<std::unique_ptr<ITextureBase>>& GetTextureList() { return m_TextureList; }
	};
}

