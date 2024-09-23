#pragma once
#include "OtobeLib/StackScene/Interface/ISceneBase.h"
#include "OtobeLib/Texture/Interface/ITextureBase.h"
#include "OtobeLib/Texture/Manager/TextureManager.h"
#include "TemplateState.h"
#include <string>
#include <vector>
namespace OtobeLib
{
	class PoceScene :public ISceneBase,State<int>
	{
	public:
		//�t�H���g�̏��
		static const std::vector<std::pair<std::string, DirectX::SimpleMath::Vector2>> FONT_INFO;
	private:
		//�摜�̊Ǘ��N���X
		std::unique_ptr<TextureManager>	m_TextureManager;

		//�e�V�[���ւ̑J�ډ摜
		std::vector<ITextureBase*>		m_sceneTexture;
	public:
		PoceScene();
		~PoceScene()override {};
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
	};
}

