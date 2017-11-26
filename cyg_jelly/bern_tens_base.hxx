#ifndef __BTBASE_H__MDBMA__
#define __BTBASE_H__MDBMA__
#include "defs.hxx"
#include "bern_base.hxx"

class bern_tens_basis	/* baza tensorowa */
{
	bern_basis my_U;
	bern_basis my_V;
public:
	bern_tens_basis(void) {};
	~bern_tens_basis(void) {};

	void rebuild_u_basis(void) { my_U.calc_basis(); };
	void rebuild_v_basis(void) { my_V.calc_basis(); };
	void set_u_par_range(real minTvalue, real maxTvalue, bool recreate = FALSE)
	{ my_U.set_par_range(minTvalue, maxTvalue, recreate); };
	void set_v_par_range(real minTvalue, real maxTvalue, bool recreate = FALSE)
	{ my_V.set_par_range(minTvalue, maxTvalue, recreate); };
	void redef_u_basis(int degree, int steps, bool recreate = TRUE)
	{ my_U.recreate(degree, steps, recreate); };
	void redef_v_basis(int degree, int steps, bool recreate = TRUE)
	{ my_V.recreate(degree, steps, recreate); };
	void def_u_basis(int degree, int steps, real minTvalue = 0, real maxTvalue = 1, bool recreate = TRUE)
	{ my_U.create(degree, steps, minTvalue, maxTvalue, recreate); };
	void def_v_basis(int degree, int steps, real minTvalue = 0, real maxTvalue = 1, bool recreate = TRUE)
	{ my_V.create(degree, steps, minTvalue, maxTvalue); };
	int get_u_degree(void) { return my_U.get_degree(); };
	int get_v_degree(void) { return my_V.get_degree(); };
	int get_u_steps(void) { return my_U.get_steps(); };
	int get_v_steps(void) { return my_V.get_steps(); };
	real get_u_min_val(void) { return my_U.get_min_range(); };			 
	real get_v_min_val(void) { return my_V.get_min_range(); };
	real get_u_max_val(void) { return my_U.get_max_range(); };
	real get_v_max_val(void) { return my_V.get_max_range(); };
	real real_u_basis(int i, real t) 
	{ 
		return my_U.get_value(i, t);
	};
	real u_basis(int i, int t) 
	{ 
		return my_U[t][i];
	};
	real real_v_basis(int i, real t)
	{ 
		return my_V.get_value(i, t);
	};
	real v_basis(int i, int t)
	{ 
		return my_V[t][i];
	};
};

#endif
