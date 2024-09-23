#pragma once
#include <string>
#include <map>
#include "AnimationImporter.h"
namespace OtobeLib
{
	//�^���������̂ŏȗ�
	using AnimationList = std::list<std::unique_ptr<AnimationImporter>>;
	using AnimationMap = std::map<std::string,AnimationList>;

	//�A�j���[�V�����N���X
	class Animation
	{
	private:
		//�w�肵���L�[�̃A�j���[�V�����̏��
		std::map<std::string, RECT>			m_Animation;
		//���ɍĐ�����A�j���[�V�����̏��
		std::map<std::string, std::string>	m_NextAnimation;
		//�A�j���[�V�����̏I������
		std::map<std::string, int>			m_EndTime;
		//���݂̃A�j���[�V����
		RECT								m_NowRect;
		//���݂̃A�j���[�V�����̏��
		std::string							m_NowInfo;
		//�A�j���[�V�����̃��[�v���
		std::string							m_LoopInfo;
		//�A�j���[�V�����J�E���g
		int									m_AniCount;
		//�A�j���[�V�����̍X�V�t���O
		bool								m_AniFlag;
		//�A�j���[�V�����̃��[�v�t���O
		bool								m_LoopFlag;
		//�A�j���[�V�����̃O���[�v��ݒ肷��
		static	AnimationMap s_AniGrop;
	public:
		//�R���X�g���N�^
		Animation();
		//�f�X�g���N�^
		~Animation(){}
		//������
		void Initialize();
		//�X�V
		void Update();
		//�I��
		void Finalize();
		//--------------------�Z�b�^�[------------------------------------------//
		
		//�A�j���[�V�����̏���ݒ肷��
		virtual void SetAnimation(const std::string& name, const RECT& rect) { m_Animation[name] = rect; }

		//���ɍĐ�����A�j���[�V�����̏���ݒ肷��
		virtual void SetNextAnimation(const std::string& now, const std::string& next) { m_NextAnimation[now] = next; }

		//�A�j���[�V�����̏I�����Ԃ�ݒ肷��
		virtual void SetEndTime(const std::string& name, const int& time) { m_EndTime[name] = time; }

		//���݂̃A�j���[�V������ݒ肷��
		virtual void SetNowAnimation(const RECT& rect) { m_NowRect = rect; }

		//���݂̃A�j���[�V�����̏���ݒ肷��
		virtual void SetNowInfo(const std::string& info) { m_NowInfo = info; }

		//�A�j���[�V�����̃��[�v����ݒ肷��
		virtual void SetLoopInfo(const std::string& loop) { m_LoopInfo = loop; }

		//�A�j���[�V�����̍X�V�t���O��ݒ肷��
		virtual void SetAniFlag(const bool& flag) { m_AniFlag = flag; }

		//�A�j���[�V�����̃��[�v�t���O��ݒ肷��
		virtual void SetLoopFlag(const bool& flag) { m_LoopFlag = flag; }

		//�A�j���[�V�����̃p�����[�^�[�������p��
		virtual void SetNewAnimation(Animation* ani);

		//�A�j���[�V�����̃^�C�}�[������������
		virtual void SetInitTimer(const int& timer) { m_AniCount = timer; }

		//--------------------�Z�b�^�[------------------------------------------//

		//--------------------�Q�b�^�[------------------------------------------//
		
		//�A�j���[�V�����̏����擾����
		virtual const RECT& GetAnimation(const std::string& name) { return m_Animation[name]; }

		//���ɍĐ�����A�j���[�V�����̏����擾����
		virtual const std::string& GetNextAnimation(const std::string& now) { return m_NextAnimation[now]; }

		//�A�j���[�V�����̏I�����Ԃ�ݒ肷��
		virtual const int& GetEndTime(const std::string& name) { return m_EndTime[name]; }

		//���݂̃A�j���[�V�������擾����
		virtual const RECT& GetNowAnimation()const { return m_NowRect; }

		//���݂̃A�j���[�V�����̏���ݒ肷��
		virtual const std::string& GetNowInfo()const { return m_NowInfo; }

		//�A�j���[�V�����̃��[�v����ݒ肷��
		virtual const std::string& GetLoopInfo()const {return m_LoopInfo; }

		//�A�j���[�V�����̍X�V�t���O���擾����
		virtual const bool& GetAniFlag()const { return m_AniFlag; }

		//�A�j���[�V�����̃��[�v�t���O���擾����
		virtual const bool& GetLoopFlag()const { return m_LoopFlag; }

		//�A�j���[�V�����J�E���g���擾����
		virtual const int& GetAniCount()const { return m_AniCount; }

		//�A�j���[�V������Map���擾����
		virtual const std::map<std::string, RECT>& GetAnimationMap()const { return m_Animation; }

		//���̃A�j���[�V������Map���擾����
		virtual const std::map<std::string, std::string>& GetNextAnimationMap()const { return m_NextAnimation; }

		//�A�j���[�V�����̏I�����Ԃ�Map���擾����
		virtual const std::map<std::string, int>& GetEndTimeMap()const { return m_EndTime; }

		//�������g���擾����
		virtual Animation* GetThisAnimation() { return this; }

		//�A�j���[�V�����̏������֐����Ă�
		virtual void InitMiniMapAnimation();
		
		//--------------------�Q�b�^�[------------------------------------------//

	public:
		//�e�N�X�`���̏���ǉ�����
		static void Add(const std::vector<AnimationImporter>& Importer)
		{
			for (const AnimationImporter& importer : Importer)
			{
				//�O���[�v�����擾����
				std::string grop = importer.GetGrop();

				//�C���X�^���X�𐶐�����
				s_AniGrop[grop].push_back(std::make_unique<AnimationImporter>(importer));
			}
		}

		//�A�j���[�V�����R���e�i���擾����
		static const AnimationMap& GetAnimeMap() { return s_AniGrop; }

		//�R���e�i�̒��̃��X�g���擾����
		static const AnimationList& GetAnimeList(const std::string& grop)
		{
			return s_AniGrop[grop];
		}
	};
}

