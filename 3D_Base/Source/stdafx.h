// stdafx.h : 標準のシステム インクルード ファイルのインクルード ファイル、または
// 参照回数が多く、かつあまり変更されない、プロジェクト専用のインクルード ファイル
// を記述します.
//
#pragma once

//#include "targetver.h"

#include <stdio.h>
#include <tchar.h>

// TODO: プログラムに必要な追加ヘッダーをここで参照してください
#include <Windows.h>
#include<vector>
#include<unordered_map>
#include<memory>
#include<string>
#include<iostream>


#include <wrl/client.h>
using Microsoft::WRL::ComPtr;

#include "Global.h"
#include "MyMacro.h"
#include "Assets/AssetManager.h"
#include "DebugText/DebugDrawManager/DebugDrawManager.h"
#include "CDirectX11.h"
#include "CDirectX9.h"