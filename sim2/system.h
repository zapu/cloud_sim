#pragma once

#include <stdint.h>
#include <list>
#include <unordered_map>
#include <unordered_set>
#include <set>

struct Result;
class Job;
struct AssumedResult;

class Node {
public:
	Node();

	enum State {
		IDLING,
		COMPUTING,		
	};

	//Next action time - Node waits till nextActionTime
	//before doing next thing: starting of finishing computation
	uint64_t m_nextActionTime;
	uint64_t m_lastActionTime;

	//How fast the node is: 0.0f (slowest) - 1.0f (fastest)
	float m_performance;

	//All results that the node has submitted.
	std::list<Result*> m_results;
	std::unordered_set<Job*> m_resultsJob;

	float m_trust;

	float m_falseRatio;

	AssumedResult* m_currentWork;

	bool hasSubmitted(Job* job);
	void startJob(Job* job, float corr, uint64_t currentTick);
	void endJob();
};

struct Result {
	int hash;

	Node* node;
	Job* job;

	float correctness;
};

struct AssumedResult {
	Node* node;
	Job* job;
	float correctness;
};

class Job {
public:
	Job();

	float getCorrectness() const;

	void nodeStarted(AssumedResult* res);
	Result* workDone(AssumedResult* res, int hash);

	//"Winning" result correctness sum
	float m_bestCorrectness;

	//How difficult the task is: 0.0f (easiest) - 1.0f (hardest).
	float m_difficulty;

	//Submitted results, grouped by hash.
	std::unordered_multimap<int, Result*> m_results;

	std::unordered_map<int, float> m_correctnessPerHash;

	std::unordered_map<Node*, AssumedResult*> m_assumedResults;

	//Sum of assumed results correctness
	float m_assumedCorrectness;

	bool m_active;
};

struct JobCompare {
	bool operator() (const Job* a, const Job* b) const;
};

struct NodeCompare {
	bool operator() (const Node* a, const Node* b) const;
};

class Project {
public:
	Project();

	void addNode(Node* node);

	void updateTrust( Node* node );

	Job* findJobForNode(Node* node);
	float getTrust(Node* node);

	std::set<Node*, NodeCompare> m_nodes;
	std::set<Job*, JobCompare> m_jobs;

	void simulate();
	void activateJob();
protected:
	float m_bestTrust;
};