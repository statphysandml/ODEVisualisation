#ifndef PROJECT_THREEPOINTSYSTEMJACOBIAN_HPP
#define PROJECT_THREEPOINTSYSTEMJACOBIAN_HPP

#include <math.h>
#include <tuple>

#include "../ode_solver/include/flow_equation_interface/jacobian_equation.hpp"


struct comp_func_three_point_system3
{
	const cudaT const_expr0;

	comp_func_three_point_system3(const cudaT const_expr0_)
		: const_expr0(const_expr0_) {}

	template <typename Tuple>
	__host__ __device__
	void operator()(Tuple t)
	{
		thrust::get<3>(t) = -2 + (thrust::get<2>(t) * (pow((1 + thrust::get<0>(t)), -4)) * (1 + (8 * thrust::get<0>(t)) + (-16 * thrust::get<1>(t) * (-1 + (4 * thrust::get<1>(t)) + thrust::get<0>(t)))) * const_expr0);
	}
};

struct ThreePointSystemJacobianEquation0 : public JacobianEquation
{
	ThreePointSystemJacobianEquation0(const cudaT k_) : k(k_),
		const_expr0((1*1.0/2) * (pow(M_PI, -1)))
	{}

	void operator() (DimensionIteratorC &derivatives, const DevDatC &variables) override
	{
		thrust::for_each(thrust::make_zip_iterator(thrust::make_tuple(variables[0].begin(), variables[1].begin(), variables[2].begin(), derivatives.begin())),thrust::make_zip_iterator(thrust::make_tuple(variables[0].end(), variables[1].end(), variables[2].end(), derivatives.end())), comp_func_three_point_system3(const_expr0));
	}

private:
	const cudaT k;

	const cudaT const_expr0;
};


struct comp_func_three_point_system4
{
	const cudaT const_expr0;

	comp_func_three_point_system4(const cudaT const_expr0_)
		: const_expr0(const_expr0_) {}

	template <typename Tuple>
	__host__ __device__
	void operator()(Tuple t)
	{
		thrust::get<3>(t) = thrust::get<2>(t) * (pow((1 + thrust::get<0>(t)), -3)) * (-1 + (16 * thrust::get<1>(t)) + (3 * thrust::get<0>(t))) * const_expr0;
	}
};

struct ThreePointSystemJacobianEquation1 : public JacobianEquation
{
	ThreePointSystemJacobianEquation1(const cudaT k_) : k(k_),
		const_expr0((4*1.0/3) * (pow(M_PI, -1)))
	{}

	void operator() (DimensionIteratorC &derivatives, const DevDatC &variables) override
	{
		thrust::for_each(thrust::make_zip_iterator(thrust::make_tuple(variables[0].begin(), variables[1].begin(), variables[2].begin(), derivatives.begin())),thrust::make_zip_iterator(thrust::make_tuple(variables[0].end(), variables[1].end(), variables[2].end(), derivatives.end())), comp_func_three_point_system4(const_expr0));
	}

private:
	const cudaT k;

	const cudaT const_expr0;
};


struct comp_func_three_point_system5
{
	const cudaT const_expr0;

	comp_func_three_point_system5(const cudaT const_expr0_)
		: const_expr0(const_expr0_) {}

	__host__ __device__
	cudaT operator()(const cudaT &val1, const cudaT &val2)
	{
		return (pow((1 + val1), -3)) * (-17 + (64 * (pow(val2, 2))) + (8 * val2 * (-1 + (3 * val1))) + (-12 * val1 * (4 + (val1 * (3 + val1))))) * const_expr0;
	}
};

struct ThreePointSystemJacobianEquation2 : public JacobianEquation
{
	ThreePointSystemJacobianEquation2(const cudaT k_) : k(k_),
		const_expr0((1*1.0/6) * (pow(M_PI, -1)))
	{}

	void operator() (DimensionIteratorC &derivatives, const DevDatC &variables) override
	{
		thrust::transform(variables[0].begin(), variables[0].end(), variables[1].begin(), derivatives.begin(), comp_func_three_point_system5(const_expr0));
	}

private:
	const cudaT k;

	const cudaT const_expr0;
};


struct comp_func_three_point_system6
{
	const cudaT const_expr0;

