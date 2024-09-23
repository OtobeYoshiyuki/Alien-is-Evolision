#include "pch.h"
#include "MiniMap.h"
#include "OtobeLib/FrameWork/Graphics/Graphics.h"
#include "GameContext.h"
#include "OtobeLib/StackScene/Scene/Play/PlayScene.h"
#include "GameObject/Stage/Stage.h"
#include "OtobeLib/FrameWork/ResourceManager/ResourceManager.h"
#include "GameObject/Player/CoroKun.h"
#include "GameObject/Enemy/Super/SuperEnemy.h"
#include "GameObject/Enemy/Hyper/HyperEnemy.h"
#include "Utils.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace OtobeLib;

MiniMap::MiniMap():m_world(Matrix::Identity)
{
}

void MiniMap::Initialize(std::vector<std::vector<std::string>>& before, std::vector<std::vector<int>>& after)
{
	//ResourceManager���擾����
	ResourceManager* resourceManager = ResourceManager::GetInstance();

	//�g���e�N�X�`�����擾����
	ITextureBase* useTexture = resourceManager->GetTexture("WhiteBack");

	//TextureManager���擾����
	TextureManager* texManager = GameContext::Get<PlayScene>()->GetMiniMapManager().get();

	//�X�e�[�W�����擾����
	Stage* stage = GameContext::Get<Stage>();

	//�����l��ݒ肷��
	Vector3 firstPos = Vector3(820, 60, 0);
	RECT firstRect = RECT{ 0,0,4,4};

	m_miniMapTex.resize(stage->GetMap().size());

	//�v���C���[�̃C���X�^���X���擾����
	CoroKun* player = GameContext::Get<CoroKun>();

	for (int i = 0; i < (int)after.size(); i++)
	{
		for (int j = 0; j < (int)after[i].size(); j++)
		{
			//�f�[�^����̏ꍇ�́A�������΂�
			if (after[i][j] == (int)MapTip::MAP_TYPE::NONE)continue;

			//�~�j�}�b�v�̃e�N�X�`���𐶐�����
			std::unique_ptr<ITextureBase> pmap = std::make_unique<TextureBase>(useTexture);

			//���W��ݒ肷��
			pmap->SetPosition(Vector3(10 * j, 10 * i, 0) + firstPos);

			//�F��ݒ肷��
			Color color = Colors::White;

			//�؂���ʒu��ݒ肷��
			RECT rect = firstRect;

			//�����𕪊򂳂���
			switch (MapTip::MAP_TYPE(after[i][j]))
			{
				//�����I�u�W�F�N�g��ǉ�����
			case MapTip::MAP_TYPE::WALL:
				//�F��ύX����
				color = Colors::Black;
				break;
			case MapTip::MAP_TYPE::BABUL:
				//�F��ύX����
				color = Colors::Cyan;
				break;
			case MapTip::MAP_TYPE::REBOUND:
				//�F��ύX����
				color = Colors::Coral;
				break;
			case MapTip::MAP_TYPE::GOAL:
				//�F��ύX����
				color = Colors::Lime;
				break;
			case MapTip::MAP_TYPE::PLAYER:
				m_moveTex.push_back(pmap.get());
				//�v���C���[�̐F��ύX����
				color = Colors::Yellow;
				//�؂���ʒu��ύX����
				rect = RECT{ 0,0,10,10 };
				//���O��ύX����
				pmap->SetName("SickChar");
				break;
			case MapTip::MAP_TYPE::ENEMYN:
				m_moveTex.push_back(pmap.get());
				//�F��ύX����
				color = Colors::Green;
				//�؂���ʒu��ύX����
				rect = RECT{ 0,0,8,8 };
				//���O��ύX����
				pmap->SetName("NormalEnemy");
				break;
			case MapTip::MAP_TYPE::ENEMYS:
				m_moveTex.push_back(pmap.get());
				//�F��ύX����
				color = Colors::Orange;
				//�؂���ʒu��ύX����
				rect = RECT{ 0,0,8,8 };
				//���O��ύX����
				pmap->SetName("SuperEnemy");
				break;
			case MapTip::MAP_TYPE::ENEMYH:
				m_moveTex.push_back(pmap.get());
				//�F��ύX����
				color = Colors::Blue;
				//�؂���ʒu��ύX����
				rect = RECT{ 0,0,8,8 };
				//���O��ύX����
				pmap->SetName("HyperEnemy");
				break;
			case MapTip::MAP_TYPE::LEFTJUMPPANEL:
			case MapTip::MAP_TYPE::RIGHTJUMPPANEL:
			case MapTip::MAP_TYPE::LEFTPANEL:
			case MapTip::MAP_TYPE::RIGHTPANEL:
				//�F��ύX����
				color = Colors::SlateBlue;
				break;
			}

			//����������̂Ƃ��čl���Ă���̂ŁASwitch�ł͐��䂵�Ȃ�
			if ((MapTip::MAP_TYPE)after[i][j] >= MapTip::MAP_TYPE::HOLE)
			{
				//�N���X�C�b�`�̐ݒ������(�N���X�C�b�`�͕K������)
				if (after[i][j] % 2 == 0)
				{
					//�F��ύX����
					color = Colors::Red;
				}
				//���Ƃ����̐ݒ������(���Ƃ����͕K���)
				else
				{
					//�F��ύX����
					color = Colors::DimGray;
				}
			}
			
			pmap->SetColor(color);
			pmap->SetRect(rect);

			//�z��ɐ��|�C���^��o�^����
			m_miniMapTex[i].push_back(pmap.get());
			texManager->Add(std::move(pmap));
		}
	}
}

