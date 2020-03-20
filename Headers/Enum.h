#pragma once

#ifndef __ENUM_H__
#define __ENUM_H__

namespace BULLET
{
	enum DIR { LEFT, RIGHT, UP, DOWN, END };
}

namespace OBJID
{
	enum ID { PLAYER, MONSTER, BULLET, UI, MOUSE, END };
}


namespace SAVEDATA
{	
	//세이브시 타입을 알기 위함
	enum TAG {PILLBUG, TERRAIN, END};
}


namespace TILE
{
	enum STATE { NORMAL, DAMAGED, BROKEN, STATE_END };
	enum TYPE { MOVEABLE, UNMOVABLE, TYPE_END };
}

#endif // !__ENUM_H__
