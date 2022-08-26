#include "EnemyBullet.h"
#include <cassert>

EnemyBullet::EnemyBullet()
{
}

EnemyBullet::~EnemyBullet()
{
}

void EnemyBullet::Initialize(Model* model, const Vector3& position, const Vector3& velocity)
{
	// NULL�`�F�b�N
	assert(model);

	//���f���ǂݍ���
	eBulletModel_ = model;

	//�e�N�X�`���ǂݍ���
	eBulletHandle_ = TextureManager::Load("EnemyBullet.png");

	//�����Ŏ󂯎�������x�������o�ϐ��ɑ��
	velocity_ = velocity;

	//���[���h�g�����X�t�H�[���̏�����
	eBulletWorldTransform_.Initialize();

	//�����Ŏ󂯎�����������W���Z�b�g
	eBulletWorldTransform_.translation_ = position;

}

void EnemyBullet::Update()
{
	//���W���ړ�������(1�t���[�����̈ړ��ʂ𑫂�����)
	eBulletWorldTransform_.translation_ += velocity_;

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
	matRot.Matrix4RotationX(eBulletWorldTransform_.rotation_.x);
	matRot.Matrix4RotationY(eBulletWorldTransform_.rotation_.y);
	matRot.Matrix4RotationZ(eBulletWorldTransform_.rotation_.z);

	eBulletWorldTransform_.matWorld_.IdentityMatrix();

	//���W���s��ɐݒ�
	matTrans.Matrix4Translation(
		eBulletWorldTransform_.translation_.x,
		eBulletWorldTransform_.translation_.y,
		eBulletWorldTransform_.translation_.z);

	eBulletWorldTransform_.matWorld_ = matScale;
	eBulletWorldTransform_.matWorld_ *= matRot;
	eBulletWorldTransform_.matWorld_ *= matTrans;

	//WorldMatrix(eBulletWorldTransform_.matWorld_, matScale, matRot, matTrans);

	//�e�s��Ɗ|���Z���
	/*if (eBulletWorldTransform_.parent_ != nullptr)
	{
		eBulletWorldTransform_.matWorld_ *= eBulletWorldTransform_.parent_->matWorld_;
	}*/

	//���[���h�s���]��
	eBulletWorldTransform_.TransferMatrix();

#pragma endregion

	if (--deathTimer_ <= 0)
	{
		isDead_ = true;
	}
}

//�Q�b�^�[
//bool IsDead() const { return isDead_; }

//�`��
void EnemyBullet::Draw(const ViewProjection& viewProjection)
{
	eBulletModel_->Draw(eBulletWorldTransform_, viewProjection, eBulletHandle_);
}

void EnemyBullet::OnCollision()
{
	//�f�X�t���O�𗧂Ă�
	isDead_ = true;
}

Vector3 EnemyBullet::GetLocalPosition()
{
	//���[���h���W������ϐ�
	Vector3 worldPos;
	//���[���h�s��̕��s�ړ��������擾(���[���h���W)
	worldPos = eBulletWorldTransform_.translation_;
	return worldPos;
}

float EnemyBullet::GetRadius()
{
	//���a������ϐ�
	float bulletRadius;
	//���[���h�s��̕��s�ړ��������擾(���[���h���W)
	bulletRadius = radius;
	return bulletRadius;
}