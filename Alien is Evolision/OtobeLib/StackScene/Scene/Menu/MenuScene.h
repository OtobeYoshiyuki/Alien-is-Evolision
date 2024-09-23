#pragma once
#include "OtobeLib/StackScene/Interface/ISceneBase.h"
#include "OtobeLib/Texture/Interface/ITextureBase.h"
#include "OtobeLib/Texture/TextureBase/TextureBase.h"
#include "OtobeLib/Texture/Manager/TextureManager.h"
#include "OtobeLib/Texture/FadeTexture/FadeTexture.h"
#include <TemplateState.h>
#include <vector>
#include <functional>

namespace OtobeLib
{
	class MenuScene:public ISceneBase,State<int>
	{
	public:
		enum class SELECT_INFO :int
		{
			NONE = -1,//�����I�����Ă��Ȃ�
			STATUS,//�X�e�[�^�X��I��
			ITEM,//�A�C�e����I��
			CHARBOOK,//�L�����N�^�[�u�b�N��I��
		};
	public:
		//�摜�̍��W
		static const std::vector<DirectX::SimpleMath::Vector3> TEXPOS;

		//�X�e�[�^�X�̍��W
		static const DirectX::SimpleMath::Vector3 STATUS_POS;

		//�A�C�e���̍��W
		static const DirectX::SimpleMath::Vector3 ITEM_POS;

		//�L�����u�b�N�̍��W
		static const DirectX::SimpleMath::Vector3 CHARBOOK_POS;

		//�eScene�̏��
		static const std::vector<std::string> SCENES_INFO;
	private:
		//�摜�̊Ǘ��N���X
		std::unique_ptr<TextureManager>			m_TextureManager;

		//�I��p�̉摜
		std::vector<OtobeLib::FadeTexture*>		m_selectTextures;

		//�X�y�[�X�L�[�̗��ʂ̉摜
		TextureBase*							m_backSpaceTexture;
	public:
		//�R���X�g���N�^
		MenuScene();

		//�f�X�g���N�^
		~MenuScene()override {};
		void Initialize()override;
		void Update()override;
		void Render()override;
		void Finalize()override;
		void Create()override;

	public:
		//���݂̏�Ԃ�ݒ肷��
		void SetNowState(const int& now)override { State::SetNowState(now); };

		//���݂̏�Ԃ��擾����
		const int& GetNowState()const override { return State::GetNowState(); };

		//�ЂƂO�̏�Ԃ��擾����
		const int& GetPreState()const override { return State::GetPreState(); };

	private:
		//�L�[���͂ɂ��I��
		void InputSelect(const int& add, std::function<void(int&)> func);

		//�I�������肷��
		void InputEntry();

	public:
		//�摜�̊Ǘ��N���X���擾����
		TextureManager* GetTextureManager() { return m_TextureManager.get(); }
	};
}

