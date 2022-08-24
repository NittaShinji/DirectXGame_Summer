#pragma once

#include "WorldTransform.h"
#include "Model.h"
#include "DebugCamera.h"
#include "ViewProjection.h"
#include "Input.h"
#include "Debugtext.h"
#include "EnemyBullet.h"
#include <memory>
#include <list>

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
	void Fire();
	//方向計算
	Vector3 Root(Vector3 velocity, WorldTransform worldTransform_);
	//発射間隔
	static const int kFireInterval = 60;
	//描画
	void Draw(const ViewProjection& viewProjection);

	//接近フェーズ
	void PhsaeApproach();
	//接近フェーズ初期化
	void ApproachInitialize();
	void PhsaeLeave();

	
private:

	// ワールド変換データ
	WorldTransform enemyWorldTransform_;
	// モデル
	Model* enemyModel_ = nullptr;
	// インプット
	Input* input_ = nullptr;
	// デバックテキスト
	DebugText* debugText_ = nullptr;
	// テクスチャハンドル
	uint32_t enemyHandle_ = 0u;

	// 敵の速度
	const float kEnemySpeed = 0.1f;
	
	//フェーズ
	Phase phase_ = Phase::Approach;

	//接近フェーズの速度
	Vector3 ApproachSpeed = { 0.1,0,0 };

	//離脱フェーズの速度
	Vector3 LeaveSpeed = { 0.1,0,0 };

	//弾
	std::list<std::unique_ptr<EnemyBullet>> bullets_;

	//発射タイマー
	int32_t bulletCoolTimer = 0;

};

