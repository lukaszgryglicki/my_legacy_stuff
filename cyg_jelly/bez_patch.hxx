#ifndef __BPATCH_H__MDBMA__
#define __BPATCH_H__MDBMA__
#include "defs.hxx"
#include "vec3d.hxx"
#include "bern_tens_base.hxx"

class bez_patch	/* platek beziera */
{
protected:
	bern_tens_basis tensor_basis;

	bool		my_bSet;

	int			u_degree;
	int			v_degree;
	vector3***	control_array;
	int			u_steps;
	int			v_steps;										 
	vector3**	mesh_array;
	void clear_mesh_array(void) { if(!mesh_array) return; for(int i = 0; i < u_steps; ++i) free_array(mesh_array[i]); free_array(mesh_array); };
	void clear_control_array(void) { if(!control_array) return; for(int i = 0; i <= u_degree; ++i) free_array(control_array[i]); free_array(control_array); };
	void create_mesh_array(int usteps, int vsteps) 
	{ 
		u_steps = usteps; v_steps = vsteps;
		clear_mesh_array();
		mesh_array = new vector3*[u_steps];
		for(int i = 0; i < u_steps; ++i) 
			mesh_array[i] = new vector3[v_steps];
	};
	void create_control_array(int udegree, int vdegree)
	{
		u_degree = udegree; v_degree = vdegree;
		clear_control_array();
		control_array = new vector3**[u_degree+1];
		for(int i = 0; i <= u_degree; ++i) 
			control_array[i] = new vector3*[v_degree+1];
	};
public:
	virtual void calc_mesh(void);
	bez_patch(void) : control_array(NULL), mesh_array(NULL), u_degree(0), v_degree(0), u_steps(0), v_steps(0), my_bSet(TRUE) {};
	bez_patch(int udegree, int vdegree, int usteps, int vsteps, real UminTvalue = 0, real UmaxTvalue = 1, real VminTvalue = 0, real VmaxTvalue = 1)
		: control_array(NULL), mesh_array(NULL), u_degree(0), v_degree(0), u_steps(0), v_steps(0), my_bSet(TRUE)
	{
		create_mesh_array(usteps, vsteps);
		create_control_array(udegree, vdegree);
		tensor_basis.def_u_basis(udegree, usteps, UminTvalue, UmaxTvalue, TRUE); 
		tensor_basis.def_v_basis(vdegree, vsteps, VminTvalue, VmaxTvalue, TRUE); 
	};
	virtual ~bez_patch(void) 
	{ 
		clear_control_array();
		clear_mesh_array();
	};
	vector3*** get_control_array(void) { return control_array; };
	vector3** get_mesh_array(void) { return mesh_array; };
	void add_ctl_point(vector3* point, int Uindex, int Vindex)
	{ 
		control_array[Uindex][Vindex] = point;
	};
	int get_u_degree(void) { return u_degree; };
	int get_v_degree(void) { return v_degree; };
	int get_u_steps(void) { return u_steps; };
	int get_v_steps(void) { return v_steps; };
	void preprocess(void) { preprocess_u_base(); preprocess_v_base(); };
	void preprocess_u_base(void) { tensor_basis.rebuild_u_basis(); };
	void preprocess_v_base(void) { tensor_basis.rebuild_v_basis(); };
	void rebuild_u_basis(int degree, int steps, bool recreate = TRUE)
	{ tensor_basis.redef_u_basis(degree, steps, recreate); };
	void rebuild_v_basis(int degree, int steps, bool recreate = TRUE)
	{ tensor_basis.redef_v_basis(degree, steps, recreate); };
	void set_u_par_range(real minTvalue = 0, real maxTvalue = 1, bool recreate = FALSE)
	{ tensor_basis.set_u_par_range(minTvalue, maxTvalue, recreate); };
	void set_v_par_range(real minTvalue = 0, real maxTvalue = 1, bool recreate = FALSE)
	{ tensor_basis.set_v_par_range(minTvalue, maxTvalue, recreate); };
	void createU(int udegree, int usteps, bool preprocess = TRUE)
	{ tensor_basis.redef_u_basis(udegree, usteps, preprocess); };
	void createV(int vdegree, int vsteps, bool preprocess = TRUE)
	{ tensor_basis.redef_v_basis(vdegree, vsteps, preprocess); };
	vector3 get_mesh_pointR(real u, real v);
	vector3& get_mesh_pointI(int u, int v);
	vector3** operator[](int i) 
	  { 
	    return control_array[i]; 
	  };
};

#endif
