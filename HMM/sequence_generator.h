#pragma once
#include <vector>
#include <random>
#include <numeric>
#include <utility>

class SequenceGenerator {
public:
	SequenceGenerator(int state_num, std::vector<double> head_prob,
		std::vector<std::vector<double>> transition_prob);
	/*
	1 -> head
	0 -> tail
	*/
	std::vector<std::pair<int, int>> Generate(int length);
private:
	int state_num_;
	std::vector<double> head_prob_;
	std::vector<std::vector<double>> transition_prob_;
};