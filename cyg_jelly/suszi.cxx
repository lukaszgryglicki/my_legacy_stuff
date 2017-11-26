#include "defs.hxx"
#include "suszi.hxx"
#include "scene.hxx"

suszi::suszi(int ID, scene* manager)
: object(ID)
, scene_ptr(manager)
{ 
};

suszi::suszi(std::istream &is, int ID, scene* manager)
: object(ID)
, scene_ptr(manager)
{
	is>>*this;
	//chk_conf();
};

suszi::~suszi(void) 
{
	for(list<vector3*>::iterator i = vectors.begin(); i != vectors.end(); ++i)
		free_ptr (*i);
	for(list<point3*>::iterator i = points.begin(); i != points.end(); ++i)
		free_ptr (*i);
	for(list<tri*>::iterator i = tri_list.begin(); i != tri_list.end(); ++i)
		free_ptr (*i);
};

void suszi::chk_conf(void)
{
	printf("chkconf.\n");
	int k = 0;
	printf("vectors\n");
	for(list<vector3*>::iterator i = vectors.begin(); i != vectors.end(); ++i)
	   {
		vector3* v = *i;
		printf("%d) (%f,%f,%f) (%p)\n", k, v->x, v->y, v->z, (void*)v);
		k++;
	   }
	k = 0;
	printf("points\n");
	for(list<point3*>::iterator i = points.begin(); i != points.end(); ++i)
	   {
		point3* v = *i;
		printf("%d) (%f,%f,%f,%f,%f,%f) (%p)\n", k, v->x, v->y, v->z, v->get_normal().x, v->get_normal().y, v->get_normal().z, (void*)v);
		k++;
	   }
	k = 0;
	printf("tris\n");
	for(list<tri*>::iterator i = tri_list.begin(); i != tri_list.end(); ++i)
	   {
		tri* t = *i;
		printf("%d) (%p,%p,%p) (%p)\n", k, (void*)t->get_vertex(0), (void*)t->get_vertex(1), (void*)t->get_vertex(2), (void*)t);
		printf("   ---> (%f,%f,%f) (%f,%f,%f) (%f,%f,%f)\n",
			t->get_vertex(0)->x, t->get_vertex(0)->y, t->get_vertex(0)->z,
			t->get_vertex(1)->x, t->get_vertex(1)->y, t->get_vertex(1)->z,
			t->get_vertex(2)->x, t->get_vertex(2)->y, t->get_vertex(2)->z);
		k++;
	   }
}

void suszi::Calculate(void)
{
	if(!scene_ptr || !scene_ptr->my_size)
		return;
	//chk_conf();
	int dim = scene_ptr->my_dim-1;
	my_BB.redef_u_basis(dim, 1, FALSE);
	my_BB.redef_v_basis(dim, 1, FALSE);
	my_BB.redef_s_basis(dim, 1, FALSE);
	list<vector3*>::iterator j = vectors.begin();
	int m, n, o;
/*	printf("defforming...\n");*/
	for(list<point3*>::iterator i = points.begin(); i != points.end(); ++i, ++j)
	{
		(*j)->clear();
		m = n = o = 0;
		for(m = 0; m <= dim; ++m)
			for(n = 0; n <= dim; ++n)
				for(o = 0; o <= dim; ++o)
		{
			(*(*j)) += 
				(*scene_ptr->my_array[scene_ptr->get_idx(m, n, o)]->get_pos())*
				my_BB.real_u_basis(m, (*i)->x)*
				my_BB.real_v_basis(n, (*i)->y)*
				my_BB.real_s_basis(o, (*i)->z);
			/*vector3 a = (*scene_ptr->my_array[scene_ptr->get_idx(m, n, o)]->get_pos());
			a += 1.5;
			a /= 3.;
			real bu = my_BB.real_u_basis(m, a.x);
			real bv = my_BB.real_v_basis(n, a.y);
			real bs = my_BB.real_s_basis(o, a.z);
			(*(*j)) += 
			printf("(%f %f %f) (%f %f %f) (%f,%f,%f)\n", a.x, a.y, a.z, bu, bv, bs, (*i)->x, (*i)->y, (*i)->z);*/

			//++m;			n = (m == dim) ? ++n : n;		o = (n == dim) ? ++o : o;
			//m %= dim;		n %= dim;						o %= dim;
		}
	}
 //chk_conf();
};

void suszi::MatrixMultiply(const matrix4& m)
{
	vector4 tmp;
	list<vector3*>::iterator j = vectors.begin();
	for(list<point3*>::iterator i = points.begin(); i != points.end(); ++i, ++j)
	{
		(*i)->get_normal() = (m*(*i)->get_normal()).uniformized();
		((vector3)(*(*i))) = (m*(*(*i))).uniformized();
		(*(*j)) = (m*(*(*j))).uniformized();
	}
};

