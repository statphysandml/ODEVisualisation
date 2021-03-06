#include "../include/fixed_point_search.hpp"

// FixedPointSearchParameters Constructors

FixedPointSearchParameters::FixedPointSearchParameters(const json params_, const PathParameters path_parameters_) : FRGVisualizationParameters(params_, path_parameters_),
                                                    dim(get_value_by_key<int>("dim")),
                                                    maximum_recursion_depth(get_value_by_key<int>("maximum_recursion_depth")),
                                                    k(get_value_by_key<cudaT>("k"))
{
    auto n_branches_per_depth_ = get_value_by_key<json>("n_branches_per_depth");
    auto lambda_ranges_ = get_value_by_key<json>("lambda_ranges");

    std::transform(n_branches_per_depth_.begin(), n_branches_per_depth_.end(), std::back_inserter(n_branches_per_depth),
                   [] (json &dat) { return dat.get< std::vector<int> >(); });
    std::transform(lambda_ranges_.begin(), lambda_ranges_.end(), std::back_inserter(lambda_ranges),
                   [] (json &dat) { return dat.get< std::pair<cudaT, cudaT> >(); });
;
    flow_equations = FlowEquationsWrapper::make_flow_equation(path_parameters.theory);
}

FixedPointSearchParameters::FixedPointSearchParameters(const std::string theory,
                                                       const std::string mode_type,
                                                       const std::string results_dir,
                                                       const std::string root_dir,
                                                       const bool relative_path) : FixedPointSearchParameters(
                                                               Parameters::read_parameter_file(
        root_dir + "/" + theory + "/" + results_dir + "/", "config", relative_path),
        PathParameters(theory, mode_type, root_dir, relative_path))
{}

FixedPointSearchParameters::FixedPointSearchParameters(
        const std::string theory,
        const int maximum_recursion_depth_,
        const std::vector< std::vector<int> > n_branches_per_depth_,
        const std::vector <std::pair<cudaT, cudaT> > lambda_ranges_,
        const std::string mode_,
        const std::string root_dir,
        const bool relative_path
) : FixedPointSearchParameters(
        json {{"maximum_recursion_depth", maximum_recursion_depth_},
              {"n_branches_per_depth", n_branches_per_depth_},
              {"lambda_ranges", lambda_ranges_},
              {"mode", mode_}},
        PathParameters(theory, mode_, root_dir, relative_path)
)
{}


FixedPointSearchParameters::ClusterParameters::ClusterParameters(const json params_) : Parameters(params_),
                                        maximum_expected_number_of_clusters(get_value_by_key<uint>("maximum_expected_number_of_clusters")),
                                        upper_bound_for_min_distance(get_value_by_key<double>("upper_bound_for_min_distance")),
                                        maximum_number_of_iterations(get_value_by_key<uint>("maximum_number_of_iterations"))
{}

FixedPointSearchParameters::ClusterParameters::ClusterParameters(
        const uint maximum_expected_number_of_clusters_,
        const double upper_bound_for_min_distance_,
        const uint maximum_number_of_iterations_
) : ClusterParameters(
        json {{"maximum_expected_number_of_clusters", maximum_expected_number_of_clusters_},
              {"upper_bound_for_min_distance", upper_bound_for_min_distance_},
              {"maximum_number_of_iterations", maximum_number_of_iterations_}}
)
{}


FixedPointSearch::FixedPointSearch(const FixedPointSearchParameters &sp_): sp(sp_)
{
    Node * root_node_ptr = new Node(0, compute_internal_end_index(sp.n_branches_per_depth[0]), std::vector< int >{});
    buffer = Buffer(root_node_ptr);

    // Tests
    if (sp.n_branches_per_depth.size() != sp.maximum_recursion_depth)
    {
        std::cout << "\nERROR: Maximum recursion depth " << sp.maximum_recursion_depth
                  << " and number of branches per depth " << sp.n_branches_per_depth.size()
                  << " do not coincide" << std::endl;
        std::exit(EXIT_FAILURE);
    }

    for (const auto &n_branches : sp.n_branches_per_depth)
    {
        if (n_branches.size() != sp.dim) {
            std::cout << "\nERROR: Number of branches per depth " << n_branches.size() << " do not coincide with dimension " << sp.dim <<  std::endl;
            std::exit(EXIT_FAILURE);
        }
    }

    if(sp.lambda_ranges.size() != sp.dim)
    {
        std::cout << "\nERROR: Number of lambda ranges " << sp.lambda_ranges.size() << " do not coincide with dimension" << sp.dim << std::endl;
        std::exit(EXIT_FAILURE);
    }

    if(sp.flow_equations->get_dim() != sp.dim)
    {
        std::cout << "\nERROR: Dimensions and number of flow equation do not coincide" << sp.dim << std::endl;
        std::exit(EXIT_FAILURE);
    }
}


// Main functions

