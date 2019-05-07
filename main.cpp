#include <iostream>
#include <memory>
#include <vector>
#include <numeric>
#include <algorithm>
#include <chrono>
#include <assert.h>

void raw_recursive_increment(uint* out, uint bound) {
	if (*out >= bound) {
		return;
	} else {
		(*out)++;
		raw_recursive_increment(out, bound);
	}
}

void sp_recursive_increment(std::shared_ptr<uint> out, uint bound) {
	if (*out >= bound) {
		return;
	} else {
		(*out)++;
		sp_recursive_increment(out, bound);
	}
}

void print_stats(std::vector<int64_t> v) {
	auto min = std::min_element(v.begin(), v.end());
	auto max = std::max_element(v.begin(), v.end());
	auto average = std::accumulate(v.begin(), v.end(), 0.0) / v.size();

	std::cout << "	Min: " << *min << std::endl;
	std::cout << "	Max: " << *max << std::endl;
	std::cout << "	Average: " << average << std::endl;
}

int main()
{
	const uint iterations = 1024;
	const uint bound = 8192;

	auto raw_ptr = new uint();
	auto raw_results = std::vector<int64_t>(iterations);
	for (uint i = 0; i < iterations; i++) {
		*raw_ptr = 0;
		auto begin = std::chrono::high_resolution_clock::now();
		raw_recursive_increment(raw_ptr, bound);
		auto end = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count();
		assert(*raw_ptr == bound);
		raw_results[i] = duration;
	}
	std::cout << "Raw recursion depth: " << bound << std::endl;
	std::cout << "Raw test repetitions: " << iterations << std::endl;
	std::cout << "Raw results (in nanoseconds):" << std::endl;
	print_stats(raw_results);

	auto sp_results = std::vector<int64_t>(iterations);
	for (uint i = 0; i < iterations; i++) {
		auto sp = std::make_shared<uint>(0);
		auto begin = std::chrono::high_resolution_clock::now();
		sp_recursive_increment(sp, bound);
		auto end = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count();
		assert(*sp == bound);
		sp_results[i] = duration;
	}

	std::cout << "SP recursion depth: " << bound << std::endl;
	std::cout << "SP test repetitions: " << iterations << std::endl;
	std::cout << "SP results (in nanoseconds):" << std::endl;
	print_stats(sp_results);
}