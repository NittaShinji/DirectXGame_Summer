#include "AirCamera.h"

AirCamera::AirCamera() {}
AirCamera::~AirCamera() {}
using namespace MathUtility;

//������
void AirCamera::Initialize(const Vector3& position, const Vector3& radian)
{
	debugText_ = DebugText::GetInstance();

	//���[���h�g�����X�t�H�[���̏�����
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.rotation_ = radian;

	//�r���[�v���W�F�N�V�����̏�����
	viewProjection_.Initialize();
}
//�X�V
void AirCamera::Update()
{
	//worldTransform_.translation_.z -= 0.02;
	//worldTransform_.rotation_.z += 0.2;

	viewProjection_.eye = worldTransform_.translation_;
	//���[���h�O���x�N�g��
	Vector3 forward(0, 0, 1);
	//���[���J�����̉�]�𔽉f
	forward = Transform(forward, worldTransform_.matWorld_);
	//���_����O���ɓK���ȋ����i�񂾈ʒu�������_
	viewProjection_.target = viewProjection_.eye + forward;
	//���[���h����x�N�g��
	Vector3 up(0, 1, 0);
	//���[���J�����̉�]�𔽉f(���[���J�����̏���x�N�g��)
	viewProjection_.up = Transform(up, worldTransform_.matWorld_);

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

	//WorldMatrix(enemyWorldTransform_.matWorld_, matScale, matRot, matTrans);
#pragma endregion

	//���[���h�s���]��
	worldTransform_.TransferMatrix();

	//�r���[�v���W�F�N�V�������X�V
	viewProjection_.UpdateMatrix();
	viewProjection_.TransferMatrix();

	/*debugText_->SetPos(50, 230);
	debugText_->Printf("viewProjection_.eye.z) : %f", viewProjection_.eye.z);*/

}

Matrix4 AirCamera::GetMatWorld()
{
	//���[���h���W������ϐ�
	Matrix4 matWorld;
	//���[���h�s��̕��s�ړ��������擾(���[���h���W)
	matWorld = worldTransform_.matWorld_;
	return matWorld;
}

WorldTransform AirCamera::GetWorldTransform()
{
	//���[���h���W������ϐ�
	WorldTransform worldTransform;
	//���[���h�s��̕��s�ړ��������擾(���[���h���W)
	worldTransform = worldTransform_;
	return worldTransform;
}


