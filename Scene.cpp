#include "Scene.h"
#include <cassert>

Scene::Scene(){}
Scene::~Scene(){}

void Scene::Initialize(Model* model){
	// NULL�`�F�b�N
	assert(model);

	//�󂯎�����f�[�^�������o�ϐ��ɋL�^����
	model_ = model;

	//�e�N�X�`���ǂݍ���
	titleHandle_ = TextureManager::Load("title.png");
	startHandle_ = TextureManager::Load("start.png");
	clearHandle_ = TextureManager::Load("gameclear.png");
	loseHandle_ = TextureManager::Load("gameover.png");
	controlHandle_ = TextureManager::Load("sousa.png");
	spaceHandle_ = TextureManager::Load("space.png");

	
	// ���[���h�ϊ��̏�����
	titleTransform_.Initialize();
	startTransform_.Initialize();
	controlTransform_.Initialize();

	//worldTransform_.scale_ = { 100,100,100 };
	titleTransform_.translation_ = { 0,5,0 };
	startTransform_.translation_ = { 0,-12,0 };
	controlTransform_.translation_ = { 160,1,50 };
	//controlTransform_.rotation_.x = -2.0;

}

int Scene::GetScene()
{
	return sceneNow;
}

void Scene::Update() {

#pragma region �^�C�g���̍s��̍X�V
	//�X�P�[�����O�p�s���錾
	Matrix4 matScale;
	//�����p��]�s���錾
	Matrix4 matRot;
	//�e���p��]�p�s���錾
	Matrix4 matRotX, matRotY, matRotZ;
	//���W�p�s���錾
	Matrix4 matTrans = MathUtility::Matrix4Identity();

	//�L�����N�^�[�ړ�����
	float scaleX = 20.0f;
	float scaleY = 12.0f;
	float scaleZ = 0.01f;

	//�X�P�[�����O�{�����s��ɐݒ�
	matScale.Matrix4Scaling(scaleX, scaleY, scaleZ);

	//titleTransform_.rotation_.x = 0.24;

	//��]�s����s��ɐݒ�
	matRot.Matrix4RotationX(titleTransform_.rotation_.x);
	matRot.Matrix4RotationY(titleTransform_.rotation_.y);
	matRot.Matrix4RotationZ(titleTransform_.rotation_.z);

	titleTransform_.matWorld_.IdentityMatrix();

	//���W���s��ɐݒ�
	matTrans.Matrix4Translation(
		titleTransform_.translation_.x,
		titleTransform_.translation_.y,
		titleTransform_.translation_.z);

	titleTransform_.matWorld_ = matScale;
	titleTransform_.matWorld_ *= matRot;
	titleTransform_.matWorld_ *= matTrans;

	//WorldMatrix(titleTransform_.matWorld_, matScale, matRot, matTrans);

	//�e�s��Ɗ|���Z���
	if (titleTransform_.parent_ != nullptr)
	{
		titleTransform_.matWorld_ *= titleTransform_.parent_->matWorld_;
	}

	//���[���h�s���]��
	titleTransform_.TransferMatrix();

#pragma endregion

#pragma region �X�^�[�g�̍X�V
	//�X�P�[�����O�p�s���錾
	Matrix4 matScale2;
	//�����p��]�s���錾
	Matrix4 matRot2;
	//�e���p��]�p�s���錾
	Matrix4 matRotX2, matRotY2, matRotZ2;
	//���W�p�s���錾
	Matrix4 matTrans2 = MathUtility::Matrix4Identity();

	//�L�����N�^�[�ړ�����
	float scaleX2 = 8.0f;
	float scaleY2 = 4.0f;
	float scaleZ2 = 0.01f;

	//�X�P�[�����O�{�����s��ɐݒ�
	matScale2.Matrix4Scaling(scaleX2, scaleY2, scaleZ2);

	//startTransform_.rotation_.x = 0.24;

	//��]�s����s��ɐݒ�
	matRot2.Matrix4RotationX(startTransform_.rotation_.x);
	matRot2.Matrix4RotationY(startTransform_.rotation_.y);
	matRot2.Matrix4RotationZ(startTransform_.rotation_.z);

	startTransform_.matWorld_.IdentityMatrix();

	//���W���s��ɐݒ�
	matTrans2.Matrix4Translation(
		startTransform_.translation_.x,
		startTransform_.translation_.y,
		startTransform_.translation_.z);

	startTransform_.matWorld_ = matScale2;
	startTransform_.matWorld_ *= matRot2;
	startTransform_.matWorld_ *= matTrans2;

	//WorldMatrix(startTransform_.matWorld_, matScale, matRot, matTrans);
	//���[���h�s���]��
	startTransform_.TransferMatrix();

#pragma endregion


#pragma region ������@�̍X�V
	//�X�P�[�����O�p�s���錾
	Matrix4 matScale3;
	//�����p��]�s���錾
	Matrix4 matRot3;
	//�e���p��]�p�s���錾
	Matrix4 matRotX3, matRotY3, matRotZ3;
	//���W�p�s���錾
	Matrix4 matTrans3 = MathUtility::Matrix4Identity();

	//�L�����N�^�[�ړ�����
	float scaleX3 = 55.0f;
	float scaleY3 = 1.0f;
	float scaleZ3 = 50.f;

	//�X�P�[�����O�{�����s��ɐݒ�
	matScale3.Matrix4Scaling(scaleX3, scaleY3, scaleZ3);

	
	//��]�s����s��ɐݒ�
	matRot3.Matrix4RotationX(controlTransform_.rotation_.x);
	matRot3.Matrix4RotationY(controlTransform_.rotation_.y);
	matRot3.Matrix4RotationZ(controlTransform_.rotation_.z);

	controlTransform_.matWorld_.IdentityMatrix();

	//���W���s��ɐݒ�
	matTrans3.Matrix4Translation(
		controlTransform_.translation_.x,
		controlTransform_.translation_.y,
		controlTransform_.translation_.z);

	controlTransform_.matWorld_ = matScale3;
	controlTransform_.matWorld_ *= matRot3;
	controlTransform_.matWorld_ *= matTrans3;

	//WorldMatrix(controlTransform_.matWorld_, matScale, matRot, matTrans);
	//���[���h�s���]��
	controlTransform_.TransferMatrix();
#pragma endregion


}

void Scene::TitleDraw(ViewProjection& viewProjection) 
{
	model_->Draw(titleTransform_, viewProjection, titleHandle_);
	model_->Draw(startTransform_, viewProjection, startHandle_);
}

//void Scene::

void Scene::LoseDraw(ViewProjection& viewProjection) 
{
	model_->Draw(titleTransform_, viewProjection, loseHandle_);
	model_->Draw(startTransform_, viewProjection, spaceHandle_);

}

void Scene::ClearDraw(ViewProjection& viewProjection) 
{
	model_->Draw(titleTransform_, viewProjection, clearHandle_);
}

void Scene::GameDraw(ViewProjection& viewProjection)
{
	model_->Draw(controlTransform_, viewProjection, controlHandle_);
}


void Scene::UpdateMatrix()
{

}