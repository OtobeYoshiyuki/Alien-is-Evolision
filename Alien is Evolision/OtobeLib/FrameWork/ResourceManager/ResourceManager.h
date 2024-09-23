#pragma once
#include "OtobeLib/FrameWork/Singleton/Singleton.h"
#include "OtobeLib/Texture/Interface/ITextureBase.h"
#include "OtobeLib/Texture/TextureBase/TextureBase.h"
namespace OtobeLib
{
	class ResourceManager:public Singleton<ResourceManager>
	{
		friend Singleton<ResourceManager>;
		using MapTexture = std::map<std::string, std::unique_ptr<ITextureBase>>;
	private:
		//�ǂݍ��܂ꂽ�e�N�X�`���̏��
		MapTexture			m_TextureList;
	private:
		//�R���X�g���N�^
		ResourceManager();
	public:
		//�f�X�g���N�^
		~ResourceManager();

		//���\�[�X��ǂݍ���
		bool LoadResource();

		//�w�肵���e�N�X�`���̏����擾
		ITextureBase* GetTexture(const std::string name)const
		{
			//�w�肵���L�[�̃e�N�X�`����T��
			MapTexture::const_iterator iter = m_TextureList.find(name);
			//����������e�N�X�`���̏���Ԃ�
			if (iter != m_TextureList.end())return iter->second.get();
			//������Ȃ�����
			return nullptr;
		}
	private:
		//�e�N�X�`����ǂݍ���
		bool LoadTexture();

		//�e�N�X�`���̃L�[�̑}��
		void AddTex(TextureBase* tex) 
		{ 
			m_TextureList.insert(std::make_pair(tex->GetName(),std::make_unique<TextureBase>(tex)));
		}

		//Texture��Map���擾
		const MapTexture& GetMapTexture()const { return m_TextureList; }
	};
}

