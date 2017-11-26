#ifndef _BERN_TRIBASE__MDBMA_
#define _BERN_TRIBASE__MDBMA_
#include "defs.hxx"
#include "bern_base.hxx"

class bern_tri_tens_basis	/* baza tensorowa 3-go stopnia */
{
	bern_basis m_U;
	bern_basis m_V;
	bern_basis m_S;
public:
	bern_tri_tens_basis(void) {};
	~bern_tri_tens_basis(void) {};

	void rebuild_u_basis(void) { m_U.calc_basis(); };
	void rebuild_v_basis(void) { m_V.calc_basis(); };
	void rebuild_s_basis(void) { m_S.calc_basis(); };
	void set_u_par_range(real minTvalue, real maxTvalue, bool recreate = FALSE)
	{ m_U.set_par_range(minTvalue, maxTvalue, recreate); };
	void set_v_par_range(real minTvalue, real maxTvalue, bool recreate = FALSE)
	{ m_V.set_par_range(minTvalue, maxTvalue, recreate); };
	void set_s_par_range(real minTvalue, real maxTvalue, bool recreate = FALSE)
	{ m_S.set_par_range(minTvalue, maxTvalue, recreate); };
	void redef_u_basis(int degree, int steps, bool recreate = TRUE)
	{ m_U.recreate(degree, steps, recreate); };
	void redef_v_basis(int degree, int steps, bool recreate = TRUE)
	{ m_V.recreate(degree, steps, recreate); };
	void redef_s_basis(int degree, int steps, bool recreate = TRUE)
	{ m_S.recreate(degree, steps, recreate); };
	void def_u_basis(int degree, int steps, real minTvalue = 0, real maxTvalue = 1, bool recreate = TRUE)
	{ m_U.create(degree, steps, minTvalue, maxTvalue, recreate); };
	void def_v_basis(int degree, int steps, real minTvalue = 0, real maxTvalue = 1, bool recreate = TRUE)
	{ m_V.create(degree, steps, minTvalue, maxTvalue); };
	void def_s_basis(int degree, int steps, real minTvalue = 0, real maxTvalue = 1, bool recreate = TRUE)
	{ m_S.create(degree, steps, minTvalue, maxTvalue); };
	int get_u_degree(void) { return m_U.get_degree(); };
	int get_v_degree(void) { return m_V.get_degree(); };
	int get_s_degree(void) { return m_S.get_degree(); };
	int get_u_steps(void) { return m_U.get_steps(); };
	int get_v_steps(void) { return m_V.get_steps(); };
	int get_s_steps(void) { return m_S.get_steps(); };
	real get_u_min_val(void) { return m_U.get_min_range(); };			 
	real get_v_min_val(void) { return m_V.get_min_range(); };
	real get_s_min_val(void) { return m_S.get_min_range(); };
	real get_u_max_val(void) { return m_U.get_max_range(); };
	real get_v_max_val(void) { return m_V.get_max_range(); };
	real get_s_max_val(void) { return m_S.get_max_range(); };
	real real_u_basis(int i, real t) 
	{ 
		return m_U.get_value3(i, t);	//stupid hack
	};
	real u_basis(int i, int t) 
	{ 
		return m_U[t][i];
	};
	real real_v_basis(int i, real t)
	{ 
		return m_V.get_value3(i, t);	//stupid hack
	};
	real v_basis(int i, int t)
	{ 
		return m_V[t][i];
	};
	real real_s_basis(int i, real t)
	{ 
		return m_S.get_value3(i, t);	//stupid hack
	};
	real s_basis(int i, int t)
	{ 
		return m_S[t][i];
	};
};

#endif

