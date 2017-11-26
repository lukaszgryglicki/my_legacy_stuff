#ifndef __BBASE_H__MDBMA__
#define __BBASE_H__MDBMA__
#include "defs.hxx"

class bern_basis
{
protected:
	int		bern_degree;
	int		bern_steps;
	real**	bern_array_basis;

	real	bern_min_t;
	real	bern_max_t;

	void clear_array(void)
	{
		if(bern_array_basis)
		{
			for(int i = 0; i < bern_steps; ++i)
				free_array(bern_array_basis[i]);
			free_array(bern_array_basis);
		}
	};
	void create_array(int degree, int steps)
	{
		if(bern_array_basis)
			clear_array();
		bern_array_basis = new real*[bern_steps = steps];
		for(int i = 0; i < bern_steps; ++i)
			bern_array_basis[i] = new real[(bern_degree = degree)+1];
	};
public:
	virtual void calc_basis();
	bern_basis(void) : bern_array_basis(NULL), bern_degree(0), bern_steps(0), bern_min_t(0), bern_max_t(0) {};
	bern_basis(int degree, int steps, real minTvalue = 0, real maxTvalue = 1) 
		: bern_array_basis(NULL), bern_degree(degree), bern_steps(steps), bern_min_t(minTvalue), bern_max_t(maxTvalue) 
	{ create_array(degree, steps); };
	real** get_basis_array(void) { return bern_array_basis; };
	void set_min_range(real minTvalue, bool recreate = FALSE)
	{
		bern_min_t = minTvalue;
		if(recreate) calc_basis();
	}
	void set_max_range(real maxTvalue, bool recreate = FALSE)
	{
		bern_max_t = maxTvalue;
		if(recreate) calc_basis();
	}
	void set_par_range(real minTvalue, real maxTvalue, bool recreate = FALSE)
	{
		bern_min_t = minTvalue; bern_max_t = maxTvalue;
		if(recreate) calc_basis();
	}
	void set_degree(int degree, bool recreate = FALSE) 
	{
		create_array(degree, bern_steps);
		if(recreate) calc_basis();
	};
	void set_steps(int steps, bool recreate = FALSE) 
	{
		create_array(bern_degree, steps);
		if(recreate) calc_basis();
	};
	void recreate(int degree, int steps, bool recreate = FALSE)
	{
		create_array(degree, steps);
		if(recreate) calc_basis();
	};
	void create(int degree, int steps, real minTvalue = 0, real maxTvalue = 1, bool recreate = TRUE)
	{
		bern_min_t = minTvalue; bern_max_t = maxTvalue;
		create_array(degree, steps);
		if(recreate) calc_basis();
	};
	bool is_initialized(void) const { return ((bern_array_basis) ? TRUE : FALSE); };
	int get_degree(void) const { return bern_degree; };
	int get_steps(void) const { return bern_steps; };
	real get_min_range(void) const { return bern_min_t; };
	real get_max_range(void) const { return bern_max_t; };
	const real* operator[](int step) const 
	  { 
		return bern_array_basis[step]; 
	  };
	real get_value(int i, real t);
	real get_value3(int i, real t);
	~bern_basis(void) { clear_array(); };
};

#endif
