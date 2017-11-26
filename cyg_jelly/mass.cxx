#include "defs.hxx"
#include "mass.hxx"
#include "frame.hxx"
#include "spring.hxx"
#include "room.hxx"

void mass::opengl_draw(void) 
{ 
	glPushMatrix(); 
	glPushAttrib(GL_CURRENT_BIT); 
	glTranslated(object_pos.x, object_pos.y, object_pos.z);
	if(object_selected)
		glColor3d(0.6, 0.0, 0.0);
	else 
		glColor3d(0.1, 0.1, 0.7);
	glCallList(object_id); 
	glPopAttrib(); 
	glPopMatrix(); 
};

void mass::create_obj(void) 
{ 
	object_id = glGenLists(1);
	glLoadIdentity();
	glNewList(object_id, GL_COMPILE_AND_EXECUTE);
	glPushName(object_id);
	glutSolidSphere(BALLR*2., 8, 8);
	glPopName();
	glEndList();
};


mass::mass(int ID, const vector3& pos, const vector3& vel, double* dmass, double* dDelta, double* dK, double* dFadeOut) 
: object(ID, pos, vel, dDelta)
, frame_spring(NULL)
, object_id(0) 
, own_mass(dmass)
, own_k(dK)
, own_fade_out(dFadeOut)
{  
	object_type = OBJ_ID_mass; 
	for(int i = 0; i < 18; ++i) 
		array_springs[i] = NULL; 
	object_selected = false; 
//	object_old_pos = object_pos - my_dDelta * my_vVelocity;
};

void mass::run_physics_step()
{
	// tablica 18tu springow (wskaznikow)
// Y |			 6   2  4  Z |		12   8 10	 Y |		16   2 14
//	 |			 1 x--  0	 |		 1 x--  0	   |		 9 x--  8
//	 |____X		 5   3  7	 |____X 11   9 13	   |_____Z  15   3 17
	vector3 f(0.0,0.0,0.0);
	double fval = 0.0;
	vector3 r(0.0,0.0,0.0);
	for(int i=0; i<18; ++i)
	{
		if(!array_springs[i]) continue;		//dla tych na zewnatrz
		r = object_pos - array_springs[i]->get_oposite_mass(this)->object_pos;
		fval = array_springs[i]->get_c() * (array_springs[i]->get_def_length() - r.magnitude());
		if(r.magnitude() != 0) f += r.normalized() * fval;
		else f += object_vel.normalized() * fval;
	}
	if (frame_spring)
	{
		r = object_pos - frame_spring->get_oposite_mass(this)->object_pos;
		fval = frame_spring->get_c() * (frame_spring->get_def_length() - r.magnitude());
		if(r.magnitude() != 0) f += r.normalized() * fval;
	}
	f -= (*own_k) * object_vel;
	current_pos = f * (*delta) * (*delta) / (*own_mass);
	current_pos += object_pos * 2;
	current_pos -= object_old_pos;
	object_vel = (current_pos - object_old_pos) / (2 * (*delta));
	object_old_pos = object_pos;
}

