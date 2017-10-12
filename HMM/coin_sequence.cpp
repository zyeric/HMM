#include "coin_sequence.h"
#include "uniform_generator.h"

CoinSequence::CoinSequence() {
	head_prob_ = { 0.5, 0.5, 0.5 };
	transition_prob_ =
	{ {1.0 / 3.0, 1.0 / 3.0, 1.0 / 3.0}, 
	  {1.0 / 3.0, 1.0 / 3.0, 1.0 / 3.0},
	  {1.0 / 3.0, 1.0 / 3.0, 1.0 / 3.0} };
}

CoinSequence::CoinSequence(std::vector<double> head_prob,
	std::vector<std::vector<double>> transition_prob) {
	head_prob_ = head_prob;
	transition_prob_ = transition_prob;
}

void CoinSequence::Generate(int length, std::vector<int>& state_sequence,
	std::vector<int>& observed_sequence) {
	UniformGenerator *gen = new UniformGenerator(0, 1);

	int cur_coin = 0;
	for (int i = 0; i < length; ++i) {
		state_sequence.push_back(cur_coin);
		if (gen->NextNumber() < head_prob_[cur_coin]) { observed_sequence.push_back(1); }
		else { observed_sequence.push_back(0); }
		double val = gen->NextNumber();
		if (val <= transition_prob_[cur_coin][0]) {
			cur_coin = 0;
		}
		else if (val <= transition_prob_[cur_coin][0] + transition_prob_[cur_coin][1]) {
			cur_coin = 1;
		}
		else {
			cur_coin = 2;
		}
	}

	delete gen;
}
