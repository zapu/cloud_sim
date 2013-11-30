#include "main.h"
#include <iostream>
#include <stdio.h>
#include <omp.h>
#include <algorithm>

int total_market_correctness = 0;

int lastJobId = 0;
int lastNodeId = 0;

int globalRand = 0;
float globalRandF = 0;

std::vector<Node> nodes;
std::vector<Job> jobs;

template<class T>
const T& clamp(const T& val, const T& lower, const T& upper)
{
	return std::min(upper, std::max(lower, val));
}

float randf() 
{
	return (float)rand() / RAND_MAX;
}

float getRand(float lower, float upper)
{
	float d = upper - lower;
	return lower + randf() * d;
}

bool Job::hasSubmitted( Node* node )
{
	return submits.find(node) != submits.end();
}

Job::Job()
{
	id = lastJobId++;
	done = false;

	confirmed_correctness = 0;
	assumed_correctness_sum = 0;
}

void Job::submit(Node* node, int hash)
{
	float new_correctness = 0.0f;
	for(auto it = submits.begin(); it != submits.end(); ++it) {
		if(it->second == hash) {
			new_correctness += it->first->getCorrectnessScaledRandom();
			it->first->correctness += node->getCorrectnessScaledRandom();
		}
	}

	node->correctness += new_correctness;
	if(node->correctness > total_market_correctness) {
		total_market_correctness = node->correctness;
	}
	submits[node] = hash;
}

Node::Node()
{
	id = lastNodeId++;

	correctness = 0;
	experience = 0;

	worked_for = 0;
	sleeping_for = 0;

	current_job = NULL;
	current_work_left = 0;
	work_scale = 10;
}

float Node::getCorrectnessScaled()
{
	if(total_market_correctness == 0.0f) {
		return 0.0f;
	}

	return (float)correctness / total_market_correctness;
}

float Node::getCorrectnessScaledRandom()
{
	return clamp(getCorrectnessScaled() + getRand(-0.1f, 0.1f), 0.0f, 1.0f);
}

Job* findJob(Node* node)
{
	float correctness = node->getCorrectnessScaled();

	for(size_t i = 0; i < jobs.size(); i++) {
		Job* job = &jobs[i];
		if(job->done) {
			continue;
		}

		if(job->hasSubmitted(node)) {
			continue;
		}

		if(job->assumed_correctness_sum >= 1.0f) {
			continue;
		}

		return job;
	}

	return NULL;
}

void handleNode(Node* node)
{
	if(node->sleeping_for > 0) {
		node->sleeping_for--;
		return;
	}

	if(!node->current_job) {
		#pragma omp critical 
		{
			Job* job = findJob(node);
			if(job) {
				float corr = node->getCorrectnessScaledRandom();
				job->assumed_correctness_sum += corr;
				job->assumed_correctness[node] = corr;
				node->current_job = job;
			}
		}

		if(!node->current_job) {
			return;
		}

		node->work_scale = 10;
	}

	node->worked_for++;

	if(--node->work_scale == 0) {
#pragma omp critical
		{
			Job* job = node->current_job;
			job->submit(node, 0);
		}

		node->current_job = NULL;
	}
}

int main() {
	srand(0);

	int tickNum = 0;

	nodes.resize(100);
	jobs.resize(10000);

	std::cout << "Init done." << std::endl;

	FILE* pipe = _popen("\"C:/Program Files (x86)/gnuplot/bin/pgnuplot.exe\" -persist", "w");
	fprintf(pipe, "set terminal png crop size 640,480\n");
	fprintf(pipe, "set output \"%s\"\n", "output.png");
	fprintf(pipe, "plot '-' with lines\n");

	while(tickNum < 1000) {
#pragma omp parallel for
		for(int i = 0; i < nodes.size(); i++) {
			handleNode(&nodes[i]);
		}

		if((tickNum % 10) == 0) {
			fprintf(pipe, "%d %g\n", tickNum, nodes[0].getCorrectnessScaled());
		}
		++tickNum;
	}

	fprintf(pipe, "e\n");
	fflush(pipe);
	_pclose(pipe);
}