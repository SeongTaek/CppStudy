#pragma once
#include <vector>

class MySort
{
public:
	enum class SortAlgorithm
	{
		BubbleSort = 0,
		SelectionSort,
		InsertionSort,
		ShellSort,
		MergeSort,
		QuickSort,
		Last,
	};

public:
	MySort() = default;
	~MySort() = default;

	long long operator()(SortAlgorithm sortType, std::vector<int>& elements);

public:
	static void PrintPerformance(const int numOfElements);

private:
	void BubbleSort(std::vector<int>& elements);
	void SelectionSort(std::vector<int>& elements);
	void InsertionSort(std::vector<int>& elements);
	void ShellSort(std::vector<int>& elements);
	void MergeSort(std::vector<int>& elements);
	void QuickSort(std::vector<int>& elements);

private:
	void GapInsertionSort(std::vector<int>& elements, const int start, const size_t end, const int gap);
	std::vector<int> Merge(const std::vector<int>& left, const std::vector<int>& right);
	size_t Partition(std::vector<int>& elements, const size_t start, const size_t end);
	void QuickSortInternal(std::vector<int>& elements, const size_t start, const size_t end);
};

