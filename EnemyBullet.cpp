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
	// NULLチェック
	assert(model);

	//モデル読み込み
	eBulletModel_ = model;

	//テクスチャ読み込み
	eBulletHandle_ = TextureManager::Load("EnemyBullet.png");

	//引数で受け取った速度をメンバ変数に代入
	velocity_ = velocity;

	//ワールドトランスフォームの初期化
	eBulletWorldTransform_.Initialize();

	//引数で受け取った初期座標をセット
	eBulletWorldTransform_.translation_ = position;

}

void EnemyBullet::Update()
{
	//座標を移動させる(1フレーム分の移動量を足しこむ)
	eBulletWorldTransform_.translation_ += velocity_;

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
	matRot.Matrix4RotationX(eBulletWorldTransform_.rotation_.x);
	matRot.Matrix4RotationY(eBulletWorldTransform_.rotation_.y);
	matRot.Matrix4RotationZ(eBulletWorldTransform_.rotation_.z);

	eBulletWorldTransform_.matWorld_.IdentityMatrix();

	//座標を行列に設定
	matTrans.Matrix4Translation(
		eBulletWorldTransform_.translation_.x,
		eBulletWorldTransform_.translation_.y,
		eBulletWorldTransform_.translation_.z);

	eBulletWorldTransform_.matWorld_ = matScale;
	eBulletWorldTransform_.matWorld_ *= matRot;
	eBulletWorldTransform_.matWorld_ *= matTrans;

	//WorldMatrix(eBulletWorldTransform_.matWorld_, matScale, matRot, matTrans);

	//親行列と掛け算代入
	/*if (eBulletWorldTransform_.parent_ != nullptr)
	{
		eBulletWorldTransform_.matWorld_ *= eBulletWorldTransform_.parent_->matWorld_;
	}*/

	//ワールド行列を転送
	eBulletWorldTransform_.TransferMatrix();

#pragma endregion

	if (--deathTimer_ <= 0)
	{
		isDead_ = true;
	}
}

//ゲッター
//bool IsDead() const { return isDead_; }

//描画
void EnemyBullet::Draw(const ViewProjection& viewProjection)
{
	eBulletModel_->Draw(eBulletWorldTransform_, viewProjection, eBulletHandle_);
}

void EnemyBullet::OnCollision()
{
	//デスフラグを立てる
	isDead_ = true;
}

Vector3 EnemyBullet::GetLocalPosition()
{
	//ワールド座標を入れる変数
	Vector3 worldPos;
	//ワールド行列の平行移動成分を取得(ワールド座標)
	worldPos = eBulletWorldTransform_.translation_;
	return worldPos;
}

float EnemyBullet::GetRadius()
{
	//半径を入れる変数
	float bulletRadius;
	//ワールド行列の平行移動成分を取得(ワールド座標)
	bulletRadius = radius;
	return bulletRadius;
}