void MiniMap::Update()
{
	//CameraManager���擾����
	CameraManager* cameraManager = GameContext::Get<CameraManager>();

	//���݉ғ����Ă���J�������擾����
	CameraBase* nowCamera = cameraManager->GetNowCamera();

	for (int i = 0;i < (int)m_moveTex.size();i++)
	{
		//�Q�[���f�[�^���擾����
		ITextureBase* gameData = m_gameTex[i];

		//�����~�j�}�b�v�̃f�[�^���擾����
		ITextureBase* moveMap = m_moveTex[i];

		//���W���擾����
		Vector3 pos = gameData->GetPosition();

		//�Q�[����ʂƃ~�j�}�b�v�̋���X�����߂�
		float x = pos.x - 225.0f;
		x *= 0.25f;

		//�摜�̊g��{�����擾����
		Vector3 scale = gameData->GetScale();

		if (scale.x <= 1.0f)scale.x = 1.0f;
		if (scale.y <= 1.0f)scale.y = 1.0f;
		if (scale.z <= 1.0f)scale.y = 1.0f;

		//�摜�̊g��{����ݒ肷��
		moveMap->SetScale(scale);

		//���W�̕␳���s��
		moveMap->SetPosition(Vector3(820 + x,pos.y * 0.27f,0.0f));
	}

	//���[���h�s���ݒ肷��
	//�J�����̍s������ƂɁA�Q�[����ʂƃ~�j�}�b�v�̔䗦��������
	Vector3 trans = nowCamera->GetMatrix().Translation();
	trans *= 0.25f;

	m_world = Matrix::CreateScale(Vector3(1.0f, 0.7f, 1.0f));

	//���E�l���̓X�N���[�����Ȃ�
	if (trans.x > -530.0f)
	{
		m_world *= Matrix::CreateTranslation(Vector3(trans.x - 15.0f, 12.5f, 0.0f));
	}
	else
	{
		m_world *= Matrix::CreateTranslation(Vector3(-530.0f, 12.5f, 0.0f));
	}

	//RECT�A�j���[�V�������~�j�}�b�v�ɓK������
	for (int i = 0; i < (int)m_miniAnimeTex.size(); i++)
	{
		//�~�j�}�b�v�̃e�N�X�`�����擾����
		ITextureBase* miniMapTex = m_miniAnimeTex[i];

		//�Q�[���p�̃e�N�X�`�����擾����
		ITextureBase* gameTex = m_gameAnimeTex[i];

		//�Q�[���p�̌��_���擾����
		Vector2 origin = gameTex->GetOrigin();

		//���_��␳����
		origin *= 0.25f;

		//���_��␳����
		miniMapTex->SetOrigin(origin);

		//���W�����ɂ��炷
		miniMapTex->SetPosition(miniMapTex->GetPosition() + Vector3(0.0f, 10.0f, 0.0f));

		//SpriteEffect���Q�[���f�[�^�ƘA��������
		miniMapTex->SetSpriteEffect(gameTex->GetSpriteEffect());

		//�Q�[���p��RECT���擾����
		RECT gameRect = gameTex->GetRect();

		//�~�j�}�b�v�p��RECT���擾����
		RECT miniRect = miniMapTex->GetRect();

		//�Q�[���p��RECT���~�j�}�b�v�ƃQ�[����ʂ̔䗦�ɒ���
		gameRect.right *= 0.25f;

		//�␳�����l���~�j�}�b�v��RECT�ɓn��
		miniRect.right = gameRect.right;

		//RECT���Q�[���f�[�^�ƘA��������
		miniMapTex->SetRect(miniRect);
	}

	//�X�e�[�W�̃C���X�^���X���擾
	Stage* stage = GameContext::Get<Stage>();

	//�}�b�v�`�b�v�̔z����擾����
	std::vector<std::vector<ITextureBase*>> mapArray = stage->GetMap();

	//�X�e�[�g�ɕύX���������ꍇ�́A�ύX����
	for (int i = 0; i < (int)m_miniMapTex.size(); i++)
	{
		for (int j = 0; j < (int)m_miniMapTex[i].size(); j++)
		{
			//�~�j�}�b�v�̃e�N�X�`�����擾����
			ITextureBase* miniMap = m_miniMapTex[i][j];

			//�}�b�v�`�b�v�̃e�N�X�`�����擾����
			ITextureBase* maptip = mapArray[i][j];

			//�X�e�[�g�́A�Q�[���f�[�^�ɍ��킹��
			miniMap->SetState(maptip->GetState());
		}
	}
}

