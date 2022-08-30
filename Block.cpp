#include "Block.h"
#include <cassert>

Block::Block() {}
Block::~Block() {}

//������
void Block::Initialize(Model* model, const Vector3& position)
{
	// NULL�`�F�b�N
	assert(model);

	//���f�����C���v�b�g
	model_ = model;

	//�e�N�X�`���ǂݍ���
	blockHandle_ = TextureManager::Load("testBlock.png");
	selectHandle_ = TextureManager::Load("testBlock2.png");
	wasSelectHandle_ = TextureManager::Load("testBlock3.png");
	slimeHandle_ = TextureManager::Load("suraimu.png");

	////�����Ŏ󂯎�������x�������o�ϐ��ɑ��
	//enemyVelocity_ = velocity;

	// �V���O���g���C���X�^���X���擾����
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	for (int i = 0; i < blockWidth; i++)
	{
		for (int j = 0; j < blockHeight; j++)
		{
			worldTransforms_[i][j].Initialize();
			//�����Ŏ󂯎�����������W���Z�b�g
			worldTransforms_[i][j].translation_ = position;

			//�J�[�\���̏����l��ݒ�
			if (i == 1 && j == 1)
			{
				form_[i][j] = Form::IsSelected;
			}

			//form_[i][j] = Form::IsSelected;

			if (i > 0)
			{
				worldTransforms_[i][j].translation_.x += i * (scaleX * 2);
				//if()
			}
			if (j > 0)
			{
				//if() 
				worldTransforms_[i][j].translation_.z += j * (scaleZ * 2);
			}

		}
	}

	wasChangedSelect = true;
	changedSelect = true;
	prevBlockX = 0;
}

