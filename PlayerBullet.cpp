#include "PlayerBullet.h"
#include <cassert>

PlayerBullet::PlayerBullet()
{
}

PlayerBullet::~PlayerBullet()
{
}

void PlayerBullet::Initialize(Model* model, const Vector3& position, const Vector3& velocity)
{
	// NULL�`�F�b�N
	assert(model);

	debugText_ = DebugText::GetInstance();

	bulletModel_ = model;
	//�e�N�X�`���ǂݍ���
	bulletHandle_ = TextureManager::Load("bullet.png");

	//�����Ŏ󂯎�������x�������o�ϐ��ɑ��
	velocity_ = velocity;

	//���[���h�g�����X�t�H�[���̏�����
	worldTransform_.Initialize();

	//�����Ŏ󂯎�����������W���Z�b�g
	worldTransform_.translation_= position;

	canMoved = true;
	
}

void PlayerBullet::Update()
{
	if (canMoved == true)
	{
		//���W���ړ�������(1�t���[�����̈ړ��ʂ𑫂�����)
		worldTransform_.translation_ += velocity_;

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
	matRot.Matrix4RotationX(worldTransform_.rotation_.x);
	matRot.Matrix4RotationY(worldTransform_.rotation_.y);
	matRot.Matrix4RotationZ(worldTransform_.rotation_.z);

	worldTransform_.matWorld_.IdentityMatrix();

	//���W���s��ɐݒ�
	matTrans.Matrix4Translation(
		worldTransform_.translation_.x,
		worldTransform_.translation_.y,
		worldTransform_.translation_.z);

	worldTransform_.matWorld_ = matScale;
	worldTransform_.matWorld_ *= matRot;
	worldTransform_.matWorld_ *= matTrans;

	//WorldMatrix(bulletWorldTransform_.matWorld_, matScale, matRot, matTrans);

	//�e�s��Ɗ|���Z���
	/*if (bulletWorldTransform_.parent_ != nullptr)
	{
		bulletWorldTransform_.matWorld_ *= bulletWorldTransform_.parent_->matWorld_;
	}*/

	//���[���h�s���]��
	worldTransform_.TransferMatrix();

#pragma endregion

	if (--deathTimer_ <= 0)
	{
		isDead_ = true;
	}

	/*debugText_->SetPos(50, 400);
	debugText_->Printf("translation_.z) : %f", bulletWorldTransform_.translation_.z);*/

}

void PlayerBullet::OnCollisionStop()
{
	canMoved = false;
}

void PlayerBullet::Draw(const ViewProjection& viewProjection)
{
	bulletModel_->Draw(worldTransform_, viewProjection, bulletHandle_);
}

void PlayerBullet::OnCollision()
{
	//�f�X�t���O�𗧂Ă�
	isDead_ = true;
}

float PlayerBullet::GetRadius()
{
	//���a������ϐ�
	float bulletRadius;
	//���[���h�s��̕��s�ړ��������擾(���[���h���W)
	bulletRadius = radius;
	return bulletRadius;
}

Vector3 PlayerBullet::GetWorldPosition()
{
	worldResultTransform.x = worldTransform_.matWorld_.m[3][0];
	worldResultTransform.y = worldTransform_.matWorld_.m[3][1];
	worldResultTransform.z = worldTransform_.matWorld_.m[3][2];
	return worldResultTransform;
}
Vector3 PlayerBullet::GetLocalPosition()
{
	//���[���h���W������ϐ�
	Vector3 worldPos;
	//���[���h�s��̕��s�ړ��������擾(���[���h���W)
	worldPos = worldTransform_.translation_;
	return worldPos;
}


