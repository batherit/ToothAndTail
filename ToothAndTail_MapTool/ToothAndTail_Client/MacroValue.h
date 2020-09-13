#pragma once

#define BASE_SCALE			3.f

// 맵
#define MAP_WIDTH			2048
#define MAP_HEIGHT			2048

// 타일
#define TILE_RADIUS			21.f * BASE_SCALE
#define TILE_WIDTH			42
#define TILE_HEIGHT			28

// 커멘더
#define COMMANDER_WIDTH				72
#define COMMANDER_HEIGHT			72
#define COMMANDER_SPEED				100.f * BASE_SCALE
#define COMMANDER_DETECTION_RANGE	150.f * BASE_SCALE

// 제분소
#define WINDMILL_COST		60
#define WINDMILL_WIDTH		140
#define WINDMILL_HEIGHT		140
#define WINDMILL_RADIUS		42.f * BASE_SCALE
// 제분소 터빈
#define TURBINE_WIDTH		140
#define TURBINE_HEIGHT		140
// 제분소 토지
#define WINDMILL_GROUND_WIDTH	216 
#define WINDMILL_GROUND_HEIGHT	144
// 농장
#define FARMLAND_COST			60
#define FARMLAND_WIDTH			80
#define FARMLAND_HEIGHT			54
#define FARMLAND_FERTILITY		270	// 활성화된 농장의 초기 토지력(CROP 하나당 30.f의 지력이라 표현.)
#define FARMLAND_BUILD_SEC		5.f	// 농장을 활성화하는데 걸리는 시간초
// 작물
#define CROP_WIDTH		24
#define CROP_HEIGHT		22
//#define CROP_AMOUNT		30		// 작물 하나에서 얻을 수 있는 식량
// 돼지
#define PIG_WIDTH			32
#define PIG_HEIGHT			32
#define PIG_SPEED			3.f * BASE_SCALE
#define PIG_GEN_SEC			5.f
#define PIG_CROP_TIME		0.9f		// PIG_CROP_TIME마다 PIG_CROP_AMOUNT만큼 수확
#define PIG_CROP_AMOUNT		1	
#define PIG_COST			60
#define PIG_SUPPLY_NUM		1
#define PIG_DETECTION_RANGE	120.f * BASE_SCALE	// 돼지는 감지범위가 곧 공격범위다.
#define PIG_BULLET_DAMAGE	0 //5
#define PIG_BULLET_SPEED	250.f * BASE_SCALE
#define PIG_ATTACK_DELAY	1.f		

// 땅굴
// 1) 소형
#define TUNNEL_SMALL_WIDTH			40
#define TUNNEL_SMALL_HEIGHT			40
#define TUNNEL_SMALL_MAX_HP			70.f
#define TUNNEL_SMALL_BUILD_SEC		7.f
#define TUNNEL_SMALL_BUILD_COST		60
// 2) 중형
#define TUNNEL_MIDDLE_WIDTH			54
#define TUNNEL_MIDDLE_HEIGHT		54
#define TUNNEL_MIDDLE_MAX_HP		100.f
#define TUNNEL_MIDDLE_BUILD_SEC		10.f
#define TUNNEL_MIDDLE_BUILD_COST	120

// 3) 대형
#define TUNNEL_BIG_WIDTH			70
#define TUNNEL_BIG_HEIGHT			70
#define TUNNEL_BIG_MAX_HP			130.f
#define TUNNEL_BIG_BUILD_SEC		13.f
#define TUNNEL_BIG_BUILD_COST		180

