#pragma once
#include <vector>
#include "coin_sequence.h"
#include "uniform_generator.h"

class GibbsSample {
public:
	GibbsSample(int length, std::vector<int> observerd_sequence);
	~GibbsSample();
	void Estimate();

private:
	void InitSequence();
	std::vector<double> CalProb(int index);
	double UpdatePara(std::vector<std::vector<int>> sequences);
	int RandomSelectCoin(std::vector<double> prob);
	std::vector<int> GibbsSample::RandomSampleSequence(std::vector<std::vector<double>> probs);
	UniformGenerator* gen;

public:
	int length_;
	std::vector<int> observed_sequence_;
	std::vector<int> state_sequence_;
	CoinSequence estimated_para_;
};