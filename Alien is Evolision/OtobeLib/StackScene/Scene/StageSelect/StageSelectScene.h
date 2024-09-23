#pragma once
#include "OtobeLib/StackScene/Interface/ISceneBase.h"
#include "OtobeLib/Texture/Interface/ITextureBase.h"
#include "OtobeLib/Texture/Manager/TextureManager.h"
#include "TemplateState.h"
#include "GameObject/Arrow/Arrow.h"
#include "BACKOUT.h"
#include <vector>
#include <string>
#include <functional>
namespace OtobeLib
{
	class StageSelectScene :public ISceneBase,StateArray<int,BACKOUT>
	{
	public:
		enum class STAGEINFO :int
		{
			CLEARTAG,//�N���A�󋵂̃^�O
			STAGENAME,//�X�e�[�W��
			RELEASEINFO,//�����
			CLEARINFO,//�N���A��
		};
	public:
		//�X�N���[���O�̍��W
		static const DirectX::SimpleMath::Vector3 BEFORE_POS;

		//�X�N���[����̍��W
		static const DirectX::SimpleMath::Vector3 AFTER_POS;
	private:
		//�摜�̊Ǘ��N���X
		std::unique_ptr<TextureManager>	m_TextureManager;

		//�X�e�[�W�摜�̔z��
		std::vector<ITextureBase*> m_stageTexture;

		//�C�[�W���O�֐��̌o�ߎ���
		float						m_time;

		//�x�N�g���̌���
		int						m_direct;

		//�֐��I�u�W�F�N�g
		std::function<void(int&)> m_ifState;

		//�X�e�[�W�̃N���A���
		std::vector<std::vector<std::string>> m_clearInfo;

		//�E���
		Arrow* m_rightArrow;

		//�����
		Arrow* m_leftArrow;

		//�X�y�[�X�L�[�̔w�ʂ̃e�N�X�`��
		TextureBase* m_spaceBackTexture;
	public:
		StageSelectScene();
		~StageSelectScene()override {};
		void Initialize()override;
		void Update()override;
		void Render()override;
		void Finalize()override;
		void Create()override;

	public:
		//���݂̏�Ԃ�ݒ肷��
		void AddData1(const int& state)override { StateArray::AddData1(state); };
		void AddData2(const BACKOUT& state)override { StateArray::AddData2(state); };

		//���݂̏�Ԃ��擾����
		const int& GetNowData1()const override { return StateArray::GetNowData1(); };
		const BACKOUT& GetNowData2()const override { return StateArray::GetNowData2(); };

		//�ЂƂO�̏�Ԃ��擾����
		const int& GetPreData1()const override { return StateArray::GetPreData1(); };
		const BACKOUT& GetPreData2()const override { return StateArray::GetPreData2(); };

	private:
		//�C�[�W���O�֐��̍X�V
		void UpdateEasing();

		//State_None�̏���
		void State_None();

		//State_Admisson�̏���
		void State_Admisson();

		//State_Exit�̏���
		void State_Exit();

		//State_Result�̏���
		void State_Result();

		//�X�N���[���ΏۈȊO�̍��W������������
		void OtherInitPos(const int& target,const DirectX::SimpleMath::Vector3& pos);
	};
}

