#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "WinApp.h"
#include "AxisIndicator.h"
#include "PrimitiveDrawer.h"
#include<random>
#define PI  3.14159

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete model_;
	delete debugCamera_;
	delete player_;
	delete enemy_;
}
float GameScene::Angle(float angle) {
	return angle * PI / 180;
}
float Degree(const float& degree) {
	float n = degree * PI / 180;
	return n;
}



void GameScene::Initialize() {

	textureHandle_ = TextureManager::Load("mario.png");
	enemyTexHandle_ = TextureManager::Load("teki.png");
	model_ = Model::Create();

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();

	player_ = new Player();
	player_->Initialize(model_, textureHandle_);

	enemy_ = new Enemy();
	enemy_->Initialize(model_, enemyTexHandle_);
	//敵キャラに自キャラのアドレスを渡す
	enemy_->SetPlayer(player_);



	//カメラ垂直方向視野角を設定
	viewProjection_.fovAngleY = Angle(50.0f);
	//アス比を設定
	//viewProjection_.aspectRatio = 1.0f;
	//ニアクリップ距離を設定
	viewProjection_.nearZ = 1.0f;
	//ファークリップ距離を設定
	viewProjection_.farZ = 180.0f;

	//ビュープロジェクション 初期化
	viewProjection_.Initialize();


	//乱数シード設定
	std::random_device speed_gen;
	//メルセンヌ・ツイスター
	std::mt19937_64 engine(speed_gen());

	//乱数範囲(回転角用)
	std::uniform_real_distribution<float> rotDist(0.0f, 3.141592654f);
	//乱数範囲(座標用)
	std::uniform_real_distribution<float> posDist(-10.0f, 10.0f);
	//乱数エンジンを渡し、指定範囲からランダムで数値を得る
	float rotValue = rotDist(engine);
	float posValue = posDist(engine);

	//軸方向表示を有効に
	AxisIndicator::GetInstance()->SetVisible(true);
	//軸方向表示が参照するビュープロジェクションを指定(アドレス)
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);

	//ライン描画が参照するビュープロジェクションを指定(アドレス)
	PrimitiveDrawer::GetInstance()->SetViewProjection(&viewProjection_);



}