void MiniMap::Render()
{
}

void MiniMap::ArraySetUp()
{
	//ResourceManager���擾����
	ResourceManager* resourceManager = ResourceManager::GetInstance();

	//�g���e�N�X�`�����擾����
	ITextureBase* useTexture = resourceManager->GetTexture("WhiteBack");

	//�X�e�[�W�̃C���X�^���X���擾����
	Stage* stage = GameContext::Get<Stage>();

	//�}�b�v�`�b�v�̔z����擾����
	std::vector<std::vector<ITextureBase*>>mapArray = stage->GetMap();

	//�F����ݒ肷��i�f�t�H���g�j
	XMVECTORF32 color = Colors::White;

	//�؂���ʒu��ݒ肷��(�f�t�H���g)
	RECT rect = RECT{ 0,0,2,2, };

	//�z��̏���ݒ肷��
	for (std::vector<ITextureBase*>mapVec:mapArray)
	{
		for (ITextureBase* map:mapVec)
		{
			//�C���X�^���X�𐶐�
			std::unique_ptr<ITextureBase>pchild = std::make_unique<TextureBase>(useTexture);

			//�q�����ǂ������ׂ�
			if (this->CheckChild(map->GetName()))
			{
				//�~�j�}�b�v�p�̔z��ɒǉ�����
				if (map->GetName() == "Hypersyabondama")
				{
					//�F����ݒ肷��
					color = Colors::DeepSkyBlue;
				}
				else {
					//�F����ݒ肷��
					color = Colors::Aqua;
				}

				//�~�j�}�b�v�̃C���X�^���X�𐶐�����
				this->CreateMiniMap(pchild, m_miniMapTex.size() - 1, color, map->GetName(), rect);
			}

			//�v���C���[�֘A�̂��̂��ǂ���
			if (this->CheckPlayers(map->GetName()))
			{
				//�Փ˂̏ꍇ
				if (map->GetName() == "�Փ�")
				{
					//�F����ݒ肷��
					color = Colors::DeepPink;

					//�؂���ʒu��ݒ肷��
					rect = RECT{ 0,0,10,10 };
				}

				//�E�C���X�r�[���̏ꍇ
				if (map->GetName() == "�E�C���X�r�[��")
				{
					//�F����ݒ肷��
					color = Colors::MediumVioletRed;

					//�؂���ʒu��ݒ肷��
					rect = RECT{ 0,0,5,5 };

					//RECT�A�j���[�V�����p�̃~�j�}�b�v�̔z��ɒǉ�����
					m_miniAnimeTex.push_back(pchild.get());

					//RECT�A�j���[�V�����p�̃Q�[���f�[�^�̔z��ɒǉ�����
					m_gameAnimeTex.push_back(map);
				}

				//�j�̏ꍇ
				if (map->GetName() == "shortNeedle" || map->GetName() == "longNeedle")
				{
					//�F����ݒ肷��
					color = Colors::Black;

					//�؂���ʒu��ݒ肷��
					rect = RECT{ 0,0,2,5 };
				}

				//�񕜃A�C�e���̏ꍇ
				if (map->GetName() == "��" || map->GetName() == "��1" || 
					map->GetName() == "��2" || map->GetName() == "��3")
				{
					//�F����ݒ肷��
					color = Colors::Lime;

					//�؂���ʒu��ݒ肷��
					rect = RECT{ 0,0,4,4 };
				}

				//�~�j�}�b�v�̃C���X�^���X�𐶐�����
				this->CreateMiniMap(pchild, m_miniMapTex.size() - 1, color, map->GetName(), rect);
			}

			//�������̂��ǂ������ׂ�
			if (this->CheckArray(map->GetName()))
			{
				//�X�e�[�W�̃Q�[���f�[�^�p�̔z��ɒǉ�����
				m_gameTex.push_back(map);
			}
		}
	}
}

