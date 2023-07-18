#include "PlayerBullet.h"
#include "model.h"
#include "worldtransform.h"
#include <cassert>
#include "MyMath.h"

void PlayerBullet::Initialize(Model* model, const Vector3& position, const Vector3& velocity) {
	// NULLポインタチェック
	assert(model);

	model_ = model;
	// テクスチャ読み込み
	textureHandle_ = TextureManager::Load("black.png");

	// ワールドトランスフォームの初期化
	// 引数で受け取った初期座標をセット
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;

	////因数で受け取った速度をメンバ変数に代入
	velocity_ = velocity;
}

void PlayerBullet::Update() { 
	worldTransform_.UpdateMatrix();
	////座標を移動させる(1フレーム分の移動量を足しこむ)
	Move(worldTransform_.translation_, velocity_);
	

		//時間経過でデス
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}
}

void PlayerBullet::Draw(const ViewProjection& viewProjection) {
	// モデルの描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}
