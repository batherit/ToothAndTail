#pragma once

#define BASE_SCALE			3.f

// 맵
#define MAP_WIDTH			2048
#define MAP_HEIGHT			2048

// 타일
#define TILE_WIDTH			42
#define TILE_HEIGHT			28

// 커멘더
#define COMMANDER_WIDTH			72
#define COMMANDER_HEIGHT		72
#define COMMANDER_SPEED			100.f * BASE_SCALE

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
#define PIG_COST	60
#define PIG_SUPPLY_NUM	1

// 땅굴
// 1) 소형
#define TUNNEL_SMALL_WIDTH			40
#define TUNNEL_SMALL_HEIGHT			40
#define TUNNEL_SMALL_BUILD_SEC		5.f
#define TUNNEL_SMALL_BUILD_COST		60
// 2) 중형
#define TUNNEL_MIDDLE_WIDTH			54
#define TUNNEL_MIDDLE_HEIGHT		54
#define TUNNEL_MIDDLE_BUILD_SEC		5.f
#define TUNNEL_MIDDLE_BUILD_COST	120

// 3) 대형
#define TUNNEL_BIG_WIDTH			70
#define TUNNEL_BIG_HEIGHT			70
#define TUNNEL_BIG_BUILD_SEC		5.f
#define TUNNEL_BIG_BUILD_COST		180

// 유닛 //청솔모(SQUIRREL), 도마뱀(LIZARD), 두더지(MOLE), 스컹크(SKUNK), 오소리(BADGER), 여우(FOX)
// 1) 청솔모(SQUIRREL, 소형)
#define SQUIRREL_COST			20
#define SQUIRREL_SUPPLY_NUM		3
#define SQUIRREL_GEN_TIME		5.f
#define SQUIRREL_WIDTH			26
#define SQUIRREL_HEIGHT			26
#define SQUIRREL_SPEED			90.f * BASE_SCALE
// 2) 도마뱀(LIZARD, 소형)
#define LIZARD_COST				20
#define LIZARD_SUPPLY_NUM		3
#define LIZARD_GEN_TIME			5.f
#define LIZARD_WIDTH			32
#define LIZARD_HEIGHT			32
#define LIZARD_SPEED			90.f * BASE_SCALE
// 3) 두더지(MOLE, 소형)
#define MOLE_COST				60
#define MOLE_SUPPLY_NUM			2
#define MOLE_GEN_TIME			5.f
#define MOLE_WIDTH				28
#define MOLE_HEIGHT				28
#define MOLE_SPEED				85.f * BASE_SCALE
// 4) 스컹크(MOLE, 중형)
#define SKUNK_COST				120
#define SKUNK_SUPPLY_NUM		2
#define SKUNK_GEN_TIME			5.f
#define SKUNK_WIDTH				32
#define SKUNK_HEIGHT			32
#define SKUNK_SPEED				85.f
// 5) 오소리(BADGER, 대형)
#define BADGER_COST				180
#define BADGER_SUPPLY_NUM		2	
#define BADGER_GEN_TIME			5.f
#define BADGER_WIDTH			52
#define BADGER_HEIGHT			52
#define BADGER_SPEED			85.f
// 6) 여우(FOX, 대형)
#define FOX_COST				180
#define FOX_SUPPLY_NUM			2
#define FOX_GEN_TIME			5.f
#define FOX_WIDTH				72
#define FOX_HEIGHT				72
#define FOX_SPEED				90.f

// 금액
#define MIN_MONEY	static_cast<UINT>(0)
#define MAX_MONEY	static_cast<UINT>(9999)
