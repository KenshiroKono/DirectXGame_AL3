#pragma once


#include"WorldTransform.h"
#include"DirectXCommon.h"

Matrix4 matIdentity();
//スケーリング
Matrix4 Mat_size(WorldTransform& w);
//平行移動
Matrix4 Mat_move(WorldTransform& w);
//回転
Matrix4 Mat_rot(WorldTransform& w);
//合成
Matrix4 Mat(WorldTransform w);

Vector3 Vec_rot(Vector3 velocity, Matrix4 amount);