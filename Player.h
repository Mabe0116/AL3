#pragma once
#include "WorldTransform.h"
#include "Model.h"
#include "Input.h"
#include "PlayerBullet.h"
#include <list>
#pragma once

class Player {
public:
	void Initialize(Model* model, uint32_t textureHandle);
	void Update();
	void Draw(ViewProjection viewProjection);
	void Attack();
/// <summary>
/// デストラクタ
/// </summary>
	~Player();

private:
	//ワールド座標データ
	WorldTransform worldTransform_;
	//モデル
	Model* model_ = nullptr;
	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	//キーボード入力
	Input* input_ = nullptr;
	//弾
	std::list<PlayerBullet*> bullets_;
};
