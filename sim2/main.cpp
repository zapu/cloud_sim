#include <stdio.h>
#include <stdlib.h>
#include "tools.h"
#include "system.h"
#include <iostream>
#include <ctime>
#define BOOST_ALL_DYN_LINK
#include <boost/program_options.hpp>

Project project;

namespace prog_opt = boost::program_options;

int main(int argc, char** argv) {
	prog_opt::options_description option_desc("Allowed options");
	option_desc.add_options()
		("help,h", "produce help message")
		("nodes", prog_opt::value<int>()->default_value(1000), "Node count")
		("jobs", prog_opt::value<int>()->default_value(15000), "Job count")
		("false", prog_opt::value<float>()->default_value(0), "Probability of a dishonest node returning false result (0 - node always returns correct, 1 - node always returns false)")
		("dishonest", prog_opt::value<float>()->default_value(0), "Ratio of dishonest nodes (0 - no dishonest nodes, 1 - whole network is dishonest)")
		("static_perf", "Static performance of nodes.")
		("srand", prog_opt::value<int>()->default_value(1), "Random seed")
	;

	prog_opt::variables_map opt_map;
	try {
		prog_opt::store(prog_opt::parse_command_line(argc, argv, option_desc), opt_map);
		prog_opt::notify(opt_map);
	} catch(...) {
		std::cout << "exception in program opts." << std::endl;
		return -1;
	}

	if(opt_map.count("help")) {
		std::cout << option_desc << std::endl;
		return 1;
	}

	srand(opt_map["srand"].as<int>());

	int node_count = opt_map["nodes"].as<int>();
	int job_count = opt_map["jobs"].as<int>();
	float ratio = opt_map["false"].as<float>();
	int dishonestCount = opt_map["dishonest"].as<float>() * node_count;
	bool perf = opt_map.count("static_perf") > 0;

	for(int i = 0; i < job_count; i++) {
		auto job = new Job();
		job->m_difficulty = randf();
		//job->m_active = i < 250;
		job->m_active = true;
		project.m_jobs.insert(job);
	}

	for(int i = 0; i < node_count; i++) {
		auto node = new Node();
		node->m_performance = (perf ? 0.5f : randf());
		node->m_trust = 0.0f;
		node->m_nextActionTime = getRand(0, 10);
		if(i < dishonestCount) {
			node->m_falseRatio = ratio;
		}
		project.addNode(node);
	}

	std::cout << "Starting simulation with " << node_count << " nodes and " << job_count << " jobs." << std::endl;
	project.simulate();
}