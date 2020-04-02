#pragma once

#ifndef __ENUM_H__
#define __ENUM_H__

namespace BULLET
{
	enum DIR { LEFT, RIGHT, UP, DOWN, END };
}

namespace OBJID
{

	enum ID { UI, PLAYER, MONSTER, TILE, WEAPON, TRAP, END };
	

}

namespace OBJTAG
{
	enum TAG { PLAYER, MONSTER, TILE, WEAPON, END };
}

namespace SAVEDATA
{
	enum TAG { PILLBUG, CHASER, TERRAIN, FLY, SPIKE, JUMPER, HUSKGAURD, END };
}


namespace TILE
{
	enum STATE { NORMAL, DAMAGED, BROKEN, STATE_END };
	enum TYPE { MOVEABLE, UNMOVABLE, TYPE_END };
}

#endif // !__ENUM_H__
