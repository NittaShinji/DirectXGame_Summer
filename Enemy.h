#pragma once

#include "WorldTransform.h"
#include "Model.h"
#include "DebugCamera.h"
#include "ViewProjection.h"
#include "Debugtext.h"
#include "Input.h"

//行動フェーズ

class Enemy
{
	
public:

	enum class Phase
	{
		Cube,	//ブロック状態
		Move,	//移動する
		Dead	//死亡する
	};

	Enemy();
	~Enemy();

	//初期化
	void Initialize(Model* model, const Vector3& position);
	//更新
	void Update();
	//描画
	void Draw(const ViewProjection& viewProjection);

	void PhsaeCube();
	void PhsaeMove();


private:

	// ワールド変換データ
	WorldTransform enemyWorldTransforms_[100];
	// モデル
	Model* enemyModel_ = nullptr;
	// テクスチャハンドル
	uint32_t blockHandle_ = 0u;
	uint32_t enemyHandle_ = 0u;

	// インプット
	Input* input_ = nullptr;
	// デバックテキスト
	DebugText* debugText_ = nullptr;

	// 敵の速度
	const float kEnemySpeed = 0.1f;
	
	//フェーズ
	Phase phase_ = Phase::Cube;

	//ブロックが壊れているかどうか
	bool isblock[100] = { 0 };

	bool isMove[100] = { 0 };

	//接近フェーズの速度
	Vector3 CubeSpeed = { 0,0,0.1 };

	//離脱フェーズの速度
	Vector3 MoveSpeed = { 0,0,0.1 };

	//死亡フェーズの速度
	float deadSpeed = 0.1;

	//移動の切り替えフラグ
	bool moveChange[100] = { 0 };

	//寿命
	static const int32_t kLifeTime = 400 * 5;

	//デスタイマー
	int32_t blockTimer_[100];
};

