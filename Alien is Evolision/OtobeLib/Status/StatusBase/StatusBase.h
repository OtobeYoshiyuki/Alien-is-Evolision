#pragma once
#include "OtobeLib/Status/Interface/IStatusBase.h"
#include "OtobeLib/Status/StatusImporter/StatusImporter.h"

//�X�e�[�^�X�̊��N���X
namespace OtobeLib
{
	//�^���������̂ŏȗ�
	using StatusList = std::map<int,std::unique_ptr<StatusImporter>>;
	using StatusMap = std::map<std::string, StatusList>;

	class StatusBase :public IStatusBase
	{
	private:
		//HP
		int m_Hp;

		//MP
		int m_Mp;

		//�U����
		int m_Atack;

		//�h���
		int m_Defence;

		//�X�y�V�����|�C���g
		int m_Special;

		//���݂̃��x��
		int m_Level;

		//�o���l
		int m_LevelPoint;

		//�o���l���v�Z����ۂ̕␳�l
		int m_compensation;

		//�ő�HP
		int m_MaxHP;

		//�ő�MP
		int m_MaxMP;

		//�ő�U����
		int m_MaxAtack;

		//�ő�h���
		int m_MaxDefence;

		//�ő�X�y�V�����|�C���g		
		int m_MaxSpecial;
		
		//�������E
		int m_MaxLevel;

		//�o���l�̍ő�
		int m_MaxNextLevelPoint;

		//�A�j���[�V�����̃O���[�v��ݒ肷��
		static StatusMap s_statusGrop;
	public:
		//�R���X�g���N�^
		StatusBase();

		//�f�X�g���N�^(�f�t�H���g)
		~StatusBase() = default;

		//-----------------------�Z�b�^�[----------------------------------------------------------------//

		//�X�e�[�^�X��ݒ肷��
		virtual void SetStatus(const int& hp, const int& mp, const int& atack, 
			const int& defence,const int& special,const int& level, const int& next)override;

		//�X�e�[�^�X�̍ő�l��ݒ肷��
		virtual void SetMaxStatus(const int& maxhp, const int& maxmp, const int& maxatack,
			const int& maxdefence, const int& maxspecial, const int& maxlevel,const int& next)override;

		//HP��ݒ肷��
		virtual void SetHP(const int& hp)override { m_Hp = hp; }

		//MP��ݒ肷��
		virtual void SetMP(const int& mp)override { m_Mp = mp; }

		//�U���͂�ݒ肷��
		virtual void SetAtack(const int& atack)override { m_Atack = atack; }

		//�h��͂�ݒ肷��
		virtual void SetDefence(const int& defence)override { m_Defence = defence; }

		//�X�y�V�����|�C���g��ݒ肷��
		virtual void SetSpecial(const int& special)override { m_Special = special; }

		//���x����ݒ肷��
		virtual void SetLevel(const int& level) override { m_Level = level; };

		//���̃��x���܂ł̌o���l��ݒ肷��
		virtual void SetNextLevelPoint(const int& point)override { m_LevelPoint = point; };

		//�o���l���v�Z����ۂ̕␳�l��ݒ肷��
		virtual void SetCompensation(const int& com)override { m_compensation = com; };

		//HP�̍ő�l��ݒ肷��
		virtual void SetMaxHP(const int& maxhp)override { m_MaxHP = maxhp; };

		//MP�̍ő�l��ݒ肷��
		virtual void SetMaxMP(const int& maxmp)override { m_MaxMP = maxmp; };

		//�U���͂̍ő�l��ݒ肷��
		virtual void SetMaxAtack(const int& maxatack)override { m_MaxAtack = maxatack; };

		//�h��͂̍ő�l��ݒ肷��
		virtual void SetMaxDefence(const int& maxdefence)override { m_MaxDefence = maxdefence; };

		//�X�y�V�����|�C���g�̍ő�l��ݒ肷��
		virtual void SetMaxSpecialPoint(const int& maxspecial)override { m_MaxSpecial = maxspecial; };

		//�������E��ݒ肷��
		virtual void SetMaxLevel(const int& maxlevel)override { m_MaxLevel = maxlevel; };

		//�o���l�̍ő�l��ݒ肷��
		virtual void SetMaxNextLevelPoint(const int& maxnext)override { m_MaxNextLevelPoint = maxnext; };

		//-----------------------�Z�b�^�[----------------------------------------------------------------//


		//-----------------------�Q�b�^�[----------------------------------------------------------------//

		//HP���擾����
		virtual const int& GetHP()const override { return m_Hp; }

		//MP���擾����
		virtual const int& GetMP()const override { return m_Mp; }

		//�U���͂��擾����
		virtual const int& GetAtack()const override { return m_Atack; }

		//�h��͂��擾����
		virtual const int& GetDefence()const override { return m_Defence; }

		//�X�y�V�����|�C���g���擾����
		virtual const int& GetSpecial()const override { return m_Special; }

		//���x����ݒ肷��
		virtual const int& GetLevel()const override { return m_Level; };

		//���̃��x���܂ł̌o���l��ݒ肷��
		virtual const int& GetNextLevelPoint()const override { return m_LevelPoint; };

		//�o���l���v�Z����ۂ̕␳�l��ݒ肷��
		virtual const int& GetCompensation()const override { return m_compensation; };

		//HP�̍ő�l��ݒ肷��
		virtual const int& GetMaxHP()const override { return m_MaxHP; };

		//MP�̍ő�l��ݒ肷��
		virtual const int& GetMaxMP()const override { return m_MaxMP; };

		//�U���͂̍ő�l��ݒ肷��
		virtual const int& GetMaxAtack()const override { return m_MaxAtack; };

		//�h��͂̍ő�l��ݒ肷��
		virtual const int& GetMaxDefence()const override { return m_MaxDefence; };

		//�X�y�V�����|�C���g�̍ő�l��ݒ肷��
		virtual const int& GetMaxSpecialPoint()const override { return m_MaxSpecial; };

		//�������E��ݒ肷��
		virtual const int& GetMaxLevel()const override { return m_MaxLevel; };

		//�o���l�̍ő�l��ݒ肷��
		virtual const int& GetMaxNextLevelPoint()const override { return m_MaxNextLevelPoint; };

		//���g�̃C���X�^���X���擾����
		virtual IStatusBase* GetStatus()override { return this; };

		//-----------------------�Q�b�^�[----------------------------------------------------------------//

		public:
			//�X�e�[�^�X�̏���ǉ�����
			static void Add(const std::vector<StatusImporter>& Importer)
			{
				for (const StatusImporter& importer : Importer)
				{
					//�O���[�v�����擾����
					std::string grop = importer.GetGrop();

					//�L�����̃��x�����擾����
					int level = importer.GetLevel();

					//�C���X�^���X�𐶐�����
					s_statusGrop[grop][level] = std::make_unique<StatusImporter>(importer);
				}
			}

			//�A�j���[�V�����R���e�i���擾����
			static const StatusMap& GetStatusMap() { return s_statusGrop; }

			//�R���e�i�̒��̃��X�g���擾����
			static const StatusList& GetStatusList(const std::string& grop)
			{
				return s_statusGrop[grop];
			}

			//�w�肵�����x���̃X�e�[�^�X���擾����
			static StatusImporter* FindStatus(const std::string& grop, const int& level)
			{
				return s_statusGrop[grop][level].get();
			}
	};
}