#define UNIT_RADIUS					9.f * BASE_SCALE
// 유닛 //청솔모(SQUIRREL), 도마뱀(LIZARD), 두더지(MOLE), 스컹크(SKUNK), 오소리(BADGER), 여우(FOX)
// 1) 청솔모(SQUIRREL, 소형)
#define SQUIRREL_COST				20
#define SQUIRREL_SUPPLY_NUM			3
#define SQUIRREL_GEN_SEC			5.f
#define SQUIRREL_WIDTH				26
#define SQUIRREL_HEIGHT				26
#define SQUIRREL_SPEED				50.f * BASE_SCALE
#define SQUIRREL_DETECTION_RANGE	80.f * BASE_SCALE
#define SQUIRREL_ATTACK_RANGE		80.f * BASE_SCALE
#define SQUIRREL_BULLET_DAMAGE		3
#define SQUIRREL_BULLET_SPEED		250.f * BASE_SCALE
#define SQUIRREL_ATTACK_DELAY		1.f	
// 2) 도마뱀(LIZARD, 소형)
#define LIZARD_COST				25
#define LIZARD_SUPPLY_NUM		3
#define LIZARD_GEN_SEC			5.f
#define LIZARD_WIDTH			32
#define LIZARD_HEIGHT			32
#define LIZARD_SPEED			50.f * BASE_SCALE
#define LIZARD_DETECTION_RANGE	90.f * BASE_SCALE
#define LIZARD_ATTACK_RANGE		90.f * BASE_SCALE
#define LIZARD_BULLET_DAMAGE	5
#define LIZARD_BULLET_SPEED		250.f * BASE_SCALE
#define LIZARD_ATTACK_DELAY		1.f	
// 3) 두더지(MOLE, 소형)
#define MOLE_COST				40
#define MOLE_SUPPLY_NUM			2
#define MOLE_GEN_SEC			5.f
#define MOLE_WIDTH				28
#define MOLE_HEIGHT				28
#define MOLE_DETECTION_RANGE	90.f * BASE_SCALE
#define MOLE_ATTACK_RANGE		40.f * BASE_SCALE
#define MOLE_SPEED				50.f * BASE_SCALE
#define MOLE_BULLET_DAMAGE		0//8
#define MOLE_BULLET_SPEED		300.f * BASE_SCALE
#define MOLE_ATTACK_DELAY		0.6f	
// 4) 스컹크(MOLE, 중형)
#define SKUNK_COST				60
#define SKUNK_SUPPLY_NUM		2
#define SKUNK_GEN_SEC			5.f
#define SKUNK_WIDTH				32
#define SKUNK_HEIGHT			32
#define SKUNK_SPEED				50.f * BASE_SCALE
#define SKUNK_DETECTION_RANGE	100.f * BASE_SCALE
#define SKUNK_ATTACK_RANGE		100.f * BASE_SCALE
#define SKUNK_BULLET_DAMAGE		12
#define SKUNK_BULLET_SPEED		250.f * BASE_SCALE
#define SKUNK_ATTACK_DELAY		1.f	
// 5) 오소리(BADGER, 대형)
#define BADGER_COST				120
#define BADGER_SUPPLY_NUM		2	
#define BADGER_GEN_SEC			5.f
#define BADGER_WIDTH			52
#define BADGER_HEIGHT			52
#define BADGER_SPEED			50.f * BASE_SCALE
#define BADGER_DETECTION_RANGE	100.f * BASE_SCALE
#define BADGER_ATTACK_RANGE		100.f * BASE_SCALE
#define BADGER_BULLET_DAMAGE	5
#define BADGER_BULLET_SPEED		250.f * BASE_SCALE
#define BADGER_ATTACK_DELAY		0.3f	
// 6) 여우(FOX, 대형)
#define FOX_COST				120
#define FOX_SUPPLY_NUM			2
#define FOX_GEN_SEC			5.f
#define FOX_WIDTH				72
#define FOX_HEIGHT				72
#define FOX_SPEED				50.f * BASE_SCALE
#define FOX_DETECTION_RANGE		120.f * BASE_SCALE	
#define FOX_ATTACK_RANGE		120.f * BASE_SCALE
#define FOX_BULLET_DAMAGE		20
#define FOX_BULLET_SPEED		450.f * BASE_SCALE
#define FOX_ATTACK_DELAY		1.f	

// 폭발 이펙트
#define BURST_WIDTH			120
#define BURST_HEIGHT			170

// 금액
#define MIN_MONEY	static_cast<UINT>(0)
#define MAX_MONEY	static_cast<UINT>(9999)

// 정지 범위
#define STOP_RANGE	10.f * BASE_SCALE

// AI
#define AI_ATTACK_UNITS_NUM		15	// 병력이 15가 되면 공격하러 간다.
#define AI_BUILD_MIN_COST		60	// 자본이 60이 되면 농장, 땅굴 중 하나를 설치
// 자본이 60이상인데 자기 자신의 제분소가 없다면, 다른 제분소를 점령한다.
// 점령할 제분소마저 없다면, 그냥 왔다갔다 한다.//