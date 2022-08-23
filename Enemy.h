#pragma once

#include "Model.h"

//行動フェーズ

class Enemy
{
	
public:

	enum class Phase
	{
		Approach,	//接近する
		Leave,		//離脱する
	};

	Enemy();
	~Enemy();

	//初期化
	void Initialize(Model* model, const Vector3& position);
	//更新
	void Update();
	//描画
	void Draw(const ViewProjection& viewProjection);

	//敵の攻撃フェーズ
	void PhsaeApproach();
	void PhsaeLeave();

private:

	// ワールド変換データ
	WorldTransform enemyWorldTransform_;
	// モデル
	Model* enemyModel_ = nullptr;
	// テクスチャハンドル
	uint32_t enemyHandle_ = 0u;

	// 敵の速度
	const float kEnemySpeed = 0.1f;
	
	//フェーズ
	Phase phase_ = Phase::Approach;
	//接近フェーズの速度
	Vector3 ApproachSpeed = { 0,0,0.1 };
	//離脱フェーズの速度
	Vector3 LeaveSpeed = { 0,0,0.1 };

	//メンバ関数ポインタのテーブル
	static void (Enemy::*spFuncTable[])();

};

