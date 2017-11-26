#include "defs.hxx"
#include "frame.hxx"

void frame::create_obj(void) 
{ 
	//object_id = glGenLists(1);
	//glLoadIdentity();
	//glNewList(object_id, GL_COMPILE_AND_EXECUTE);
	glPushName(object_id);
	glBegin(GL_QUADS);

	glNormal3d(0, 0, -1);
	//FRONT
	glVertex3d(mass_position[0].get_pos()->x-2*BALLR, mass_position[0].get_pos()->y-BALLR,   mass_position[0].get_pos()->z-2*BALLR);
	glVertex3d(mass_position[1].get_pos()->x+2*BALLR, mass_position[1].get_pos()->y-BALLR,   mass_position[1].get_pos()->z-2*BALLR);
	glVertex3d(mass_position[1].get_pos()->x+2*BALLR, mass_position[1].get_pos()->y-2*BALLR, mass_position[1].get_pos()->z-2*BALLR);
	glVertex3d(mass_position[0].get_pos()->x-2*BALLR, mass_position[0].get_pos()->y-2*BALLR, mass_position[0].get_pos()->z-2*BALLR);

	glVertex3d(mass_position[2].get_pos()->x-2*BALLR, mass_position[2].get_pos()->y+BALLR,   mass_position[2].get_pos()->z-2*BALLR);
	glVertex3d(mass_position[2].get_pos()->x-2*BALLR, mass_position[2].get_pos()->y+2*BALLR, mass_position[2].get_pos()->z-2*BALLR);
	glVertex3d(mass_position[3].get_pos()->x+2*BALLR, mass_position[3].get_pos()->y+2*BALLR, mass_position[3].get_pos()->z-2*BALLR);
	glVertex3d(mass_position[3].get_pos()->x+2*BALLR, mass_position[3].get_pos()->y+BALLR,   mass_position[3].get_pos()->z-2*BALLR);

	glVertex3d(mass_position[0].get_pos()->x-BALLR,   mass_position[0].get_pos()->y-BALLR, mass_position[0].get_pos()->z-2*BALLR);
	glVertex3d(mass_position[0].get_pos()->x-2*BALLR, mass_position[0].get_pos()->y-BALLR, mass_position[0].get_pos()->z-2*BALLR);
	glVertex3d(mass_position[2].get_pos()->x-2*BALLR, mass_position[2].get_pos()->y+BALLR, mass_position[2].get_pos()->z-2*BALLR);
	glVertex3d(mass_position[2].get_pos()->x-BALLR,   mass_position[2].get_pos()->y+BALLR, mass_position[2].get_pos()->z-2*BALLR);

	glVertex3d(mass_position[1].get_pos()->x+2*BALLR, mass_position[1].get_pos()->y-BALLR, mass_position[1].get_pos()->z-2*BALLR);
	glVertex3d(mass_position[1].get_pos()->x+BALLR,   mass_position[1].get_pos()->y-BALLR, mass_position[1].get_pos()->z-2*BALLR);
	glVertex3d(mass_position[3].get_pos()->x+BALLR,   mass_position[3].get_pos()->y+BALLR, mass_position[3].get_pos()->z-2*BALLR);
	glVertex3d(mass_position[3].get_pos()->x+2*BALLR, mass_position[3].get_pos()->y+BALLR, mass_position[3].get_pos()->z-2*BALLR);
	//BACK
	glVertex3d(mass_position[4].get_pos()->x-BALLR, mass_position[4].get_pos()->y-BALLR,   mass_position[4].get_pos()->z+BALLR);
	glVertex3d(mass_position[5].get_pos()->x+BALLR, mass_position[5].get_pos()->y-BALLR,   mass_position[5].get_pos()->z+BALLR);
	glVertex3d(mass_position[5].get_pos()->x+BALLR, mass_position[5].get_pos()->y-2*BALLR, mass_position[5].get_pos()->z+BALLR);
	glVertex3d(mass_position[4].get_pos()->x-BALLR, mass_position[4].get_pos()->y-2*BALLR, mass_position[4].get_pos()->z+BALLR);

	glVertex3d(mass_position[6].get_pos()->x-BALLR, mass_position[6].get_pos()->y+BALLR,   mass_position[6].get_pos()->z+BALLR);
	glVertex3d(mass_position[6].get_pos()->x-BALLR, mass_position[6].get_pos()->y+2*BALLR, mass_position[6].get_pos()->z+BALLR);
	glVertex3d(mass_position[7].get_pos()->x+BALLR, mass_position[7].get_pos()->y+2*BALLR, mass_position[7].get_pos()->z+BALLR);
	glVertex3d(mass_position[7].get_pos()->x+BALLR, mass_position[7].get_pos()->y+BALLR,   mass_position[7].get_pos()->z+BALLR);

	glVertex3d(mass_position[4].get_pos()->x-BALLR,   mass_position[4].get_pos()->y-BALLR, mass_position[4].get_pos()->z+BALLR);
	glVertex3d(mass_position[4].get_pos()->x-2*BALLR, mass_position[4].get_pos()->y-BALLR, mass_position[4].get_pos()->z+BALLR);
	glVertex3d(mass_position[6].get_pos()->x-2*BALLR, mass_position[6].get_pos()->y+BALLR, mass_position[6].get_pos()->z+BALLR);
	glVertex3d(mass_position[6].get_pos()->x-BALLR,   mass_position[6].get_pos()->y+BALLR, mass_position[6].get_pos()->z+BALLR);

	glVertex3d(mass_position[5].get_pos()->x+2*BALLR, mass_position[5].get_pos()->y-BALLR, mass_position[5].get_pos()->z+BALLR);
	glVertex3d(mass_position[5].get_pos()->x+BALLR,   mass_position[5].get_pos()->y-BALLR, mass_position[5].get_pos()->z+BALLR);
	glVertex3d(mass_position[7].get_pos()->x+BALLR,   mass_position[7].get_pos()->y+BALLR, mass_position[7].get_pos()->z+BALLR);
	glVertex3d(mass_position[7].get_pos()->x+2*BALLR, mass_position[7].get_pos()->y+BALLR, mass_position[7].get_pos()->z+BALLR);

	glNormal3d(0, 0, +1);
	//FRONT
	glVertex3d(mass_position[0].get_pos()->x-BALLR, mass_position[0].get_pos()->y-BALLR,   mass_position[0].get_pos()->z-BALLR);
	glVertex3d(mass_position[0].get_pos()->x-BALLR, mass_position[0].get_pos()->y-2*BALLR, mass_position[0].get_pos()->z-BALLR);
	glVertex3d(mass_position[1].get_pos()->x+BALLR, mass_position[1].get_pos()->y-2*BALLR, mass_position[1].get_pos()->z-BALLR);
	glVertex3d(mass_position[1].get_pos()->x+BALLR, mass_position[1].get_pos()->y-BALLR,   mass_position[1].get_pos()->z-BALLR);

	glVertex3d(mass_position[2].get_pos()->x-BALLR, mass_position[2].get_pos()->y+BALLR,   mass_position[2].get_pos()->z-BALLR);
	glVertex3d(mass_position[3].get_pos()->x+BALLR, mass_position[3].get_pos()->y+BALLR,   mass_position[3].get_pos()->z-BALLR);
	glVertex3d(mass_position[3].get_pos()->x+BALLR, mass_position[3].get_pos()->y+2*BALLR, mass_position[3].get_pos()->z-BALLR);
	glVertex3d(mass_position[2].get_pos()->x-BALLR, mass_position[2].get_pos()->y+2*BALLR, mass_position[2].get_pos()->z-BALLR);

	glVertex3d(mass_position[0].get_pos()->x-BALLR,   mass_position[0].get_pos()->y-BALLR, mass_position[0].get_pos()->z-BALLR);
	glVertex3d(mass_position[2].get_pos()->x-BALLR,   mass_position[2].get_pos()->y+BALLR, mass_position[2].get_pos()->z-BALLR);
	glVertex3d(mass_position[2].get_pos()->x-2*BALLR, mass_position[2].get_pos()->y+BALLR, mass_position[2].get_pos()->z-BALLR);
	glVertex3d(mass_position[0].get_pos()->x-2*BALLR, mass_position[0].get_pos()->y-BALLR, mass_position[0].get_pos()->z-BALLR);

	glVertex3d(mass_position[1].get_pos()->x+2*BALLR, mass_position[1].get_pos()->y-BALLR, mass_position[1].get_pos()->z-BALLR);
	glVertex3d(mass_position[3].get_pos()->x+2*BALLR, mass_position[3].get_pos()->y+BALLR, mass_position[3].get_pos()->z-BALLR);
	glVertex3d(mass_position[3].get_pos()->x+BALLR,   mass_position[3].get_pos()->y+BALLR, mass_position[3].get_pos()->z-BALLR);
	glVertex3d(mass_position[1].get_pos()->x+BALLR,   mass_position[1].get_pos()->y-BALLR, mass_position[1].get_pos()->z-BALLR);
	//BACK
	glVertex3d(mass_position[4].get_pos()->x-2*BALLR, mass_position[4].get_pos()->y-BALLR,   mass_position[4].get_pos()->z+2*BALLR);
	glVertex3d(mass_position[4].get_pos()->x-2*BALLR, mass_position[4].get_pos()->y-2*BALLR, mass_position[4].get_pos()->z+2*BALLR);
	glVertex3d(mass_position[5].get_pos()->x+2*BALLR, mass_position[5].get_pos()->y-2*BALLR, mass_position[5].get_pos()->z+2*BALLR);
	glVertex3d(mass_position[5].get_pos()->x+2*BALLR, mass_position[5].get_pos()->y-BALLR,   mass_position[5].get_pos()->z+2*BALLR);

	glVertex3d(mass_position[6].get_pos()->x-2*BALLR, mass_position[6].get_pos()->y+BALLR,   mass_position[6].get_pos()->z+2*BALLR);
	glVertex3d(mass_position[7].get_pos()->x+2*BALLR, mass_position[7].get_pos()->y+BALLR,   mass_position[7].get_pos()->z+2*BALLR);
	glVertex3d(mass_position[7].get_pos()->x+2*BALLR, mass_position[7].get_pos()->y+2*BALLR, mass_position[7].get_pos()->z+2*BALLR);
	glVertex3d(mass_position[6].get_pos()->x-2*BALLR, mass_position[6].get_pos()->y+2*BALLR, mass_position[6].get_pos()->z+2*BALLR);

	glVertex3d(mass_position[4].get_pos()->x-BALLR,   mass_position[4].get_pos()->y-BALLR, mass_position[4].get_pos()->z+2*BALLR);
	glVertex3d(mass_position[6].get_pos()->x-BALLR,   mass_position[6].get_pos()->y+BALLR, mass_position[6].get_pos()->z+2*BALLR);
	glVertex3d(mass_position[6].get_pos()->x-2*BALLR, mass_position[6].get_pos()->y+BALLR, mass_position[6].get_pos()->z+2*BALLR);
	glVertex3d(mass_position[4].get_pos()->x-2*BALLR, mass_position[4].get_pos()->y-BALLR, mass_position[4].get_pos()->z+2*BALLR);

	glVertex3d(mass_position[5].get_pos()->x+2*BALLR, mass_position[5].get_pos()->y-BALLR, mass_position[5].get_pos()->z+2*BALLR);
	glVertex3d(mass_position[7].get_pos()->x+2*BALLR, mass_position[7].get_pos()->y+BALLR, mass_position[7].get_pos()->z+2*BALLR);
	glVertex3d(mass_position[7].get_pos()->x+BALLR,   mass_position[7].get_pos()->y+BALLR, mass_position[7].get_pos()->z+2*BALLR);
	glVertex3d(mass_position[5].get_pos()->x+BALLR,   mass_position[5].get_pos()->y-BALLR, mass_position[5].get_pos()->z+2*BALLR);

	glNormal3d(-1, 0, 0);
	//LEFT
	glVertex3d(mass_position[0].get_pos()->x-2*BALLR, mass_position[0].get_pos()->y-BALLR,   mass_position[0].get_pos()->z-2*BALLR);
	glVertex3d(mass_position[0].get_pos()->x-2*BALLR, mass_position[0].get_pos()->y-2*BALLR, mass_position[0].get_pos()->z-2*BALLR);
	glVertex3d(mass_position[4].get_pos()->x-2*BALLR, mass_position[4].get_pos()->y-2*BALLR, mass_position[4].get_pos()->z+2*BALLR);
	glVertex3d(mass_position[4].get_pos()->x-2*BALLR, mass_position[4].get_pos()->y-BALLR,   mass_position[4].get_pos()->z+2*BALLR);

	glVertex3d(mass_position[2].get_pos()->x-2*BALLR, mass_position[2].get_pos()->y+BALLR,   mass_position[2].get_pos()->z-2*BALLR);
	glVertex3d(mass_position[6].get_pos()->x-2*BALLR, mass_position[6].get_pos()->y+BALLR,   mass_position[6].get_pos()->z+2*BALLR);
	glVertex3d(mass_position[6].get_pos()->x-2*BALLR, mass_position[6].get_pos()->y+2*BALLR, mass_position[6].get_pos()->z+2*BALLR);
	glVertex3d(mass_position[2].get_pos()->x-2*BALLR, mass_position[2].get_pos()->y+2*BALLR, mass_position[2].get_pos()->z-2*BALLR);

	glVertex3d(mass_position[0].get_pos()->x-2*BALLR, mass_position[0].get_pos()->y-BALLR, mass_position[0].get_pos()->z-2*BALLR);
	glVertex3d(mass_position[0].get_pos()->x-2*BALLR, mass_position[0].get_pos()->y-BALLR, mass_position[0].get_pos()->z-BALLR);
	glVertex3d(mass_position[2].get_pos()->x-2*BALLR, mass_position[2].get_pos()->y+BALLR, mass_position[2].get_pos()->z-BALLR);
	glVertex3d(mass_position[2].get_pos()->x-2*BALLR, mass_position[2].get_pos()->y+BALLR, mass_position[2].get_pos()->z-2*BALLR);

	glVertex3d(mass_position[4].get_pos()->x-2*BALLR, mass_position[4].get_pos()->y-BALLR, mass_position[4].get_pos()->z+2*BALLR);
	glVertex3d(mass_position[6].get_pos()->x-2*BALLR, mass_position[6].get_pos()->y+BALLR, mass_position[6].get_pos()->z+2*BALLR);
	glVertex3d(mass_position[6].get_pos()->x-2*BALLR, mass_position[6].get_pos()->y+BALLR, mass_position[6].get_pos()->z+BALLR);
	glVertex3d(mass_position[4].get_pos()->x-2*BALLR, mass_position[4].get_pos()->y-BALLR, mass_position[4].get_pos()->z+BALLR);
	//RIGHT
	glVertex3d(mass_position[1].get_pos()->x+BALLR, mass_position[1].get_pos()->y-BALLR,   mass_position[1].get_pos()->z-BALLR);
	glVertex3d(mass_position[1].get_pos()->x+BALLR, mass_position[1].get_pos()->y-2*BALLR, mass_position[1].get_pos()->z-BALLR);
	glVertex3d(mass_position[5].get_pos()->x+BALLR, mass_position[5].get_pos()->y-2*BALLR, mass_position[5].get_pos()->z+BALLR);
	glVertex3d(mass_position[5].get_pos()->x+BALLR, mass_position[5].get_pos()->y-BALLR,   mass_position[5].get_pos()->z+BALLR);

	glVertex3d(mass_position[3].get_pos()->x+BALLR, mass_position[3].get_pos()->y+BALLR,   mass_position[3].get_pos()->z-BALLR);
	glVertex3d(mass_position[7].get_pos()->x+BALLR, mass_position[7].get_pos()->y+BALLR,   mass_position[7].get_pos()->z+BALLR);
	glVertex3d(mass_position[7].get_pos()->x+BALLR, mass_position[7].get_pos()->y+2*BALLR, mass_position[7].get_pos()->z+BALLR);
	glVertex3d(mass_position[3].get_pos()->x+BALLR, mass_position[3].get_pos()->y+2*BALLR, mass_position[3].get_pos()->z-BALLR);

	glVertex3d(mass_position[1].get_pos()->x+BALLR, mass_position[1].get_pos()->y-BALLR, mass_position[1].get_pos()->z-2*BALLR);
	glVertex3d(mass_position[1].get_pos()->x+BALLR, mass_position[1].get_pos()->y-BALLR, mass_position[1].get_pos()->z-BALLR);
	glVertex3d(mass_position[3].get_pos()->x+BALLR, mass_position[3].get_pos()->y+BALLR, mass_position[3].get_pos()->z-BALLR);
	glVertex3d(mass_position[3].get_pos()->x+BALLR, mass_position[3].get_pos()->y+BALLR, mass_position[3].get_pos()->z-2*BALLR);

	glVertex3d(mass_position[5].get_pos()->x+BALLR, mass_position[5].get_pos()->y-BALLR, mass_position[5].get_pos()->z+2*BALLR);
	glVertex3d(mass_position[7].get_pos()->x+BALLR, mass_position[7].get_pos()->y+BALLR, mass_position[7].get_pos()->z+2*BALLR);
	glVertex3d(mass_position[7].get_pos()->x+BALLR, mass_position[7].get_pos()->y+BALLR, mass_position[7].get_pos()->z+BALLR);
	glVertex3d(mass_position[5].get_pos()->x+BALLR, mass_position[5].get_pos()->y-BALLR, mass_position[5].get_pos()->z+BALLR);

	glNormal3d(+1, 0, 0);
	//LEFT
	glVertex3d(mass_position[0].get_pos()->x-BALLR, mass_position[0].get_pos()->y-BALLR,   mass_position[0].get_pos()->z-BALLR);
	glVertex3d(mass_position[4].get_pos()->x-BALLR, mass_position[4].get_pos()->y-BALLR,   mass_position[4].get_pos()->z+BALLR);
	glVertex3d(mass_position[4].get_pos()->x-BALLR, mass_position[4].get_pos()->y-2*BALLR, mass_position[4].get_pos()->z+BALLR);
	glVertex3d(mass_position[0].get_pos()->x-BALLR, mass_position[0].get_pos()->y-2*BALLR, mass_position[0].get_pos()->z-BALLR);

	glVertex3d(mass_position[2].get_pos()->x-BALLR, mass_position[2].get_pos()->y+BALLR,   mass_position[2].get_pos()->z-BALLR);
	glVertex3d(mass_position[2].get_pos()->x-BALLR, mass_position[2].get_pos()->y+2*BALLR, mass_position[2].get_pos()->z-BALLR);
	glVertex3d(mass_position[6].get_pos()->x-BALLR, mass_position[6].get_pos()->y+2*BALLR, mass_position[6].get_pos()->z+BALLR);
	glVertex3d(mass_position[6].get_pos()->x-BALLR, mass_position[6].get_pos()->y+BALLR,   mass_position[6].get_pos()->z+BALLR);

	glVertex3d(mass_position[0].get_pos()->x-BALLR, mass_position[0].get_pos()->y-BALLR, mass_position[0].get_pos()->z-2*BALLR);
	glVertex3d(mass_position[2].get_pos()->x-BALLR, mass_position[2].get_pos()->y+BALLR, mass_position[2].get_pos()->z-2*BALLR);
	glVertex3d(mass_position[2].get_pos()->x-BALLR, mass_position[2].get_pos()->y+BALLR, mass_position[2].get_pos()->z-BALLR);
	glVertex3d(mass_position[0].get_pos()->x-BALLR, mass_position[0].get_pos()->y-BALLR, mass_position[0].get_pos()->z-BALLR);

	glVertex3d(mass_position[4].get_pos()->x-BALLR, mass_position[4].get_pos()->y-BALLR, mass_position[4].get_pos()->z+2*BALLR);
	glVertex3d(mass_position[4].get_pos()->x-BALLR, mass_position[4].get_pos()->y-BALLR, mass_position[4].get_pos()->z+BALLR);
	glVertex3d(mass_position[6].get_pos()->x-BALLR, mass_position[6].get_pos()->y+BALLR, mass_position[6].get_pos()->z+BALLR);
	glVertex3d(mass_position[6].get_pos()->x-BALLR, mass_position[6].get_pos()->y+BALLR, mass_position[6].get_pos()->z+2*BALLR);
	//RIGHT
	glVertex3d(mass_position[1].get_pos()->x+2*BALLR, mass_position[1].get_pos()->y-BALLR,   mass_position[1].get_pos()->z-2*BALLR);
	glVertex3d(mass_position[5].get_pos()->x+2*BALLR, mass_position[5].get_pos()->y-BALLR,   mass_position[5].get_pos()->z+2*BALLR);
	glVertex3d(mass_position[5].get_pos()->x+2*BALLR, mass_position[5].get_pos()->y-2*BALLR, mass_position[5].get_pos()->z+2*BALLR);
	glVertex3d(mass_position[1].get_pos()->x+2*BALLR, mass_position[1].get_pos()->y-2*BALLR, mass_position[1].get_pos()->z-2*BALLR);

	glVertex3d(mass_position[3].get_pos()->x+2*BALLR, mass_position[3].get_pos()->y+BALLR,   mass_position[3].get_pos()->z-2*BALLR);
	glVertex3d(mass_position[3].get_pos()->x+2*BALLR, mass_position[3].get_pos()->y+2*BALLR, mass_position[3].get_pos()->z-2*BALLR);
	glVertex3d(mass_position[7].get_pos()->x+2*BALLR, mass_position[7].get_pos()->y+2*BALLR, mass_position[7].get_pos()->z+2*BALLR);
	glVertex3d(mass_position[7].get_pos()->x+2*BALLR, mass_position[7].get_pos()->y+BALLR,   mass_position[7].get_pos()->z+2*BALLR);

	glVertex3d(mass_position[1].get_pos()->x+2*BALLR, mass_position[1].get_pos()->y-BALLR, mass_position[1].get_pos()->z-2*BALLR);
	glVertex3d(mass_position[3].get_pos()->x+2*BALLR, mass_position[3].get_pos()->y+BALLR, mass_position[3].get_pos()->z-2*BALLR);
	glVertex3d(mass_position[3].get_pos()->x+2*BALLR, mass_position[3].get_pos()->y+BALLR, mass_position[3].get_pos()->z-BALLR);
	glVertex3d(mass_position[1].get_pos()->x+2*BALLR, mass_position[1].get_pos()->y-BALLR, mass_position[1].get_pos()->z-BALLR);

	glVertex3d(mass_position[5].get_pos()->x+2*BALLR, mass_position[5].get_pos()->y-BALLR, mass_position[5].get_pos()->z+2*BALLR);
	glVertex3d(mass_position[5].get_pos()->x+2*BALLR, mass_position[5].get_pos()->y-BALLR, mass_position[5].get_pos()->z+BALLR);
	glVertex3d(mass_position[7].get_pos()->x+2*BALLR, mass_position[7].get_pos()->y+BALLR, mass_position[7].get_pos()->z+BALLR);
	glVertex3d(mass_position[7].get_pos()->x+2*BALLR, mass_position[7].get_pos()->y+BALLR, mass_position[7].get_pos()->z+2*BALLR);

	glNormal3d(0, -1, 0);
	//DOWN
	glVertex3d(mass_position[0].get_pos()->x-2*BALLR, mass_position[0].get_pos()->y-2*BALLR, mass_position[0].get_pos()->z-2*BALLR);
	glVertex3d(mass_position[1].get_pos()->x+2*BALLR, mass_position[1].get_pos()->y-2*BALLR, mass_position[1].get_pos()->z-2*BALLR);
	glVertex3d(mass_position[1].get_pos()->x+2*BALLR, mass_position[1].get_pos()->y-2*BALLR, mass_position[1].get_pos()->z-BALLR);
	glVertex3d(mass_position[0].get_pos()->x-2*BALLR, mass_position[0].get_pos()->y-2*BALLR, mass_position[0].get_pos()->z-BALLR);

	glVertex3d(mass_position[4].get_pos()->x-2*BALLR, mass_position[4].get_pos()->y-2*BALLR, mass_position[4].get_pos()->z+2*BALLR);
	glVertex3d(mass_position[4].get_pos()->x-2*BALLR, mass_position[4].get_pos()->y-2*BALLR, mass_position[4].get_pos()->z+BALLR);
	glVertex3d(mass_position[5].get_pos()->x+2*BALLR, mass_position[5].get_pos()->y-2*BALLR, mass_position[5].get_pos()->z+BALLR);
	glVertex3d(mass_position[5].get_pos()->x+2*BALLR, mass_position[5].get_pos()->y-2*BALLR, mass_position[5].get_pos()->z+2*BALLR);

	glVertex3d(mass_position[0].get_pos()->x-BALLR,   mass_position[0].get_pos()->y-2*BALLR, mass_position[0].get_pos()->z-BALLR);
	glVertex3d(mass_position[4].get_pos()->x-BALLR,   mass_position[4].get_pos()->y-2*BALLR, mass_position[4].get_pos()->z+BALLR);
	glVertex3d(mass_position[4].get_pos()->x-2*BALLR, mass_position[4].get_pos()->y-2*BALLR, mass_position[4].get_pos()->z+BALLR);
	glVertex3d(mass_position[0].get_pos()->x-2*BALLR, mass_position[0].get_pos()->y-2*BALLR, mass_position[0].get_pos()->z-BALLR);

	glVertex3d(mass_position[1].get_pos()->x+2*BALLR, mass_position[1].get_pos()->y-2*BALLR, mass_position[1].get_pos()->z-BALLR);
	glVertex3d(mass_position[5].get_pos()->x+2*BALLR, mass_position[5].get_pos()->y-2*BALLR, mass_position[5].get_pos()->z+BALLR);
	glVertex3d(mass_position[5].get_pos()->x+BALLR,   mass_position[5].get_pos()->y-2*BALLR, mass_position[5].get_pos()->z+BALLR);
	glVertex3d(mass_position[1].get_pos()->x+BALLR,   mass_position[1].get_pos()->y-2*BALLR, mass_position[1].get_pos()->z-BALLR);
	//UP
	glVertex3d(mass_position[2].get_pos()->x-BALLR, mass_position[2].get_pos()->y+BALLR, mass_position[2].get_pos()->z-2*BALLR);
	glVertex3d(mass_position[3].get_pos()->x+BALLR, mass_position[3].get_pos()->y+BALLR, mass_position[3].get_pos()->z-2*BALLR);
	glVertex3d(mass_position[3].get_pos()->x+BALLR, mass_position[3].get_pos()->y+BALLR, mass_position[3].get_pos()->z-BALLR);
	glVertex3d(mass_position[2].get_pos()->x-BALLR, mass_position[2].get_pos()->y+BALLR, mass_position[2].get_pos()->z-BALLR);

	glVertex3d(mass_position[6].get_pos()->x-BALLR, mass_position[6].get_pos()->y+BALLR, mass_position[6].get_pos()->z+2*BALLR);
	glVertex3d(mass_position[6].get_pos()->x-BALLR, mass_position[6].get_pos()->y+BALLR, mass_position[6].get_pos()->z+BALLR);
	glVertex3d(mass_position[7].get_pos()->x+BALLR, mass_position[7].get_pos()->y+BALLR, mass_position[7].get_pos()->z+BALLR);
	glVertex3d(mass_position[7].get_pos()->x+BALLR, mass_position[7].get_pos()->y+BALLR, mass_position[7].get_pos()->z+2*BALLR);

	glVertex3d(mass_position[2].get_pos()->x-BALLR,   mass_position[2].get_pos()->y+BALLR, mass_position[2].get_pos()->z-BALLR);
	glVertex3d(mass_position[6].get_pos()->x-BALLR,   mass_position[6].get_pos()->y+BALLR, mass_position[6].get_pos()->z+BALLR);
	glVertex3d(mass_position[6].get_pos()->x-2*BALLR, mass_position[6].get_pos()->y+BALLR, mass_position[6].get_pos()->z+BALLR);
	glVertex3d(mass_position[2].get_pos()->x-2*BALLR, mass_position[2].get_pos()->y+BALLR, mass_position[2].get_pos()->z-BALLR);

	glVertex3d(mass_position[3].get_pos()->x+2*BALLR, mass_position[3].get_pos()->y+BALLR, mass_position[3].get_pos()->z-BALLR);
	glVertex3d(mass_position[7].get_pos()->x+2*BALLR, mass_position[7].get_pos()->y+BALLR, mass_position[7].get_pos()->z+BALLR);
	glVertex3d(mass_position[7].get_pos()->x+BALLR,   mass_position[7].get_pos()->y+BALLR, mass_position[7].get_pos()->z+BALLR);
	glVertex3d(mass_position[3].get_pos()->x+BALLR,   mass_position[3].get_pos()->y+BALLR, mass_position[3].get_pos()->z-BALLR);

	glNormal3d(0, +1, 0);
	//DOWN
	glVertex3d(mass_position[0].get_pos()->x-BALLR, mass_position[0].get_pos()->y-BALLR, mass_position[0].get_pos()->z-2*BALLR);
	glVertex3d(mass_position[0].get_pos()->x-BALLR, mass_position[0].get_pos()->y-BALLR, mass_position[0].get_pos()->z-BALLR);
	glVertex3d(mass_position[1].get_pos()->x+BALLR, mass_position[1].get_pos()->y-BALLR, mass_position[1].get_pos()->z-BALLR);
	glVertex3d(mass_position[1].get_pos()->x+BALLR, mass_position[1].get_pos()->y-BALLR, mass_position[1].get_pos()->z-2*BALLR);

	glVertex3d(mass_position[4].get_pos()->x-BALLR, mass_position[4].get_pos()->y-BALLR, mass_position[4].get_pos()->z+2*BALLR);
	glVertex3d(mass_position[5].get_pos()->x+BALLR, mass_position[5].get_pos()->y-BALLR, mass_position[5].get_pos()->z+2*BALLR);
	glVertex3d(mass_position[5].get_pos()->x+BALLR, mass_position[5].get_pos()->y-BALLR, mass_position[5].get_pos()->z+BALLR);
	glVertex3d(mass_position[4].get_pos()->x-BALLR, mass_position[4].get_pos()->y-BALLR, mass_position[4].get_pos()->z+BALLR);

	glVertex3d(mass_position[0].get_pos()->x-BALLR,   mass_position[0].get_pos()->y-BALLR, mass_position[0].get_pos()->z-BALLR);
	glVertex3d(mass_position[0].get_pos()->x-2*BALLR, mass_position[0].get_pos()->y-BALLR, mass_position[0].get_pos()->z-BALLR);
	glVertex3d(mass_position[4].get_pos()->x-2*BALLR, mass_position[4].get_pos()->y-BALLR, mass_position[4].get_pos()->z+BALLR);
	glVertex3d(mass_position[4].get_pos()->x-BALLR,   mass_position[4].get_pos()->y-BALLR, mass_position[4].get_pos()->z+BALLR);

	glVertex3d(mass_position[1].get_pos()->x+2*BALLR, mass_position[1].get_pos()->y-BALLR, mass_position[1].get_pos()->z-BALLR);
	glVertex3d(mass_position[1].get_pos()->x+BALLR,   mass_position[1].get_pos()->y-BALLR, mass_position[1].get_pos()->z-BALLR);
	glVertex3d(mass_position[5].get_pos()->x+BALLR,   mass_position[5].get_pos()->y-BALLR, mass_position[5].get_pos()->z+BALLR);
	glVertex3d(mass_position[5].get_pos()->x+2*BALLR, mass_position[5].get_pos()->y-BALLR, mass_position[5].get_pos()->z+BALLR);
	//UP
	glVertex3d(mass_position[2].get_pos()->x-2*BALLR, mass_position[2].get_pos()->y+2*BALLR, mass_position[2].get_pos()->z-2*BALLR);
	glVertex3d(mass_position[2].get_pos()->x-2*BALLR, mass_position[2].get_pos()->y+2*BALLR, mass_position[2].get_pos()->z-BALLR);
	glVertex3d(mass_position[3].get_pos()->x+2*BALLR, mass_position[3].get_pos()->y+2*BALLR, mass_position[3].get_pos()->z-BALLR);
	glVertex3d(mass_position[3].get_pos()->x+2*BALLR, mass_position[3].get_pos()->y+2*BALLR, mass_position[3].get_pos()->z-2*BALLR);

	glVertex3d(mass_position[6].get_pos()->x-2*BALLR, mass_position[6].get_pos()->y+2*BALLR, mass_position[6].get_pos()->z+2*BALLR);
	glVertex3d(mass_position[7].get_pos()->x+2*BALLR, mass_position[7].get_pos()->y+2*BALLR, mass_position[7].get_pos()->z+2*BALLR);
	glVertex3d(mass_position[7].get_pos()->x+2*BALLR, mass_position[7].get_pos()->y+2*BALLR, mass_position[7].get_pos()->z+BALLR);
	glVertex3d(mass_position[6].get_pos()->x-2*BALLR, mass_position[6].get_pos()->y+2*BALLR, mass_position[6].get_pos()->z+BALLR);

	glVertex3d(mass_position[2].get_pos()->x-BALLR,   mass_position[2].get_pos()->y+2*BALLR, mass_position[2].get_pos()->z-BALLR);
	glVertex3d(mass_position[2].get_pos()->x-2*BALLR, mass_position[2].get_pos()->y+2*BALLR, mass_position[2].get_pos()->z-BALLR);
	glVertex3d(mass_position[6].get_pos()->x-2*BALLR, mass_position[6].get_pos()->y+2*BALLR, mass_position[6].get_pos()->z+BALLR);
	glVertex3d(mass_position[6].get_pos()->x-BALLR,   mass_position[6].get_pos()->y+2*BALLR, mass_position[6].get_pos()->z+BALLR);

	glVertex3d(mass_position[3].get_pos()->x+2*BALLR, mass_position[3].get_pos()->y+2*BALLR, mass_position[3].get_pos()->z-2*BALLR);
	glVertex3d(mass_position[3].get_pos()->x+BALLR,   mass_position[3].get_pos()->y+2*BALLR, mass_position[3].get_pos()->z-2*BALLR);
	glVertex3d(mass_position[7].get_pos()->x+BALLR,   mass_position[7].get_pos()->y+2*BALLR, mass_position[7].get_pos()->z+2*BALLR);
	glVertex3d(mass_position[7].get_pos()->x+2*BALLR, mass_position[7].get_pos()->y+2*BALLR, mass_position[7].get_pos()->z+2*BALLR);

	glEnd();
	glPopName();
	//glEndList();
};
frame::frame(int ID) 
: object(ID)
, object_id(0) 
{  
	object_type = OBJ_ID_frame; 
	for(int i = 0; i < 8; ++i) 
		array_springs[i] = NULL; 
	object_selected = false; 
};

