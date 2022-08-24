#pragma once

#include "Model.h"

class EnemyBullet
{
public:
	EnemyBullet();
	~EnemyBullet();

	void Initialize(Model* model, const Vector3& position, const Vector3& velocity);
	void Update();

	//ゲッター
	bool IsDead() const { return isDead_; }

	//描画
	void Draw(const ViewProjection& viewProjection);

private:

	// ワールド変換データ
	WorldTransform eBulletWorldTransform_;
	// モデル
	Model* eBulletModel_ = nullptr;

	// テクスチャハンドル
	uint32_t eBulletHandle_ = 0u;

	//速度
	Vector3 velocity_;

	//寿命
	static const int32_t kLifeTime = 60 * 5;

	//デスタイマー
	int32_t deathTimer_ = kLifeTime;
	//デスフラグ
	bool isDead_ = false;

};