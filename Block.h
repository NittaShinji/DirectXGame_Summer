#pragma once

#include "Audio.h"
#include "WorldTransform.h"
#include "Model.h"
#include "Input.h"
#include "Debugtext.h"
#include "Monster.h"
#include <list>
#include <memory>

static const int blockWidth = 13;
static const int blockHeight = 13;

class Block
{
public:

	
	//形態フェーズ
	enum class Form
	{
		Block = 0,		//生まれていない状態
		IsSelected,		//選択されている状態(ブロック状態のまま)
		WasSelected,	//選択された状態	(ブロックがない状態)
		None,			//何もない状態
		Slime,			//スライム
	};

	//行動フェーズ
	enum class Phase
	{
		Approach,	//接近する
		Leave,		//離脱する
	};

	Block();
	~Block();

	//初期化
	void Initialize(Model* model, const Vector3& position);
	//更新
	void Update();
	//描画
	void Draw(const ViewProjection& viewProjection);

	//ワールド座標を取得
	Vector3 GetSelectPosition();

	void GetWorldPosition(Vector3 blockPos[blockWidth][blockHeight]);

	//モンスター出現用に座標を渡す関数
	//Vector3 GetBlockPosition();


	//モンスターを出現させてもいいかどうか教えるフラグ関数
	bool GetBirthMonster();

	// 衝突を検知したら呼び出されるコールバック関数
	//bool OnCollision(Vector3 wallPos[blockWidth][blockHeight],const int x,const int y);
	bool OnCollision(const int x,const int y);


	//モンスターリストを取得
	const std::list<std::unique_ptr<Monster>>& GetMonsters() { return monsters_; }

	//モンスターを生成する
	void Birth();

	//半径を取得
	float GetRadius();

	//壁の横の長さを渡す
	int GetBlockWidth();
	//壁の縦の長さを渡す
	int GetBlockHight();

	//スコアを渡す
	int GetScore();

	int GetBreakWallCount();

	bool GetBreakAll();

	void IsSceneChanged();

	void IsMonsterDead();

private:

	// ワールド変換データ
	//WorldTransform worldTransforms_[blockWidth][blockHeight];
	WorldTransform worldTransforms_[blockWidth][blockHeight];

	// モデル
	Model* model_ = nullptr;
	// インプット
	Input* input_ = nullptr;
	// デバックテキスト
	DebugText* debugText_ = nullptr;

	Audio* audio_ = nullptr;

	// テクスチャハンドル
	uint32_t blockHandle_ = 0u;
	uint32_t selectHandle_ = 0u;
	uint32_t wasSelectHandle_ = 0u;
	uint32_t slimeHandle_ = 0u;

	//音楽
	uint32_t iceAxAudio_ = 0u;

	//状態変化変数
	Form form_[blockWidth][blockHeight] = { Form::Block };
	//Form form_ = { Form::Block };

	//キャラクターの大きさ
	float scaleX = 8.0f;
	float scaleY = 8.0f;
	float scaleZ = 8.0f;

	//前のフレームにおいて選択用のブロックの情報を保存しておく変数
	int prevBlockX;
	int prevBlockY;

	//選択可能かどうかの変数
	bool changedSelect;
	bool wasChangedSelect;

	//選択カーソルのクールタイマーの設定時間
	static const int32_t kSelectTime = 17;

	//選択カーソルのクールタイマー
	int32_t selectTimer_ = kSelectTime;
	int32_t wasSelectTimer_ = kSelectTime;

	//モンスター
	std::list<std::unique_ptr<Monster>> monsters_;

	//ブロックが破壊されたフラグ
	bool breakBlock[blockWidth][blockHeight];
	//bool birthMonster;

	//モンスターが生まれたフラグ
	bool birthMonster;

	//モンスター初期化用の変数
	Vector3 worldMonsterTransform;

	//bool breakBlock[blockWidth][blockHeight] = { 0 };

	//bool breakBlock2[blockWidth][blockHeight] = { 0 };


	//ブロック破壊後の孵化用の設定時間
	static const int32_t kBirthTime = 90;
	//孵化時間
	int32_t birthTimer_[blockWidth][blockHeight] = { 0 };
	//int32_t wasSelectTimer_ = kSelectTime;


	//ブロックが選択されているか
	//int isSelected[blockWidth][blockHeight] = { 0 };

	////個々のブロック用の状態変化変数
	//int formChange[blockWidth][blockHeight] = { 0 };

	//ワールド座標を入れる変数
	Vector3 monsterPos;

	//衝突した壁を入れる用の変数
	Vector3 collisionPos;

	//半径
	const float radius = 4.0f;

	//スコア
	int score;

	//壁が何枚壊れたか
	int breakWallCount;

	//デスフラグの管理
	int comeDeath;

	//int isBreakWall[blockWidth][blockHeight];

	
};

