#include "defs.hxx"
#include "scene.hxx"

scene::scene(void) 
: current_id(0)
, my_size(0)
, my_springsize(0)
, my_spring(NULL)
, my_array(NULL)
, my_room(NULL)
, my_frame(NULL)
, my_current_selection(NULL) 
, delta(0.1)
, my_K(0.6)
, my_Fade(0.75)
, my_mass(1)
, my_c_internal(2)
, my_c_external(4)
, current_mass_sel(0)
{ 
	my_show_room = my_show_frame =  my_show_bez_wall = my_show_cube = my_show_suszi = true;
	my_dim = 0; my_size = 0; 
	for(int i = 0; i < 8; ++i) 
	{ 
		my_springframe[i] = NULL; 
		bezier_walls[i] = NULL; 
	} 
};

mass* scene::operator[](int& index) 
{ 
	if(!my_array || index < 0 || index >= my_size) 
		return NULL; 
	return my_array[index]; 
};

mass* scene::get_oposite_mass(int x, int y, int z) 
{ 
	if(!my_array || x < 0 || x >= my_dim || y < 0 || y >= my_dim || z < 0 || z >= my_dim) 
		return NULL; 
	return my_array[get_idx(x, y, z)]; 
};

bez_wall* scene::get_bez_wall(int index) const 
{ 
	if(index  < 0 || index > 5) 
		return NULL; 
	return bezier_walls[index]; 
};

bool scene::is_cube_created(void) const 
{ 
	return (my_array != NULL); 
};

bool scene::is_frame_created(void) const 
{ 
	return (my_frame != NULL); 
}
;
bool scene::is_suszi_created(void) const 
{ 
	return (my_suszi != NULL); 
};

frame* scene::get_frame(void) const
{ 
	return my_frame; 
};

bool scene::is_bez_wall_created(void) const
{ 
	return (bezier_walls[0] != NULL); 
};

bool scene::is_room_created(void) const
{
	return (my_room != NULL);
};

object* scene::get_selection(void) const
{ 
	return my_current_selection; 
};

void scene::clear_room(void) 
{ 
	if(my_room) 
		free_ptr(my_room); 
};

void scene::clear_bez_wall(void)
{
	if(bezier_walls)
		for(int i = 0; i < 6; ++i)
			free_ptr(bezier_walls[i]);
};

void scene::clear_cube(void)
{
	if(!my_array)
		return;
	for(int i = 0; i < my_size; ++i)
		free_ptr(my_array[i]);
	free_array(my_array);
	my_size = 0;
	if(!my_spring)
		return;
	for(int i = 0; i < my_springsize; ++i)
		free_ptr(my_spring[i]);
	free_array(my_spring);
	my_springsize = 0;
	clear_attach();
};

void scene::clear_attach(void)
{
	if(my_springframe[0])
		for(int i = 0; i < 8; ++i)
		{
			if(my_array) my_springframe[i]->get_second_mass()->frame_spring = NULL;
			free_ptr(my_springframe[i]);
		}
};

void scene::clear_frame(void)
{
	if(!my_frame)
		return;
	free_ptr(my_frame);
	clear_attach();
};

void scene::clear_suszi(void)
{
	if(!my_suszi)
		return;
	free_ptr(my_suszi);
};


void scene::clear(void)
{
	clear_frame();
	clear_bez_wall();
	clear_cube();
	clear_room();
	clear_suszi();
};

void scene::create_suszi(std::istream& is)
{
 clear_suszi();
 my_suszi = new suszi(is, ++current_id, this);
}

