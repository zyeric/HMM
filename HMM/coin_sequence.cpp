#include "coin_sequence.h"

CoinSequence::CoinSequence(std::vector<double> head_prob,
	std::vector<std::vector<double>> transition_prob) {
	head_prob_ = head_prob;
	transition_prob_ = transition_prob;
}

std::vector<std::pair<int, int>> CoinSequence::Generate(int length) {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<> dis(0, 1);
	
	std::vector<std::pair<int, int>> ret;
	int cur_coin = 0;
	for (int i = 0; i < length; ++i) {
		if (dis(gen) < head_prob_[cur_coin]) { ret.push_back({cur_coin, 1}); }
		else { ret.push_back({cur_coin, 0}); }
		double val = dis(gen);
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
	return ret;
}
