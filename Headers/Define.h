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

#define TILEX 160
#define TILEY 68
#define TILECX 25
#define TILECY 25

#define DIR_LEN 100




#endif // !__DEFINE_H__