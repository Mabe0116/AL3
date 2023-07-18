#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "AxisIndicator.h"

GameScene::GameScene() {}

GameScene::~GameScene() 
{ 
	delete debugCamera_; 
	delete model_;
	delete player_;
	delete enemy_;
}
 
void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	//デバックカメラの生成
	debugCamera_ = new DebugCamera(1280, 720);
	viewProjection_.Initialize();
	TextureHandle_ = TextureManager::Load("debugfont.png");
	EnemyTextureHandle_ = TextureManager::Load("cube/cube.jpg");
	model_ = Model::Create();
	player_ = new Player();
	player_->Initialize(model_,TextureHandle_);
	enemy_ = new Enemy();
	enemy_->Initialize(model_, EnemyTextureHandle_);
	AxisIndicator::GetInstance()->SetVisible(true);
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);
}

void GameScene::Update() {

	player_ -> Update();

	enemy_->Update();

	// デバックカメラの更新
	debugCamera_->Update();

	#ifdef _DEBUG 
	if (input_->TriggerKey(DIK_1))
	{
		isDebugCameraActive_ = true;
	}
	#endif

//カメラの処理
	if (isDebugCameraActive_) 
	{
		
		viewProjection_.matView = debugCamera_ -> GetViewProjection().matView;
		viewProjection_.matProjection = debugCamera_ ->GetViewProjection().matProjection;
		//ビュープロジェクション行列の転送
		viewProjection_.TransferMatrix();
	} else {
	//ビュープロジェクション行列の更新と転送
		viewProjection_.UpdateMatrix();
	}



}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	player_->Draw(viewProjection_);

	enemy_->Draw(viewProjection_);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
