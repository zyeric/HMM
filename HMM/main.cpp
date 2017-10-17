#include "coin_sequence.h" 
#include "gibbs_sample.h"

void GenerateSequence(int length, std::vector<int>& state_sequence,
	std::vector<int>& observed_sequence) {
	std::vector<double> init_prob = { 1.0, 0.0, 0.0 };
	std::vector<double> head_prob = { 0.1, 0.5, 0.9 };
	std::vector<std::vector<double>> transition_prob =
	{ {0.8, 0.2, 0.0}, {0.1, 0.7, 0.2}, {0.1, 0.0, 0.9} };
	auto gen = new CoinSequence(init_prob, head_prob, transition_prob);
	gen->Generate(length, state_sequence, observed_sequence);
	delete gen;
}

int main() {
	int test_length = 10000;
	std::vector<int> state_sequence;
	std::vector<int> observed_sequence;
	GenerateSequence(test_length, state_sequence, observed_sequence);
	GibbsSample gs = GibbsSample(test_length, observed_sequence);
	gs.Estimate();
	auto ret = gs.estimated_para_.CalHiddenState(observed_sequence);
	int correct_cnt = 0;
	for (int i = 0; i < test_length; ++i) {
		if (observed_sequence[i] == ret[i])	++correct_cnt;
	}
	std::cout << "Correct:\t" << correct_cnt << std::endl;
	return 0;
}