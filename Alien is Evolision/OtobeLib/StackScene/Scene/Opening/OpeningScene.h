#pragma once
#include "OtobeLib/StackScene/Interface/ISceneBase.h"
#include "OtobeLib/Texture/TextureBase/TextureBase.h"
#include "OtobeLib/Texture/Manager/TextureManager.h"
#include "TemplateState.h"
#include "GameObject/Arrow/Arrow.h"
#include "BACKOUT.h"
#include "OtobeLib/FrameWork/Novel/Novel.h"

namespace OtobeLib
{
	class OpeningScene :public ISceneBase, StateArray<int, BACKOUT>
	{
	public:
		//�X�N���[���O�̍��W
		static const DirectX::SimpleMath::Vector3 BEFORE_POS;

		//�X�N���[����̍��W
		static const DirectX::SimpleMath::Vector3 AFTER_POS;

		//�X�N���[������摜�̐�
		static const int SCROLL_NUM = 7;

	private:
		//�摜�̊Ǘ��N���X
		std::unique_ptr<TextureManager>	m_textureManager;

		//�X�N���[������摜
		std::vector<ITextureBase*> m_scrollTexture;

		//�C�[�W���O�֐��̌o�ߎ���
		float						m_time;

		//�x�N�g���̌���
		int						m_direct;

		//�֐��I�u�W�F�N�g
		std::function<void(int&)> m_ifState;

		//�m�x���̊Ǘ��N���X
		std::unique_ptr<Novel>	m_novel;

	public:
		//�R���X�g���N�^
		OpeningScene();

		//�f�X�g���N�^
		~OpeningScene()override {};

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
		void OtherInitPos(const int& target, const DirectX::SimpleMath::Vector3& pos);

		//�L�[�̍X�V����
		void UpdateInput();

	};

}

