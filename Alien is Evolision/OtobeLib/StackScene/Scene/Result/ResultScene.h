#pragma once
#include "OtobeLib/StackScene/Interface/ISceneBase.h"
#include "OtobeLib/Texture/Interface/ITextureBase.h"
#include "OtobeLib/Texture/Manager/TextureManager.h"
#include "OtobeLib/FrameWork/TimerJudge/TimerJudge.h"
#include "OtobeLib/FrameWork/Score/Score.h"
#include "OtobeLib/FrameWork/Ranking/Ranking.h"
#include "TemplateState.h"
#include "CLEARRESULT.h"
#include <functional>
namespace OtobeLib
{
	class ResultScene :public ISceneBase,State<CLEARSTATE>
	{
	public:
		enum RANKING_INFO :int
		{
			STAGE1 = 1,
			STAGE2 = 11,
			STAGE3 = 21,
			STAGE4 = 31,
			BOSSSTAGE = 41,
			STAGENUM = 5,
		};
		enum class RESULT_INFO :int
		{
			LEVEL,//���x��
			SCORE,//�X�R�A
			CLEATIME,//�N���A�^�C��
			RANKING_NUM,//�����L���O�̐�
		};
	public:
		//�X�e�[�W�p�̃f�[�^��ǉ�����Ԋu
		static const int STAGE_INTERVAL = 3;

		//�N���A�f�[�^�͈̔�
		static const int CREAR_RESULT_RANGE = 9;

		//�ŏ��ɕ\�������UI�ƃX�N���[������UI�̋���
		static const float UI_INTERVAL;
	private:
		//�摜�̊Ǘ��N���X
		std::unique_ptr<TextureManager>	m_TextureManager;

		//�N���A�^�C��
		std::unique_ptr<TimerJudge>		m_timerJudge;

		//�l���X�R�A
		std::unique_ptr<Score>			m_score;

		//�g�[�^���X�R�A
		std::unique_ptr<Score>			m_totalScore;

		//�X�y�[�X�L�[�̓_��
		std::unique_ptr<Blinking>		m_spacekeyBlink;

		//�o�ߎ���
		float							m_time;

		//�L�[����������
		int								m_pushCount;

		//�֐��I�u�W�F�N�g
		std::function<void()>			m_func;

		//�����L���O�f�[�^
		std::vector<std::vector<std::string>>	m_rankingData;

		//�����L���O�f�[�^
		std::vector<std::unique_ptr<Ranking>>	m_rankings;

		//�t�H���g�̃x�[�X�̍��WX
		float			m_fontX;

	public:
		ResultScene();
		~ResultScene()override;
		void Initialize()override;
		void Update()override;
		void Render()override;
		void Finalize()override;
		void Create()override;

	private:
		//�X�e�[�g�̍X�V����
		void UpdateState();

		//State_None�̏���
		void State_None();

		//State_WaitKey�̏���
		void State_WaitKey();

		//State_Next�̏���
		void State_Next();

		//State_Admisson�̏���
		void State_Admisson();

		//State_Ranking�̏���
		void State_Ranking();

	public:
		//���݂̏�Ԃ�ݒ肷��
		void SetNowState(const CLEARSTATE& now)override { State::SetNowState(now); };

		//���݂̏�Ԃ��擾����
		const CLEARSTATE& GetNowState()const override { return State::GetNowState(); };

		//�ЂƂO�̏�Ԃ��擾����
		const CLEARSTATE& GetPreState()const override { return State::GetPreState(); };

		//�w�肵���f�[�^��z��ɂ��ĕԂ�(std::vector<>)
		const std::vector<std::string> GetSaveDataArray(const int& data);

		//�w�肵���͈͂̃f�[�^��2�����z��ɂ��ĕԂ�(std::vector<std::vector<>>)
		const std::vector<std::vector<std::string>> GetSaveDataDoubleArray(const int& first, const int& finishi,const int& target, const int interval = 1);


	};
}

