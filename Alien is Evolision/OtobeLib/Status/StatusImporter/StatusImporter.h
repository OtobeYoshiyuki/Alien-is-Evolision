#pragma once
#include <string>
#include <vector>

namespace OtobeLib
{
	//�X�e�[�^�X�̏��𑗐M����N���X
	class StatusImporter
	{
	public:
		enum class STATUSINFO :int
		{
			GROP,//�O���[�v��
			LEVEL,//���x��
			HP,//HP
			MP,//MP
			ATACK,//�U����
			DEFENCE,//�h���
			SPECIAL,//�X�y�V�����|�C���g
			NEXT,//���̃��x���܂ł̌o���l
		};

	private:
		//�X�e�[�^�X�O���[�v�̖��O
		std::string			m_gropName;
		//���x��
		int					m_level;
		//HP
		int					m_hp;
		//MP
		int					m_mp;
		//�U����
		int					m_atack;
		//�h���
		int					m_defence;
		//�X�y�V�����|�C���g
		int					m_special;
		//���̃��x���܂�
		int					m_next;

	public:
		//�R���X�g���N�^
		StatusImporter();

		//�R���X�g���N�^(�����A��)
		StatusImporter(const StatusImporter& importer);

		//�f�X�g���N�^
		~StatusImporter() = default;

//----------------------------------------�Z�b�^�[-----------------------------------------------------//

	public:
		//�O���[�v����ݒ肷��
		void SetGrop(const std::string& grop) { m_gropName = grop; }

		//���x����ݒ肷��
		void SetLevel(const int& level) { m_level = level; }

		//HP��ݒ肷��
		void SetHp(const int& hp) { m_hp = hp; }

		//MP��ݒ肷��
		void SetMp(const int& mp) { m_mp = mp; }

		//�U���͂�ݒ肷��
		void SetAtack(const int& atack) { m_atack = atack; }

		//�h��͂�ݒ肷��
		void SetDefence(const int& defence) { m_defence = defence; }

		//�X�y�V�����|�C���g��ݒ肷��
		void SetSpecial(const int& special) { m_special = special; }

		//���̃��x���܂ł̌o���l��ݒ肷��
		void SetNextLevelPoint(const int& next) { m_next = next; }

//----------------------------------------�Z�b�^�[-----------------------------------------------------//


//----------------------------------------�Q�b�^�[-----------------------------------------------------//

	public:
		//�O���[�v�����擾����
		const std::string& GetGrop()const { return m_gropName; }

		//���x�����擾����
		const int& GetLevel()const { return m_level; }

		//HP���擾����
		const int& GetHp()const { return m_hp; }

		//MP���擾����
		const int& GetMp()const { return m_mp; }

		//�U���͂��擾����
		const int& GetAtack()const { return m_atack; }

		//�h��͂��擾����
		const int& GetDefence()const { return m_defence; }

		//�X�y�V�����|�C���g���擾����
		const int& GetSpecial()const { return m_special; }

		//���̃��x���܂ł̌o���l���擾����
		const int& GetNextLevelPoint()const { return m_next; }

//----------------------------------------�Q�b�^�[-----------------------------------------------------//

	public:
		//�f�[�^���A�j���[�V�����p�ɕϊ�����
		static const std::vector<StatusImporter> Cast(const std::vector<std::vector<std::string>>& Input);
	};
	
}
