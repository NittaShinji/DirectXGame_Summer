#include "Enemy.h"
#include "Player.h"
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
	enemyHandle_ = TextureManager::Load("suraimu.png");

	////�����Ŏ󂯎�������x�������o�ϐ��ɑ��
	//enemyVelocity_ = velocity;

	// �V���O���g���C���X�^���X���擾����
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	//���[���h�g�����X�t�H�[���̏�����
	enemyWorldTransform_.Initialize();

	//�����Ŏ󂯎�����������W���Z�b�g
	enemyWorldTransform_.translation_ = position;

	ApproachInitialize();
}

//�X�V
void Enemy::Update()
{
	//�f�X�t���O�̗������e���폜
	bullets_.remove_if([](std::unique_ptr<EnemyBullet>& bullet) {
		return bullet->IsDead();
		});

	switch (phase_)
	{
	case Phase::Approach:
	default:
		PhsaeApproach();

		break;

	case Phase::Leave:

		PhsaeLeave();
		break;
	}

	Vector3 enemyVelocity_ = { 0, 0, kEnemySpeed };

	//���W���ړ�������(1�t���[�����̈ړ��ʂ𑫂�����)
	//enemyWorldTransform_.translation_ -= enemyVelocity_;

#pragma region �L�����N�^�[�̍U��

	//�e�X�V
	for (std::unique_ptr<EnemyBullet>& bullet : bullets_)
	{
		bullet->Update();
	}

#pragma endregion


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

Vector3 Enemy::GetLocalPosition()
{
	//���[���h���W������ϐ�
	Vector3 worldPos;
	//���[���h�s��̕��s�ړ��������擾(���[���h���W)
	worldPos = enemyWorldTransform_.translation_;
	return worldPos;
}

void Enemy::Fire()
{
	assert(player_);

	//�@�e�̑��x
	const float kBulletSpeed = -1.0f;
	Vector3 velocity(kBulletSpeed, 0, 0);

	//���@�̃��[���h���W���擾
	Vector3 playerPos = player_->GetLocalPosition();
	//�G�̃��[���h���W���擾
	Vector3 enemyPos = enemyWorldTransform_.translation_;

	//�����x�N�g��
	Vector3 difVec;
	//�����x�N�g�������߂�
	difVec = playerPos - enemyPos;
	//�����x�N�g���𐳋K��
	difVec.normalize();
	//�x�N�g���̒����𑁂��ɍ��킹��
	const float bulletSpeed = 0.2f;
	difVec *= bulletSpeed;

	// ���x�x�N�g�������@�̌����ɍ��킹�ĉ�]������
	//velocity = Root(velocity, enemyWorldTransform_);

	//�e�𐶐����A������
		//PlayerBullet* newBullet = new PlayerBullet();
	std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
	newBullet->Initialize(enemyModel_, enemyWorldTransform_.translation_, difVec);

	//�e��o�^����
	//bullet_.reset(newBullet);
	bullets_.push_back(std::move(newBullet));

}

//�`��
void Enemy::Draw(const ViewProjection& viewProjection)
{
	enemyModel_->Draw(enemyWorldTransform_, viewProjection, enemyHandle_);
	for (std::unique_ptr<EnemyBullet>& bullet : bullets_)
	{
		bullet->Draw(viewProjection);
	}
}

void Enemy::ApproachInitialize()
{
	//���˃^�C�}�[��������
	bulletCoolTimer = kFireInterval;
}

void Enemy::PhsaeApproach()
{
	//�ړ�(�x�N�g�������Z)
	//enemyWorldTransform_.translation_ -= ApproachSpeed;
	//����̈ʒu�ɓ��B�����痣�E
	if (enemyWorldTransform_.translation_.x < -40.0f)
	{
		phase_ = Phase::Leave;
	}

	//���˃^�C�}�[���J�E���g�_�E��
	bulletCoolTimer--;
	//�w�莞�ԂɒB����
	if (bulletCoolTimer <= 0)
	{
		//�e�𔭎�
		Fire();
		//���˃^�C�}�[��������
		bulletCoolTimer = kFireInterval;
	}
}

void Enemy::PhsaeLeave()
{
	//�ړ�(�x�N�g�������Z)
	//enemyWorldTransform_.translation_ += LeaveSpeed;
	if (enemyWorldTransform_.translation_.x > 40.0f)
	{
		phase_ = Phase::Approach;
	}
}

Vector3 Enemy::Root(Vector3 velocity, WorldTransform worldTransform_)
{
	//�P�ʃx�N�g��
	Vector3 frontVec = { 0.0f, 0.0f, 1.0f };
	//���ʗp�̃x�N�g��
	Vector3 resultVec = { 0.0f, 0.0f, 0.0f };

	resultVec.x = (cos(worldTransform_.rotation_.y) * frontVec.x +
		sin(worldTransform_.rotation_.y) * frontVec.z);
	resultVec.z = (-sinf(worldTransform_.rotation_.y) * frontVec.x +
		cosf(worldTransform_.rotation_.y) * frontVec.z);

	return resultVec;
}

void Enemy::OnCollision() {}

float Enemy::GetRadius()
{
	//���a������ϐ�
	float bulletRadius;
	//���[���h�s��̕��s�ړ��������擾(���[���h���W)
	bulletRadius = radius;
	return bulletRadius;
}