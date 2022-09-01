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
	worldTransform_.Initialize();

	//引数で受け取った初期座標をセット
	worldTransform_.translation_= position;

	canMoved = true;
	
}

void PlayerBullet::Update()
{
	if (canMoved == true)
	{
		//座標を移動させる(1フレーム分の移動量を足しこむ)
		worldTransform_.translation_ += velocity_;

	}
	
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

	//WorldMatrix(bulletWorldTransform_.matWorld_, matScale, matRot, matTrans);

	//親行列と掛け算代入
	/*if (bulletWorldTransform_.parent_ != nullptr)
	{
		bulletWorldTransform_.matWorld_ *= bulletWorldTransform_.parent_->matWorld_;
	}*/

	//ワールド行列を転送
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

Vector3 PlayerBullet::GetWorldPosition()
{
	worldResultTransform.x = worldTransform_.matWorld_.m[3][0];
	worldResultTransform.y = worldTransform_.matWorld_.m[3][1];
	worldResultTransform.z = worldTransform_.matWorld_.m[3][2];
	return worldResultTransform;
}
Vector3 PlayerBullet::GetLocalPosition()
{
	//ワールド座標を入れる変数
	Vector3 worldPos;
	//ワールド行列の平行移動成分を取得(ワールド座標)
	worldPos = worldTransform_.translation_;
	return worldPos;
}


