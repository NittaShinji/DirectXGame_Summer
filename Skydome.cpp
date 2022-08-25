#include "Skydome.h"
#include <cassert>

Skydome::Skydome(){}

Skydome::~Skydome(){}

void Skydome::Initialize(Model* model)
{
	// NULLチェック
	assert(model);

	//受け取ったデータをメンバ変数に記録する
	model_ = model;

	// ワールド変換の初期化
	worldTransform_.Initialize();

	//worldTransform_.scale_ = { 100,100,100 };
	worldTransform_.translation_ = {0,0,0};
}

void Skydome::Update()
{

#pragma region 行列の更新
	//スケーリング用行列を宣言
	Matrix4 matScale;
	//合成用回転行列を宣言
	Matrix4 matRot;
	//各軸用回転用行列を宣言
	Matrix4 matRotX, matRotY, matRotZ;
	//座標用行列を宣言
	Matrix4 matTrans = MathUtility::Matrix4Identity();

	//キャラクター移動処理
	float scaleX = 100.0f;
	float scaleY = 100.0f;
	float scaleZ = 100.0f;

	//スケーリング倍率を行列に設定
	matScale.Matrix4Scaling(scaleX, scaleY, scaleZ);

	//回転行列を行列に設定
	matRot.Matrix4RotationX(worldTransform_.rotation_.x);
	matRot.Matrix4RotationY(worldTransform_.rotation_.y);
	matRot.Matrix4RotationZ(worldTransform_.rotation_.z);

	worldTransform_.matWorld_.IdentityMatrix();

	//座標を行列に設定
	matTrans.Matrix4Translation(
		worldTransform_.translation_.x,
		worldTransform_.translation_.y,
		worldTransform_.translation_.z);

	worldTransform_.matWorld_ = matScale;
	worldTransform_.matWorld_ *= matRot;
	worldTransform_.matWorld_ *= matTrans;

	//WorldMatrix(worldTransform_.matWorld_, matScale, matRot, matTrans);

	//親行列と掛け算代入
	if (worldTransform_.parent_ != nullptr)
	{
		worldTransform_.matWorld_ *= worldTransform_.parent_->matWorld_;
	}

	//ワールド行列を転送
	worldTransform_.TransferMatrix();

#pragma endregion

}

void Skydome::Draw(ViewProjection& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection);
}
