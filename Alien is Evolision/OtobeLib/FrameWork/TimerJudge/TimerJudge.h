#pragma once
#include <string>
namespace OtobeLib
{
	class TimerJudge
	{
	public:
		//1�b
		static const int ONESECOND = 60;
		//1��
		static const int ONEMINUTE = 3600;
	private:
		//�t���[�������J�E���g����
		float						m_FrameCount;
		//��������
		float						m_LimitTimer;
		//�^�C�}�[���X�V����t���O
		bool						m_TimerUp;
		//�������Ԃ𒴂��������肷��t���O
		bool						m_Judge;
		//1�b�P�ʂ̕␳�l
		float						m_correction;
	public:
		//�R���X�g���N�^
		TimerJudge();

		//�f�X�g���N�^
		~TimerJudge(){}

		//����������
		void Initialize(const int& limit, const bool& up);

		//�X�V����
		void Update();

	public:
		//--------------------------------�Z�b�^�[-----------------------------------------------------//

		//�������Ԃ�ݒ肷��
		void SetLimitTimer(const int& limit) { m_LimitTimer = limit; }

		//�^�C�}�[�̍X�V�t���O��ݒ肷��
		void SetTimerUp(const bool& up) { m_TimerUp = up; }

		//1�b�P�ʂ̕␳�l��ݒ肷��
		void SetCorrection(const float& cor) { m_correction = cor; }

		//--------------------------------�Z�b�^�[-----------------------------------------------------//

		//--------------------------------�Q�b�^�[-----------------------------------------------------//
		
		//�������Ԃ��擾����
		const int& GetLimitTimer()const { return m_LimitTimer; }

		//�^�C�}�[�̍X�V�t���O���擾����
		const bool& GetTimerUp()const { return m_TimerUp; }

		//�������Ԃ̗L�����擾����
		const bool& GetTimerJudge()const { return m_Judge; }
		
		//--------------------------------�Q�b�^�[-----------------------------------------------------//

	public:
		//�������Ԃ���A�\������b�������߂�
		const std::string AskTimeStringSecound();

		//�������Ԃ���A�\�����镪�������߂�
		const std::string AskTimeStringMinute();
	};
}