	comp_func_three_point_system6(const cudaT const_expr0_)
		: const_expr0(const_expr0_) {}

	template <typename Tuple>
	__host__ __device__
	void operator()(Tuple t)
	{
		thrust::get<3>(t) = thrust::get<2>(t) * (pow((1 + thrust::get<0>(t)), -6)) * ((160 * (pow(thrust::get<1>(t), 4)) * (289 + (216 * thrust::get<0>(t)))) + (-57 * (1 + thrust::get<0>(t)) * (-1 + (10 * thrust::get<0>(t) * (2 + thrust::get<0>(t))))) + (-40 * (pow(thrust::get<1>(t), 3)) * (424 + (thrust::get<0>(t) * (14 + (45 * thrust::get<0>(t)))))) + (20 * (pow(thrust::get<1>(t), 2)) * (1220 + (thrust::get<0>(t) * (1546 + (771 * thrust::get<0>(t)))))) + (5 * thrust::get<1>(t) * (-1278 + (thrust::get<0>(t) * (-1171 + (3 * thrust::get<0>(t) * (3 + (67 * thrust::get<0>(t))))))))) * const_expr0;
	}
};

struct ThreePointSystemJacobianEquation3 : public JacobianEquation
{
	ThreePointSystemJacobianEquation3(const cudaT k_) : k(k_),
		const_expr0((1*1.0/285) * (pow(M_PI, -1)))
	{}

	void operator() (DimensionIteratorC &derivatives, const DevDatC &variables) override
	{
		thrust::for_each(thrust::make_zip_iterator(thrust::make_tuple(variables[0].begin(), variables[1].begin(), variables[2].begin(), derivatives.begin())),thrust::make_zip_iterator(thrust::make_tuple(variables[0].end(), variables[1].end(), variables[2].end(), derivatives.end())), comp_func_three_point_system6(const_expr0));
	}

private:
	const cudaT k;

	const cudaT const_expr0;
};


struct comp_func_three_point_system7
{
	const cudaT const_expr0;
	const cudaT const_expr1;

	comp_func_three_point_system7(const cudaT const_expr0_, const cudaT const_expr1_)
		: const_expr0(const_expr0_), const_expr1(const_expr1_) {}

	template <typename Tuple>
	__host__ __device__
	void operator()(Tuple t)
	{
		thrust::get<3>(t) = (pow((1 + thrust::get<0>(t)), -5)) * (((pow((1 + thrust::get<0>(t)), 5)) * const_expr0) + (thrust::get<2>(t) * (3113 + (-256 * (pow(thrust::get<1>(t), 3)) * (343 + (270 * thrust::get<0>(t)))) + (240 * (pow(thrust::get<1>(t), 2)) * (87 + (thrust::get<0>(t) * (11 + (15 * thrust::get<0>(t)))))) + (-80 * thrust::get<1>(t) * (347 + (thrust::get<0>(t) * (515 + (257 * thrust::get<0>(t)))))) + (5 * thrust::get<0>(t) * (557 + (3 * thrust::get<0>(t) * (-19 + (thrust::get<0>(t) * (-17 + (5 * thrust::get<0>(t) * (5 + thrust::get<0>(t)))))))))))) * const_expr1;
	}
};

struct ThreePointSystemJacobianEquation4 : public JacobianEquation
{
	ThreePointSystemJacobianEquation4(const cudaT k_) : k(k_),
		const_expr0(-1140 * M_PI),
		const_expr1((1*1.0/570) * (pow(M_PI, -1)))
	{}

	void operator() (DimensionIteratorC &derivatives, const DevDatC &variables) override
	{
		thrust::for_each(thrust::make_zip_iterator(thrust::make_tuple(variables[0].begin(), variables[1].begin(), variables[2].begin(), derivatives.begin())),thrust::make_zip_iterator(thrust::make_tuple(variables[0].end(), variables[1].end(), variables[2].end(), derivatives.end())), comp_func_three_point_system7(const_expr0, const_expr1));
	}

private:
	const cudaT k;

	const cudaT const_expr0;
	const cudaT const_expr1;
};


struct comp_func_three_point_system8
{
	const cudaT const_expr0;

	comp_func_three_point_system8(const cudaT const_expr0_)
		: const_expr0(const_expr0_) {}

