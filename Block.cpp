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

			//�����X�^�[�̐������Ԃ̐ݒ�
			birthTimer_[i][j] = kBirthTime;

			breakBlock[i][j] = false;
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
	birthMonster = false;

}

//�X�V
void Block::Update()
{
	//monster_.remove_if([]);

	monsters_.remove_if([](std::unique_ptr<Monster>& monster) {
		return monster->IsDead();
		});


	for (int i = 0; i < blockWidth; i++)
	{
		for (int j = 0; j < blockHeight; j++)
		{
#pragma region �J�[�\������
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
#pragma endregion �J�[�\������

			if (input_->PushKey(DIK_SPACE) && form_[i][j] == Form::IsSelected && wasChangedSelect == true)
			{
				form_[i][j] = Form::WasSelected;
				wasChangedSelect = false;
				//breakBlock[i][j] = true;
				breakBlock[i][j] = true;
				//breakBlock = true;
			}

			if (input_->PushKey(DIK_1))
			{
				if (j > 0 && j < 3)
				{
					if (i > 0 && i < 3)
					{
						form_[i][j] = Form::None;
					}
				}
			}

			if (breakBlock[i][j] == true)
			{
				if (form_[i][j] == Form::WasSelected)
				{
					//���[���h�s��̕��s�ړ��������擾(���[���h���W)
					monsterPos = worldTransforms_[i][j].translation_;
				}

				birthMonster = true;
				breakBlock[i][j] = false;
			}

			//if (breakBlock = true)
			//{
			//	if (form_[i][j] == Form::WasSelected)
			//	{
			//		//���[���h�s��̕��s�ړ��������擾(���[���h���W)
			//		monsterPos = worldTransforms_[i][j].translation_;
			//	}

			//	birthMonster[i][j] = true;
			//	//birthTimer_[i][j] = kBirthTime;
			//	//breakBlock = false;
			//}

			//if (breakBlock[i][j] == true)
			//{
			//	if (--birthTimer_[i][j] <= 0)
			//	{
			//		birthMonster[i][j] = true;
			//		Birth();
			//		//birthTimer_[i][j] = kBirthTime;
			//		breakBlock[i][j] = false;
			//	}
			//}

			//if (birthMonster[i][j] == true)
			//{
			//	
			//	//birthMonster[i][j] = false;
			//}

			//X���W�̈�O�̔ԍ���ۑ�
			prevBlockY = j;
		}
		//Y���W�̈�O�̃u���b�N�ԍ���ۑ�
		prevBlockX = i;
	}

	if (birthMonster == true)
	{
		Birth();
		//birthTimer_[i][j] = kBirthTime;
		birthMonster = false;
	}

	//�����X�^�[�X�V
	for (std::unique_ptr<Monster>& monster : monsters_)
	{
		monster->Update();
	}

	for (int i = 0; i < blockWidth; i++)
	{
		for (int j = 0; j < blockHeight; j++)
		{

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

		}
	}

#pragma region ���Z�b�g����

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

#pragma endregion

#pragma region �f�o�b�O�e�L�X�g
	/*debugText_->SetPos(50, 200);
	debugText_->Printf("worldTranslation[0]:(%f,%f,%f)",
		worldTransforms_[1][1].translation_.x, worldTransforms_[1][1].translation_.y, worldTransforms_[1][1].translation_.z);*/

	debugText_->SetPos(50, 200);
	debugText_->Printf("Form:(%d)",
		form_[1][1]);

	/*debugText_->SetPos(50, 250);
	debugText_->Printf("breakBlock[1][0]:(%d)",
		breakBlock[1][0]);
	debugText_->SetPos(50, 280);
	debugText_->Printf("breakBlock[1][1]:(%d)",
		breakBlock[1][1]);
	debugText_->SetPos(50, 310);
	debugText_->Printf("breakBlock[1][2]:(%d)",
		breakBlock[1][2]);*/

	debugText_->SetPos(50, 350);
	debugText_->Printf("birthTimer_[1][0]:(%d)",
		birthTimer_[1][0]);
	debugText_->SetPos(50, 380);
	debugText_->Printf("birthTimer_[1][1]:(%d)",
		birthTimer_[1][1]);
	debugText_->SetPos(50, 410);
	debugText_->Printf("birthTimer_[1][2]:(%d)",
		birthTimer_[1][2]);

	debugText_->SetPos(50, 450);
	debugText_->Printf("birthMonster[1][0]:(%d)",
		breakBlock[1][0]);
	debugText_->SetPos(50, 480);
	debugText_->Printf("birthMonster[1][1]:(%d)",
		breakBlock[1][1]);
	debugText_->SetPos(50, 510);
	debugText_->Printf("birthMonster[1][2]:(%d)",
		breakBlock[1][2]);
#pragma endregion

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
			case Form::None:
				break;

			default:
				break;
			}
		}
	}

	//�����X�^�[�`��
	for (std::unique_ptr<Monster>& monster : monsters_)
	{
		monster->Draw(viewProjection);
	}
}

