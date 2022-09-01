#pragma once

#include "WorldTransform.h"
#include "Model.h"
#include "Debugtext.h"
#include "ViewProjection.h"

class Score
{
public:
	Score();
	~Score();

	//初期化
	void Initialize(Model* model);
	//更新
	void Update();
	//描画
	void Draw(const ViewProjection& viewProjection);

	void GetScore(int score);

	void UpdateTransform();

private:

	// ワールド変換データ
	//WorldTransform worldTransforms_[10];
	WorldTransform worldTransform_;

	// モデル
	Model* model_ = nullptr;

	// テクスチャハンドル
	//uint32_t textureHandle_ = 0u;

	// 数字ハンドル
	uint32_t numHandle_[10];

	// デバックテキスト
	DebugText* debugText_ = nullptr;
	
	int score;

	//スコアの割る変数
	int divideNum;

	/// <summary>
	/// スコアを配列の要素数に変換
	/// </summary>
	  
	
	
	int scoreToCount = 0;
	int scoreToCountTen = 0;

	//モデルの大きさ
	float scaleX = 8.0f;
	float scaleY = 8.0f;
	float scaleZ = 8.0f;

};