const bool MiniMap::CheckArray(const std::string& target)
{
	//�`�F�b�N���郊�X�g�̒��g
	const std::vector<std::string> ARRAY_LIST =
	{ "SickChar","NormalEnemy","SuperEnemy","HyperEnemy","Hypersyabondama","Supersyabondama",
		"�Փ�","�E�C���X�r�[��","���S�̗��q","����̗��q","shortNeedle","longNeedle","��","��1","��2","��3"};

	for (const std::string& name : ARRAY_LIST)
	{
		//�ړI�������������ꍇ�́Atrue��Ԃ�
		if (target == name)return true;
	}
	return false;
}

const bool MiniMap::CheckChild(const std::string& child)
{
	//�`�F�b�N���郊�X�g�̒��g
	const std::vector<std::string> ARRAY_LIST =
	{ "Hypersyabondama","Supersyabondama"};

	for (const std::string& name : ARRAY_LIST)
	{
		//�ړI�������������ꍇ�́Atrue��Ԃ�
		if (child == name)return true;
	}

	return false;
}

const bool MiniMap::CheckPlayers(const std::string& players)
{
	//�`�F�b�N���郊�X�g�̒��g
	const std::vector<std::string> ARRAY_LIST =
	{ "�Փ�","�E�C���X�r�[��","���S�̗��q","����̗��q","shortNeedle","longNeedle","��","��1","��2","��3" };

	for (const std::string& name : ARRAY_LIST)
	{
		//�ړI�������������ꍇ�́Atrue��Ԃ�
		if (players == name)return true;
	}

	return false;
}

void MiniMap::CreateMiniMap(std::unique_ptr<OtobeLib::ITextureBase>& pchild,const int& col,
	const XMVECTORF32& color, const std::string& name, const RECT& rect)
{
	//TextureManager���擾����
	TextureManager* texManager = GameContext::Get<PlayScene>()->GetMiniMapManager().get();

	//�`��͐؂�
	pchild->SetState(ITextureBase::STATE::NONE);

	//�摜�̕���ݒ肷��
	pchild->SetRect(rect);

	//�F��ύX����
	pchild->SetColor(color);

	//���O��ύX����
	pchild->SetName(name);

	//�|�C���^��o�^����
	m_miniMapTex[col].push_back(pchild.get());

	//�|�C���^��o�^����
	m_moveTex.push_back(pchild.get());

	//TextureManager�ɒǉ�����
	texManager->Add(std::move(pchild));
}