frame::~frame(void) 
{ 
	glDeleteLists(object_id, 1); 
};

const mass* frame::get_oposite_mass(int x, int y, int z) const
{ 
	if(x < 0 || x > 1 || y < 0 || y > 1 || z < 0 || z > 1) 
		return NULL; 
	return &mass_position[4*z+2*y+x]; 
};

vector3* frame::get_pos(int x, int y, int z) const
{ 
	if(x < 0 || x > 1 || y < 0 || y > 1 || z < 0 || z > 1) 
		return NULL; 
	return ((object)mass_position[4*z+2*y+x]).get_pos(); 
};

vector3* frame::inc_pos(real x, real y, real z) 
{ 
	for(int i = 0; i < 8; ++i) 
	{ 
		mass_position[i].get_pos()->x += x; 
		mass_position[i].get_pos()->y += y; 
		mass_position[i].get_pos()->z += z; 
	} 
	return &object_pos; 
};

vector3* frame::inc_pos(vector3* pos) 
{ 
	for(int i = 0; i < 8; ++i) 
		(*mass_position[i].get_pos()) = *pos; 
	return &object_pos; 
};

spring* frame::operator[](int index) 
{ 
	if(index < 0 || index > 8) 
		return NULL; 
	return array_springs[index]; 
};

void frame::opengl_draw(void) 
{ 
	glPushMatrix(); 
	glPushAttrib(GL_CURRENT_BIT);
	if(object_selected)
		glColor3d(0.6, 0.0, 0.0);
	//glCallList(object_id); 
	create_obj();
	glPopAttrib(); 
	glPopMatrix(); 
};