//�X�V
void Block::Update()
{
	for (int i = 0; i < blockWidth; i++)
	{
		for (int j = 0; j < blockHeight; j++)
		{
			if (input_->PushKey(DIK_RIGHT) && i < blockWidth && changedSelect == true)
			{
				//�O�J�[�\���̏�Ԃ���J�[�\����Ԃ��Ȃ����āA�J�[�\�������O�̏�Ԃɂ���B
				if (form_[i][j] == Form::IsSelected)
				{
					//�u���b�N
					form_[i][j] = Form::Block;
					if (form_[i + 1][j] == Form::Block)
					{
						//�u���b�N�J�[�\��
						form_[i + 1][j] = Form::IsSelected;
						changedSelect = false;
					}
					else if (form_[i + 1][j] == Form::None)
					{
						//��ԃJ�[�\��
						form_[i + 1][j] = Form::WasSelected;
						changedSelect = false;
					}

				}
				else if (form_[i][j] == Form::WasSelected)
				{
					//���
					form_[i][j] = Form::None;
					if (form_[i + 1][j] == Form::Block)
					{
						//�u���b�N�J�[�\��
						form_[i + 1][j] = Form::IsSelected;
						changedSelect = false;
					}
					else if (form_[i + 1][j] == Form::None)
					{
						//��ԃJ�[�\��
						form_[i + 1][j] = Form::WasSelected;
						changedSelect = false;
					}
				}
			}
			else if (input_->PushKey(DIK_LEFT) && i > 0 && changedSelect == true)
			{
				//if (form_[i][j] == Form::IsSelected || form_[i][j] == Form::WasSelected)
				//{
				//	form_[i][j] = Form::Block;
				//	//form_[i - 1][j] = Form::IsSelected;
				//	form_[prevBlockX][j] = Form::IsSelected;

				//	changedSelect = false;
				//}

				//�O�J�[�\���̏�Ԃ���J�[�\����Ԃ��Ȃ����āA�J�[�\�������O�̏�Ԃɂ���B
				if (form_[i][j] == Form::IsSelected)
				{
					//�u���b�N
					form_[i][j] = Form::Block;
					if (form_[prevBlockX][j] == Form::Block)
					{
						//�u���b�N�J�[�\��
						form_[prevBlockX][j] = Form::IsSelected;
						changedSelect = false;
					}
					else if (form_[prevBlockX][j] == Form::None)
					{
						//��ԃJ�[�\��
						form_[prevBlockX][j] = Form::WasSelected;
						changedSelect = false;
					}
				}
				else if (form_[i][j] == Form::WasSelected)
				{
					//���
					form_[i][j] = Form::None;
					if (form_[prevBlockX][j] == Form::Block)
					{
						//�u���b�N�J�[�\��
						form_[prevBlockX][j] = Form::IsSelected;
						changedSelect = false;
					}
					else if (form_[prevBlockX][j] == Form::None)
					{
						//��ԃJ�[�\��
						form_[prevBlockX][j] = Form::WasSelected;
						changedSelect = false;
					}
				}
			}
			else if (input_->PushKey(DIK_UP) && j < blockHeight && changedSelect == true)
			{
				//�O�J�[�\���̏�Ԃ���J�[�\����Ԃ��Ȃ����āA�J�[�\�������O�̏�Ԃɂ���B
				if (form_[i][j] == Form::IsSelected)
				{
					//�u���b�N
					form_[i][j] = Form::Block;

					if (form_[i][j + 1] == Form::Block)
					{
						//�u���b�N�J�[�\��
						form_[i][j + 1] = Form::IsSelected;
						changedSelect = false;
					}
					else if (form_[i][j + 1] == Form::None)
					{
						//��ԃJ�[�\��
						form_[i][j + 1] = Form::WasSelected;
						changedSelect = false;
					}

				}
				else if (form_[i][j] == Form::WasSelected)
				{
					//���
					form_[i][j] = Form::None;

					if (form_[i][j + 1] == Form::Block)
					{
						//�u���b�N�J�[�\��
						form_[i][j + 1] = Form::IsSelected;
						changedSelect = false;
					}
					else if (form_[i][j + 1] == Form::None)
					{
						//��ԃJ�[�\��
						form_[i][j + 1] = Form::WasSelected;
						changedSelect = false;
					}

				}

				/*if (form_[i][j] == Form::IsSelected || form_[i][j] == Form::WasSelected)
				{
					form_[i][j] = Form::Block;
					form_[i][j + 1] = Form::IsSelected;
					changedSelect = false;
				}*/
			}
			else if (input_->PushKey(DIK_DOWN) && j > 0 && changedSelect == true)
			{
				if (form_[i][j] == Form::IsSelected)
				{
					//�u���b�N
					form_[i][j] = Form::Block;

					if (form_[i][prevBlockY] == Form::Block)
					{
						//�u���b�N�J�[�\��
						form_[i][prevBlockY] = Form::IsSelected;
						changedSelect = false;
					}
					else if (form_[i][prevBlockY] == Form::None)
					{
						//��ԃJ�[�\��
						form_[i][prevBlockY] = Form::WasSelected;
						changedSelect = false;
					}
				}
				else if (form_[i][j] == Form::WasSelected)
				{
					//���
					form_[i][j] = Form::None;

					if (form_[i][prevBlockY] == Form::Block)
					{
						//�u���b�N�J�[�\��
						form_[i][prevBlockY] = Form::IsSelected;
						changedSelect = false;
					}
					else if (form_[i][prevBlockY] == Form::None)
					{
						//��ԃJ�[�\��
						form_[i][prevBlockY] = Form::WasSelected;
						changedSelect = false;
					}

				}
			}
			else
			{

			}

			if (input_->PushKey(DIK_SPACE) && form_[i][j] == Form::IsSelected && wasChangedSelect == true)
			{
				form_[i][j] = Form::WasSelected;
				wasChangedSelect = false;
			}

			if (input_->PushKey(DIK_1))
			{
				if (j > 1 && j < 4)
				{
					if (i > 2 && i < 5)
					{
						form_[i][j] = Form::None;
					}
				}
			}

#pragma region �s��̍X�V
			//�X�P�[�����O�p�s���錾
			Matrix4 matScale;
			//�����p��]�s���錾
			Matrix4 matRot;
			//�e���p��]�p�s���錾
			Matrix4 matRotX, matRotY, matRotZ;
			//���W�p�s���錾
			Matrix4 matTrans = MathUtility::Matrix4Identity();

			//�X�P�[�����O�{�����s��ɐݒ�
			matScale.Matrix4Scaling(scaleX, scaleY, scaleZ);

			//��]�s����s��ɐݒ�
			matRot.Matrix4RotationX(worldTransforms_[i][j].rotation_.x);
			matRot.Matrix4RotationY(worldTransforms_[i][j].rotation_.y);
			matRot.Matrix4RotationZ(worldTransforms_[i][j].rotation_.z);

			worldTransforms_[i][j].matWorld_.IdentityMatrix();

			//���W���s��ɐݒ�
			matTrans.Matrix4Translation(
				worldTransforms_[i][j].translation_.x,
				worldTransforms_[i][j].translation_.y,
				worldTransforms_[i][j].translation_.z);

			worldTransforms_[i][j].matWorld_ = matScale;
			worldTransforms_[i][j].matWorld_ *= matRot;
			worldTransforms_[i][j].matWorld_ *= matTrans;

			//WorldMatrix(worldTransform_.matWorld_, matScale, matRot, matTrans);

			//�e�s��Ɗ|���Z���
			/*if (worldTransform_.parent_ != nullptr)
			{
				worldTransform_.matWorld_ *= worldTransform_.parent_->matWorld_;
			}*/

			//���[���J�����̃��[���h�s����擾
			/*Matrix4 railCameraMatWorld = railCamera_->GetMatWorld();
			worldTransform_.matWorld_ *= railCameraMatWorld;*/

			//worldTransform_.matWorld_ *= 

			//���[���h�s���]��
			worldTransforms_[i][j].TransferMatrix();


#pragma endregion

			//X���W�̈�O�̔ԍ���ۑ�
			prevBlockY = j;
		}
		//Y���W�̈�O�̃u���b�N�ԍ���ۑ�
		prevBlockX = i;
	}

	if (changedSelect == false)
	{
		if (--selectTimer_ <= 0)
		{
			changedSelect = true;
			selectTimer_ = kSelectTime;
		}
	}

	if (wasChangedSelect == false)
	{
		if (--wasSelectTimer_ <= 0)
		{
			wasChangedSelect = true;
			wasSelectTimer_ = kSelectTime;
		}
	}
}
//�`��
void Block::Draw(const ViewProjection& viewProjection)
{
	for (int i = 0; i < blockWidth; i++)
	{
		for (int j = 0; j < blockHeight; j++)
		{
			switch (form_[i][j])
			{

			case Form::WasSelected:

				//model_->Draw(worldTransforms_[i][j], viewProjection, wasSelectHandle_);
				break;

			case Form::Block:

				model_->Draw(worldTransforms_[i][j], viewProjection, blockHandle_);

				break;

			case Form::IsSelected:

				model_->Draw(worldTransforms_[i][j], viewProjection, selectHandle_);
				break;
			case Form::Slime:

				model_->Draw(worldTransforms_[i][j], viewProjection, slimeHandle_);

				break;
			case Form::None:
				break;

			default:
				break;
			}
		}
	}
}

Vector3 Block::GetLocalPosition()
{
	//���[���h���W������ϐ�
	Vector3 worldPos;

	//���݃J�[�\��������ʒu��n��
	for (int i = 0; i < blockWidth; i++)
	{
		for (int j = 0; j < blockHeight; j++)
		{
			if (form_[i][j] == Form::IsSelected || form_[i][j] == Form::WasSelected)
			{
				//���[���h�s��̕��s�ړ��������擾(���[���h���W)
				worldPos = worldTransforms_[i][j].translation_;
			}
		}
	}	

	return worldPos;
}