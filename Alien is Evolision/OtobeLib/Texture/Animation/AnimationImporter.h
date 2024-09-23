#pragma once
#include <vector>
#include <string>

namespace OtobeLib
{
	//�A�j���[�V�����̏��𑗐M����N���X
	class AnimationImporter
	{
	public:
		enum class ANIMATIONINFO :int
		{
			GROP,//�O���[�v��
			ANIMATION,//�A�j���[�V����
			NEXT,//���̃A�j���[�V����
			ENDTIME,//�A�j���[�V�����̏I���t���[��
			TEXTURE,//�g�p����e�N�X�`��
		};
	private:
		//�A�j���[�V�����O���[�v�̖��O
		std::string			m_GropName;
		//�A�j���[�V�����̏��
		std::string			m_Animation;
		//���ɍĐ�����A�j���[�V�����̏��
		std::string	        m_Next;
		//�A�j���[�V�����̏I������
		int					m_End;
		//�g�p����e�N�X�`��
		std::string			m_Refference;
	public:
		//�R���X�g���N�^
		AnimationImporter();

		//�R���X�g���N�^(��������)
		AnimationImporter(const AnimationImporter& importer);

		//�f�X�g���N�^
		~AnimationImporter() {}

		//�O���[�v����ݒ肷��
		void SetGrop(const std::string& grop) { m_GropName = grop; }

		//�A�j���[�V�����̏���ݒ肷��
		void SetAnimation(const std::string& animation) { m_Animation = animation; }

		//���̃A�j���[�V�����̏���ݒ肷��
		void SetNext(const std::string& next) { m_Next = next; }

		//�A�j���[�V�����̏I�����Ԃ�ݒ肷��
		void SetEnd(const int& end) { m_End = end; }

		//�g�p����e�N�X�`����ݒ肷��
		void SetRefference(const std::string& ref) { m_Refference = ref; }

		//�O���[�v�����擾����
		const std::string& GetGrop()const { return m_GropName; }

		//�A�j���[�V�����̏����擾����
		const std::string& GetAnimation()const { return m_Animation; }

		//���̃A�j���[�V�����̏����擾����
		const std::string& GetNext()const { return m_Next; }

		//�A�j���[�V�����̏I�����Ԃ�ݒ肷��
		const int& GetEnd()const { return m_End; }

		//�g�p����e�N�X�`�����擾����
		const std::string& GetRefference()const { return m_Refference; }
	public:
		//�f�[�^���A�j���[�V�����p�ɕϊ�����
		static const std::vector<AnimationImporter> Cast(const std::vector<std::vector<std::string>>& Input);
	};
}

