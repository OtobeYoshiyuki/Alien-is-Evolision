#pragma once
#include "GameObject/Stage/Maptip/MapTip.h"
#include <vector>
#include <string>
#include <map>


class HoleFloor :public MapTip
{
	//�^���������̂ŏȗ�
	using HoleList = std::list<HoleFloor*>;
	using HoleMap = std::map<int, HoleList>;
private:
	//��������J�E���g
	 float m_ExprosionCount;
	//�J�E���g���X�V����t���O
	 bool m_ExprosionFlag;
	 //�V���A���i���o�[
	 int	m_SelealNumber;
	 //�ʂ̖��O
	 std::string m_SelealName;
	 //���Ƃ������O���[�v�Ƃ��ĊǗ�����
	 static HoleMap s_holeGrop;
public:
	//�R���X�g���N�^
	HoleFloor(OtobeLib::ITextureBase* texture);

	//�f�X�g���N�^
	~HoleFloor()override {};

	//������
	void Initialize()override;

	//�X�V
	void Update()override;

	//�`��
	void Render()override;

	//�I��
	void Finalize()override;

	//���̃I�u�W�F�N�g�Ɠ����������ɌĂ΂��֐�
	void OnCollisionEnter(ITextureBase* obj)override;

	//���̃I�u�W�F�N�g�Ɠ������Ă��Ȃ��Ƃ��ɌĂ΂��֐�
	void NonCollisionEnter(ITextureBase* obj)override;

	//--------------------------------------------------------------//

public:
	//��������^�C�~���O��ݒ肷��
	void SetExproTiming(const float& timing) { m_ExprosionCount = timing; }

	//�t�@�C������ǂݍ��񂾃f�[�^��convert����
	static const std::vector<float> FileDataConvert(const std::vector<std::vector<std::string>>& Input);

	//�V���A���i���o�[��ݒ肷��
	void SetSerealNumber(const int& number) { m_SelealNumber = number; }

	//�V���A���i���o�[���擾����
	const int& GetSerealNumber()const { return m_SelealNumber; }

	//�w�肵���O���[�v���擾����
	static HoleList& GetHoleGrop(const int& number) { return s_holeGrop[number]; }
	
	//�O���[�v����������
	static void ResetHoleGrop() { s_holeGrop.clear(); }
private:
	//��������^�C�~���O���X�V����
	void UpdateExprosion();
};

