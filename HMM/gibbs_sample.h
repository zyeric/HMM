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
	int UpdateFront();
	int UpdateMid(int index);
	int UpdateBack();
	double UpdatePara();
	int RandomSelectCoin(double *prob);
	UniformGenerator* gen;

public:
	int length_;
	std::vector<int> observed_sequence_;
	std::vector<int> state_sequence_;
	CoinSequence estimated_para_;
};