	__host__ __device__
	cudaT operator()(const cudaT &val1, const cudaT &val2)
	{
		return (pow((1 + val1), -5)) * ((-128 * (pow(val2, 4)) * (343 + (270 * val1))) + (160 * (pow(val2, 3)) * (87 + (val1 * (11 + (15 * val1))))) + (-80 * (pow(val2, 2)) * (347 + (val1 * (515 + (257 * val1))))) + (57 * (1 + val1) * (33 + (4 * val1 * (2 + val1) * (17 + (6 * val1 * (2 + val1)))))) + (2 * val2 * (3113 + (5 * val1 * (557 + (3 * val1 * (-19 + (val1 * (-17 + (5 * val1 * (5 + val1))))))))))) * const_expr0;
	}
};

struct ThreePointSystemJacobianEquation5 : public JacobianEquation
{
	ThreePointSystemJacobianEquation5(const cudaT k_) : k(k_),
		const_expr0((1*1.0/1140) * (pow(M_PI, -1)))
	{}

	void operator() (DimensionIteratorC &derivatives, const DevDatC &variables) override
	{
		thrust::transform(variables[0].begin(), variables[0].end(), variables[1].begin(), derivatives.begin(), comp_func_three_point_system8(const_expr0));
	}

private:
	const cudaT k;

	const cudaT const_expr0;
};


struct comp_func_three_point_system9
{
	const cudaT const_expr0;

	comp_func_three_point_system9(const cudaT const_expr0_)
		: const_expr0(const_expr0_) {}

	template <typename Tuple>
	__host__ __device__
	void operator()(Tuple t)
	{
		thrust::get<3>(t) = (pow(thrust::get<2>(t), 2)) * (pow((1 + thrust::get<0>(t)), -6)) * (252 + (-32 * (pow(thrust::get<1>(t), 3)) * (289 + (216 * thrust::get<0>(t)))) + (8 * (pow(thrust::get<1>(t), 2)) * (367 + (thrust::get<0>(t) * (-43 + (45 * thrust::get<0>(t)))))) + (thrust::get<0>(t) * (145 + (3 * thrust::get<0>(t) * (111 + (47 * thrust::get<0>(t)))))) + (-4 * thrust::get<1>(t) * (308 + (thrust::get<0>(t) * (-50 + (87 * thrust::get<0>(t))))))) * const_expr0;
	}
};

struct ThreePointSystemJacobianEquation6 : public JacobianEquation
{
	ThreePointSystemJacobianEquation6(const cudaT k_) : k(k_),
		const_expr0((2*1.0/57) * (pow(M_PI, -1)))
	{}

	void operator() (DimensionIteratorC &derivatives, const DevDatC &variables) override
	{
		thrust::for_each(thrust::make_zip_iterator(thrust::make_tuple(variables[0].begin(), variables[1].begin(), variables[2].begin(), derivatives.begin())),thrust::make_zip_iterator(thrust::make_tuple(variables[0].end(), variables[1].end(), variables[2].end(), derivatives.end())), comp_func_three_point_system9(const_expr0));
	}

private:
	const cudaT k;

	const cudaT const_expr0;
};


struct comp_func_three_point_system10
{
	const cudaT const_expr0;

	comp_func_three_point_system10(const cudaT const_expr0_)
		: const_expr0(const_expr0_) {}

	template <typename Tuple>
	__host__ __device__
	void operator()(Tuple t)
	{
		thrust::get<3>(t) = (pow(thrust::get<2>(t), 2)) * (pow((1 + thrust::get<0>(t)), -5)) * ((24 * (pow(thrust::get<1>(t), 2)) * (343 + (270 * thrust::get<0>(t)))) + (5 * (62 + (thrust::get<0>(t) * (2 + (29 * thrust::get<0>(t)))))) + (-5 * thrust::get<1>(t) * (291 + (thrust::get<0>(t) * (-13 + (60 * thrust::get<0>(t))))))) * const_expr0;
	}
};

struct ThreePointSystemJacobianEquation7 : public JacobianEquation
{
	ThreePointSystemJacobianEquation7(const cudaT k_) : k(k_),
		const_expr0((8*1.0/285) * (pow(M_PI, -1)))
	{}