void scene::create_room(real xdim, real ydim, real zdim)
{
	clear_room();
	my_room = new room(++current_id);
	my_room->room_edge_pos[0].x = -xdim/2; my_room->room_edge_pos[0].y = -ydim/2; my_room->room_edge_pos[0].z = -zdim/2;
	my_room->room_edge_pos[1].x = xdim/2; my_room->room_edge_pos[1].y = -ydim/2; my_room->room_edge_pos[1].z = -zdim/2;
	my_room->room_edge_pos[2].x = -xdim/2; my_room->room_edge_pos[2].y = ydim/2; my_room->room_edge_pos[2].z = -zdim/2;
	my_room->room_edge_pos[3].x = xdim/2; my_room->room_edge_pos[3].y = ydim/2; my_room->room_edge_pos[3].z = -zdim/2;

	my_room->room_edge_pos[4].x = -xdim/2; my_room->room_edge_pos[4].y = -ydim/2; my_room->room_edge_pos[4].z = zdim/2;
	my_room->room_edge_pos[5].x = xdim/2; my_room->room_edge_pos[5].y = -ydim/2; my_room->room_edge_pos[5].z = zdim/2;
	my_room->room_edge_pos[6].x = -xdim/2; my_room->room_edge_pos[6].y = ydim/2; my_room->room_edge_pos[6].z = zdim/2;
	my_room->room_edge_pos[7].x = xdim/2; my_room->room_edge_pos[7].y = ydim/2; my_room->room_edge_pos[7].z = zdim/2;
};

void scene::create_bez_wall(int usteps, int vsteps)
{
	clear_bez_wall();
	if(!my_array)
		return;
	bezier_walls[0] = new bez_wall(++current_id, my_dim-1, my_dim-1, usteps, vsteps);
	bezier_walls[1] = new bez_wall(++current_id, my_dim-1, my_dim-1, usteps, vsteps);
	bezier_walls[2] = new bez_wall(++current_id, my_dim-1, my_dim-1, usteps, vsteps);
	bezier_walls[3] = new bez_wall(++current_id, my_dim-1, my_dim-1, usteps, vsteps);
	bezier_walls[4] = new bez_wall(++current_id, my_dim-1, my_dim-1, usteps, vsteps);
	bezier_walls[5] = new bez_wall(++current_id, my_dim-1, my_dim-1, usteps, vsteps);
	for(int u = 0; u < my_dim; ++u)
		for(int v = 0; v < my_dim; ++v)
		{
			bezier_walls[0]->add_ctl_point(my_array[get_idx(my_dim-u-1, v, 0)]->get_pos(), u, v);
			bezier_walls[1]->add_ctl_point(my_array[get_idx(my_dim-u-1, my_dim-v-1, my_dim-1)]->get_pos(), u, v);
			bezier_walls[2]->add_ctl_point(my_array[get_idx(0, my_dim-v-1, my_dim-u-1)]->get_pos(), u, v);
			bezier_walls[3]->add_ctl_point(my_array[get_idx(my_dim-1, my_dim-v-1, u)]->get_pos(), u, v);
			bezier_walls[4]->add_ctl_point(my_array[get_idx(my_dim-v-1, my_dim-1, my_dim-u-1)]->get_pos(), u, v);
			bezier_walls[5]->add_ctl_point(my_array[get_idx(my_dim-v-1, 0, u)]->get_pos(), u, v);
		}
		bezier_walls[0]->preprocess();
		bezier_walls[1]->preprocess();
		bezier_walls[2]->preprocess();
		bezier_walls[3]->preprocess();
		bezier_walls[4]->preprocess();
		bezier_walls[5]->preprocess();
};

