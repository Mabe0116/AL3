﻿#include "Player.h"
#include "PlayerBullet.h"
#include <cassert>
#include "MyMath.h"
#include "ImGuiManager.h"


Matrix4x4 MakeRotateXMatrix(float radian) {
	Matrix4x4 result;
	result.m[0][0] = 1;
	result.m[0][1] = 0;
	result.m[0][2] = 0;
	result.m[0][3] = 0;

	result.m[1][0] = 0;
	result.m[1][1] = std::cos(radian);
	result.m[1][2] = std::sin(radian);
	result.m[1][3] = 0;

	result.m[2][0] = 0;
	result.m[2][1] = -std::sin(radian);
	result.m[2][2] = std::cos(radian);
	result.m[2][3] = 0;

	result.m[3][0] = 0;
	result.m[3][1] = 0;
	result.m[3][2] = 0;
	result.m[3][3] = 1;

	return result;
}

Matrix4x4 MakeRotateYMatrix(float radian) {
	Matrix4x4 result;
	result.m[0][0] = std::cos(radian);
	result.m[0][1] = 0;
	result.m[0][2] = -std::sin(radian);
	result.m[0][3] = 0;

	result.m[1][0] = 0;
	result.m[1][1] = 1;
	result.m[1][2] = 0;
	result.m[1][3] = 0;

	result.m[2][0] = std::sin(radian);
	result.m[2][1] = 0;
	result.m[2][2] = std::cos(radian);
	result.m[2][3] = 0;

	result.m[3][0] = 0;
	result.m[3][1] = 0;
	result.m[3][2] = 0;
	result.m[3][3] = 1;

	return result;
}

Player::~Player() {
	//bullet_の解放
	for (PlayerBullet* bullet : bullets_)
	{
		delete bullet;
	}
}


void Player::Initialize(Model* model, uint32_t textureHandle) {
	assert(model);
	model_ = model;
	textureHandle_ = textureHandle;
	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	input_ = Input::GetInstance();
	//X,Y,Z方向の回転を設定
	worldTransform_.rotation_ = {0.0f, 0.0f, 0.0f};
}

void Player::Update() {

	// デスフラグの立った弾を削除
	bullets_.remove_if([](PlayerBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});


	//キャラクターの移動ベクトル
	Vector3 move = { 0,0,0 };

	//キャラクターの移動の速さ
	const float kCharacterSpeed = 0.2f;

	//押した方向で移動ベクトルを変更(左右)
	if (input_->PushKey(DIK_LEFT))
	{
		move.x -= kCharacterSpeed;
	}
	else if (input_->PushKey(DIK_RIGHT))
	{
		move.x += kCharacterSpeed;
	}

	//押した方向で移動ベクトルを変更(上下)
	if (input_->PushKey(DIK_UP))
	{
		move.y += kCharacterSpeed;
	}

	if (input_->PushKey(DIK_DOWN))
	{
		move.y -= kCharacterSpeed;
	}

	// 回転の速さ[ラジアン]
	const float kRotSpeed = 0.02f;

	// 押した方向で移動ベクトルを変更
	if (input_->PushKey(DIK_A)) {
		worldTransform_.rotation_.y -= kRotSpeed;
	} else if (input_->PushKey(DIK_D)) {
		worldTransform_.rotation_.y += kRotSpeed;
	}

	const float kMoveLimitX = 30;
	const float kMoveLimitY = 10;
	
	//自機移動処理
	Move(worldTransform_.translation_, move);

	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLimitY);

	//キャラクターの攻撃処理
	Attack();

	//弾更新
	for (PlayerBullet* bullet : bullets_)
	{
		bullet->Update();
	}

	// 行列の転送
	worldTransform_.UpdateMatrix();

	//キャラクターの座標を画面表示する処理
	ImGui::Begin("Debug");

	float playerPos[] = 
	{
	    worldTransform_.translation_.x,
		worldTransform_.translation_.y,
	    worldTransform_.translation_.z
	};
	ImGui::SliderFloat3("PlayerPos", playerPos, 0, 1280);

	worldTransform_.translation_.x = playerPos[0];
	worldTransform_.translation_.y = playerPos[1];
	worldTransform_.translation_.z = playerPos[2];

	ImGui::End();

}

void Player::Draw(ViewProjection viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);

	//弾描画
	for (PlayerBullet* bullet : bullets_)
	{
		bullet->Draw(viewProjection);
	}
}

void Player::Attack() {

	if (input_->PushKey(DIK_SPACE)) {

		//弾の速度
		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0, 0, kBulletSpeed);

		//速度ベクトルを自機の向きに合わせて回転させる
		velocity = TransformNormal(velocity, worldTransform_.matWorld_);

		// 弾を生成し、初期化
		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Initialize(model_, worldTransform_.translation_,velocity);

		// 弾を登録する
		bullets_.push_back(newBullet);
	}
}

