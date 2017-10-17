#pragma once
#include "util.h"

class CoinSequence{
public:
	CoinSequence();
	CoinSequence(std::vector<double> init_prob, 
		std::vector<double> head_prob,
		std::vector<std::vector<double>> transition_prob);
	/*
	1 -> head
	0 -> tail
	*/
	void Generate(int length, std::vector<int>& state_sequence,
		std::vector<int>& observed_sequence);
	void PrintPara();
	std::vector<int> CalHiddenState(std::vector<int>& observed_sequence);

	std::vector<double> init_prob_;
	std::vector<double> head_prob_;
	std::vector<std::vector<double>> transition_prob_;
};