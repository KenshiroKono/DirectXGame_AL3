#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "DebugText.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "DebugCamera.h"	

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

	DebugCamera* debugCamera_ = nullptr;


public:
	enum PartId {
		Root,	//大元
		Spine,	//脊椎	
		Chest,	//胴体
		Head,	//頭部
		ArmL,	//左腕
		ArmR,	//右腕
		Hip,	//腰部
		LegL,	//左足
		LegR,	//右足	
	};

	


  public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();


	/// <summary>
	/// 度数法を返す
	/// </summary>
	float Angle(float angle);
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

  private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	DebugText* debugText_ = nullptr;


	uint32_t textureHandle_ = 0;
	Model* model_ = nullptr;
	//ワールドトランスフォーム
	WorldTransform worldTransforms[100];
	//ビュープロジェクション
	ViewProjection viewProjection_;
	//カメラの上方向の角度
	float viewAngle = 0.0f;


	/// <summary>
	/// ゲームシーン用
	/// </summary>
};
