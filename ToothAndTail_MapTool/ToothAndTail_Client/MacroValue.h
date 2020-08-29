#pragma once

#define BASE_SCALE			3.f

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
#define PIG_CROP_SPEED	1.f		// 초당 수확 속도