#pragma once
#include "OtobeLib/StackScene/Interface/ISceneBase.h"
#include "OtobeLib/Texture/Interface/ITextureBase.h"
#include "OtobeLib/Texture/TextureBase/TextureBase.h"
#include "OtobeLib/Texture/Manager/TextureManager.h"
#include <map>
#include <vector>
#include <string>

namespace OtobeLib
{
	class StatusScene:public ISceneBase
	{
		enum class STATUS_INFO :int
		{
			LEVEL = 1,HP,MP,AT,DF,SP,NEXT,TOTAL,
		};
	private:
		//�X�e�[�^�X�֘A�̘A�z�z��
		static std::vector<std::pair<std::string, DirectX::SimpleMath::Vector2>> s_statusInfo;

		//�R�}���h�֘A�̘A�z�z��
		static const std::vector<std::pair<std::string, DirectX::SimpleMath::Vector2>> COMMAND_INFO;
	private:
		//�摜�̊Ǘ��N���X
		std::unique_ptr<TextureManager>	m_TextureManager;

		//�X�y�[�X�L�[�̗��ʂ̉摜
		TextureBase* m_backSpaceTexture;
	public:
		//�R���X�g���N�^
		StatusScene();

		//�f�X�g���N�^
		~StatusScene()override {};

		//������
		void Initialize()override;

		//�X�V����
		void Update()override;

		//�`�揈��
		void Render()override;

		//�I������
		void Finalize()override;

		//��������
		void Create()override;
	};
}

