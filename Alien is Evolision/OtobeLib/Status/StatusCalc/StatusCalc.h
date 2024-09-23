#pragma once
namespace OtobeLib
{
	//�_���[�W�v�Z������N���X
	class StatusCalc
	{
	public:
		//�_���[�W�l���v�Z����
		static void CalcDamage(int& nowhp, const int& atack, const int& defence,
			const int& texLength, const int& maxhp, LONG& element,float correction = 1.0f);

		//�o���l�����߂�
		static void CalcExp(const int& level, const float& strength,
			const int& rand_max,const int& texLength,const int& maxep,int& exp,int& texexp,float& ratio);

		//�񕜒l���v�Z����
		static void CalcHeal(int& status,int& heal, const int& texLength, 
			const int& maxLength, LONG& element, float corrrection = 1.0f);
	};
}

