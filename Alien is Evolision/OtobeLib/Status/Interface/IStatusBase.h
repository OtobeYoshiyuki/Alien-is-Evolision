#pragma once

//�X�e�[�^�X�̃C���^�[�t�F�[�X�N���X

namespace OtobeLib
{
	class IStatusBase
	{
	public:
		//�f�X�g���N�^
		virtual ~IStatusBase() {}

		//-----------------------�Z�b�^�[----------------------------------------------------------------//

		//�X�e�[�^�X��ݒ肷��
		virtual void SetStatus(const int& hp, const int& mp, const int& atack, 
			const int& defence,const int& special, const int& level,const int& next) = 0;

		//�X�e�[�^�X�̍ő�l��ݒ肷��
		virtual void SetMaxStatus(const int& maxhp, const int& maxmp, const int& maxatack,
			const int& maxdefence, const int& maxspecial, const int& maxlevel,const int& maxnext) = 0;

		//HP��ݒ肷��
		virtual void SetHP(const int& hp) = 0;

		//MP��ݒ肷��
		virtual void SetMP(const int& mp) = 0;

		//�U���͂�ݒ肷��
		virtual void SetAtack(const int& atack) = 0;

		//�h��͂�ݒ肷��
		virtual void SetDefence(const int& defence) = 0;

		//�X�y�V�����|�C���g��ݒ肷��
		virtual void SetSpecial(const int& special) = 0;

		//���x����ݒ肷��
		virtual void SetLevel(const int& level) = 0;

		//���̃��x���܂ł̌o���l��ݒ肷��
		virtual void SetNextLevelPoint(const int& point) = 0;

		//�o���l���v�Z����ۂ̕␳�l��ݒ肷��
		virtual void SetCompensation(const int& com) = 0;

		//HP�̍ő�l��ݒ肷��
		virtual void SetMaxHP(const int& maxhp) = 0;

		//MP�̍ő�l��ݒ肷��
		virtual void SetMaxMP(const int& maxmp) = 0;

		//�U���͂̍ő�l��ݒ肷��
		virtual void SetMaxAtack(const int& maxatack) = 0;

		//�h��͂̍ő�l��ݒ肷��
		virtual void SetMaxDefence(const int& maxdefence) = 0;
		
		//�X�y�V�����|�C���g�̍ő�l��ݒ肷��
		virtual void SetMaxSpecialPoint(const int& maxspecial) = 0;

		//�������E��ݒ肷��
		virtual void SetMaxLevel(const int& maxlevel) = 0;

		//�o���l�̍ő�l��ݒ肷��
		virtual void SetMaxNextLevelPoint(const int& maxnext) = 0;


		//-----------------------�Z�b�^�[----------------------------------------------------------------//


		//-----------------------�Q�b�^�[----------------------------------------------------------------//

		//HP���擾����
		virtual const int& GetHP()const = 0;

		//MP���擾����
		virtual const int& GetMP()const = 0;

		//�U���͂��擾����
		virtual const int& GetAtack()const = 0;

		//�h��͂��擾����
		virtual const int& GetDefence()const = 0;

		//�X�y�V�����|�C���g���擾����
		virtual const int& GetSpecial()const = 0;

		//���x����ݒ肷��
		virtual const int& GetLevel()const = 0;

		//���̃��x���܂ł̌o���l��ݒ肷��
		virtual const int& GetNextLevelPoint()const = 0;

		//�o���l���v�Z����ۂ̕␳�l��ݒ肷��
		virtual const int& GetCompensation()const = 0;

		//HP�̍ő�l��ݒ肷��
		virtual const int& GetMaxHP()const = 0;

		//MP�̍ő�l��ݒ肷��
		virtual const int& GetMaxMP()const = 0;

		//�U���͂̍ő�l��ݒ肷��
		virtual const int& GetMaxAtack()const = 0;

		//�h��͂̍ő�l��ݒ肷��
		virtual const int& GetMaxDefence()const = 0;

		//�X�y�V�����|�C���g�̍ő�l��ݒ肷��
		virtual const int& GetMaxSpecialPoint()const = 0;

		//�������E��ݒ肷��
		virtual const int& GetMaxLevel()const = 0;

		//�o���l�̍ő�l��ݒ肷��
		virtual const int& GetMaxNextLevelPoint()const = 0;

		//���g�̃C���X�^���X���擾����
		virtual IStatusBase* GetStatus() = 0;

		//-----------------------�Q�b�^�[----------------------------------------------------------------//
	};
}
