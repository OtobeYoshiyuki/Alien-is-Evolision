#pragma once
#include "OtobeLib/StackScene/Interface/ISceneBase.h"
#include "OtobeLib/Texture/TextureBase/TextureBase.h"
#include "OtobeLib/Texture/Manager/TextureManager.h"

namespace OtobeLib
{
	class LogoScene :public ISceneBase
	{
	private:
		//�摜�̊Ǘ��N���X
		std::unique_ptr<TextureManager>	m_TextureManager;
	public:
		//�R���X�g���N�^
		LogoScene();

		//�f�X�g���N�^
		~LogoScene()override {};

		//����������
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