void mass::run_physics_step_with_room(room* room)
{
	vector3 f(0.0,0.0,0.0);
	double fval = 0.0;
	vector3 r(0.0,0.0,0.0);
	for(int i=0; i<18; ++i)
	{
		if(!array_springs[i]) continue;
		r = object_pos - array_springs[i]->get_oposite_mass(this)->object_pos;
		fval = array_springs[i]->get_c() * (array_springs[i]->get_def_length() - r.magnitude());
		if(r.magnitude() != 0) f += r.normalized() * fval;
		else f += object_vel.normalized() * fval;
	}
	if(frame_spring)
	{
		r = object_pos - frame_spring->get_oposite_mass(this)->object_pos;
		fval = frame_spring->get_c() * (frame_spring->get_def_length() - r.magnitude());
		if(r.magnitude() != 0) f += r.normalized() * fval;
	}
	f -= (*own_k) * object_vel;
	vector3 newPos = f * (*delta) * (*delta) / (*own_mass);
	newPos += object_pos * 2;
	newPos -= object_old_pos;
	vector3 oldPos = object_pos;
	//po wszystkich scianach
	vector3 tempV1, tempV2, tempV3, tempR;
	double tempDelta = (*delta);
	tempV3 = object_pos;
	while(true)
	{
		//XY (000->110) (001->111)
		tempV1 = *(room->get_pos(0,0,0));
		tempV2 = *(room->get_pos(1,1,1));
		if((newPos.z < tempV1.z) && (object_pos.z > tempV1.z))
		{
			tempV3.z = tempV1.z;
			//(z1 - z2) / (y1 - y2) = (z3 - z2) / (y3 - y2)
			//(y3 - y2) = (z3 - z2) * (y1 - y2) / (z1 - z2)
			tempV3.y = ((tempV1.z  - oldPos.z) * (newPos.y - oldPos.y) / (newPos.z - oldPos.z)) + oldPos.y;
			//(x1 - x2) / (y1 - y2) = (x3 - x2) / (y3 - y2)
			//(x3 - x2) = (x1 - x2) * (y3 - y2) / (y1 - y2)
			tempV3.x = ((newPos.x - oldPos.x) * (tempV3.y - oldPos.y) / (newPos.y - oldPos.y)) + oldPos.x;
			newPos.z -= 2 * (newPos.z - tempV3.z);
			r = newPos - tempV3;
			tempR = newPos - oldPos;
			tempDelta *= tempR.magnitude() / r.magnitude();
			newPos = tempV3 + r * (*own_fade_out);
			oldPos = tempV3;
			continue;
		} else if((newPos.z > tempV2.z) && (object_pos.z < tempV2.z))
		{
			tempV3.z = tempV2.z;
			//(z1 - z2) / (y1 - y2) = (z3 - z2) / (y3 - y2)
			//(y3 - y2) = (z3 - z2) * (y1 - y2) / (z1 - z2)
			tempV3.y = ((tempV2.z  - newPos.z) * (oldPos.y - newPos.y) / (oldPos.z - newPos.z)) + newPos.y;
			//(x1 - x2) / (y1 - y2) = (x3 - x2) / (y3 - y2)
			//(x3 - x2) = (x1 - x2) * (y3 - y2) / (y1 - y2)
			tempV3.x = ((oldPos.x - newPos.x) * (tempV3.y - newPos.y) / (oldPos.y - newPos.y)) + newPos.x;
			newPos.z += 2 * (tempV3.z - newPos.z);
			r = newPos - tempV3;
			tempR = newPos - oldPos;
			tempDelta *= tempR.magnitude() / r.magnitude();
			newPos = tempV3 + r * (*own_fade_out);
			oldPos = tempV3;
			continue;
		}
		//YZ (000->011) (100->111)
		if((newPos.x < tempV1.x) && (object_pos.x > tempV1.x))
		{
			tempV3.x = tempV1.x;
			//(x1 - x2) / (y1 - y2) = (x3 - x2) / (y3 - y2)
			//(y3 - y2) = (x3 - x2) * (y1 - y2) / (x1 - x2)
			tempV3.y = ((tempV1.x  - oldPos.x) * (newPos.y - oldPos.y) / (newPos.x - oldPos.x)) + oldPos.y;
			//(z1 - z2) / (y1 - y2) = (z3 - z2) / (y3 - y2)
			//(z3 - z2) = (z1 - z2) * (y3 - y2) / (y1 - y2)
			tempV3.z = ((newPos.z - oldPos.z) * (tempV3.y - oldPos.y) / (newPos.y - oldPos.y)) + oldPos.z;
			newPos.x -= 2 * (newPos.x - tempV3.x);
			r = newPos - tempV3;
			tempR = newPos - oldPos;
			tempDelta *= tempR.magnitude() / r.magnitude();
			newPos = tempV3 + r * (*own_fade_out);
			oldPos = tempV3;
			continue;
		} else if((newPos.x > tempV2.x) && (object_pos.x < tempV2.x))
		{
			tempV3.x = tempV2.x;
			//(x1 - x2) / (y1 - y2) = (x3 - x2) / (y3 - y2)
			//(y3 - y2) = (x3 - x2) * (y1 - y2) / (x1 - x2)
			tempV3.y = ((tempV2.x  - newPos.x) * (oldPos.y - newPos.y) / (oldPos.x - newPos.x)) + newPos.y;
			//(z1 - z2) / (y1 - y2) = (z3 - z2) / (y3 - y2)
			//(z3 - z2) = (z1 - z2) * (y3 - y2) / (y1 - y2)
			tempV3.z = ((oldPos.z - newPos.z) * (tempV3.y - newPos.y) / (oldPos.y - newPos.y)) + newPos.z;
			newPos.x += 2 * (tempV3.x - newPos.x);
			r = newPos - tempV3;
			tempR = newPos - oldPos;
			tempDelta *= tempR.magnitude() / r.magnitude();
			newPos = tempV3 + r * (*own_fade_out);
			oldPos = tempV3;
			continue;
		}
		//XZ (000->101) (010->111)
		if((newPos.y < tempV1.y) && (object_pos.y > tempV1.y))
		{
			tempV3.y = tempV1.y;
			//(y1 - y2) / (z1 - z2) = (y3 - y2) / (z3 - z2)
			//(z3 - z2) = (y3 - y2) * (z1 - z2) / (y1 - y2)
			tempV3.z = ((tempV1.y  - oldPos.y) * (newPos.z - oldPos.z) / (newPos.y - oldPos.y)) + oldPos.z;
			//(x1 - x2) / (z1 - z2) = (x3 - x2) / (z3 - z2)
			//(x3 - x2) = (x1 - x2) * (z3 - z2) / (z1 - z2)
			tempV3.x = ((newPos.x - oldPos.x) * (tempV3.z - oldPos.z) / (newPos.z - oldPos.z)) + oldPos.x;
			newPos.y -= 2 * (newPos.y - tempV3.y);
			r = newPos - tempV3;
			tempR = newPos - oldPos;
			tempDelta *= tempR.magnitude() / r.magnitude();
			newPos = tempV3 + r * (*own_fade_out);
			oldPos = tempV3;
			continue;
		} else if((newPos.y > tempV2.y) && (object_pos.y < tempV2.y))
		{
			tempV3.y = tempV2.y;
			//(y1 - y2) / (z1 - z2) = (y3 - y2) / (z3 - z2)
			//(z3 - z2) = (y3 - y2) * (z1 - z2) / (y1 - y2)
			tempV3.z = ((tempV2.y  - newPos.y) * (oldPos.z - newPos.z) / (oldPos.y - newPos.y)) + newPos.z;
			//(x1 - x2) / (z1 - z2) = (x3 - x2) / (z3 - z2)
			//(x3 - x2) = (x1 - x2) * (z3 - z2) / (z1 - z2)
			tempV3.x = ((oldPos.x - newPos.x) * (tempV3.z - newPos.z) / (oldPos.z - newPos.z)) + newPos.x;
			newPos.y += 2 * (tempV3.y - newPos.y);
			r = newPos - tempV3;
			tempR = newPos - oldPos;
			tempDelta *= tempR.magnitude() / r.magnitude();
			newPos = tempV3 + r * (*own_fade_out);
			oldPos = tempV3;
			continue;
		}
		break;
	}

//	object_old_pos = object_pos;
	object_old_pos = tempV3;
	current_pos = newPos;
	object_vel = (current_pos - object_old_pos) / (tempDelta);
}

mass::~mass(void) 
{ 
	glDeleteLists(object_id, 1); 
};

spring* mass::operator[](int index) 
{ 
	if(index < -1 || index > 17) 
		return NULL; 
	if(index = -1) 
		return frame_spring; 
	return array_springs[index]; 
};

