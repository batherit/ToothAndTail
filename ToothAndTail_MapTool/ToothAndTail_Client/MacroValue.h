#pragma once

#define BASE_SCALE			3.f

// ��
#define MAP_WIDTH			2048
#define MAP_HEIGHT			2048

// Ÿ��
#define TILE_RADIUS			21.f * BASE_SCALE
#define TILE_WIDTH			42
#define TILE_HEIGHT			28

// Ŀ���
#define COMMANDER_WIDTH				72
#define COMMANDER_HEIGHT			72
#define COMMANDER_SPEED				100.f * BASE_SCALE
#define COMMANDER_DETECTION_RANGE	150.f * BASE_SCALE

// ���м�
#define WINDMILL_COST		60
#define WINDMILL_WIDTH		140
#define WINDMILL_HEIGHT		140
#define WINDMILL_RADIUS		42.f * BASE_SCALE
// ���м� �ͺ�
#define TURBINE_WIDTH		140
#define TURBINE_HEIGHT		140
// ���м� ����
#define WINDMILL_GROUND_WIDTH	216 
#define WINDMILL_GROUND_HEIGHT	144
// ����
#define FARMLAND_COST			60
#define FARMLAND_WIDTH			80
#define FARMLAND_HEIGHT			54
#define FARMLAND_FERTILITY		270	// Ȱ��ȭ�� ������ �ʱ� ������(CROP �ϳ��� 30.f�� �����̶� ǥ��.)
#define FARMLAND_BUILD_SEC		5.f	// ������ Ȱ��ȭ�ϴµ� �ɸ��� �ð���
// �۹�
#define CROP_WIDTH		24
#define CROP_HEIGHT		22
//#define CROP_AMOUNT		30		// �۹� �ϳ����� ���� �� �ִ� �ķ�
// ����
#define PIG_WIDTH			32
#define PIG_HEIGHT			32
#define PIG_SPEED			3.f * BASE_SCALE
#define PIG_GEN_SEC			5.f
#define PIG_CROP_TIME		0.9f		// PIG_CROP_TIME���� PIG_CROP_AMOUNT��ŭ ��Ȯ
#define PIG_CROP_AMOUNT		1	
#define PIG_COST			60
#define PIG_SUPPLY_NUM		1
#define PIG_DETECTION_RANGE	120.f * BASE_SCALE	// ������ ���������� �� ���ݹ�����.
#define PIG_BULLET_DAMAGE	0 //5
#define PIG_BULLET_SPEED	250.f * BASE_SCALE
#define PIG_ATTACK_DELAY	1.f		

// ����
// 1) ����
#define TUNNEL_SMALL_WIDTH			40
#define TUNNEL_SMALL_HEIGHT			40
#define TUNNEL_SMALL_MAX_HP			70.f
#define TUNNEL_SMALL_BUILD_SEC		7.f
#define TUNNEL_SMALL_BUILD_COST		60
// 2) ����
#define TUNNEL_MIDDLE_WIDTH			54
#define TUNNEL_MIDDLE_HEIGHT		54
#define TUNNEL_MIDDLE_MAX_HP		100.f
#define TUNNEL_MIDDLE_BUILD_SEC		10.f
#define TUNNEL_MIDDLE_BUILD_COST	120

// 3) ����
#define TUNNEL_BIG_WIDTH			70
#define TUNNEL_BIG_HEIGHT			70
#define TUNNEL_BIG_MAX_HP			130.f
#define TUNNEL_BIG_BUILD_SEC		13.f
#define TUNNEL_BIG_BUILD_COST		180

#define UNIT_RADIUS					9.f * BASE_SCALE
// ���� //û�ָ�(SQUIRREL), ������(LIZARD), �δ���(MOLE), ����ũ(SKUNK), ���Ҹ�(BADGER), ����(FOX)
// 1) û�ָ�(SQUIRREL, ����)
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
// 2) ������(LIZARD, ����)
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
// 3) �δ���(MOLE, ����)
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
// 4) ����ũ(MOLE, ����)
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
// 5) ���Ҹ�(BADGER, ����)
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
// 6) ����(FOX, ����)
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

// ���� ����Ʈ
#define BURST_WIDTH			120
#define BURST_HEIGHT			170

// �ݾ�
#define MIN_MONEY	static_cast<UINT>(0)
#define MAX_MONEY	static_cast<UINT>(9999)

// ���� ����
#define STOP_RANGE	10.f * BASE_SCALE