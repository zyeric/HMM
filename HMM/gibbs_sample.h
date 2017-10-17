#pragma once
#include "util.h"
#include "coin_sequence.h"
#include "uniform_generator.h"

class GibbsSample {
public:
	GibbsSample(size_t length, std::vector<int>& observerd_sequence);
	~GibbsSample();
	void Estimate();

private:
	void InitSequence();
	std::vector<double> CalProb(size_t index);
	double UpdatePara(std::vector<std::vector<int>>& sequences);
	int RandomSelectCoin(std::vector<double>& prob);
	std::vector<int> RandomSampleSequence(std::vector<std::vector<double>>& probs);
	UniformGenerator* gen;

public:
	bool is_converged_;
	size_t length_;
	std::vector<int> observed_sequence_;
	std::vector<int> state_sequence_;
	CoinSequence estimated_para_;
};