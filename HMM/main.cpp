#include <vector>
#include <cstdio>
#include "sequence_generator.h" 

void GenerateSequence() {
	std::vector<double> head_prob = { 0.1, 0.5, 0.9 };
	std::vector<std::vector<double>> transition_prob =
	{ {0.8, 0.2, 0.0}, {0.1, 0.7, 0.2}, {0.1, 0.0, 0.9} };
	auto gen = new SequenceGenerator(3, head_prob, transition_prob);

	auto X = gen->Generate(100);
	auto Y = gen->Generate(1000);
	auto Z = gen->Generate(10000);

	/*
	for (auto item : X) {
		printf("%d %d\n", item.first, item.second);
	}
	*/
}

int main() {
	GenerateSequence();
	return 0;
}