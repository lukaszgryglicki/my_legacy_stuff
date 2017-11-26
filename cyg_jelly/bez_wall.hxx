#ifndef __BWALL_H__MDBMA__
#define __BWALL_H__MDBMA__
#include "defs.hxx"
#include "obj3d.hxx"
#include "bez_patch.hxx"


class bez_wall : public bez_patch, public object	/* platek beziera (sciany boczne zelki) */
{
protected:
	friend class scene;
public:
	bez_wall(int ID, int udegree, int vdegree, int usteps, int vsteps);
	~bez_wall(void);
	void calc_mesh(void);
	void opengl_draw(void);
};

#endif