void scene::attach_frame(void)
{
	clear_attach();
	my_springframe[0] = my_frame->array_springs[0] = new spring(&my_frame->mass_position[0], my_array[get_idx(0, 0, 0)], 0, &my_c_external);
	my_array[get_idx(0, 0, 0)]->frame_spring = my_springframe[0];
	my_springframe[1] = my_frame->array_springs[1] = new spring(&my_frame->mass_position[1], my_array[get_idx(my_dim-1, 0, 0)], 0, &my_c_external);
	my_array[get_idx(my_dim-1, 0, 0)]->frame_spring = my_springframe[1];
	my_springframe[2] = my_frame->array_springs[2] = new spring(&my_frame->mass_position[2], my_array[get_idx(0, my_dim-1, 0)], 0, &my_c_external);
	my_array[get_idx(0, my_dim-1, 0)]->frame_spring = my_springframe[2];
	my_springframe[3] = my_frame->array_springs[3] = new spring(&my_frame->mass_position[3], my_array[get_idx(my_dim-1, my_dim-1, 0)], 0, &my_c_external);
	my_array[get_idx(my_dim-1, my_dim-1, 0)]->frame_spring = my_springframe[3];
	my_springframe[4] = my_frame->array_springs[4] = new spring(&my_frame->mass_position[4], my_array[get_idx(0, 0, my_dim-1)], 0, &my_c_external);
	my_array[get_idx(0, 0, my_dim-1)]->frame_spring = my_springframe[4];
	my_springframe[5] = my_frame->array_springs[5] = new spring(&my_frame->mass_position[5], my_array[get_idx(my_dim-1, 0, my_dim-1)], 0, &my_c_external);
	my_array[get_idx(my_dim-1, 0, my_dim-1)]->frame_spring = my_springframe[5];
	my_springframe[6] = my_frame->array_springs[6] = new spring(&my_frame->mass_position[6], my_array[get_idx(0, my_dim-1, my_dim-1)], 0, &my_c_external);
	my_array[get_idx(0, my_dim-1, my_dim-1)]->frame_spring = my_springframe[6];
	my_springframe[7] = my_frame->array_springs[7] = new spring(&my_frame->mass_position[7], my_array[get_idx(my_dim-1, my_dim-1, my_dim-1)], 0, &my_c_external);
	my_array[get_idx(my_dim-1, my_dim-1, my_dim-1)]->frame_spring = my_springframe[7];
};

void scene::create_frame(int a)
{
	vector3 translate(a*0.5, a*0.5, a*0.5);

	my_frame = new frame(++current_id);
	my_frame->mass_position[0].object_pos = vector3(-translate.x, -translate.y, -translate.z);
	my_frame->mass_position[1].object_pos = vector3(+translate.x, -translate.y, -translate.z);
	my_frame->mass_position[2].object_pos = vector3(-translate.x, +translate.y, -translate.z);
	my_frame->mass_position[3].object_pos = vector3(+translate.x, +translate.y, -translate.z);
	my_frame->mass_position[4].object_pos = vector3(-translate.x, -translate.y, +translate.z);
	my_frame->mass_position[5].object_pos = vector3(+translate.x, -translate.x, +translate.z);
	my_frame->mass_position[6].object_pos = vector3(-translate.x, +translate.y, +translate.z);
	my_frame->mass_position[7].object_pos = vector3(+translate.x, +translate.y, +translate.z);

	glInitNames();
	my_frame->create_obj();

	if(my_array)
		attach_frame();
};

