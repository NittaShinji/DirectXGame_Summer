#pragma once

#include "Model.h"
#include "Debugtext.h"

//自キャラの弾
class PlayerBullet
{
public:
	PlayerBullet();
	~PlayerBullet();

	void Initialize(Model* model, const Vector3& position,const Vector3& velocity);
	void Update();

	//ゲッター
	bool IsDead() const { return isDead_; }

	//描画
	void Draw(const ViewProjection& viewProjection);

	// 衝突を検知したら呼び出されるコールバック関数
	void OnCollision();

	void OnCollisionStop();

	//半径を取得
	float GetRadius();

	//ワールド座標を取得
	Vector3 GetLocalPosition();

	//ワールド座標を取得
	Vector3 GetWorldPosition();



private:

	// ワールド変換データ
	WorldTransform worldTransform_;
	// モデル
	Model* bulletModel_ = nullptr;

	// テクスチャハンドル
	uint32_t bulletHandle_ = 0u;

	//速度
	Vector3 velocity_;

	//寿命
	static const int32_t kLifeTime = 60 * 5;

	//デスタイマー
	int32_t deathTimer_ = kLifeTime;
	//デスフラグ
	bool isDead_ = false;

	//半径
	const float radius = 0.5f;

	// デバックテキスト
	DebugText* debugText_ = nullptr;

	//移動できるかどうか
	int canMoved;

	//結果を入れるためのベクトル
	Vector3 worldResultTransform;
};

