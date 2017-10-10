#pragma once
#include <vector>
#include <random>
#include <numeric>

class SequenceGenerator {
public:
	SequenceGenerator(int state_num, std::vector<double> head_prob,
		std::vector<std::vector<double>> transition_prob);
	std::vector<int> Generate(int length);
private:
	int state_num_;
	std::vector<double> head_prob_;
	std::vector<std::vector<double>> transition_prob_;
};