#pragma once
#include "OtobeLib/StackScene/Interface/ISceneBase.h"
#include "OtobeLib/Texture/Interface/ITextureBase.h"
#include "OtobeLib/Texture/Manager/TextureManager.h"
#include "GameObject/Item/Item.h"
#include "GameObject/Arrow/Arrow.h"
#include <vector>
#include <string>
#include <TemplateState.h>

//�����̂ŏȗ�
using FontInfo = std::map<std::string, std::vector<std::pair<std::string, DirectX::SimpleMath::Vector2>>>;

namespace OtobeLib
{
	class ItemScene :public ISceneBase, State<int>
	{
	public:
		enum class ITEMFONT :int
		{
			TAG,//�^�O
			MOJI,//����
			X,//X
			Y,//Y
		};
	public:
		//�w�i�̏������W
		static const DirectX::SimpleMath::Vector3 INIT_POS_BACK;

		//�A�C�e���̏������W
		static const DirectX::SimpleMath::Vector3 INIT_POS_ITEM;

		//�A�C�e���̗���
		static const DirectX::SimpleMath::Vector3 INIT_POS_ITEM_BACK;

		//�������̗���
		static const DirectX::SimpleMath::Vector3 INIT_POS_EXPLANATION_COLUMN_BACK;

		//�w�i�̐؂���ʒu
		static const RECT INIT_RECT_BACK;

		//�A�C�e���̗��ʂ̐؂���ʒu
		static const RECT INIT_RECT_ITEM_BACK;

		//�������̐؂���ʒu
		static const RECT INIT_RECT_EXPLANATION_COLUMN_BACK;

		//�A�C�e�������ɕ��ׂ鐔
		static const int ROWITEM = 4;

		//�A�C�e�����c�ɕ��ׂ鐔
		static const int COLITEM = 2;

		//�A�C�e�����̏������W
		static const DirectX::SimpleMath::Vector3 INIT_POS_ITEMTEXT;

		//�A�C�e���̊Ԋu
		static const DirectX::SimpleMath::Vector3 ITEM_INTERVAL;

	private:
		//�摜�̊Ǘ��N���X
		std::unique_ptr<TextureManager>	m_TextureManager;

		//�A�C�e�����̗���
		std::vector<ITextureBase*> m_itemBackText;

		//�A�C�e�����̃A�C�e��
		std::vector<Item*> m_itemText;

		//�g�債�āA�\������Ă���A�C�e��
		ITextureBase*				m_zoomItem;

		//���݂̃y�[�W��
		int							m_pageNum;

		//�y�[�X����
		int							m_allPageNum;

		//���ݑI�𒆂̉摜
		ITextureBase*				m_selectTexture;

		//�t�H���g�̏��
		FontInfo					m_fontInfo;

		//Arrow�N���X
		Arrow* m_rightArrowTexture;
		Arrow* m_leftArrowTexture;
	public:
		//�R���X�g���N�^
		ItemScene();

		//�f�X�g���N�^
		~ItemScene()override {};

		//����������
		void Initialize()override;

		//�X�V����
		void Update()override;

		//�`�揈��
		void Render()override;

		//�I������
		void Finalize()override;

		//��������
		void Create()override;

	private:
		//�擾�����A�C�e���ɑΉ������^�O���擾����
		const std::pair<std::string, float> ChangeItemTag(const std::string& name);

		//�I�����Ă���A�C�e������A���̃^�O�ƁA���W���擾����
		const std::pair<std::string, DirectX::SimpleMath::Vector3> ReverseItem(const std::string& name);

		//�A�C�e�����̍X�V
		void UpdateItemText();

		//�I�𒆂̉摜�̍X�V
		void UpdateSelectTexture();

		//�I�𒆂̏���
		void InputSelect(const int& move, std::function<void(int&, DirectX::SimpleMath::Vector3&)>func);

		//�I�𒆂̂��̂ɁA�A�C�e�������邩�ǂ���
		const bool OnSelectItem();

		//�I������Ă���A�C�e�����擾����
		Item* SelectNowItem();

		//�A�C�e�����g�p����
		void UseItem();

		//���݁A�I�����Ă���ꏊ���擾����
		const int OnSelectPlace();

		//�g�p�����A�C�e��������

	public:
		//���݂̏�Ԃ�ݒ肷��
		void SetNowState(const int& now)override { State::SetNowState(now); };

		//���݂̏�Ԃ��擾����
		const int& GetNowState()const override { return State::GetNowState(); };

		//�ЂƂO�̏�Ԃ��擾����
		const int& GetPreState()const override { return State::GetPreState(); };

		//�f�[�^��ϊ�����
		const FontInfo Cast(const std::vector<std::vector<std::string>>& Input);
	};
}

