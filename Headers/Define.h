#pragma once

#ifndef __DEFINE_H__
#define __DEFINE_H__

#define WINCX 1000
#define WINCY 600

#define SAFE_DELETE(p) if(p) { delete p; p = nullptr; }
#define SAFE_DELETE_ARR(p) if(p) { delete[] p; p = nullptr; }


#define OBJ_NOEVENT 0
#define OBJ_DEAD 1

#define PI 3.141592f

#define TILEX 30
#define TILEY 20
#define TILECX 64
#define TILECY 64




#endif // !__DEFINE_H__
