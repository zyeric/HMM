#include <vector>
#include <cstdio>
#include "coin_sequence.h" 
#include "gibbs_sample.h"

void GenerateSequence(int length, std::vector<int>& state_sequence,
	std::vector<int>& observed_sequence) {
	std::vector<double> head_prob = { 0.1, 0.5, 0.9 };
	std::vector<std::vector<double>> transition_prob =
	{ {0.8, 0.2, 0.0}, {0.1, 0.7, 0.2}, {0.1, 0.0, 0.9} };
	auto gen = new CoinSequence(head_prob, transition_prob);
	gen->Generate(length, state_sequence, observed_sequence);
	delete gen;
}

int main() {
	std::vector<int> state_sequence;
	std::vector<int> observed_sequence;
	GenerateSequence(10000, state_sequence, observed_sequence);
	GibbsSample gs = GibbsSample(10000, observed_sequence);
	gs.Estimate();
	for (int i = 0; i < 100; ++i) {
		printf("Ground truth: %d, estimated: %d\n", state_sequence[i], gs.state_sequence_[i]);
	}

	return 0;
}