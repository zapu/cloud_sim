#include <stdio.h>
#include <stdlib.h>
#include "tools.h"
#include "system.h"
#include <iostream>
#include <ctime>

Project project;

int main() {
	srand(1);

	for(int i = 0; i < 1000; i++) {
		auto job = new Job();
		job->m_difficulty = randf();
		job->m_active = i < 100;
		project.m_jobs.insert(job);
	}

	for(int i = 0; i < 10; i++) {
		auto node = new Node();
		node->m_performance = randf();
		node->m_trust = 0.0f;
		project.addNode(node);
	}

	std::cout << "Simulating..." << std::endl;
	project.simulate();
}