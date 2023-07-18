#pragma once
#include "Matrix4x4.h"
#include "Vector3.h"

//ベクトル変換
Vector3 TransformNormal(const Vector3& v, const Matrix4x4& m);

// プロトタイプ宣言
void Add(Vector3& v1, Vector3& v2);

Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2);
// アフィン変換行列の作成
Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rot, const Vector3& translate);

void Move(Vector3& transform, Vector3& move);