void scene::create_cube(int dim, int a) 
{ 
	clear_cube();
	my_dim = dim;
	my_array = new mass*[my_size = dim*dim*dim];
	int x = 0, y = 0, z = 0;
	vector3 translate(-(my_dim-1)*a*0.5, -(my_dim-1)*a*0.5, -(my_dim-1)*a*0.5);
	glInitNames();
	for(int i = 0; i < my_size; ++i)
	{
		my_array[i] = new mass(++current_id, vector3(translate.x+x*a, translate.y+y*a, translate.z+z*a), vector3(0), &my_mass, &delta, &my_K, &my_Fade);
		my_array[i]->create_obj();

		++x;			y = (x == my_dim) ? ++y : y;		z = (y == my_dim) ? ++z : z;
		x %= my_dim;		y %= my_dim;						z %= my_dim;
	}

	x = y = z = 0;
	my_spring = new spring*[my_springsize = 3*dim*(dim-1)*(3*dim-2)];
	mass* mass;
	int currspring = 0;
	real diagonal = a*sqrt(2.0);
	for(int i = 0; i < my_size; ++i)
	{
		mass = my_array[i];

		// Y|
		//	|
		//	|______X
		if(x)
			mass->array_springs[1] = my_array[get_idx(x-1, y, z)]->array_springs[0];
		if(y)
			mass->array_springs[3] = my_array[get_idx(x, y-1, z)]->array_springs[2];
		if(x < dim-1)
			my_spring[currspring++] = mass->array_springs[0] = new spring(mass, my_array[get_idx(x+1, y, z)], a, &my_c_internal);
		if(y < dim-1)
			my_spring[currspring++] = mass->array_springs[2] = new spring(mass, my_array[get_idx(x, y+1, z)], a, &my_c_internal);
		if(x && y)
			mass->array_springs[5] = my_array[get_idx(x-1, y-1, z)]->array_springs[4];
		if(x < dim-1 && y)
			mass->array_springs[7] = my_array[get_idx(x+1, y-1, z)]->array_springs[6];
		if(x && y < dim-1)
			my_spring[currspring++] = mass->array_springs[6] = new spring(mass, my_array[get_idx(x-1, y+1, z)], diagonal, &my_c_internal);
		if(x < dim-1 && y < dim-1)
			my_spring[currspring++] = mass->array_springs[4] = new spring(mass, my_array[get_idx(x+1, y+1, z)], diagonal, &my_c_internal);

		// Z|
		//	|
		//	|______X
		if(z)
		{
			mass->array_springs[9] = my_array[get_idx(x, y, z-1)]->array_springs[8];
			if(x)
				mass->array_springs[11] = my_array[get_idx(x-1, y, z-1)]->array_springs[10];
			if(x < dim-1)
				mass->array_springs[13] = my_array[get_idx(x+1, y, z-1)]->array_springs[12];
		}
		if(z < dim-1)
		{
			my_spring[currspring++] = mass->array_springs[8] = new spring(mass, my_array[get_idx(x, y, z+1)], a, &my_c_internal);
			if(x)
				my_spring[currspring++] = mass->array_springs[12] = new spring(mass, my_array[get_idx(x-1, y, z+1)], diagonal, &my_c_internal);
			if(x < dim-1)
				my_spring[currspring++] = mass->array_springs[10] = new spring(mass, my_array[get_idx(x+1, y, z+1)], diagonal, &my_c_internal);
		}

		// Y|
		//	|
		//	|______Z
		if(y && z)
			mass->array_springs[15] = my_array[get_idx(x, y-1, z-1)]->array_springs[14];
		if(y && z < dim-1)
			mass->array_springs[17] = my_array[get_idx(x, y-1, z+1)]->array_springs[16];
		if(y < dim-1 && z)
			my_spring[currspring++] = mass->array_springs[16] = new spring(mass, my_array[get_idx(x, y+1, z-1)], diagonal, &my_c_internal);
		if(y < dim-1 && z < dim-1)
			my_spring[currspring++] = mass->array_springs[14] = new spring(mass, my_array[get_idx(x, y+1, z+1)], diagonal, &my_c_internal);

		++x;			y = (x == my_dim) ? ++y : y;		z = (y == my_dim) ? ++z : z;
		x %= my_dim;		y %= my_dim;						z %= my_dim;
	}
	if(my_frame)
		attach_frame();
};

object* scene::find_object(int ID) 
{ 
	for(int i = 0; i < my_size; ++i)
		if(my_array[i]->get_object_id() == ID)
			return my_array[i];
	if(my_frame && my_frame->get_object_id() == ID)
		return my_frame;
	if(my_suszi && my_suszi->get_object_id() == ID)
		return my_suszi;
	if(bezier_walls[0])
	{
		for(int i = 0; i < 6; ++i)
			if(bezier_walls[i]->get_object_id() == ID)
				return bezier_walls[i];
	}
	return NULL;
};

