#include "defs.hxx"
#include "canvas.hxx"
#include <time.h>
static time_t t1;
static int frame, pframe;
enum
{
 SCALING,
 TRANSLATE_XY,
 TRANSLATE_XZ,
 TRANSLATE_YZ,
 ROTATE_XY,
 ROTATE_XZ,
 ROTATE_YZ
};

glut_scene_window::glut_scene_window()
{
/*	TRACE_LINE;*/
	init_opengl();
	rotation_angles[0]		= rotation_angles[1]	= rotation_angles[2]	= 20.0f;
	translation_values[0]	= translation_values[1] = 0.0f;
	translation_values[2] = -10.0f;
	scale_value = 1.;
	spring_visible = false;
	physics_running = true;
	something_selected = false;
	masses_visible = true;
	cur_op = TRANSLATE_XY;
	objects = new scene;
	keyboard('c', 0, 0);
	keyboard('f', 0, 0);
	keyboard('s', 0, 0);
	//keyboard('b', 0, 0);
	keyboard('u', 0, 0);
	keyboard('r', 0, 0);
        time(&t1);
	frame = 0;
	pframe = 0;
/*	TRACE_LINE;*/
}

glut_scene_window::~glut_scene_window(void)
{
	clear_scene();
/*	TRACE_LINE;*/
}

int			// USING GL_SELECT to find object
glut_scene_window::find_object_id(const int posX, const int posY)
{
/*	TRACE_LINE;*/
	int l_objectsFound = 0;
	int	l_viewportCoords[4] = {0};
	unsigned int l_selectBuffer[32] = {0};				

	glSelectBuffer(32, l_selectBuffer);
	glGetIntegerv(GL_VIEWPORT, l_viewportCoords);
	glMatrixMode(GL_PROJECTION);

	glPushMatrix();
	glRenderMode(GL_SELECT);
	glLoadIdentity();
	gluPickMatrix(posX, l_viewportCoords[3] - posY, 2, 2, l_viewportCoords);
	gluPerspective(45.0f, sizeX/(GLfloat)sizeY, 0.1f, 100.0f);
	glMatrixMode(GL_MODELVIEW);

	render_objects();
	l_objectsFound = glRenderMode(GL_RENDER);

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);

	if(l_objectsFound > 0)
	{		
		unsigned int l_lowestDepth = l_selectBuffer[1];
		int l_selectedObject = l_selectBuffer[3];

		for(int i = 1; i < l_objectsFound; ++i)
		{
			if(l_selectBuffer[(i*4)+1] < l_lowestDepth)
			{
				l_lowestDepth = l_selectBuffer[(i*4)+1];
				l_selectedObject = l_selectBuffer[(i*4)+3];
			}
		}
/*	TRACE_LINE;*/
		return l_selectedObject;
	}
/*	TRACE_LINE;*/
	return -1;											
}

void 
glut_scene_window::set_window_title(void)
{
 char tstr[256];
 char params[192];
 int fpss;
 time_t t2;
 switch (cur_op)
		   {
		       case TRANSLATE_XY:
		       case TRANSLATE_XZ:
		       case TRANSLATE_YZ:
			   strcpy(tstr, "Translations; ");
			   break;
		       case ROTATE_XY:
		       case ROTATE_XZ:
		       case ROTATE_YZ:
			   strcpy(tstr, "Rotations; ");
			   break;
		       case SCALING:
			   strcpy(tstr, "Scaling; ");
			   break;
		   }
 time(&t2);
 sprintf(params, "Ci=%f Delta=%f, M=%f K=%f Fade=%f Ce=%f"
	 , objects->my_c_internal
	 , objects->delta
	 , objects->my_mass
	 , objects->my_K
	 , objects->my_Fade
	 , objects->my_c_external
	 );
 strcat(tstr, params);
 glutSetWindowTitle(tstr);
 if (((t2 - t1) > 0) && ((t2 - t1) < 1e5))
 printf("elapsed: %ds, frames: %d, phys: %d, FPS: %f, PPS: %f\n"
	 , (int)t2-(int)t1
	 , frame, pframe 
	 , (float)frame/(float)((int)t2-(int)t1)
	 , (float)pframe/(float)((int)t2-(int)t1)
       );
}

