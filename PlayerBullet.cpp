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
	// NULLチェック
	assert(model);

	debugText_ = DebugText::GetInstance();

	bulletModel_ = model;
	//テクスチャ読み込み
	bulletHandle_ = TextureManager::Load("bullet.png");

	//引数で受け取った速度をメンバ変数に代入
	velocity_ = velocity;

	//ワールドトランスフォームの初期化
	bulletWorldTransform_.Initialize();

	//引数で受け取った初期座標をセット
	bulletWorldTransform_.translation_= position;
	
}

void PlayerBullet::Update()
{
	//座標を移動させる(1フレーム分の移動量を足しこむ)
	bulletWorldTransform_.translation_ += velocity_;

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
	float scaleX = 1.0f;
	float scaleY = 1.0f;
	float scaleZ = 1.0f;

	//スケーリング倍率を行列に設定
	matScale.Matrix4Scaling(scaleX, scaleY, scaleZ);

	//回転行列を行列に設定
	matRot.Matrix4RotationX(bulletWorldTransform_.rotation_.x);
	matRot.Matrix4RotationY(bulletWorldTransform_.rotation_.y);
	matRot.Matrix4RotationZ(bulletWorldTransform_.rotation_.z);

	bulletWorldTransform_.matWorld_.IdentityMatrix();

	//座標を行列に設定
	matTrans.Matrix4Translation(
		bulletWorldTransform_.translation_.x,
		bulletWorldTransform_.translation_.y,
		bulletWorldTransform_.translation_.z);

	bulletWorldTransform_.matWorld_ = matScale;
	bulletWorldTransform_.matWorld_ *= matRot;
	bulletWorldTransform_.matWorld_ *= matTrans;

	//WorldMatrix(bulletWorldTransform_.matWorld_, matScale, matRot, matTrans);

	//親行列と掛け算代入
	/*if (bulletWorldTransform_.parent_ != nullptr)
	{
		bulletWorldTransform_.matWorld_ *= bulletWorldTransform_.parent_->matWorld_;
	}*/

	//ワールド行列を転送
	bulletWorldTransform_.TransferMatrix();

#pragma endregion

	if (--deathTimer_ <= 0)
	{
		isDead_ = true;
	}

	debugText_->SetPos(50, 400);
	debugText_->Printf("translation_.z) : %f", bulletWorldTransform_.translation_.z);

}

void PlayerBullet::Draw(const ViewProjection& viewProjection)
{
	bulletModel_->Draw(bulletWorldTransform_, viewProjection, bulletHandle_);
}

void PlayerBullet::OnCollision()
{
	//デスフラグを立てる
	isDead_ = true;
}

float PlayerBullet::GetRadius()
{
	//半径を入れる変数
	float bulletRadius;
	//ワールド行列の平行移動成分を取得(ワールド座標)
	bulletRadius = radius;
	return bulletRadius;
}

Vector3 PlayerBullet::GetLocalPosition()
{
	//ワールド座標を入れる変数
	Vector3 worldPos;
	//ワールド行列の平行移動成分を取得(ワールド座標)
	worldPos = bulletWorldTransform_.translation_;
	return worldPos;
}