#include "Enemy.h"
#include "WorldTransform.h"
#include "Vector3.h"
#include <cassert>

Enemy::Enemy()
{
}

Enemy::~Enemy()
{
}

//������
void Enemy::Initialize(Model* model, const Vector3& position)
{
	// NULL�`�F�b�N
	assert(model);

	enemyModel_ = model;

	//�e�N�X�`���ǂݍ���
	blockHandle_ = TextureManager::Load("suraimuBlock.png");
	enemyHandle_ = TextureManager::Load("suraimu.png");

	////�����Ŏ󂯎�������x�������o�ϐ��ɑ��
	//enemyVelocity_ = velocity;

	//���[���h�g�����X�t�H�[���̏�����

	for (int i = 0; i < 100; i++)
	{
		enemyWorldTransforms_[i].Initialize();

		//�����Ŏ󂯎�����������W���Z�b�g
		enemyWorldTransforms_[i].translation_ = position;
		enemyWorldTransforms_[i].translation_.x = i * 2 + -10;

		blockTimer_[i] = kLifeTime + (i * 50);
	}
}

//�X�V
void Enemy::Update()
{
	/*switch (phase_)
	{
	case Phase::Cube:
		
	default:
		PhsaeCube();
		break;

	case Phase::Move:
		
		PhsaeMove();
		break;
	}*/

	PhsaeCube();
	PhsaeMove();
	
	Vector3 enemyVelocity_ = { 0, 0, kEnemySpeed } ;

	//���W���ړ�������(1�t���[�����̈ړ��ʂ𑫂�����)
	//enemyWorldTransform_.translation_ -= enemyVelocity_;

#pragma region �s��̍X�V
	for (int i = 0; i < 100; i++)
	{
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
		matRot.Matrix4RotationX(enemyWorldTransforms_[i].rotation_.x);
		matRot.Matrix4RotationY(enemyWorldTransforms_[i].rotation_.y);
		matRot.Matrix4RotationZ(enemyWorldTransforms_[i].rotation_.z);

		enemyWorldTransforms_[i].matWorld_.IdentityMatrix();

		//���W���s��ɐݒ�
		matTrans.Matrix4Translation(
			enemyWorldTransforms_[i].translation_.x,
			enemyWorldTransforms_[i].translation_.y,
			enemyWorldTransforms_[i].translation_.z);

		enemyWorldTransforms_[i].matWorld_ = matScale;
		enemyWorldTransforms_[i].matWorld_ *= matRot;
		enemyWorldTransforms_[i].matWorld_ *= matTrans;

		//WorldMatrix(enemyWorldTransforms_[i].matWorld_, matScale, matRot, matTrans);

		//���[���h�s���]��
		enemyWorldTransforms_[i].TransferMatrix();
	}

#pragma endregion

}

//�`��
void Enemy::Draw(const ViewProjection& viewProjection)
{
	for (int i = 0; i < 100; i++)
	{
		/*switch (phase_)
		{
		case Phase::Cube:
		default:
			enemyModel_->Draw(enemyWorldTransforms_[i], viewProjection, blockHandle_);
			break;

		case Phase::Move:
			enemyModel_->Draw(enemyWorldTransforms_[i], viewProjection, enemyHandle_);
			break;
		}*/

		enemyModel_->Draw(enemyWorldTransforms_[i], viewProjection, blockHandle_);
		enemyModel_->Draw(enemyWorldTransforms_[i], viewProjection, enemyHandle_);
	}
	
	//enemyModel_->Draw(enemyWorldTransform_, viewProjection, enemyHandle_);
}

void Enemy::PhsaeCube()
{
	for (int i = 0; i < 100; i++)
	{
		if (--blockTimer_[i] <= 0)
		{
			isblock[i] = true;
		}

		//�u���b�N�����鏈��
		/*if (input_->PushKey(DIK_2))
		{
			isblock = 1;
		}*/
		//enemyWorldTransform_.translation_ -= CubeSpeed;
		//����̈ʒu�ɓ��B�����痣�E
		if (isblock[i] == 1)
		{
			isMove[i] = 1;
		}
	}
}

void Enemy::PhsaeMove()
{
	for (int i = 0; i < 100; i++)
	{
		if (isMove[i] == 1)
		{
			if (moveChange[i] == 0)
			{
				enemyWorldTransforms_[i].translation_ -= MoveSpeed;

				if (enemyWorldTransforms_[i].translation_.z <= -40.0f)
				{
					moveChange[i] = 1;
				}
			}

			//�ړ�(�x�N�g�������Z)

				//enemyWorldTransform_.translation_ -= MoveSpeed;
			if (moveChange[i] == 1)
			{
				enemyWorldTransforms_[i].translation_ += MoveSpeed;

				if (enemyWorldTransforms_[i].translation_.z >= 40.0f)
				{
					moveChange[i] = 0;
				}
			}
		}
	}
}