void GameScene::Update() {//----------------------

	//視点移動処理
#pragma region Eye

//視点の移動ベクトル
	{

		Vector3 move = { 0,0,0 };
		//視点移動の速さ
		const float kEyespeed = 0.2f;
		//押した方向にベクトルを変更
		if (input_->PushKey(DIK_W)) {
			move = { 0,0,kEyespeed };
		}
		else if (input_->PushKey(DIK_S)) {
			move = { 0,0,-kEyespeed };
		}

		if (input_->PushKey(DIK_D)) {
			move = { kEyespeed,0,0 };
		}
		else if (input_->PushKey(DIK_A)) {
			move = { -kEyespeed,0,0 };
		}
		//視点の移動(ベクトルを加算)
		viewProjection_.eye.x += move.x;
		viewProjection_.eye.y += move.y;
		viewProjection_.eye.z += move.z;
		//行列の再計算
		viewProjection_.UpdateMatrix();


	}


#pragma endregion

	//注視点移動処理
#pragma region Target

	//注視点のベクトル
	{
		/*
		Vector3 move = { 0,0,0 };

		//注視点の移動速度
		const float kTargetSpeed = 0.2f;

		//押した方向にベクトルを変更
		if (input_->PushKey(DIK_LEFT)) {
			move = { -kTargetSpeed,0,0 };
		}
		else if (input_->PushKey(DIK_RIGHT)) {
			move = { kTargetSpeed,0,0 };
		}

		//注視点を移動(ベクトルを加算)
		viewProjection_.target.x += move.x;
		viewProjection_.target.y += move.y;
		viewProjection_.target.z += move.z;

		//行列の再計算
		viewProjection_.UpdateMatrix();
		*/

	}
#pragma endregion

	//上方向回転処理
#pragma region UpRot
	{
		/*
				//上方向の回転の速さ[ラジアン/f]
				const float kUpRotSpeed = 0.05f;

				//押した方向で移動ベクトルを変更
				if (input_->PushKey(DIK_SPACE)) {
					viewAngle += kUpRotSpeed;
					//2πを超えたら0に戻す
					viewAngle = fmodf(viewAngle,PI*2 );
				}

				//上方向のベクトルを計算(半径１の円周上の座標)
				viewProjection_.up = { cosf(viewAngle),sinf(viewAngle),0.0f };

				//行列の再計算
				viewProjection_.UpdateMatrix();
		*/


	}

#pragma endregion

	//FoV変更処理
#pragma region Fov

	//押した方向にベクトルを変更
	{

		if (input_->PushKey(DIK_S)) {//Sキーで視野角が広がる
			viewProjection_.fovAngleY += 0.01f;
			viewProjection_.fovAngleY = min(viewProjection_.fovAngleY, PI);
		}
		else if (input_->PushKey(DIK_W)) {//Wキーで視野角が狭まる
			viewProjection_.fovAngleY -= 0.01f;
			viewProjection_.fovAngleY = max(viewProjection_.fovAngleY, 0.01f);
		}
		//行列の再計算
		viewProjection_.UpdateMatrix();

		//debugText_->SetPos(50, 110);
		//debugText_->Printf("fovAngleY(Degree):%f", Degree(viewProjection_.fovAngleY));

	}
#pragma endregion

	//クリップ距離変更処理
#pragma region Near
	{
		if (input_->PushKey(DIK_UP)) {
			viewProjection_.nearZ += 0.1f;
			viewProjection_.nearZ = min(viewProjection_.nearZ, 200.0f);
		}
		else if (input_->PushKey(DIK_DOWN)) {
			viewProjection_.nearZ -= 0.1f;
			viewProjection_.nearZ = max(viewProjection_.nearZ, 0.1f);

		}
		//行列の再計算
		viewProjection_.UpdateMatrix();

		// デバック用表示
		//debugText_->SetPos(50, 130);
		//debugText_->Printf("nearZ: % f", viewProjection_.nearZ);
	}
#pragma endregion



	player_->Update();
	if (enemy_ != nullptr)enemy_->Update();
	CheckAllCollision();

	debugText_->SetPos(50, 110);
	debugText_->Printf("WASD:camera");
	debugText_->SetPos(50, 130);
	debugText_->Printf("ARROW:moob");
	debugText_->SetPos(50, 150);
	debugText_->Printf("SPACE:fire");
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

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる-----------------
	/// </summary>

	player_->Draw(viewProjection_);
	if (enemy_ != nullptr) enemy_->Draw(viewProjection_);


	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region PrimitiveDrawer

	/*
#pragma region DrawLine
	const float LINE_MAX = 25;
	Vector3 lineStart;
	Vector3 lineEnd;
	Vector4 lineColo;

	for (float i = 0; i <= LINE_MAX; i += 1) {

		float lineW = 2.0f;
		lineStart = {
			-(LINE_MAX / 2) * lineW,
			-5,
			(-(LINE_MAX / 2) * lineW) + (lineW * i)
		};
		lineEnd = {
			 (LINE_MAX / 2) * lineW ,
			-5,
			(-(LINE_MAX / 2) * lineW) + (lineW * i)
		};
		lineColo = { 1.0f,0,0,1.0f };

		PrimitiveDrawer::GetInstance()->DrawLine3d(lineStart, lineEnd, lineColo);

		lineStart = {
			(-(LINE_MAX / 2) * lineW) + (lineW * i),
			-5,
			-(LINE_MAX / 2) * lineW,
		};
		lineEnd = {
			(-(LINE_MAX / 2) * lineW) + (lineW * i),
			-5 ,
			 (LINE_MAX / 2) * lineW };
		lineColo = { 0,0,1.0f,1.0f };

		PrimitiveDrawer::GetInstance()->DrawLine3d(lineStart, lineEnd, lineColo);

	}

#pragma endregion

	*/

#pragma endregion 

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// デバッグテキストの描画
	debugText_->DrawAll(commandList);
	//
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameScene::CheckAllCollision() {
	//判定対象AとBの座標
	Vector3 posA, posB;

	//自弾リストを取得
	const std::list<std::unique_ptr<PlayerBullet>>& playerBullets = player_->GetBullets();
	//敵弾リストを取得
	const std::list<std::unique_ptr<EnemyBullet>>& enemyBullets = enemy_->GetBullets();

#pragma	region	自キャラと敵弾の当たり判定
	//自キャラの座標
	posA = player_->GetWorldPosition();
	//自キャラと敵弾全ての当たり判定
	for (const std::unique_ptr<EnemyBullet>& eBullet : enemyBullets) {
		//敵弾の座標
		posB = eBullet->GetBulletPosition();
		//A,Bの距離
		Vector3 vecPos = Distance(posA, posB);
		float dis = Length(vecPos);
		//
		float	radius = player_->GetRadius() + eBullet->GetRadius();
		//判定
		if (dis <= radius) {
			//自キャラのコールバックを呼び出し
			player_->OnCollision();
			//敵弾のコールバックを呼び出し
			eBullet->OnCollision();
		}
	}
#pragma	endregion

#pragma region 自弾と敵キャラの当たり判定
	//敵弾の座標
	posA = enemy_->GetWorldPosition();
	//自キャラと敵弾全ての当たり判定
	for (const std::unique_ptr<PlayerBullet>& pBullet : playerBullets) {
		//自弾の座標
		posB = pBullet->GetBulletPosition();
		// A,Bの距離
		Vector3 vecPos = Distance(posA, posB);
		float dis = Length(vecPos);
		//
		float radius = enemy_->GetRadius() + pBullet->GetRadius();
		//判定
		if (dis <= radius) {
			//敵キャラのコールバックを呼び出し
			enemy_->OnCollision();
			//自弾のコールバックを呼び出し
			pBullet->OnCollision();
		}
	}
#pragma endregion

#pragma region 自弾と敵弾の当たり判定
	//自弾の座標
	for (const std::unique_ptr<PlayerBullet>& pBullet : playerBullets) {
		posA = pBullet->GetBulletPosition();
		//自キャラと敵弾全ての当たり判定
		for (const std::unique_ptr<EnemyBullet>& eBullet : enemyBullets) {
			//敵弾の座標
			posB = eBullet->GetBulletPosition();
			// A,Bの距離
			Vector3 vecPos = Distance(posA, posB);
			float dis = Length(vecPos);
			//
			float radius = enemy_->GetRadius() + pBullet->GetRadius();
			//判定
			if (dis <= radius) {
				//自弾のコールバックを呼び出し
				pBullet->OnCollision();
				//敵弾のコールバックを呼び出し
				eBullet->OnCollision();
			}
		}
	}
#pragma endregion

	/**/
}
