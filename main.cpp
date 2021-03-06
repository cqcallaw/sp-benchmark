#include <iostream>
#include <memory>
#include <vector>
#include <numeric>
#include <algorithm>
#include <chrono>
#include <assert.h>

void raw_iterative_increment(uint* out) {
	(*out)++;
}

void sp_iterative_increment(std::shared_ptr<uint> out) {
	(*out)++;
}

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

void print_stats(std::vector<int64_t> v, uint ops) {
	auto min = std::min_element(v.begin(), v.end());
	auto max = std::max_element(v.begin(), v.end());
	auto average = std::accumulate(v.begin(), v.end(), 0.0) / v.size();

	auto min_ns_per_op = ((double)*min) / ops;
	auto max_ns_per_op = ((double)*max) / ops;
	auto average_ns_per_op = average / ops;

	std::cout << "	Min: " << *min << " ns (" << min_ns_per_op << " ns per op)" << std::endl;
	std::cout << "	Max: " << *max << " ns (" << max_ns_per_op << " ns per op)" << std::endl;
	std::cout << "	Average: " << average << " ns (" << average_ns_per_op << " ns per op)" << std::endl;
	std::cout << std::endl;
}

int main()
{
	const uint iterations = 16384;
	const uint ops = 8192;

	// test raw pointer iteration
	auto raw_iterative_results = std::vector<int64_t>(iterations);
	for (uint i = 0; i < iterations; i++) {
		auto raw_ptr = new uint();
		*raw_ptr = i;
		auto local_bound = ops + i; // force repetition of tests
		auto begin = std::chrono::high_resolution_clock::now();
		for (uint j = i; j < local_bound; j++) {
			raw_iterative_increment(raw_ptr);
		}
		auto end = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count();
		assert(*raw_ptr == local_bound);
		raw_iterative_results[i] = duration;
	}
	std::cout << "Raw iterative ops: " << ops << std::endl;
	std::cout << "Raw iterative test repetitions: " << iterations << std::endl;
	std::cout << "Raw iterative results:" << std::endl;
	print_stats(raw_iterative_results, ops);

	// test raw pointer recursion
	auto raw_recursive_results = std::vector<int64_t>(iterations);
	for (uint i = 0; i < iterations; i++) {
		auto raw_ptr = new uint();
		*raw_ptr = i;
		auto local_bound = ops + i; // force repetition of tests
		auto begin = std::chrono::high_resolution_clock::now();
		raw_recursive_increment(raw_ptr, local_bound);
		auto end = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count();
		assert(*raw_ptr == local_bound);
		raw_recursive_results[i] = duration;
	}
	std::cout << "Raw recursion ops: " << ops << std::endl;
	std::cout << "Raw recursion test repetitions: " << iterations << std::endl;
	std::cout << "Raw recursive results:" << std::endl;
	print_stats(raw_recursive_results, ops);

	// test sp pointer iteration
	auto sp_iterative_results = std::vector<int64_t>(iterations);
	for (uint i = 0; i < iterations; i++) {
		auto sp = std::make_shared<uint>(i);
		auto local_bound = ops + i; // force repetition of tests
		auto begin = std::chrono::high_resolution_clock::now();
		for (uint j = i; j < local_bound; j++) {
			sp_iterative_increment(sp);
		}
		auto end = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count();
		assert(*sp == local_bound);
		sp_iterative_results[i] = duration;
	}
	std::cout << "SP iterative ops: " << ops << std::endl;
	std::cout << "SP iterative test repetitions: " << iterations << std::endl;
	std::cout << "SP iterative results:" << std::endl;
	print_stats(sp_iterative_results, ops);

	// test smart pointer recursion
	auto sp__recursive_results = std::vector<int64_t>(iterations);
	for (uint i = 0; i < iterations; i++) {
		auto sp = std::make_shared<uint>(i);
		auto local_bound = ops + i; // force repetition of tests
		auto begin = std::chrono::high_resolution_clock::now();
		sp_recursive_increment(sp, local_bound);
		auto end = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count();
		assert(*sp == local_bound);
		sp__recursive_results[i] = duration;
	}

	std::cout << "SP recursion ops: " << ops << std::endl;
	std::cout << "SP recursion test repetitions: " << iterations << std::endl;
	std::cout << "SP recursive results:" << std::endl;
	print_stats(sp__recursive_results, ops);
}