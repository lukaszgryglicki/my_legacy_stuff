#ifndef __SCENE_H__MDBMA__
#define __SCENE_H__MDBMA__
#include "defs.hxx"
#include "bez_wall.hxx"
#include "frame.hxx"
#include "mass.hxx"
#include "obj3d.hxx"
#include "matrix4d.hxx"
#include "room.hxx"
#include "spring.hxx"
#include "canvas.hxx"
#include "suszi.hxx"

class scene		/* scena, zarzadzanie obiektami na scenie */
{
protected:
	friend class suszi;
	friend class glut_scene_window;
	int			current_id;
	bez_wall*   bezier_walls[6];
	suszi*		my_suszi;
	room*		my_room;
	mass**		my_array;
	frame*		my_frame;
	spring**	my_spring;
	spring*		my_springframe[8];
	int			my_dim;
	int			my_size;
	int			my_springsize;
	int         current_mass_sel;
public:
	real		delta;
	real		my_mass;
	real		my_K;
	real		my_Fade;
	real		my_c_internal;
	real		my_c_external;

	object*	my_current_selection;
	bool my_show_cube, my_show_bez_wall, my_show_room, my_show_frame, my_show_springs, my_show_suszi;
public:
	scene(void);
	inline int get_idx(const int x, const int y, const int z) const { return my_dim*my_dim*z+my_dim*y+x; };
	mass* operator[](int& index);
	mass* get_oposite_mass(int x, int y, int z);
	bool is_cube_created(void) const;
	bool is_frame_created(void) const;
	frame* get_frame(void) const;
	bool is_bez_wall_created(void) const;
	bool is_room_created(void) const;
	bool is_suszi_created(void) const;
	bez_wall* get_bez_wall(int index) const;
	object* get_selection(void) const;
	void move_selected(const matrix4& m);
	void show_cube(bool show = TRUE) { my_show_cube = show; };
	void show_suszi(bool show = TRUE) { my_show_suszi = show; };
	void show_bez_wall(bool show = TRUE) { my_show_bez_wall = show; };
	void show_frame(bool show = TRUE) { my_show_frame = show; };
	void show_room(bool show = TRUE) { my_show_room = show; };
	void show_springs(bool show = TRUE) { my_show_springs = show; };
	void clear_room(void);
	void clear_suszi(void);
	void clear_bez_wall(void);
	void clear_cube(void);
	void clear_attach(void);
	void clear_frame(void);
	void clear(void);
	void create_room(real xdim, real ydim, real zdim);
	void create_bez_wall(int usteps, int vsteps);
	void create_suszi(std::istream& is);
	void attach_frame(void);
	void create_frame(int a);
	void create_cube(int dim, int a);
	object* find_object(int ID);
	object* object_select(int ID);
	void object_select(mass* m);
	object* deselect(int ID);
	void deselect(object* m);
	void deselect_all(void);
	const bool is_object_selected(int ID);
	void opengl_draw(void);
	void run_physics_step();
	~scene(void);
};

#endif
