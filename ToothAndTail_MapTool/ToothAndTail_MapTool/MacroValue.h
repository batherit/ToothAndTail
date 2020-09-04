#pragma once

#define BASE_SCALE			3.f

// ��
#define MAP_WIDTH			2048
#define MAP_HEIGHT			2048

// Ÿ��
#define TILE_WIDTH			42
#define TILE_HEIGHT			28

// Ŀ���
#define COMMANDER_WIDTH		72
#define COMMANDER_HEIGHT	72
#define COMMANDER_SPEED		100.f * BASE_SCALE

// ���м�
#define WINDMILL_WIDTH		140
#define WINDMILL_HEIGHT		140
// ���м� �ͺ�
#define TURBINE_WIDTH		140
#define TURBINE_HEIGHT		140
// ���м� ����
#define WINDMILL_GROUND_WIDTH	216 
#define WINDMILL_GROUND_HEIGHT	144
// ����
#define FARMLAND_WIDTH			80
#define FARMLAND_HEIGHT			54
#define FARMLAND_LAND_POWER		300.f	// Ȱ��ȭ�� ������ �ʱ� ������
#define FARMLAND_BUILD_SEC		5.f	// ������ Ȱ��ȭ�ϴµ� �ɸ��� �ð���
// �۹�
#define CROP_WIDTH		24
#define CROP_HEIGHT		22
// ����
#define PIG_WIDTH	32
#define PIG_HEIGHT	32
#define PIG_SPEED	3.f * BASE_SCALE
#define PIG_CROP_SPEED	1.f		// �ʴ� ��Ȯ �ӵ�
#define PIG_COST	60
#define PIG_SUPPLY_NUM	1

// ����
// 1) ����
#define TUNNEL_SMALL_WIDTH			40
#define TUNNEL_SMALL_HEIGHT			40
#define TUNNEL_SMALL_BUILD_SEC		5.f
#define TUNNEL_SMALL_BUILD_COST		60
// 2) ����
#define TUNNEL_MIDDLE_WIDTH			54
#define TUNNEL_MIDDLE_HEIGHT		54
#define TUNNEL_MIDDLE_BUILD_SEC		5.f
#define TUNNEL_MIDDLE_BUILD_COST	120

// 3) ����
#define TUNNEL_BIG_WIDTH			70
#define TUNNEL_BIG_HEIGHT			70
#define TUNNEL_BIG_BUILD_SEC		5.f
#define TUNNEL_BIG_BUILD_COST		180

// ���� //û�ָ�(SQUIRREL), ������(LIZARD), �δ���(MOLE), ����ũ(SKUNK), ���Ҹ�(BADGER), ����(FOX)
// 1) û�ָ�(SQUIRREL, ����)
#define SQUIRREL_COST			20
#define SQUIRREL_SUPPLY_NUM		3
// 2) ������(LIZARD, ����)
#define LIZARD_COST				20
#define LIZARD_SUPPLY_NUM		3
// 3) �δ���(MOLE, ����)
#define MOLE_COST				60
#define MOLE_SUPPLY_NUM			2
// 4) ����ũ(MOLE, ����)
#define SKUNK_COST				120
#define SKUNK_SUPPLY_NUM		2
// 5) ���Ҹ�(BADGER, ����)
#define BADGER_COST				180
#define BADGER_SUPPLY_NUM		2		
// 6) ����(FOX, ����)
#define FOX_COST				180
#define FOX_SUPPLY_NUM			2



