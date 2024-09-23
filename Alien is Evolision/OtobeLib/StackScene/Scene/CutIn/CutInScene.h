#pragma once
#include "OtobeLib/StackScene/Interface/ISceneBase.h"
#include "OtobeLib/Texture/TextureBase/TextureBase.h"
#include "OtobeLib/Texture/Manager/TextureManager.h"
#include "Effect/CutInManager/CutInManager.h"

namespace OtobeLib
{
	class CutInScene :public ISceneBase
	{
	private:
		//�J�b�g�C���̊Ǘ��N���X
		std::unique_ptr<CutInManager>   m_CutInManager;
		//Texture�̊Ǘ��N���X
		std::unique_ptr<TextureManager>	m_TextureManager;
	public:
		//�R���X�g���N�^
		CutInScene();

		//�f�X�g���N�^
		~CutInScene()override = default;

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
		//TextureManager���擾����
		TextureManager* GetTextureManager() { return m_TextureManager.get(); }

		//CutInManager���擾����
		CutInManager* GetCutInManager() { return m_CutInManager.get(); }
	};
}

