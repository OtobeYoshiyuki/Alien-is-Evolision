#pragma once
#include <string>
#include "OtobeLib/FrameWork/Score/Score.h"
#include "OtobeLib/FrameWork/TimerJudge/TimerJudge.h"
#include "OtobeLib/Texture/Blinking/Blinking.h"
namespace OtobeLib
{
	class Ranking
	{
	private:
		//���x��
		std::string m_level;

		//�X�R�A
		std::unique_ptr<Score> m_score;

		//�^�C�}�[
		std::unique_ptr<TimerJudge> m_timer;

		//�_��
		std::unique_ptr<Blinking>	m_blink;

		//�F
		DirectX::XMVECTORF32		m_color;

	public:
		//�R���X�g���N�^
		Ranking();

		//�f�X�g���N�^
		~Ranking() = default;

		//�X�V����
		void Update();

		//���x����ݒ�
		void SetLevel(const std::string& level) { m_level = level; }

		//�F��ݒ�
		void SetColor(const DirectX::XMVECTORF32& color) { m_color = color; }

		//���x�����擾
		const std::string& GetLevel()const { return m_level; }

		//�X�R�A���擾
		Score* GetScore() { return m_score.get(); }

		//�^�C�}�[���擾
		TimerJudge* GetTimer() { return m_timer.get(); }

		//�_�ł��擾
		Blinking* GetBlinking() { return m_blink.get(); }

		//�F���擾
		const DirectX::XMVECTORF32& GetColor()const { return m_color; }
	};
}