void FixedPointSearch::find_fixed_point_solutions()
{
    auto c = 0;
    while(c < sp.computation_parameters.maximum_number_of_gpu_calls and buffer.len() > 0)
    {
        std::cout << "\n\n######### New computation round: " << c <<  "#########" << std::endl;
        NodeCounter<Node>::number_of_alive_nodes_per_depth();
        std::cout << std::endl;
        run_gpu_computing_task();
        c++;
    }
}

void FixedPointSearch::cluster_solutions_to_fixed_points(const uint maximum_expected_number_of_clusters,
        const double upper_bound_for_min_distance,
        const uint maximum_number_of_iterations)
{
    // Compute vertices of solutions
    HyperCubes solution_cubes(sp.k, sp.n_branches_per_depth, sp.lambda_ranges);

    GridComputationWrapper grcompwrap = solution_cubes.project_leaves_on_expanded_cube_and_depth_per_cube_indices(solutions);
    solution_cubes.compute_cube_center_vertices(grcompwrap);

    // Get center vertices
    const DevDatC potential_fixed_points = solution_cubes.get_vertices();

    // Cluster center vertices
    fixed_points = cluster_device_data(
            maximum_expected_number_of_clusters,
            upper_bound_for_min_distance,
            potential_fixed_points,
            maximum_number_of_iterations
            );
}

void FixedPointSearch::cluster_solutions_to_fixed_points_from_parameters(const FixedPointSearchParameters::ClusterParameters cluster_parameters)
{
    cluster_solutions_to_fixed_points(
            cluster_parameters.maximum_expected_number_of_clusters,
            cluster_parameters.upper_bound_for_min_distance,
            cluster_parameters.maximum_number_of_iterations);
}

void FixedPointSearch::cluster_solutions_to_fixed_points_from_file()
{
    auto cluster_params = sp.get_value_by_key<json>("cluster");
    auto cluster_parameters = FixedPointSearchParameters::ClusterParameters(cluster_params);
    cluster_solutions_to_fixed_points_from_parameters(cluster_parameters);
}

std::vector< Leaf* > FixedPointSearch::get_solutions()
{
    return solutions;
}

DevDatC FixedPointSearch::get_fixed_points() const
{
    return fixed_points;
}

void FixedPointSearch::compute_and_write_fixed_point_characteristics_to_file(std::string dir)
{
    // ToDo: If fixed_points are not defined - Try to load them file first, otherwise throw error

    CoordinateOperatorParameters coordinate_operator_parameters = CoordinateOperatorParameters::from_parameters(
            sp.path_parameters.theory,
            {},
            "evaluate",
            sp.path_parameters.root_dir,
            sp.path_parameters.relative_path);
    CoordinateOperator fixed_point_evaluator(coordinate_operator_parameters);
    fixed_point_evaluator.set_raw_coordinates(fixed_points);

    fixed_point_evaluator.compute_velocities();
    fixed_point_evaluator.compute_jacobians_and_eigendata();

    fixed_point_evaluator.write_characteristics_to_file(dir);
}

void FixedPointSearch::write_solutions_to_file(std::string dir) const
{
    json j;
    for(auto &sol: solutions)
        j.push_back(sol->to_json());
    Parameters::write_parameter_file(json {{"number_of_solutions", solutions.size()}, {"solutions", j}}, sp.path_parameters.get_base_path() + dir + "/", "solutions", sp.path_parameters.relative_path);
}

void FixedPointSearch::load_solutions_from_file(std::string dir)
{
    clear_solutions();

    json j = Parameters::read_parameter_file(sp.path_parameters.get_base_path() + dir + "/", "solutions", sp.path_parameters.relative_path);
    solutions.reserve(j["number_of_solutions"].get< int >());
    for(auto &sol: j["solutions"])
        solutions.push_back(new Leaf(sol["cube_indices"].get< std::vector<int> >()));
    std::cout << "solutions loaded" << std::endl;
}

void FixedPointSearch::write_fixed_points_to_file(std::string dir) const
{
    json j;
    auto transposed_fixed_points = fixed_points.transpose_device_data();
    for(auto &fixed_point : transposed_fixed_points)
        j.push_back(fixed_point);
    Parameters::write_parameter_file(json {{"number_of_fixed_points", transposed_fixed_points.size()}, {"fixed_points", j}}, sp.path_parameters.get_base_path() + "/" + dir + "/", "fixed_points", sp.path_parameters.relative_path);
}

void FixedPointSearch::load_fixed_points_from_file(std::string dir)
{
    json j = Parameters::read_parameter_file(sp.path_parameters.get_base_path() + dir + "/", "fixed_points", sp.path_parameters.relative_path);
    std::vector < std::vector<double> > fixed_points_;
    fixed_points_.reserve(j["number_of_fixed_points"].get< int >());
    for(auto &sol: j["fixed_points"])
        fixed_points_.push_back(sol.get< std::vector<double> >());
    fixed_points = DevDatC(fixed_points_);
    std::cout << "fixed points loaded" << std::endl;
}

