﻿#pragma once

/// <summary>
/// 3次元ベクトル
/// </summary>
class Vector3 {
  public:
	float x; // x成分
	float y; // y成分
	float z; // z成分

  public:

	// コンストラクタ
	Vector3();                          // 零ベクトルとする
	Vector3(float x, float y, float z); // x成分, y成分, z成分 を指定しての生成

	//内積を求める
	float dot(const Vector3& v) const;
	//外積を求める
	Vector3 cross(const Vector3& v) const;	

	  
	// 単項演算子オーバーロード
	Vector3 operator+() const;
	Vector3 operator-() const;

	// 代入演算子オーバーロード
	Vector3& operator+=(const Vector3& v);
	Vector3& operator-=(const Vector3& v);
	Vector3& operator*=(float s);
	Vector3& operator/=(float s);

	float length() const;
	Vector3& normalize();
};

const Vector3 operator*(const Vector3& v, float s);
const Vector3 operator*(float s, const Vector3& v);
