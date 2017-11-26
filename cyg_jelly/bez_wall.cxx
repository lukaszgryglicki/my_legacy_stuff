#include "defs.hxx"
#include "bez_wall.hxx"

bez_wall::bez_wall(int ID, int udegree, int vdegree, int usteps, int vsteps) 
: object(ID)
, bez_patch(udegree, vdegree, usteps, vsteps) 
{
};

bez_wall::~bez_wall(void) 
{
};

void bez_wall::calc_mesh(void)
{
	if(!control_array || !my_bSet)
		return;
	for(int u = 0; u < u_steps; ++u)
		for(int v = 0; v < v_steps; ++v)
		{
			mesh_array[u][v].clear();
			for(int i = 0; i <= u_degree; ++i)
				for(int j = 0; j <= v_degree; ++j)
				{
					if(!control_array[i][j])
					{
						my_bSet = FALSE;
						return;
					}
					mesh_array[u][v] += (*control_array[i][j])*tensor_basis.u_basis(i, u)*tensor_basis.u_basis(j, v);
				}
				mesh_array[u][v] += object_pos;
		}
		my_bSet = TRUE;
};

void bez_wall::opengl_draw(void)
{
	if(control_array)
		calc_mesh();
	glPushAttrib(GL_CURRENT_BIT); 
	if (object_selected) glColor3d(0.6, 0.0, 0.0);
	vector3 n;
	glPushName(object_id);
	glBegin(GL_TRIANGLES);
	for(int u = 0; u < u_steps-1; ++u)
		for(int v = 0; v < v_steps-1; ++v)
		{
			n = cross(mesh_array[u][v]-mesh_array[u+1][v], mesh_array[u][v]-mesh_array[u][v+1]);
			glNormal3dv(n.v());
			glVertex3dv(mesh_array[u][v+1].v()); glVertex3dv(mesh_array[u][v].v()); glVertex3dv(mesh_array[u+1][v].v());
			// blending
			n.x = -n.x; n.y = -n.y; n.z = -n.z;
			glNormal3dv(n.v());
			glVertex3dv(mesh_array[u+1][v].v()); glVertex3dv(mesh_array[u][v].v()); glVertex3dv(mesh_array[u][v+1].v());
			n = cross(mesh_array[u+1][v+1]-mesh_array[u][v+1], mesh_array[u+1][v+1]-mesh_array[u+1][v]);
			glNormal3dv(n.v());
			glVertex3dv(mesh_array[u+1][v].v()); glVertex3dv(mesh_array[u+1][v+1].v()); glVertex3dv(mesh_array[u][v+1].v());
			// blending
			n.x = -n.x; n.y = -n.y; n.z = -n.z;
			glNormal3dv(n.v());
			glVertex3dv(mesh_array[u][v+1].v()); glVertex3dv(mesh_array[u+1][v+1].v()); glVertex3dv(mesh_array[u+1][v].v());
		}
		glEnd();
		glPopName();
		glPopAttrib();
};
