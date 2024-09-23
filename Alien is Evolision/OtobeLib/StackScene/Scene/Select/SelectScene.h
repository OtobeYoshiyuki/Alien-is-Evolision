#pragma once
#include "OtobeLib/StackScene/Interface/ISceneBase.h"
#include "OtobeLib/Texture/TextureBase/TextureBase.h"
#include "OtobeLib/Texture/Manager/TextureManager.h"
#include "TemplateState.h"
#include "SELECT.h"
#include <vector>

namespace OtobeLib
{
	class SelectScene:public ISceneBase, StateArray<SELECT, int>
	{
		//�`�揇(���������̂��擪�ɕ`�悳���)
		enum class DRAWORDER :int
		{
			WHITEBACK,//�l�p�̉摜
			SELECT_FRAME,//�I�𒆂̘g
			FRONT_SELECT,//�S�ʂ̔������̉摜
			BACKGROUND,//�w�i
		};
	public:
		//�z��̐�
		static const int ARRAY_NUM = 2;

		//0
		static const int ARRAY_0 = 0;

		//1
		static const int ARRAY_1 = 1;

		//�K�E�Z�̖��O(����)
		static const std::string DEATHBLOW_NAME_EXPROSION;
		static const std::string DEATHBLOW_NAME_EXPROSION_TOP;
		static const std::string DEATHBLOW_NAME_EXPROSION_BOTOM;

		//�K�E�Z�̖��O(�A�r���e�B)
		static const std::string DEATHBLOW_NAME_ABILITY;
		static const std::string DEATHBLOW_NAME_ABILITY_TOP;
		static const std::string DEATHBLOW_NAME_ABILITY_BOTOM;

		//�_�ł̊Ԋu�̍ŏ��l
		static const float BLINK_INTERVAL_MIN;

		//�_�[�N�G�N�X�v���[�W�����̃Q�[�W�R�X�g
		static const int GAGE_COST_EXPROSION = 300;

		//���o�[�X�A�r���e�B�̃Q�[�W�R�X�g
		static const int GAGE_COST_REVASEAVIRITY = 100;

	private:
		//�摜�̊Ǘ��N���X
		std::unique_ptr<TextureManager>	m_TextureManager;

		//�w�i�̉摜
		ITextureBase* m_backTexture;

		//�S�ʂ̔������̉摜
		std::vector<ITextureBase*>	m_frontTexture;

		//�l�p�̉摜
		std::vector<TextureBase*>	m_squareTexture;

		//�I�𒆂̎���̘g�̉摜
		ITextureBase*				m_frameTexture;

	public:
		//�R���X�g���N�^
		SelectScene();

		//�f�X�g���N�^
		~SelectScene()override {};

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
		//���݂̏�Ԃ�ݒ肷��
		void AddData1(const SELECT& state)override { StateArray::AddData1(state); };
		void AddData2(const int& state)override { StateArray::AddData2(state); };

		//���݂̏�Ԃ��擾����
		const SELECT& GetNowData1()const override { return StateArray::GetNowData1(); };
		const int& GetNowData2()const override { return StateArray::GetNowData2(); };

		//�ЂƂO�̏�Ԃ��擾����
		const SELECT& GetPreData1()const override { return StateArray::GetPreData1(); };
		const int& GetPreData2()const override { return StateArray::GetPreData2(); };

	private:
		//�X�e�[�g�̍X�V����
		void UpdateState();

		//State_WaitKey�̏���
		void State_WaitKey();

		//State_Calcing�̏���
		void State_Calcing();

		//State_Change_CutIn�̏���
		void State_Change_CutIn();

		//State_Change_Delete�̏���
		void State_Change_Delete();
	};

}

