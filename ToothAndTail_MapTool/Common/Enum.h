#pragma once

namespace KEY {
	enum E_KEY { KEY_LEFT, KEY_RIGHT, KEY_UP, KEY_DOWN, KEY_LBUTTON, KEY_RBUTTON, KEY_Q, KEY_E, KEY_W, KEY_A, KEY_S, KEY_D, KEY_LSHIFT, KEY_P, KEY_G, KEY_SPACE, KEY_R, KEY_TAB, KEY_M, KEY_F, KEY_Z, KEY_X, KEY_END };
}

namespace OBJ {
	enum E_DIRECTION { DIR_LEFT, DIR_RIGHT, DIR_UP, DIR_DOWN, DIR_END };
	enum E_COLLIDED { COL_LEFT, COL_RIGHT, COL_TOP, COL_BOTTOM, COL_END };
	enum E_TYPE { TYPE_PLAYER, TYPE_ENEMY, TYPE_ITEM, TYPE_PLAYER_BULLET, TYPE_PLAYER_BOMB, TYPE_ENEMY_BULLET, TYPE_END };
	enum E_MODEL_TYPE { MODEL_TYPE_ELLIPSE, MODEL_TYPE_RECTANGLE, MODEL_TYPE_LINE, MODEL_TYPE_END };
}

namespace TILE {
	enum E_TYPE { TYPE_NORMAL, TYPE_BLOCKING, TYPE_OK, TYPE_NO, TYPE_END }; 
}

namespace UNIT
{
	enum E_TYPE { TYPE_SQUIRREL, TYPE_LIZARD, TYPE_MOLE, TYPE_SKUNK, TYPE_BADGER, TYPE_FOX, TYPE_END  };
}

namespace MAP_OBJ {
	enum E_TYPE { TYPE_NONE, TYPE_TILE, TYPE_DECO, TYPE_END };
}

namespace PARTICLE {
	enum E_TYPE { TYPE_ELLIPSE, TYPE_RECTANGLE, TYPE_TRIANGLE, TYPE_LINE, TYPE_END };
}

namespace PLAYER {
	enum E_ATTACK_MODE { ATTACK_MODE_NORMAL, ATTACK_MODE_ENERGY, ATTACK_MODE_END };
}

namespace MONSTER {
	enum E_TYPE { TYPE_NORMAL, TYPE_BOSS, TYPE_END};
}

namespace ITEM {
	enum E_TYPE { TYPE_NONE, TYPE_POWER, TYPE_MAX_POWER, TYPE_BUDDY, TYPE_END};
}

namespace UI {
	enum E_GAUGE { GAUGE_VERTICAL, GAUGE_HORIZONTAL, GAUGE_END };
	enum E_PIVOT { PIVOT_LEFT/*, PIVOT_MIDDLE*/, PIVOT_END };
}

namespace BULLET {
	enum E_TYPE { TYPE_MONSTER, TYPE_PLAYER, TYPE_END };
}