Vector3 Block::GetSelectPosition()
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

//�����X�^�[�̓����蔻��p�ɕǂ̍��W��n���֐�
//Vector3 Block::GetBlockPosition()
//{
//	//���[�J�����W������ϐ�
//	Vector3 monsterPos[blockWidth][blockHeight];
//
//	//�S�Ă̕ǂ̍��W��n��
//	for (int i = 0; i < blockWidth; i++)
//	{
//		for (int j = 0; j < blockHeight; j++)
//		{
//			//���[���h�s��̕��s�ړ��������擾(���[���h���W)
//			monsterPos[i][j] = worldTransforms_[i][j].translation_;
//			return monsterPos[i][j];
//		}
//	}
//	
//}

void Block::GetWorldPosition(Vector3 blockPos[blockWidth][blockHeight])
{
	/*Vector3 monsterPos[blockWidth][blockHeight];*/

	Vector3 givePos;

	//�S�Ă̕ǂ̍��W��n��
	for (int i = 0; i < blockWidth; i++)
	{
		for (int j = 0; j < blockHeight; j++)
		{
			//���[���h�s��̕��s�ړ��������擾(���[���h���W)
			//blockPos[i][j] = worldTransforms_[i][j].translation_;
			blockPos[i][j].x = worldTransforms_[i][j].matWorld_.m[3][0];
			blockPos[i][j].y = worldTransforms_[i][j].matWorld_.m[3][1];
			blockPos[i][j].z = worldTransforms_[i][j].matWorld_.m[3][2];
		}
	}

	//blockPos = monsterPos;
}

bool Block::GetBirthMonster()
{
	return breakBlock;
}

void Block::Birth()
{
	//�����X�^�[�𐶐����A������
		//PlayerBullet* newBullet = new PlayerBullet();
	std::unique_ptr<Monster> newMonster = std::make_unique<Monster>();
	//newBullet->Initialize(model_ ,worldTransform_.translation_,velocity);

	//�����X�^�[�̑��x
	const float kMonsterSpeed = 1.0f;
	Vector3 velocity(0, 0, kMonsterSpeed);

	//for (int i = 0; i < blockWidth; i++)
	//{
	//	for (int j = 0; j < blockHeight; j++)
	//	{
	//		if (breakBlock == true)
	//		{
	//			if (form_[i][j] == Form::WasSelected)
	//			{
	//				//���[���h�s��̕��s�ړ��������擾(���[���h���W)
	//				monsterPos = worldTransforms_[i][j].translation_;
	//			}
	//		}
	//	}
	//}

	////���@�̕��s�ړ������̏����擾
	//worldMonsterTransform.x = worldTransform_.matWorld_.m[3][0];
	//worldMonsterTransform.y = worldTransform_.matWorld_.m[3][1];
	//worldMonsterTransform.z = worldTransform_.matWorld_.m[3][2];

	//�����X�^�[�𐶐�
	newMonster->Initialize(model_, monsterPos, velocity, birthMonster);

	//�����X�^�[��o�^����
	monsters_.push_back(std::move(newMonster));
}

//bool Block::OnCollision(Vector3 wallPos[blockWidth][blockHeight],const int x, const int y)
bool Block::OnCollision(const int x, const int y)
{
	for (int i = 0; i < blockWidth; i++)
	{
		for (int j = 0; j < blockHeight; j++)
		{
			/*if (worldTransforms_[i][j].translation_.x == collisionPos.x)
			{
				if (worldTransforms_[i][j].translation_.y == collisionPos.y)
				{
					if (worldTransforms_[i][j].translation_.z == collisionPos.z)
					{
						if (form_[i][j] == Form::WasSelected || form_[i][j] == Form::None)
						{
							result = true;
							return result;
						}
						else if(form_[i][j] == Form::Block || form_[i][j] == Form::IsSelected)
						{
							result = false;
							return result;
						}
					}
				}
			}*/

			//�i�߂�
			if (form_[x][y] == Form::WasSelected || form_[x][y] == Form::None)
			{
				return true;
			}
			//�i�߂Ȃ�
			else if (form_[x][y] == Form::Block || form_[x][y] == Form::IsSelected)
			{
				return false;
			}
		}
	}
}

float Block::GetRadius()
{
	//���a������ϐ�
	float blockRadius;
	//���[���h�s��̕��s�ړ��������擾(���[���h���W)
	blockRadius = radius;
	return blockRadius;
}

//�ǂ̉��̒�����n��
int Block::GetBlockWidth()
{
	//�ǂ̉��̒���������ϐ�
	float blockWidth_;
	//���[���h�s��̕��s�ړ��������擾(���[���h���W)
	blockWidth_ = blockWidth;
	return blockWidth_;
}
//�ǂ̏c�̒�����n��
int Block::GetBlockHight()
{
	//�ǂ̉��̒���������ϐ�
	float blockHight_;
	//���[���h�s��̕��s�ړ��������擾(���[���h���W)
	blockHight_ = blockHeight;
	return blockHight_;
}
