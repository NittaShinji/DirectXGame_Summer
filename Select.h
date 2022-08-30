#pragma once
#include "WorldTransform.h"
#include "Model.h"
#include "Input.h"
#include "Debugtext.h"

//ブロッククラスの前方宣言
class Block;

class Select
{
public:
	Select();
	~Select();

	//初期化
	void Initialize(Model* model);
	//更新
	void Update();

	void receive();


	//描画
	void Draw(const ViewProjection& viewProjection);

	//セッター
	void SetBlock(std::shared_ptr<Block> block) { block_ = block; }

private:

	static const int blockWidth = 30;
	static const int blockHeight = 30;

	// ワールド変換データ
	WorldTransform worldTransform_;
	// モデル
	Model* model_ = nullptr;
	// インプット
	Input* input_ = nullptr;
	// デバックテキスト
	DebugText* debugText_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	//モデルの大きさ
	float scaleX = 8.0f;
	float scaleY = 8.0f;
	float scaleZ = 8.0f;

	std::shared_ptr<Block> block_ = nullptr;

	Vector3 boxPos;
};

