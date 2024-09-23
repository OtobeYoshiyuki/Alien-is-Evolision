#pragma once

namespace OtobeLib
{
	class Score
	{
	private:
		//�X�R�A
		float		m_score;

		//�X�R�A�̒��~
		float		m_saving;

		//�X�R�A��1�b������̉��Z��
		float	m_frameSave;

		//�X�R�A�̉��Z��
		int		m_add;

		//���̒�~�𖳎�����t���O
		bool	m_soundCheck;

	public:
		//�R���X�g���N�^
		Score();

		//�f�X�g���N�^
		virtual ~Score() = default;

		//������
		void Initialize();

		//�X�V
		void Update();

		//�X�R�A��ݒ肷��
		void SetScore(const float& score) { m_score = score; }

		//�X�R�A�̒��~��ݒ肷��
		void SetSaving(const float& save) { m_saving = save; }

		//�X�R�A�����Z���Ă���
		void AddScore(const float& score) { m_saving += score; }

		//�b�P�ʂ̉����x��ݒ肷��
		void SetFrameSave(const float& score, const float& frame) { m_frameSave = score / frame; }

		//�X�R�A�̉��Z�ʂ�ݒ肷��
		void SetAdd(const int& add) { m_add = add; }

		//���̒�~�𖳎�����t���O��ݒ肷��
		void SetSoundCheck(const bool& flag) { m_soundCheck = flag; }

		//�X�R�A���擾����
		const float& GetScore()const { return m_score; }

		//�X�R�A�̒��~���擾����
		const float& GetSaving()const { return m_saving; }
	};
}