void
glut_scene_window::keyboard(unsigned char key, int x, int y)
{
/*	TRACE_LINE;*/
 int incrLX = 0;
 int incrLY = 0;
 matrix4 m, n;
	switch (key)
	{
		case 'w':		/* transformacje */
		case 'x':
		case 'a':
		case 'd':
		 //
			if (key == 'w') incrLX = 4;
			if (key == 'x') incrLX = -4;
			if (key == 'a') incrLY = 4;
			if (key == 'd') incrLY = -4;
		 	if (cur_op == SCALING)
			{
				if(objects->get_selection())
					objects->move_selected(m.load_scale(vector3(incrLY/100.0)));
				else
					scale_value += incrLY/100.0;
			}
			else if(cur_op == TRANSLATE_XY)
			{
				if(objects->get_selection())
					objects->move_selected(m.load_translation(vector3(incrLX/100.0, -incrLY/100.0, 0)));
				else
				{
					translation_values[0] += incrLX/100.0;
					translation_values[1] -= incrLY/100.0;
				}
			}
			else if(cur_op == TRANSLATE_XZ)
			{
				if(objects->get_selection())
					objects->move_selected(m.load_translation(vector3(incrLX/100.0, 0, incrLY/100.0)));
				else
				{
					translation_values[0] += incrLX/100.0;
					translation_values[2] += incrLY/100.0;
				}
			}
			else if(cur_op == TRANSLATE_YZ)
			{
				if(objects->get_selection())
					objects->move_selected(m.load_translation(vector3(0, -incrLY/100.0, incrLX/100.0)));
				else
				{
					translation_values[1] -= incrLY/100.0;
					translation_values[2] += incrLX/100.0;
				}
			}
			else if(cur_op == ROTATE_XY)
			{
				if(objects->get_selection())
					objects->move_selected(m.load_rotx(incrLY>>2)*n.load_roty(incrLX>>2));
				else
				{
					rotation_angles[0] += incrLY>>2;
					rotation_angles[1] += incrLX>>2;
				}
			}
			else if(cur_op == ROTATE_XZ)
			{
				if(objects->get_selection())
					objects->move_selected(m.load_roty(incrLX>>2)*n.load_rotz(incrLY>>2));
				else
				{
					rotation_angles[1] += incrLX>>2;
					rotation_angles[2] += incrLY>>2;
				}
			}
			else if(cur_op == ROTATE_YZ)
			{
				if(objects->get_selection())
					objects->move_selected(m.load_rotx(incrLY>>2)*n.load_rotz(incrLX>>2));
				else
				{
					rotation_angles[0] += incrLY>>2;
					rotation_angles[2] += incrLX>>2;
				}
			}
		 //
		 break;
	 case ' ':	/* domyslne */
		rotation_angles[0] = rotation_angles[1] = rotation_angles[2] = 0;
		translation_values[0] = translation_values[1] = 0;
		translation_values[2] = -10;
		scale_value = 1;
		break;
	 case 'q': exit(0); break;
	 case 'v':
		masses_visible = ! masses_visible;
		   if (objects->is_cube_created()) 
		       objects->show_cube(masses_visible);
		break;
	 case 'c':
		   if (!objects->is_cube_created())
		     {
		      objects->create_cube(4, 1);
		      objects->show_cube();
		      printf("Cube created.\n");
		     }
		   else
		     {
		      objects->clear_cube();
		      objects->show_cube(FALSE);
		      printf("Cube destroyed.\n");
		     }
		   break;
	 case 's':
		   if (!spring_visible)
		     {
		      objects->show_springs();
		      printf("spring created.\n");
		     }
		   else
		     {
		      objects->show_springs(FALSE);
		      printf("spring destroyed.\n");
		     }
		   spring_visible = ! spring_visible;
		   break;
	 case 'p': physics_running = ! physics_running; break;
	 case 't': 
		   cur_op ++;
		   if (cur_op == ROTATE_YZ +1) cur_op = 0;
		   break;
	 case 'm':
		 if (objects->is_frame_created())
		      {
			   if (!objects->get_frame()->is_object_selected()) 
			     {
			      objects->deselect_all();
			      objects->object_select(objects->get_frame()->get_object_id());
			      something_selected = true;
			     }
			   else { objects->deselect_all(); something_selected = false; }
		      }
			break;
	 case 'n':
		 	if (objects->is_cube_created())
		      {
			   mass* mm = objects->my_array[objects->current_mass_sel];
			   int id = mm->get_object_id();
			   if (!objects->is_object_selected(id)) 
			     {
			       objects->deselect_all();
			       objects->object_select(id);
			       something_selected = true;
			     }
			   else 
			     {
				  objects->deselect_all();
				  something_selected = false;
				  objects->current_mass_sel ++;
				  if (objects->current_mass_sel == objects->my_dim * objects->my_dim * objects->my_dim)
					  objects->current_mass_sel = 0;
				 }
		      }
			else objects->current_mass_sel = 0;
		 
		 break;
	 case 'f':
		   if (!objects->is_frame_created())
		     {
		      objects->create_frame(3);
		      objects->show_frame();
		      printf("frame created.\n");
		     }
		   else
		     {
		      objects->clear_frame();
		      objects->show_frame(FALSE);
		      printf("frame destroyed.\n");
		     }
		   break;
	 case 'u':
		   if (!objects->is_suszi_created())
		     {
		      fstream file("suszi.tri");
		      objects->create_suszi(file);
		      file.close();
		      objects->show_suszi();
		      printf("suszi created.\n");
		     }
		   else
		     {
		      objects->clear_suszi();
		      objects->show_suszi(FALSE);
		      printf("suszi destroyed.\n");
		     }
		   break;
	 case 'b':
		   if (!objects->is_bez_wall_created())
		     {
		      objects->create_bez_wall(10, 10);
		      objects->show_bez_wall();
		      printf("Bezier Cube created.\n");
		     }
		   else
		     {
		      objects->clear_bez_wall();
		      objects->show_bez_wall(FALSE);
		      printf("Bezier Cube destroyed.\n");
		     }
		   break;
	 case 'r':
		   if (!objects->is_room_created())
		     {
		      objects->create_room(10, 10, 10);
		      objects->show_room();
		      printf("room created.\n");
		     }
		   else
		     {
	              objects->clear_room();
		      objects->show_room(FALSE);
		      printf("room destroyed.\n");
		     }
		   break;
	 case '1':
	 	    objects->my_c_internal *= 1.05;
		    break;
	 case '3':
	 	    objects->delta *= 1.05;
		    break;
	 case '5':
	            objects->my_mass *= 1.05;
		    break;
	 case '7':
	            objects->my_K *= 1.05;
		    break;
	 case '9':
	            objects->my_Fade *= 1.05;
		    break;
	 case '-':
	            objects->my_c_external *= 1.05;
		    break;
	 case '2':
	 	    objects->my_c_internal /= 1.05;
		    break;
	 case '4':
	 	    objects->delta /= 1.05;
		    break;
	 case '6':
	            objects->my_mass /= 1.05;
		    break;
	 case '8':
	            objects->my_K /= 1.05;
		    break;
	 case '0':
	            objects->my_Fade /= 1.05;
		    break;
	 case '=':
	            objects->my_c_external /= 1.05;
		    break;
	}
 set_window_title();
/*	TRACE_LINE;*/
}

