#ifndef _SUSZI_H__MDBMA_
#define _SUSZI_H__MDBMA_
#include "defs.hxx"
#include "tri.hxx"
#include "point3d.hxx"
#include "obj3d.hxx"
#include "matrix4d.hxx"
#include "bern_tri_base.hxx"

class scene;

class suszi : public object		/* suszara wczytywana z plikow ULI */
{
	bern_tri_tens_basis	my_BB;

	list<tri*>		tri_list;
	list<point3*>		points;
	list<vector3*>		vectors;
	scene*		scene_ptr;
public:
	suszi(int ID, scene* manager);
	suszi(std::istream &is, int ID, scene* manager);
	void MatrixMultiply(const matrix4& m);
	void chk_conf(void);
	~suszi(void);
	friend std::istream &operator>>(std::istream &is, suszi &x);
	list<tri*>& tris(void) { return tri_list; };
	list<point3*>& Points(void) { return points; };
	list<vector3*>& Mesh(void) { return vectors; };
	void Calculate(void);
	void opengl_draw(void);
};

#endif
