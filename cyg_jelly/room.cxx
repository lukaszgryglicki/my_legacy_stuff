#include "defs.hxx"
#include "room.hxx"

void room::create_obj(void) 
{ 
	//object_id = glGenLists(1);
	//glLoadIdentity();
	//glNewList(object_id, GL_COMPILE);
	glPushName(object_id);
	glBegin(GL_QUADS);

	glNormal3d(0, 0, +1);
	//FRONT
	glVertex3d(room_edge_pos[0].x, room_edge_pos[0].y, room_edge_pos[0].z);
	glVertex3d(room_edge_pos[1].x, room_edge_pos[1].y, room_edge_pos[1].z);
	glVertex3d(room_edge_pos[3].x, room_edge_pos[3].y, room_edge_pos[3].z);
	glVertex3d(room_edge_pos[2].x, room_edge_pos[2].y, room_edge_pos[2].z);

	glNormal3d(0, 0, -1);
	//BACK
	glVertex3d(room_edge_pos[4].x, room_edge_pos[4].y, room_edge_pos[4].z);
	glVertex3d(room_edge_pos[6].x, room_edge_pos[6].y, room_edge_pos[6].z);
	glVertex3d(room_edge_pos[7].x, room_edge_pos[7].y, room_edge_pos[7].z);
	glVertex3d(room_edge_pos[5].x, room_edge_pos[5].y, room_edge_pos[5].z);

	glNormal3d(+1, 0, 0);
	//LEFT
	glVertex3d(room_edge_pos[0].x, room_edge_pos[0].y, room_edge_pos[0].z);
	glVertex3d(room_edge_pos[2].x, room_edge_pos[2].y, room_edge_pos[2].z);
	glVertex3d(room_edge_pos[6].x, room_edge_pos[6].y, room_edge_pos[6].z);
	glVertex3d(room_edge_pos[4].x, room_edge_pos[4].y, room_edge_pos[4].z);

	glNormal3d(-1, 0, 0);
	//RIGHT
	glVertex3d(room_edge_pos[1].x, room_edge_pos[1].y, room_edge_pos[1].z);
	glVertex3d(room_edge_pos[5].x, room_edge_pos[5].y, room_edge_pos[5].z);
	glVertex3d(room_edge_pos[7].x, room_edge_pos[7].y, room_edge_pos[7].z);
	glVertex3d(room_edge_pos[3].x, room_edge_pos[3].y, room_edge_pos[3].z);

	glNormal3d(0, +1, 0);
	//DOWN
	glVertex3d(room_edge_pos[0].x, room_edge_pos[0].y, room_edge_pos[0].z);
	glVertex3d(room_edge_pos[4].x, room_edge_pos[4].y, room_edge_pos[4].z);
	glVertex3d(room_edge_pos[5].x, room_edge_pos[5].y, room_edge_pos[5].z);
	glVertex3d(room_edge_pos[1].x, room_edge_pos[1].y, room_edge_pos[1].z);

	glNormal3d(0, -1, 0);
	//UP
	glVertex3d(room_edge_pos[2].x, room_edge_pos[2].y, room_edge_pos[2].z);
	glVertex3d(room_edge_pos[3].x, room_edge_pos[3].y, room_edge_pos[3].z);
	glVertex3d(room_edge_pos[7].x, room_edge_pos[7].y, room_edge_pos[7].z);
	glVertex3d(room_edge_pos[6].x, room_edge_pos[6].y, room_edge_pos[6].z);

	glEnd();
	glPopName();
	//glEndList();
};

room::room(int ID) 
: object(ID) 
{
};

const vector3* room::get_pos(int x, int y, int z) const
{ 
	if(x < 0 || x > 1 || y < 0 || y > 1 || z < 0 || z > 1) 
		return NULL; 
	return &room_edge_pos[4*z+2*y+x]; 
};

room::~room(void) 
{
};

void room::opengl_draw(void)
{
	glPushMatrix(); glPushAttrib(GL_CURRENT_BIT);
	if(object_selected)
		glColor4d(0.0, 0.0, 1.0, 0.5);
	//glCallList(object_id); 
	create_obj();
	glPopAttrib(); glPopMatrix(); 
};

