#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "DebugText.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Player.h"
//#include "Monster.h"
#include "Skydome.h"
#include "RailCamera.h"
#include "Block.h"
#include "Select.h"
#include "Scene.h"
#include "Score.h"
#include "AirCamera.h"

#include <memory>

/// <summary>
/// ゲームシーン
/// 
 //形態フェーズ
enum SceneName
{
	TITLE,
	GAME,
	PAUSE,
	GAMECLEAR,
	GAMEOVER,
};
/// </summary>
class GameScene {

	enum PartID
	{
		Center,
		Right,
		Left,
		Up,
		Down,
	};

  public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	//衝突判定と応答
	void CheckAllCollisions();

	//スコア計算
	void CheckScore();

	//デスフラグを渡す
	void resetDeath();

	void reset();

  private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	DebugText* debugText_ = nullptr;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
	/// 
	/// //単位行列
	Matrix4 frontVec;

	//オーディオ
	uint32_t clickAudio_ = 0u;
	

	//テクスチャハンドル
	uint32_t textureHandle_ = 0;
	uint32_t textureRayHandle_ = 0;
	uint32_t textureGroundHandle_ = 0;

	//3Dモデル
	Model* model_ = nullptr;
	Model* rayModel = nullptr;
	Model* modelSkydome_ = nullptr;

	//ワールドトランスフォーム
	//光線
	WorldTransform  ray = { 0 };

	//始点
	Vector3 startray = { 1,0,0 };
	//終点
	Vector3 endaray = { 1,0,1 };

	//オブジェクトの半径
	float objectRadius = 0.0f;

	//当たり判定フラグ
	bool rayCollision = 0;

	float moveCount = 0.0f;

	//WorldTransform rays = { 0 };

	//ビュープロジェクション
	ViewProjection viewProjection_;

	//ビューフラグ
	bool viewChange = 0;

	//プレイヤーの生成
	std::shared_ptr<Player> player_;
	////敵の生成
	//std::unique_ptr<Monster> Monster_;
	//天球の生成
	std::unique_ptr<Skydome> skydome_;
	//レールカメラの生成
	//std::shared_ptr<RailCamera> railCamera_;

	//ブロックの生成
	std::shared_ptr<Block> block_;
	//カーソルの生成
	std::unique_ptr<Select> select_;

	//スコアクラスの作成
	std::unique_ptr<Score> score_;

	//エアーカメラクラスの作成
	std::unique_ptr<AirCamera> airCamera_;

	//シーンクラスの作成
	//Scene* scene;
	std::unique_ptr<Scene> scene_;

	//Monster* monster__;

	//シーン
	int scene;

	//敵の初期座標
	//Vector3 enemyPos = { 10,0,45 };

	//モンスターの初期座標
	Vector3 monsterPos = { -100,5,15 };

	//受け取り用の壁変数
	Vector3 wallPos[blockWidth][blockHeight];

	////レールカメラの初期座標
	//Vector3 railPos = { 0,0,0 };

	////レールカメラの初期座標
	//Vector3 angle = { 0,0,0 };

	//空気カメラの初期座標
	Vector3 airPos = { 0,250,-150 };

	//空気カメラの初期座標
	Vector3 airAngle = { 1.4,0,0 };

	//AB間の距離
	Vector3 distance;

	//デバッグカメラ
	DebugCamera* debugCamera_ = nullptr;

	//カメラ上方向の角度
	float viewAngle = 0.0f;

	//モンスターの移動をしていいのかフラグ
	bool possibleMove[4];

	//スコア関係
	int scoreMonster;
	int scoreBlock;
	int score;

	//壁が何枚壊れたか
	int breakWallCount_;

	//敵が弾とぶつかったよフラグ
	bool nowCollision;
	
	//敵が何体消えたか
	int deadMonsterCount;

	//寿命
	static const int32_t kLifeTime = 800 * 5;

	//デスタイマー
	int32_t limitTimer_;
	//デスフラグ
	bool isDead_ = false;

};
