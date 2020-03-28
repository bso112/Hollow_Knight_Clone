#pragma once

#ifndef __DEFINE_H__
#define __DEFINE_H__

#define WINCX 1000
#define WINCY 600

#define SAFE_DELETE(p) if(p) { delete p; p = nullptr; }
#define SAFE_DELETE_ARR(p) if(p) { delete[] p; p = nullptr; }


#define OBJ_NOEVENT 0
#define OBJ_DEAD 1
#define OBJ_FREEZE 2

#define PI 3.141592f

#define TILEX 500
#define TILEY 100
#define TILECX 20
#define TILECY 20

#define DIR_LEN 100
#define GRAVITY 5.f



#endif // !__DEFINE_H__
