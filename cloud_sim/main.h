#pragma once

#include <vector>
#include <map>

struct Node;

struct Job {
	Job();

	bool hasSubmitted(Node* node);
	void submit(Node* node, int hash);

	int id;
	bool done;

	float confirmed_correctness;

	std::map<Node*, float> assumed_correctness;
	float assumed_correctness_sum;

	std::map<Node*, int> submits;
};

struct Node {
	//Node constructor is not thread safe
	Node();

	int id;

	float correctness;
	float experience;

	int worked_for;
	int sleeping_for;

	Job* current_job;
	int current_work_left;
	int work_scale;

	float getCorrectnessScaled();
	float getCorrectnessScaledRandom();
};