void scene::move_selected(const matrix4& m)
{
	if(!my_current_selection)
		return;
	else if(my_current_selection == my_suszi)
		my_suszi->MatrixMultiply(m);
	else if(my_current_selection == my_frame)
	{
		for(int i = 0; i < 8; ++i)
			my_frame->mass_position[i].object_pos = (m*my_frame->mass_position[i].object_pos).uniformized();
	}
	else if(my_current_selection == my_room)
	{
		for(int i = 0; i < 8; ++i)
			my_room->room_edge_pos[i] = (m*my_room->room_edge_pos[i]).uniformized();
	}
	else
	{
		my_current_selection->object_pos = (m*my_current_selection->object_pos).uniformized();
	}
};

object* scene::object_select(int ID) 
{ 
	object* ret = find_object(ID); 
	if(ret) 
	{ 
		ret->object_select(true); 
		if(my_current_selection) 
			my_current_selection->object_select(false);
		my_current_selection = ret; 
	} 
	return ret; 
};

void scene::object_select(mass* m) 
{ 
	m->object_select(true);  
	if(my_current_selection) 
		my_current_selection->object_select(false);
	my_current_selection = m; 
};

object* scene::deselect(int ID) 
{ 
	object* ret = find_object(ID); 
	if(ret) 
	{ 
		ret->object_select(false); 
		my_current_selection = NULL; 
	} 
	return ret; 
};

void scene::deselect(object* m) 
{ 
	m->object_select(false); 
	my_current_selection = NULL; 
};

void scene::deselect_all(void) 
{ 
	if (my_current_selection) 
		my_current_selection->object_select(false);
	my_current_selection = NULL;
};

const bool scene::is_object_selected(int ID) 
{ 
	object* ret = find_object(ID); 
	if(ret) 
		return ret->is_object_selected(); 
	return false; 
};

void scene::opengl_draw(void) 
{
/*	TRACE_LINE;*/
	glInitNames();

	if(my_show_cube)
	{
		glColor3d(0.5, 0.1, 0.1);
		for(int i = 0; i < my_size && my_array[i]; ++i)
		{
			my_array[i]->opengl_draw();
		}

		if(my_show_springs)
		{
		    	glEnable(GL_LINE_STIPPLE);
		    	glLineStipple(3, 0xCCCC);
			glBegin(GL_LINES);
			glColor3d(0.1, 0.3, 0.5);
			for(int i = 0; i < my_springsize && my_spring[i]; ++i)
				my_spring[i]->opengl_draw();
			glEnd();
		    	glDisable(GL_LINE_STIPPLE);
		}
	}

	if(my_show_frame)
	{
		if(my_show_springs)
		{
			glBegin(GL_LINES);
			if(my_springframe[0])
			{
				glColor3d(0.2, 0.2, 0.7);
				for(int i = 0; i < 8; ++i)
					my_springframe[i]->opengl_draw();
			}
			glEnd();
		}

		glColor3d(0.0, 0.0, 0.7);
		if(my_frame)
			my_frame->opengl_draw();
	}

	if(my_show_bez_wall)
	{
		glColor3d(0.4, 0.4, 0.0);
		for(int i = 0; i < 6 && bezier_walls[i]; ++i)
			bezier_walls[i]->opengl_draw();
	}
	if(my_show_suszi)
	{
		glColor3d(0.6, 0.6, 0.2);
		if (my_suszi) my_suszi->opengl_draw();
	}

	if(my_show_room)
	{
		glColor4d(0.0, 0.6, 0.0, 0.4);
		if(my_room)
			my_room->opengl_draw();
	}
/*	TRACE_LINE;*/
};

void scene::run_physics_step()
{
/*	TRACE_LINE;*/
	if(!is_cube_created()) return;
	int i;
	for(i=0; i<my_size; ++i)
		if(!is_room_created()) my_array[i]->run_physics_step();
		else my_array[i]->run_physics_step_with_room(my_room);
	for(i=0; i<my_size; ++i)
		my_array[i]->object_pos = my_array[i]->current_pos;
/*	TRACE_LINE;*/
}

scene::~scene(void) 
{ 
	clear(); 
};

