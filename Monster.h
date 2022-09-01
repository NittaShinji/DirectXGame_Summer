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
#include <cassert>


//class Block;

class Monster
{
	
public:

	//形態フェーズ
	enum class MonsterForm
	{
		Slime,			//スライム
	};

	//行動フェーズ
	enum class Phase
	{
		Approach,	//接近する
		Leave,		//離脱する
	};

	//モンスターの向き
	enum  class Direction
	{
		Right = 0,
		Left,
		Up,
		Down,
	};

	Monster();
	~Monster();

	//初期化
	void Initialize(Model* model, const Vector3& position, const Vector3& velocity, bool birthMonster);
	//更新
	void Update();
	void Fire();
	//方向計算
	Vector3 Root(Vector3 velocity, WorldTransform worldTransform_);
	//発射間隔
	static const int kFireInterval = 180;
	//描画
	void Draw(const ViewProjection& viewProjection);

	//接近フェーズ
	void PhsaeApproach();
	//接近フェーズ初期化
	void ApproachInitialize();
	void PhsaeLeave();

	//void SetPlayer(std::unique_ptr<Player> player) { player_ = player; }
	
	//void SetPlayer(std::shared_ptr<Block> block) { block_ = block; }

	// 勇者の弾との衝突を検知したら呼び出されるコールバック関数
	void OnCollision();

	//移動できる際に呼び出されるコールバック関数
	void OnCollisionMove();
	//進めなくて方向転換の際に呼び出されるコールバック関数
	void ChangeDirection();

	//弾リストを取得
	const std::list<std::unique_ptr<EnemyBullet>>& GetBullets() { return bullets_; }

	//ローカル座標を取得
	Vector3 GetLocalPosition();

	//ワールド座標を取得
	Vector3 GetWorldPosition();


	//移動
	void Move();

	//半径を取得
	float GetRadius();

	void receive();

	bool IsDead() const { return isDead_; }

	void GetIsMove(bool possibleMove);

	int GetScore();

	void Dead();

private:

	// ワールド変換データ
	WorldTransform worldTransform_;

	// モデル
	Model* Model_ = nullptr;
	// インプット
	Input* input_ = nullptr;
	// デバックテキスト
	DebugText* debugText_ = nullptr;
	// テクスチャハンドル
	uint32_t monsterHandle_ = 0u;

	// 敵の速度
	const float kEnemySpeed = 0.3f;
	
	//フェーズ
	Phase phase_ = Phase::Approach;

	//モンスターの向き
	Direction direction_ = Direction::Right();

	//接近フェーズの速度
	Vector3 ApproachSpeed = { 0.1,0,0 };
	//離脱フェーズの速度
	Vector3 LeaveSpeed = { 0.1,0,0 };

	//弾
	std::list<std::unique_ptr<EnemyBullet>> bullets_;
	//発射タイマー
	int32_t bulletCoolTimer;

	//自キャラ
	/*std::shared_ptr<Player> player_ = nullptr;*/

	//ブロック情報
	//std::shared_ptr<Block> block_;

	//半径
	const float radius = 50.0f;

	//モンスターの生成座標
	//Vector3 boxPos;
	 
	//モンスターが誕生したかどうか
	bool birthMonster_;

	//寿命
	static const int32_t kLifeTime = 50 * 5;

	//デスタイマー
	int32_t deathTimer_ = kLifeTime;
	//デスフラグ
	bool isDead_ = false;

	//動いていいよフラグ
	bool isMoved;

	//初期移動フラグ
	bool firstMove = true;

	//方向転換する際の被らないようのチェック変数
	bool checkDirectionR = false;
	bool checkDirectionL = false;

	//ワールド座標を渡す用の変数
	Vector3 worldResultTransform;

	//スコア
	int score;

	//ループ用の死ぬフラグ
	int firstDead = true;
};

