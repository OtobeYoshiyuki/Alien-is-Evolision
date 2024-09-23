#pragma once
#include "OtobeLib/StackScene/Interface/ISceneBase.h"
#include "OtobeLib/Texture/Interface/ITextureBase.h"
#include "OtobeLib/Texture/Manager/TextureManager.h"
#include "OtobeLib/Texture/Blinking/Blinking.h"
#include "GameObject/Arrow/Arrow.h"
#include <string>
#include <vector>
namespace OtobeLib
{
	class TitleScene :public ISceneBase
	{
	public:
		//�X�e�[�W�Z���N�g��
		static const std::pair<std::string,DirectX::SimpleMath::Vector2> NEXT_STAGESELECT;

		//�����L���O��
		static const std::pair<std::string,DirectX::SimpleMath::Vector2> NEXT_RANKING;

		//�I�[�v�j���O��
		static const std::pair<std::string, DirectX::SimpleMath::Vector2> NEXT_OPENING;
	private:
		//�摜�̊Ǘ��N���X
		std::unique_ptr<TextureManager> m_TextureManager;

		//�����̓_�ŃN���X
		std::unique_ptr<Blinking>	m_blink;

		//Arrow�N���X
		Arrow*						m_rightArrowTexture;
		Arrow*						m_leftArrowTexture;

		//�\���^�[�Q�b�g
		std::pair<std::string,DirectX::SimpleMath::Vector2>		m_targetStrings;
	public:
		//�R���X�g���N�^
		TitleScene();

		//�f�X�g���N�^
		~TitleScene()override {};

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