	void operator() (DimensionIteratorC &derivatives, const DevDatC &variables) override
	{
		thrust::for_each(thrust::make_zip_iterator(thrust::make_tuple(variables[0].begin(), variables[1].begin(), variables[2].begin(), derivatives.begin())),thrust::make_zip_iterator(thrust::make_tuple(variables[0].end(), variables[1].end(), variables[2].end(), derivatives.end())), comp_func_three_point_system10(const_expr0));
	}

private:
	const cudaT k;

	const cudaT const_expr0;
};


struct comp_func_three_point_system11
{
	const cudaT const_expr0;
	const cudaT const_expr1;

	comp_func_three_point_system11(const cudaT const_expr0_, const cudaT const_expr1_)
		: const_expr0(const_expr0_), const_expr1(const_expr1_) {}

	template <typename Tuple>
	__host__ __device__
	void operator()(Tuple t)
	{
		thrust::get<3>(t) = (pow((1 + thrust::get<0>(t)), -5)) * (((pow((1 + thrust::get<0>(t)), 5)) * const_expr0) + (2 * thrust::get<2>(t) * (-833 + (64 * (pow(thrust::get<1>(t), 3)) * (343 + (270 * thrust::get<0>(t)))) + (40 * thrust::get<1>(t) * (62 + (thrust::get<0>(t) * (2 + (29 * thrust::get<0>(t)))))) + (-20 * (pow(thrust::get<1>(t), 2)) * (291 + (thrust::get<0>(t) * (-13 + (60 * thrust::get<0>(t)))))) + (-5 * thrust::get<0>(t) * (329 + (3 * thrust::get<0>(t) * (171 + (thrust::get<0>(t) * (97 + (5 * thrust::get<0>(t) * (5 + thrust::get<0>(t)))))))))))) * const_expr1;
	}
};

struct ThreePointSystemJacobianEquation8 : public JacobianEquation
{
	ThreePointSystemJacobianEquation8(const cudaT k_) : k(k_),
		const_expr0(570 * M_PI),
		const_expr1((1*1.0/285) * (pow(M_PI, -1)))
	{}

	void operator() (DimensionIteratorC &derivatives, const DevDatC &variables) override
	{
		thrust::for_each(thrust::make_zip_iterator(thrust::make_tuple(variables[0].begin(), variables[1].begin(), variables[2].begin(), derivatives.begin())),thrust::make_zip_iterator(thrust::make_tuple(variables[0].end(), variables[1].end(), variables[2].end(), derivatives.end())), comp_func_three_point_system11(const_expr0, const_expr1));
	}

private:
	const cudaT k;

	const cudaT const_expr0;
	const cudaT const_expr1;
};

class ThreePointSystemJacobianEquations : public JacobianWrapper
{
public:
	ThreePointSystemJacobianEquations(const cudaT k_) : k(k_)
	{
		jacobian_equations = std::vector< JacobianEquation* > {
			new ThreePointSystemJacobianEquation0(k),
			new ThreePointSystemJacobianEquation1(k),
			new ThreePointSystemJacobianEquation2(k),
			new ThreePointSystemJacobianEquation3(k),
			new ThreePointSystemJacobianEquation4(k),
			new ThreePointSystemJacobianEquation5(k),
			new ThreePointSystemJacobianEquation6(k),
			new ThreePointSystemJacobianEquation7(k),
			new ThreePointSystemJacobianEquation8(k)
		};
	}

	void operator() (DimensionIteratorC &derivatives, const DevDatC &variables, const int row_idx, const int col_idx) override
	{
		(*jacobian_equations[row_idx * dim + col_idx])(derivatives, variables);
	}

	void operator() (DimensionIteratorC &derivatives, const DevDatC &variables, const int matrix_idx) override
	{
		(*jacobian_equations[matrix_idx])(derivatives, variables);
	}

	uint8_t get_dim() override
	{
		return dim;
	}

	static std::string name()
	{
		return "three_point_system";
	}

	const static uint8_t dim = 3;

private:
	const cudaT k;
	std::vector < JacobianEquation* > jacobian_equations;
};

# endif //PROJECT_THREEPOINTSYSTEMJACOBIAN_HPP