// Iterate over nodes and generate new nodes based on the indices of pot fixed points
std::tuple< std::vector<Node* >, std::vector< Leaf* > > FixedPointSearch::generate_new_nodes_and_leaves(const thrust::host_vector<int> &host_indices_of_pot_fixed_points, const std::vector< Node* > &nodes)
{
    std::vector< Node* > new_nodes;
    std::vector< Leaf* > new_leaves;

    // No potential fixed points have been found
    if(host_indices_of_pot_fixed_points.size() > 0)
    {
        // Initial conditions
        auto pot_fixed_point_iterator = host_indices_of_pot_fixed_points.begin();
        int index_offset = 0;

        // Iterate over nodes
        for(const auto &node : nodes)
        {
            // Get first potential fix point -> is defined with respect to 0
            int index_of_pot_fixed_point = *pot_fixed_point_iterator - index_offset; // (-1 to undo offset) -> not used anymore (why initially used??)

            // Fix points have been found in node
            if(index_of_pot_fixed_point < node->get_n_cubes())
            {
                // Inspect fixed points
                while(index_of_pot_fixed_point < node->get_n_cubes() and pot_fixed_point_iterator != host_indices_of_pot_fixed_points.end())
                {
                    // Compute parent node indices
                    std::vector<int> parent_cube_indices(node->get_parent_cube_indices());
                    parent_cube_indices.push_back(index_of_pot_fixed_point + node->get_internal_start_index());

                    // Generate new nodes
                    if(node->get_depth() + 1 < sp.maximum_recursion_depth) {
                        new_nodes.push_back(
                                new Node(0, compute_internal_end_index(sp.n_branches_per_depth[node->get_depth() + 1]),
                                         parent_cube_indices));
                    }
                    else // Found solution -> Generate new leaf
                    {
                        new_leaves.push_back(new Leaf(parent_cube_indices));
                    }
                    // Update
                    pot_fixed_point_iterator++;
                    index_of_pot_fixed_point = *pot_fixed_point_iterator - index_offset; // (-1 to undo offset) -> not used anymore (why initially used??)
                }
            }

            // Update index offset
            index_offset += node->get_n_cubes();
        }

        assert(host_indices_of_pot_fixed_points.size() ==  (new_nodes.size() + new_leaves.size()) && "Number of new nodes and number of potential fixed points do not coincide");
    }
    return std::make_tuple(new_nodes, new_leaves);
}

void FixedPointSearch::run_gpu_computing_task()
{
    std::vector< Node* > nodes_to_be_computed;
    int total_number_of_cubes = 0;
    int maximum_depth = 0;

    // Get nodes for the gpu from buffer
    std::tie(nodes_to_be_computed, total_number_of_cubes, maximum_depth) = buffer.get_first_nodes(sp.computation_parameters.number_of_cubes_per_gpu_call);

    if(monitor) {
        std::cout << "\n### Nodes for the qpu: " << nodes_to_be_computed.size() << ", total number of cubes: "
                  << total_number_of_cubes << std::endl;
        buffer.get_nodes_info(nodes_to_be_computed);
    }

    HyperCubes hypercubes(sp.k, sp.n_branches_per_depth, sp.lambda_ranges);

    // Use helper class to perform gpu tasks on nodes
    GridComputationWrapper grcompwrap = hypercubes.generate_and_linearize_nodes(total_number_of_cubes, maximum_depth, nodes_to_be_computed);

    // Compute the actual vertices by first expanding each cube according to the number of vertices to
    // a vector of reference vertices of length total_number_of_cubes*dim and then computing the indices
    hypercubes.compute_vertices(grcompwrap);

    // hypercubes.test_projection();

    // Compute vertex velocities
    hypercubes.determine_vertex_velocities(sp.flow_equations);

    // Determine potential fix points
    thrust::host_vector<int> host_indices_of_pot_fixed_points = hypercubes.determine_potential_fixed_points();

    // Generate new nodes and derive solutions based on nodes and indices of potential fixed points
    std::vector< Node* > new_nodes;
    std::vector< Leaf* > new_leaves;
    std::tie(new_nodes, new_leaves) = generate_new_nodes_and_leaves(host_indices_of_pot_fixed_points, nodes_to_be_computed);
    buffer.add_nodes(new_nodes);
    solutions.insert(solutions.end(), new_leaves.begin(), new_leaves.end());

    if(monitor) {
        std::cout << "\n### New nodes" << std::endl;
        buffer.get_nodes_info(new_nodes);
    }

    // Delete evaluated nodes
    for(auto &node : nodes_to_be_computed)
    {
        --NodeCounter<Node>::objects_alive[node->get_depth()];
        delete node;
    }
}

void FixedPointSearch::clear_solutions()
{
    for(auto sol: solutions)
        delete sol;
    solutions.clear();
}
