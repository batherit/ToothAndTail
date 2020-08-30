#pragma once

#define BASE_SCALE			3.f

// 맵
#define MAP_WIDTH			2048
#define MAP_HEIGHT			2048

// 커멘더
#define COMMANDER_WIDTH		72
#define COMMANDER_HEIGHT	72
#define COMMANDER_SPEED		100.f * BASE_SCALE

// 제분소
#define WINDMILL_WIDTH		140
#define WINDMILL_HEIGHT		140
// 제분소 터빈
#define TURBINE_WIDTH		140
#define TURBINE_HEIGHT		140
// 제분소 토지
#define WINDMILL_GROUND_WIDTH	216 
#define WINDMILL_GROUND_HEIGHT	144
// 농장
#define FARMLAND_WIDTH			80
#define FARMLAND_HEIGHT			54
#define FARMLAND_LAND_POWER		300.f	// 활성화된 농장의 초기 토지력
#define FARMLAND_BUILD_SEC		5.f	// 농장을 활성화하는데 걸리는 시간초
// 작물
#define CROP_WIDTH		24
#define CROP_HEIGHT		22
// 돼지
#define PIG_WIDTH	32
#define PIG_HEIGHT	32
#define PIG_SPEED	3.f * BASE_SCALE
#define PIG_CROP_SPEED	1.f		// 초당 수확 속도

// 땅굴
// 1) 소형
#define TUNNEL_SMALL_WIDTH		40
#define TUNNEL_SMALL_HEIGHT		40
#define TUNNEL_SMALL_BUILD_SEC	5.f
// 2) 중형
#define TUNNEL_MIDDLE_WIDTH		54
#define TUNNEL_MIDDLE_HEIGHT	54
#define TUNNEL_MIDDLE_BUILD_SEC	5.f
// 3) 대형
#define TUNNEL_BIG_WIDTH		70
#define TUNNEL_BIG_HEIGHT		70
#define TUNNEL_BIG_BUILD_SEC	5.f