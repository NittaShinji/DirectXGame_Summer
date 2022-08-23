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
	enemyHandle_ = TextureManager::Load("enemy.png");

	//���[���h�g�����X�t�H�[���̏�����
	enemyWorldTransform_.Initialize();

	//�����Ŏ󂯎�����������W���Z�b�g
	enemyWorldTransform_.translation_ = position;
}



//�X�V
void Enemy::Update()
{
	/*switch (phase_)
	{
	case Phase::Approach:
	default:
		PhsaeApproach();
		
		break;

	case Phase::Leave:
		
		PhsaeLeave();
		break;
	}*/

	//�����o�֐��|�C���^�ɓ����Ă���֐����Ăяo��
	(this->*spFuncTable[static_cast<size_t>(phase_)])();

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
	matRot.Matrix4RotationX(enemyWorldTransform_.rotation_.x);
	matRot.Matrix4RotationY(enemyWorldTransform_.rotation_.y);
	matRot.Matrix4RotationZ(enemyWorldTransform_.rotation_.z);

	enemyWorldTransform_.matWorld_.IdentityMatrix();

	//���W���s��ɐݒ�
	matTrans.Matrix4Translation(
		enemyWorldTransform_.translation_.x,
		enemyWorldTransform_.translation_.y,
		enemyWorldTransform_.translation_.z);

	enemyWorldTransform_.matWorld_ = matScale;
	enemyWorldTransform_.matWorld_ *= matRot;
	enemyWorldTransform_.matWorld_ *= matTrans;

	//WorldMatrix(enemyWorldTransform_.matWorld_, matScale, matRot, matTrans);

	//���[���h�s���]��
	enemyWorldTransform_.TransferMatrix();

#pragma endregion

}

//�`��
void Enemy::Draw(const ViewProjection& viewProjection)
{
	enemyModel_->Draw(enemyWorldTransform_, viewProjection, enemyHandle_);
}

void Enemy::PhsaeApproach()
{
	//�ړ�(�x�N�g�������Z)
	enemyWorldTransform_.translation_ -= ApproachSpeed;
	//����̈ʒu�ɓ��B�����痣�E
	if (enemyWorldTransform_.translation_.z < -40.0f)
	{
		phase_ = Phase::Leave;
	}
}

void Enemy::PhsaeLeave()
{
	//�ړ�(�x�N�g�������Z)
	enemyWorldTransform_.translation_ += LeaveSpeed;
	if (enemyWorldTransform_.translation_.z > 40.0f)
	{
		phase_ = Phase::Approach;
	}
}

//static�Ő錾���������o�֐��|�C���^�e�[�u���̎���
void (Enemy::* Enemy::spFuncTable[])() = {
	&Enemy::PhsaeApproach,	//�ڋ�
	&Enemy::PhsaeLeave,		//���E
};