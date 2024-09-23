#pragma once
#include "OtobeLib/FrameWork/Graphics/Graphics.h"
namespace OtobeLib
{
	//�_�Ő���N���X
	class Blinking
	{
	private:
		//�`����Ǘ�����X�e�[�g
		bool mState;			

		//�t���[���J�E���g
		float mFrameCount;		

		//�t���[���X�V�t���O
		bool mFrameFlag;		

		//�_�ł��銴�o
		float mTarget;

		//�_�Ŋ��o�̃J�E���g
		float mTargetCount;

		//�_�ł̏I������
		int mFinal;				

		//�_�ł̏I�����Ԃ�K�����邩	
		bool mFrameFinish;		
	public:
		//�R���X�g���N�^
		Blinking();

		//�f�X�g���N�^
		~Blinking();

		//�X�V����
		void Update();

		//�_�ł��邩�ǂ����Ԃ�
		virtual bool NowBlinking();

		//�Z�b�^�[//
		void SetState(const bool& state) { mState = state; }
		virtual void SetBlink(const bool& flag) { mFrameFlag = flag; }
		virtual void SetTarget(const float& target) { mTarget = target; }
		virtual void SetFinal(const int& Final) { mFinal = Final; }
		virtual void SetFinalOption(const bool& finish) { mFrameFinish = finish; }
		virtual void SetTargetCount(const int& target) { mTargetCount = target; }

		//�Q�b�^�[//
		virtual const bool& GetBlink()const { return mFrameFlag; }
		virtual const float& GetFrameCount()const { return mFrameCount; }
		virtual const int& GetFinal()const { return mFinal; }
		virtual const float& GetTarget()const { return mTarget; }
		virtual const float& GetTargetCount()const { return mTargetCount; }
	};
}

