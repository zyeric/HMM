#include "sequence_generator.h"

SequenceGenerator::SequenceGenerator(int state_num, std::vector<double> head_prob,
	std::vector<std::vector<double>> transition_prob) {
	state_num_ = state_num;
	head_prob_ = head_prob;
	transition_prob_ = transition_prob;
}

std::vector<int> SequenceGenerator::Generate(int length) {
	return std::vector<int>();
}