void 
glut_scene_window::init_opengl(void)
{
/*	TRACE_LINE;*/
	glShadeModel(GL_SMOOTH);
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glEnable(GL_CULL_FACE);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	GLfloat l_ambientLight[]	= { 0.2f, 0.2f, 0.2f, 0.2f };
	GLfloat l_diffuseLight[]	= { 0.7f, 0.7f, 0.7f, 1.0f };
	GLfloat l_specularLight[]	= { 0.7f, 0.7f, 0.7f, 1.0f };
	GLfloat l_positionLight[]	= { 0.0f, 0.0f, 2.0f, 0.0f };
	glLightfv(GL_LIGHT0, GL_AMBIENT, l_ambientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, l_diffuseLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, l_specularLight);
	glLightfv(GL_LIGHT0, GL_POSITION, l_positionLight);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	GLfloat l_ambientMaterial[]		= { 0.7f, 0.7f, 0.7f, 1.0f };
	GLfloat l_diffuseMaterial[]		= { 0.1f, 0.5f, 0.8f, 1.0f };
	GLfloat l_specularMaterial[]	= { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat l_shininessMaterial[]	= { 10.0f };
	GLfloat l_emissionMaterial[]	= { 0.3f, 0.2f, 0.2f, 0.0f};
	glMaterialfv(GL_FRONT, GL_AMBIENT, l_ambientMaterial);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, l_diffuseMaterial);
	glMaterialfv(GL_FRONT, GL_SPECULAR, l_specularMaterial);
	glMaterialfv(GL_FRONT, GL_SHININESS, l_shininessMaterial);
	glMaterialfv(GL_FRONT, GL_EMISSION, l_emissionMaterial);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);
	
	glEnable(GL_NORMALIZE);
