#include "defs.hxx"
#include "canvas.hxx"

glut_scene_window* gl_window = NULL;
int lastBtn = 0;

void on_paint(void)
{
 gl_window->render_callback();
}

void on_size(int w, int h)
{
 gl_window->resize(w, h);
}

void on_key(unsigned char key, int x, int y)
{
 gl_window->keyboard(key, x, y);
}

void on_idle(void)
{
 gl_window->pseudo_timer();
}

void visible(int vis)
{
 if (vis == GLUT_VISIBLE) glutIdleFunc(on_idle);
 else glutIdleFunc(NULL);
}

void on_mouse(int button, int state, int x, int y)
{
 gl_window->mouse(button, state, x, y, 0);
 lastBtn = button;
}

void on_motion(int x, int y)
{
 gl_window->mouse(lastBtn, 0, x, y, 1);
}

int main(int lb, char** par)
{
 printf("known keys: SPACE\n");
 printf("cvbfsurpqtmnwxad1..0-=\n");
 glutInit(&lb, par);
 glutInitDisplayMode(GLUT_DOUBLE);
 glutInitWindowSize(800, 600);
 glutInitWindowPosition(10, 10);
 glutCreateWindow(par[0]);
 gl_window = new glut_scene_window;
 glutDisplayFunc(on_paint);
 glutReshapeFunc(on_size);
 glutKeyboardFunc(on_key);
 glutVisibilityFunc(visible);
 glutMouseFunc(on_mouse);
 glutMotionFunc(on_motion);
 glutMainLoop();
}

