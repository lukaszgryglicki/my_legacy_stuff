#ifndef __DEFINES_H__MDBMA_
#define __DEFINES_H__MDBMA_
#define TRACE_LINE { printf("file: %s line: %d\n", __FILE__, __LINE__); fflush(stdout); }
#define FALSE 0
#define TRUE 1
#include <math.h>
#include <GL/glut.h>
#include <iostream>
#include <list>
#include <fstream>
using namespace std;
typedef double real;
#define BALLR 0.05
enum OBJ_ID_TYPE_ID
{
	OBJ_ID_NONE,
	OBJ_ID_mass,
	OBJ_ID_frame,
	OBJ_ID_BEZIER,
	OBJ_ID_room
};
#ifndef free_ptr
#define free_ptr(p)       { if(p) { delete (p);     (p)=NULL; } }
#endif    
#ifndef free_array
#define free_array(p) { if(p) { delete[] (p);   (p)=NULL; } }
#endif  
#ifndef MAX
#define MAX(x,y) (((x)>(y)) ? (x) : (y))
#endif

#endif
