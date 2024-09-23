#pragma once
#include "OtobeLib/StackScene/Interface/ISceneBase.h"
#include "OtobeLib/Texture/TextureBase/TextureBase.h"
#include "OtobeLib/Texture/Manager/TextureManager.h"
#include "OtobeLib/FrameWork/Ranking/Ranking.h"
#include "TemplateState.h"
#include "GameObject/Arrow/Arrow.h"
#include "BACKOUT.h"
#include <vector>

namespace OtobeLib
{
	class RankingScene :public ISceneBase,StateArray<int,BACKOUT>
	{
	public:
		//�X�N���[���O�̍��W
		static const DirectX::SimpleMath::Vector3 BEFORE_POS;

		//�X�N���[����̍��W
		static const DirectX::SimpleMath::Vector3 AFTER_POS;
	private:
		//�摜�̊Ǘ��N���X
		std::unique_ptr<TextureManager>	m_TextureManager;

		//�����L���O�f�[�^
		std::vector<std::vector<std::unique_ptr<Ranking>>>	m_rankingDatas;

		//�����L���O�f�[�^
		std::vector<std::vector<std::string>>	m_rankingData;

		//�����L���O�̔w�ʂ̉摜
		std::vector<ITextureBase*> m_rankingBack;

		//�X�e�[�W�̃N���A���
		std::vector<std::vector<std::string>> m_clearInfo;

		//�E���
		Arrow*						m_rightArrow;

		//�����
		Arrow*						m_leftArrow;

		//Escape�L�[�̔w�ʂ̃e�N�X�`��
		TextureBase*				m_escapeBack;

		//�C�[�W���O�֐��̌o�ߎ���
		float						m_time;

		//�x�N�g���̌���
		int						m_direct;

		//�֐��I�u�W�F�N�g
		std::function<void(int&)> m_ifState;

	public:
		//�R���X�g���N�^
		RankingScene();

		//�f�X�g���N�^
		~RankingScene()override {};

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

		private:
		//�w�肵���f�[�^��z��ɂ��ĕԂ�(std::vector<>)
		const std::vector<std::string> GetSaveDataArray(const int& data);

		//�w�肵���͈͂̃f�[�^��2�����z��ɂ��ĕԂ�(std::vector<std::vector<>>)
		const std::vector<std::vector<std::string>> GetSaveDataDoubleArray(const int& first, const int& finishi, const int& target, const int interval = 1);

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

	};
}