/*	TRACE_LINE;*/
}

void 
glut_scene_window::clear_scene(void)
{
/*	TRACE_LINE;*/
	free_ptr(objects);
/*	TRACE_LINE;*/
}


void 
glut_scene_window::resize(int x, int y)
{
/*	TRACE_LINE;*/
	glViewport(0, 0, x, y);
	glMatrixMode(GL_PROJECTION); 
	glLoadIdentity(); 
	gluPerspective(45.0f, (GLfloat)x/(GLfloat)y, 0.1f, 100.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	sizeX = x;
	sizeY = y;
/*	TRACE_LINE;*/
}

void 
glut_scene_window::render_objects(void)
{
/*	TRACE_LINE;*/
	objects->opengl_draw();
    	frame++;
/*	TRACE_LINE;*/
}

void 
glut_scene_window::pseudo_timer(void)
{
/*	TRACE_LINE;*/
	if (physics_running) 
	  {
	   objects->run_physics_step();
    	   pframe++;
	  }
	// Add any code executed while window has no messages coming
	//Refresh();
}

void
glut_scene_window::mouse(int button, int state, int x, int y, int motion)
{
/*	TRACE_LINE;*/
	int left_down = (state==0) && (button==0);
	int left_up = (state==1) && (button==0);
	int right_down = (state==0) && (button==2);
	int right_up = (state==1) && (button==2);
/*	printf("lu = %d, ru = %d, ld = %d, rd = %d, mot+%d\n", */
/*		left_up, right_up, left_down, right_down, motion);*/
	if (left_down && !motion)
	{
	    	left_drag_x = x;
	    	left_drag_y = y;
/*		printf("leftdown::Get selection\n");*/
		int sel;
		if((sel = find_object_id(x, y)) == -1)
		      {
/*			printf("deselecting All.\n");*/
/*			objects->deselect_all();*/
		      }
		else if (!something_selected)
		      {
/*			printf("Selecting %d\n", sel);*/
			objects->object_select(sel);
			something_selected = true;
		      }
	}
	else if ((right_up || right_down) && !motion)
	{
/*	    		printf("leftup:: deselecting all!\n");*/
			objects->deselect_all();
			something_selected = false;
	}
	else if (motion && (left_down || right_down))
	{
/*	    printf("In motion...\n");*/
		if (left_down)
		{
			int incrLX = x-left_drag_x;
			int incrLY = y-left_drag_y;
			matrix4 m, n;
			if (cur_op == SCALING)
			{
				if(objects->get_selection())
					objects->move_selected(m.load_scale(vector3(incrLY/100.0)));
				else
					scale_value += incrLY/100.0;
			}
			else if(cur_op == TRANSLATE_XY)
			{
				if(objects->get_selection())
					objects->move_selected(m.load_translation(vector3(incrLX/100.0, -incrLY/100.0, 0)));
				else
				{
					translation_values[0] += incrLX/100.0;
					translation_values[1] -= incrLY/100.0;
				}
			}
			else if(cur_op == TRANSLATE_XZ)
			{
				if(objects->get_selection())
					objects->move_selected(m.load_translation(vector3(incrLX/100.0, 0, incrLY/100.0)));
				else
				{
					translation_values[0] += incrLX/100.0;
					translation_values[2] += incrLY/100.0;
				}
			}
			else if(cur_op == TRANSLATE_YZ)
			{
				if(objects->get_selection())
					objects->move_selected(m.load_translation(vector3(0, -incrLY/100.0, incrLX/100.0)));
				else
				{
					translation_values[1] -= incrLY/100.0;
					translation_values[2] += incrLX/100.0;
				}
			}
			else if(cur_op == ROTATE_XY)
			{
				if(objects->get_selection())
					objects->move_selected(m.load_rotx(incrLY>>2)*n.load_roty(incrLX>>2));
				else
				{
					rotation_angles[0] += incrLY>>2;
					rotation_angles[1] += incrLX>>2;
				}
			}
			else if(cur_op == ROTATE_XZ)
			{
				if(objects->get_selection())
					objects->move_selected(m.load_roty(incrLX>>2)*n.load_rotz(incrLY>>2));
				else
				{
					rotation_angles[1] += incrLX>>2;
					rotation_angles[2] += incrLY>>2;
				}
			}
			else if(cur_op == ROTATE_YZ)
			{
				if(objects->get_selection())
					objects->move_selected(m.load_rotx(incrLY>>2)*n.load_rotz(incrLX>>2));
				else
				{
					rotation_angles[0] += incrLY>>2;
					rotation_angles[2] += incrLX>>2;
				}
			}
			left_drag_x = x;
			left_drag_y = y;
		}
		if (right_down)
		{
			int incrRX = x-right_drag_x;
			int incrRY = x-right_drag_y;
			right_drag_x = x;
			right_drag_y = y;
		}
	}
/*	TRACE_LINE;*/
}

void 
glut_scene_window::render_callback(void)
{	
 /*GLfloat mdef[] = { 1.  , 0.  , 0.  , 0.,
     	            0.  , 1.  , 0.  , 0.,
		    0.  , 0.  , 1.  , 0.,
		    0.  , 0.  , 0.  , 1. };*/
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
/*	TRACE_LINE;*/
	//glMatrixMode(GL_PROJECTION);
	//glLoadIdentity();
	//glFrustum(-1.0, 1.0, -1.0, 1.0, 0.1, 100);
		//// Add any world transformation here
	//glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//glMultMatrixf(mdef);
	glTranslatef(translation_values[0], translation_values[1], translation_values[2]);
	glRotatef(rotation_angles[0], 1.f, 0.f, 0.f);
	glRotatef(rotation_angles[1], 0.f, 1.f, 0.f);
	glRotatef(rotation_angles[2], 0.f, 0.f, 1.f);
	glScalef(scale_value, scale_value, scale_value);
	render_objects();
	glFinish();
	glutSwapBuffers();
	glutPostRedisplay();
/*	TRACE_LINE;*/
}

