#pragma once

#include "WorldTransform.h"
#include "Model.h"
#include "Input.h"
#include "Debugtext.h"

class Monster
{
public:

	//形態フェーズ
	enum class Form
	{
		Block = 0,		//生まれていない状態
		Slime,		//スライム
	};

	//行動フェーズ
	enum class Phase
	{
		Approach,	//接近する
		Leave,		//離脱する
	};

	Monster();
	~Monster();

	//初期化
	void Initialize(Model* model, const Vector3& position);
	//更新
	void Update();
	//描画
	void Draw(const ViewProjection& viewProjection);

private:

	static const int blockWidth = 10;
	static const int blockHeight = 10;

	// ワールド変換データ
	//WorldTransform worldTransforms_[blockWidth][blockHeight];
	WorldTransform worldTransforms_[blockWidth][blockHeight];

	// モデル
	Model* model_ = nullptr;
	// インプット
	Input* input_ = nullptr;
	// デバックテキスト
	DebugText* debugText_ = nullptr;

	// テクスチャハンドル
	uint32_t blockHandle_ = 0u;
	uint32_t slimeHandle_ = 0u;

	//状態変化変数
	Form form_[blockWidth][blockHeight] = { Form::Block };
	//Form form_ = { Form::Block };



	////個々のブロック用の状態変化変数
	//int formChange[blockWidth][blockHeight] = { 0 };

};

