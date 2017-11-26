#ifndef __CANVAS_H__MDBMA__
#define __CANVAS_H__MDBMA__
#include "defs.hxx"
#include "scene.hxx"
#include "matrix4d.hxx"

class glut_scene_window		/* klasa okna */
{
private:
	scene* objects;
	
	int		cap_cnt;
	float	rotation_angles[3];
	float	translation_values[3];
	float	scale_value;

	int	left_drag_x;
	int	left_drag_y;
	int     right_drag_x;
	int     right_drag_y;
	int sizeX, sizeY;
	int cur_op;
	bool spring_visible;
	bool physics_running;
	bool something_selected;
	bool masses_visible;

public:
	glut_scene_window();
	~glut_scene_window(void);
	
	void init_opengl(void);
	void set_window_title();
	int find_object_id(const int posX, const int posY);
	void clear_scene(void);
	void render_objects(void);
	void keyboard(unsigned char key, int x, int y);
	//void OnViewMenu(wxCommandEvent& event);
	//void OnTimer(wxTimerEvent& WXUNUSED(event));
	void pseudo_timer(void);
	//void OnErase(wxEraseEvent& WXUNUSED(event));
	void render_callback(void);
	void resize(int w, int h);
	void mouse(int button, int state, int x, int y, int motion);
private:
};

#endif
