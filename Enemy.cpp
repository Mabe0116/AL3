#include "Enemy.h"
#include <cassert>
#include "MyMath.h"


Enemy ::~Enemy() {}

void Enemy::Initialize(Model* model, uint32_t textureHandle) {
	assert(model);
	model_ = model;
	textureHandle_ = textureHandle;
	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();

	worldTransform_.translation_.z = 100;
}


void Enemy::Update() {
	// 敵の移動ベクトル
	Vector3 move = {0, 0, -0.2f};

	Move(worldTransform_.translation_, move);
	worldTransform_.UpdateMatrix();
}


void Enemy::Draw(ViewProjection viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}