void suszi::opengl_draw(void) 
{ 
	Calculate();
	glPushAttrib(GL_CURRENT_BIT); 
	//if(object_selected) glColor3d(0.0, 1.0, 0.0);
	glPushName(object_id);
	glBegin(GL_TRIANGLES);
	for(list<tri*>::iterator i = tri_list.begin(); i != tri_list.end(); ++i)
	{
		/*printf("triangle: %p\n", (void*)tr);
		printf("drawPt: (%p,%p,%p)\n", (void*)tr->get_vertex(0), (void*)tr->get_vertex(1), (void*)tr->get_vertex(2));
		printf("drawV1: (%f,%f,%f)\n", tr->get_vertex(0)->x, tr->get_vertex(0)->y, tr->get_vertex(0)->z);
		printf("drawV2: (%f,%f,%f)\n", tr->get_vertex(1)->x, tr->get_vertex(1)->y, tr->get_vertex(1)->z);
		printf("drawV3: (%f,%f,%f)\n", tr->get_vertex(2)->x, tr->get_vertex(2)->y, tr->get_vertex(2)->z);*/
		(*i)->opengl_draw();
	}
	glEnd();
	glPopName();
	glPopAttrib();
};
std::istream& operator>>(std::istream &is, suszi &suszi)
{
 int nVertices;
 int nTris;
 int i1,i2,i3;
 vector3 maxv(-1e10), minv(1e10);
 real dummy;
 is>>nVertices;
 suszi.vectors.clear();
 suszi.points.clear();
 suszi.tri_list.clear();

 vector3** vec;
 point3**  pos;
 tri** tr;

 pos = new point3*[nVertices];
 vec = new vector3*[nVertices];
 for (int i=0;i<nVertices;i++)
   {
    pos[i] = new point3;
	vec[i] = new vector3;
    is>>pos[i]->x>>pos[i]->y>>pos[i]->z;
    is>>pos[i]->get_normal().x>>pos[i]->get_normal().y>>pos[i]->get_normal().z;
    is>>dummy>>dummy;
	/*pos[i]->x += 150.;
	pos[i]->y -= 150.;
	pos[i]->z += 80.;*/
	pos[i]->x += 150./1.6;
	pos[i]->y -= 150./1.6;
	pos[i]->z += 80./1.6;
	pos[i]->y *= -1.;
    //pos[i]->x /= 50.;
	//pos[i]->y /= 50.;
	//pos[i]->z /= 50.;
	vec[i]->x = pos[i]->x;
	vec[i]->y = pos[i]->y;
	vec[i]->z = pos[i]->z;
    if (pos[i]->x < minv.x) minv.x = pos[i]->x;
    if (pos[i]->y < minv.y) minv.y = pos[i]->y;
    if (pos[i]->z < minv.z) minv.z = pos[i]->z;
    if (pos[i]->x > maxv.x) maxv.x = pos[i]->x;
    if (pos[i]->y > maxv.y) maxv.y = pos[i]->y;
    if (pos[i]->z > maxv.z) maxv.z = pos[i]->z;
    suszi.points.push_back(pos[i]);
	//printf("pushing pts: (%f,%f,%f) pts=%p, vec=%p\n", pos[i]->x, pos[i]->y, pos[i]->z,(void*)(pos[i]), (void*)(vec[i]));
    suszi.vectors.push_back(vec[i]);
   }
 is>>nTris;
 tr = new tri*[nTris];
 for (int i=0;i<nTris;i++)
   {
    tr[i] = new tri;
    is>>i1>>i2>>i3;
	//printf("pushing ptr %p\n", (void*)tr[i]);
	//printf("set_vertex1: (%f,%f,%f) %p\n",  pos[i1-1]->x, pos[i1-1]->y, pos[i1-1]->z, (void*)(vec[i1-1]));
	//printf("set_vertex2: (%f,%f,%f) %p\n",  pos[i2-1]->x, pos[i2-1]->y, pos[i2-1]->z, (void*)(vec[i2-1]));
	//printf("set_vertex3: (%f,%f,%f) %p\n",  pos[i3-1]->x, pos[i3-1]->y, pos[i3-1]->z, (void*)(vec[i3-1]));
    tr[i]->set_vertex(vec[i1-1], 0);
    tr[i]->set_vertex(vec[i2-1], 1);
    tr[i]->set_vertex(vec[i3-1], 2);
    suszi.tri_list.push_back(tr[i]);
   }
 list<point3*>::iterator l;
 list<vector3*>::iterator k;
 vector3 scale(maxv-minv);
 fabs(scale.x); 
 fabs(scale.y); 
 fabs(scale.z);
 //scale.x *= 1.2;
 //scale.y *= 1.2;
 //scale.z *= 1.2;
 //printf("scale: %f %f %f\n", scale.x, scale.y, scale.z);
	minv.x = -(maxv.x/scale.x)*0.5-(minv.x/scale.x); minv.y = -(maxv.y/scale.y)*0.5-(minv.y/scale.y); minv.z = -(maxv.z/scale.x)*0.5-(minv.z/scale.z);
	scale.x = MAX(MAX(scale.x, scale.y), scale.z);
	k = suszi.vectors.begin();
	for(l = suszi.points.begin(); l != suszi.points.end(); ++l, ++k)
	{
		//printf("scaller: %f,%f,%f (%p)\n", (*k)->x, (*k)->y, (*k)->z, (void*)(*k));
		(*k)->x += minv.x; 
		(*k)->y += minv.y; 
		(*k)->z += minv.z;
		(*k)->x /= scale.x; 
		(*k)->y /= scale.y; 
		(*k)->z /= scale.z;
		(*l)->x += minv.x; 
		(*l)->y += minv.y; 
		(*l)->z += minv.z;
		(*l)->x /= scale.x; 
		(*l)->y /= scale.y; 
		(*l)->z /= scale.z;
		//printf("scaller: %f,%f,%f\n", (*k)->x, (*k)->y, (*k)->z);
	}
 return is;
};
