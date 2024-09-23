#pragma once
#include "OtobeLib/StackScene/Interface/ISceneBase.h"
#include "OtobeLib/FrameWork/Collision/CollisionManager/CollisionManager.h"
#include "OtobeLib/Texture/Interface/ITextureBase.h"
#include "OtobeLib/Texture/Manager/TextureManager.h"
#include "GameObject/BackGround/BackGround.h"
#include "GameObject/MiniMap/MiniMap.h"
#include "GameObject/Stage/Stage.h"
#include "GameObject/DamageUI/DamageUI.h"
#include "OtobeLib/FrameWork/TimerJudge/TimerJudge.h"
#include "GameObject/TimeLag/TimeLag.h"
#include "Effect/Manager/StarManager.h"
#include "Effect/LevelUp/LevelUp.h"
#include "OtobeLib/Camera/Manager/CameraManager.h"
#include "OtobeLib/FrameWork/Score/Score.h"
#include "GameObject/Item/Manager/HealManager.h"
#include "TemplateState.h"
#include "BACKOUT.h"

namespace OtobeLib
{
	class PlayScene :public ISceneBase,State<BACKOUT>
	{
	public:
		enum class SAVE_DATA :int
		{
			PLAYER_STATUS = 2,//�v���C���[�̃X�e�[�^�X
			HEAL_NAME = 4,//�񕜃A�C�e���̖��O
			HEAL_INFO,//�񕜃A�C�e��
		};

		enum class PLAYER_STATUS :int
		{
			LEVEL,//���x��
			HP,//HP
			MP,//MP
			ATACK,//�U����
			DEFENCE,//�h���
			SPECIAL,//�X�y�V�����|�C���g
			NEXT,//���̃��x���܂ł̌o���l
			MAX,//�z��̑傫��
		};

	private:
		//�����蔻��Ǘ��N���X
		std::unique_ptr<CollisionManager>		m_CollisionManager;

		//�Q�[���p�摜�Ǘ��N���X
		std::unique_ptr<TextureManager>			m_GameManager;

		//UI�p���C���摜�Ǘ��N���X
		std::unique_ptr<TextureManager>			m_MainInfoManager;

		//UI�p�T�u�摜�Ǘ��N���X
		std::unique_ptr<TextureManager>			m_SubInfoManager;

		//�~�j�}�b�v�Ǘ��N���X
		std::unique_ptr<TextureManager>			m_MiniMapManager;

		//�J�����̊Ǘ��N���X
		std::unique_ptr<CameraManager>			m_cameraManager;

		//�w�i�N���X
		BackGround*								m_BG;

		//�~�j�}�b�v�N���X
		std::unique_ptr<MiniMap>				m_MiniMap;

		//�X�e�[�W�N���X
		std::unique_ptr<Stage>					m_Stage;

		//�^�C�}�[�N���X
		std::unique_ptr<TimerJudge>				m_TimerJudge;

		//�^�C�����O�G�t�F�N�g
		std::unique_ptr<TimeLag>				m_LagStart;
		std::unique_ptr<TimeLag>				m_LagGo;
		std::unique_ptr<TimeLag>				m_LagFinish;

		//���x���A�b�v�G�t�F�N�g
		LevelUp*								m_LevelUp;

		//StarEffectManager
		std::unique_ptr<StarManager>			m_StarManager;

		//�Z�[�u�f�[�^
		std::vector<std::vector<std::string>>	m_saveData;

		//�X�R�A
		std::unique_ptr<Score>					m_score;

		//�񕜃A�C�e���̊Ǘ��N���X
		std::unique_ptr<HealManager>			m_healManager;

		//�o�ߎ���
		float									m_easingTime;

	public:
		PlayScene();		//�R���X�g���N�^

		~PlayScene()override {};		//�f�X�g���N�^

		void Initialize()override;		//����������

		void Update()override;		//�X�V����

		void Render()override;		//�`�揈��

		void Finalize()override;	//�I������

		void Create()override;		//����

		//�Q�[���p��TextureManager���擾
		std::unique_ptr<TextureManager>& GetGameManager() { return m_GameManager; }

		//UI�p��TextureManager���擾
		std::unique_ptr<TextureManager>& GetMainInfoManager() { return m_MainInfoManager; }

		//UI�p��TextureManager���擾
		std::unique_ptr<TextureManager>& GetSubInfoManager() { return m_SubInfoManager; }

		//�~�j�}�b�v�p��TextureManager���擾
		std::unique_ptr<TextureManager>& GetMiniMapManager() { return m_MiniMapManager; }

		//LagStart���擾
		std::unique_ptr<TimeLag>& GetLagStart() { return m_LagStart; }

		//LagGo���擾
		std::unique_ptr<TimeLag>& GetLagGo() { return m_LagGo; }

		//LagFinish���擾
		std::unique_ptr<TimeLag>& GetLagFinish() { return m_LagFinish; }

		//LevelUp���擾
		LevelUp* GetLevelUp() { return m_LevelUp; }

		//�X�R�A���擾
		Score* GetScore() { return m_score.get(); }

		//HealManager���擾
		HealManager* GetHealManager() { return m_healManager.get(); }

		//�w�肵���f�[�^��z��ɂ��ĕԂ�(std::vector<>)
		const std::vector<std::string> GetSaveDataArray(const int& data);

	public:
		//���݂̏�Ԃ�ݒ肷��
		void SetNowState(const BACKOUT& now)override { State::SetNowState(now); };

		//���݂̏�Ԃ��擾����
		const BACKOUT& GetNowState()const override { return State::GetNowState(); };

		//�ЂƂO�̏�Ԃ��擾����
		const BACKOUT& GetPreState()const override { return State::GetPreState(); };

	private:
		//Manager�N���X�𐶐�
		void CreateManager();	

		//�w�i�𐶐�
		void CreateBack();		

		//�X�e�[�W�𐶐�
		void CreateStage();		

		//�J�����𐶐�
		void CreateCamera();	

		//�^�C�}�[�𐶐�
		void CreateTimer();		

		//�G�t�F�N�g�𐶐�
		void CreateEffect();		

		//�X�R�A�𐶐�
		void CreateScore();		

		//LagEffect�̍X�V
		void UpdateLag();		
	};
}

