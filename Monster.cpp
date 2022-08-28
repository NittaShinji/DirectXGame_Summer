#include "Monster.h"
#include <cassert>

Monster::Monster() {}
Monster::~Monster() {}

//������
void Monster::Initialize(Model* model, const Vector3& position)
{
	// NULL�`�F�b�N
	assert(model);

	//���f�����C���v�b�g
	model_ = model;

	//�e�N�X�`���ǂݍ���
	blockHandle_ = TextureManager::Load("testBlock.png");
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

			if (i > 1)
			{
				worldTransforms_[i][j].translation_.x += i * 2.0;
			}
		}
	}
}
//�X�V
void Monster::Update()
{
	for (int i = 0; i < blockWidth; i++)
	{
		for (int j = 0; j < blockHeight; j++)
		{

			if (input_->PushKey(DIK_1))
			{
				if (i == 0 && j == 0)
				{
					form_[i][j] = Form::Slime;
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

			//�L�����N�^�[�ړ�����
			float scaleX = 1.0f;
			float scaleY = 1.0f;
			float scaleZ = 1.0f;

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

}
//�`��
void Monster::Draw(const ViewProjection& viewProjection)
{
	for (int i = 0; i < blockWidth; i++)
	{
		for (int j = 0; j < blockHeight; j++)
		{
			switch (form_[i][j])
			{
			case Form::Block:
			
				model_->Draw(worldTransforms_[i][j], viewProjection, blockHandle_);
				
				break;

			case Form::Slime:

				model_->Draw(worldTransforms_[i][j], viewProjection, slimeHandle_);

				break;

			default:
				break;
			}
		}
	}
}
