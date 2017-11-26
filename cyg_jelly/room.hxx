#ifndef __roomy_H__MDBMA__
#define __roomy_H__MDBMA__
#include "defs.hxx"
#include "vec3d.hxx"
#include "obj3d.hxx"


class room : public object	/* szescian od ktorego scian wewn. bedzie sie odbijac zelka */
{
protected:
	friend class scene;

	vector3	room_edge_pos[8];
	int			object_id;

	void create_obj(void);
public:
	room(int ID);
	const vector3* get_pos(int x, int y, int z) const;
	~room(void);
	void opengl_draw(void);
};

#endif


