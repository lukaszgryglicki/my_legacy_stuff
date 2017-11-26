#include "defs.hxx"
#include "bez_patch.hxx"

void bez_patch::calc_mesh(void)
{
	if(!control_array || !my_bSet)
		return;
	for(register int u = 0; u < u_steps; ++u)
		for(register int v = 0; v < v_steps; ++v)
		{
			mesh_array[u][v].clear();
			for(register int i = 0; i <= u_degree; ++i)
				for(register int j = 0; j <= v_degree; ++j)
				{
					if(!control_array[i][j])
					{
						my_bSet = FALSE;
						return;
					}
					mesh_array[u][v] += (*control_array[i][j])*tensor_basis.u_basis(i, u)*tensor_basis.v_basis(j, v);
				}
		}
		my_bSet = TRUE;
}

vector3& bez_patch::get_mesh_pointI(int u, int v)
{
	return mesh_array[u][v];
}

vector3 bez_patch::get_mesh_pointR(real u, real v)
{
	real uval = u*u_steps, vval = v*v_steps;
	if(uval-floor(uval) < 10e-6 && vval-floor(vval) < 10e-6)
		return get_mesh_pointI((int)floor(uval), (int)floor(vval));

	vector3 result;
	for(register int i = 0; i <= u_degree; ++i)
		for(register int j = 0; j <= v_degree; ++j)
		{
			if(!control_array[i][j])
				return vector3(0);
			result += (*control_array[i][j])*tensor_basis.real_u_basis(i, u)*tensor_basis.real_u_basis(j, v);
		}
		return result;
}
