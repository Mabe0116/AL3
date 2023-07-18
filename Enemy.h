#pragma once
#include "WorldTransform.h"
#include "Model.h"


/// <summary>
/// 敵
/// </summary>
 
class Enemy {
public:
	void Initialize(Model* model, uint32_t textureHandle);
	void Update();
	void Draw(ViewProjection viewProjection);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Enemy();

private:
	// ワールド座標データ
	WorldTransform worldTransform_;
	// モデル
	Model* model_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;

};


