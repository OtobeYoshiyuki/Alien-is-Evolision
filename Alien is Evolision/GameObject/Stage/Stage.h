#pragma once
#include "GameObject/Stage/Maptip/MapTip.h"
#include "OtobeLib/Texture/Manager/TextureManager.h"
#include "GameObject/Player/CoroKun.h"
#include <string>
#include <vector>

class Floor; class Wall; class Babul; class HoleSwitch; class HoleFloor; class Rebound;
class NormalEnemy; class SuperEnemy; class HyperEnemy; class CheckPoint;
class Stage
{
public:
	//�ŏ��ɗV�ԃX�e�[�W
	static const int FIRST_STAGE = 1;
	//�p�ӂ���Ă���X�e�[�W�̐�
	static const int MAX_STAGE = 1;
	//1�t���[���ɂ����X�e�[�W���ړ������邩
	static const float SCROLL_SPEED;
private:
	//----------------------���C���X�e�[�W--------------------//
	//�ϊ��O�̃f�[�^
	std::vector<std::vector<std::string>> m_mainBeforeData;
	//�ϊ���̃f�[�^
	std::vector<std::vector<int>>m_mainAfterData;
	//----------------------���C���X�e�[�W--------------------//

	//----------------------�T�u�X�e�[�W----------------------//
	//�ϊ��O�̃f�[�^
	std::vector<std::vector<std::string>> m_subBeforeData;
	//�ϊ���̃f�[�^
	std::vector<std::vector<int>>m_subAfterData;
	//----------------------�T�u�X�e�[�W----------------------//

	//�}�b�v�`�b�v�̔z��(�S��)
	std::vector<std::vector<OtobeLib::ITextureBase*>>m_MapTip;

	//Floor�̔z��
	std::vector<Floor*>m_Floors;

	//Wall�̔z��
	std::vector<Wall*>m_Walls;

	//Babul�̔z��
	std::vector<Babul*>m_Babuls;

	//Rebound�̔z��
	std::vector<Rebound*>m_Rebounds;

	//HoleSwitch�̔z��
	std::vector<HoleSwitch*>m_Switches;

	//HoleFloor�̔z��
	std::vector<HoleFloor*>m_HoleFloors;

	//NormalEnemy�̔z��
	std::vector<NormalEnemy*>m_NormalEnemies;

	//SuperEnemy�̔z��
	std::vector<SuperEnemy*>m_SuperEnemies;

	//HyperEnemy�̔z��
	std::vector<HyperEnemy*>m_HyperEnemies;

	//CheckPoint�̔z��
	std::vector<CheckPoint*>m_CheckPoints;

	//�v���C���[�N���X
	CoroKun* m_CoroKun;

	//���[�h����t�@�C���̘A��
	int								m_StageFile;
public:
	//�R���X�g���N�^
	Stage();

	//�f�X�g���N�^
	~Stage(){}
	//�X�e�[�W�̏�����
	void Initialize(const std::wstring& filenamemain,const std::wstring& filenamesub);

	//�X�e�[�W�̍X�V
	void Update();

	//���[�h����t�@�C���̘A�Ԃ��擾����
	const int& GetStageFile()const { return m_StageFile; }

	//Floor�̔z����擾
	std::vector<Floor*>& GetFloors() { return m_Floors; }

	//Wall�̔z����擾
	std::vector<Wall*>& GetWalls() { return m_Walls; }

	//Babul�̔z����擾
	std::vector<Babul*>& GetBabuls() { return m_Babuls; }

	//Rebound�̔z����擾
	std::vector<Rebound*>& GetRebounds() { return m_Rebounds; }
	
	//HoleSwitch�̔z����擾
	std::vector<HoleSwitch*>& GetHoleSwitches() { return m_Switches; }

	//HoleFloor�̔z����擾
	std::vector<HoleFloor*>& GetHoleFloors() { return m_HoleFloors; }

	//NormalEnemy�̔z����擾
	std::vector<NormalEnemy*>& GetNormalEnemies() { return m_NormalEnemies; }

	//SuperEnemy�̔z����擾
	std::vector<SuperEnemy*>& GetSuperEnemies() { return m_SuperEnemies; }

	//HyperEnemy�̔z����擾
	std::vector<HyperEnemy*>& GetHyperEnemies() { return m_HyperEnemies; }

	//CheckPoint�̔z����擾
	std::vector<CheckPoint*>& GetCheckPoints() { return m_CheckPoints; }

	//�}�b�v�S�̂̔z����擾����
	std::vector<std::vector<OtobeLib::ITextureBase*>>& GetMap() { return m_MapTip; }

	//------------------------------------------���C���X�e�[�W------------------------------------//

	//�ϊ��O�̃f�[�^���擾����
	std::vector<std::vector<std::string>>& GetMainBeforeData() { return m_mainBeforeData; }

	//�ϊ���̃f�[�^���擾����
	std::vector<std::vector<int>>& GetMainAfterData() { return m_mainAfterData; }

	//------------------------------------------���C���X�e�[�W------------------------------------//

	//------------------------------------------�T�u�X�e�[�W------------------------------------//

	//�ϊ��O�̃f�[�^���擾����
	std::vector<std::vector<std::string>>& GetSubBeforeData() { return m_subBeforeData; }

	//�ϊ���̃f�[�^���擾����
	std::vector<std::vector<int>>& GetSubAfterData() { return m_subAfterData; }

	//------------------------------------------�T�u�X�e�[�W------------------------------------//

private:
	//String����int��Convert����
	const std::vector<std::vector<int>> FileDataConvert(const std::vector<std::vector<std::string>>& Input);
	//�}�b�v�`�b�v�𐶐�����
	void CreateMapTip(const std::vector<std::vector<int>>& Input);
	//�������ƂɁA���X�g�Ƀ|�C���^��o�^����
	void AddVector();

public:
	//�^���̃}�b�v�`�b�v�𒲂ׂ�
	OtobeLib::ITextureBase* DirectlyBlowMap(OtobeLib::ITextureBase* target,MapTip* mapType);

	//�������̂̎��s�֐���ݒ肷��
	void MoveMapActive(const bool& active);
};
