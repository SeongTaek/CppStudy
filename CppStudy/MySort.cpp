#include "MySort.h"
#include <iostream>
#include <chrono>
#include <random>
#include <string>

using namespace std;

long long MySort::operator()(SortAlgorithm sortType, vector<int>& elements)
{
	std::chrono::time_point<chrono::high_resolution_clock> start = chrono::high_resolution_clock::now();

	switch (sortType)
	{
	case SortAlgorithm::BubbleSort:
		BubbleSort(elements);
		break;
	case SortAlgorithm::SelectionSort:
		SelectionSort(elements);
		break;
	case SortAlgorithm::InsertionSort:
		InsertionSort(elements);
		break;
	case SortAlgorithm::ShellSort:
		ShellSort(elements);
		break;
	case SortAlgorithm::MergeSort:
		MergeSort(elements);
		break;
	case SortAlgorithm::QuickSort:
		QuickSort(elements);
		break;
	}

	chrono::time_point<chrono::high_resolution_clock> end = chrono::high_resolution_clock::now();
	auto diff = end - start;

	return chrono::duration_cast<chrono::microseconds>(diff).count();
}

void MySort::PrintPerformance(const int numOfElements)
{
	MySort sorter;
	random_device rd;
	mt19937 gen(rd());
	string algorithmNames[]{ "BubbleSort", "SelectionSort", "InsertionSort", "ShellSort", "MergeSort", "QuickSort" };

	cout << "**********************************************" << endl;
	cout << "Num of elements : " << numOfElements << endl;

	for (int a = static_cast<int>(SortAlgorithm::BubbleSort); a != static_cast<int>(SortAlgorithm::Last); ++a)
	{
		SortAlgorithm algorithm = static_cast<SortAlgorithm>(a);
		uniform_int_distribution<int> dis(0, numOfElements);
		vector<int> elements(numOfElements);

		for (size_t i = 0; i < elements.size(); ++i)
		{
			elements[i] = dis(gen);
		}

		cout << algorithmNames[a] << " : ";
		cout << sorter(algorithm, elements) << "us" << endl;

		elements.clear();
	}

	cout << "**********************************************" << endl;
}

void MySort::BubbleSort(vector<int>& elements)
{
	for (size_t i = 1; i < elements.size(); ++i)
	{
		for (size_t j = 0; j < elements.size() - i; ++j)
		{
			if (elements[j] > elements[j + 1])
			{
				swap(elements[j], elements[j + 1]);
			}
		}
	}
}

void MySort::SelectionSort(vector<int>& elements)
{
	for (size_t i = 0; i < elements.size() - 1; ++i)
	{
		int min = i;

		for (size_t j = i + 1; j < elements.size(); ++j)
		{
			if (elements[j] < elements[min])
			{
				min = j;
			}
		}

		swap(elements[i], elements[min]);
	}
}

void MySort::InsertionSort(vector<int>& elements)
{
	size_t i;
	int j;

	for (i = 1; i < elements.size(); ++i)
	{
		int temp = elements[i];

		for (j = i - 1; j >= 0 && elements[j] > temp; --j)
		{
			elements[j + 1] = elements[j];
		}

		elements[j + 1] = temp;
	}
}

void MySort::GapInsertionSort(vector<int>& elements, const int start, const size_t end, const int gap)
{
	size_t i;
	int j;

	for (i = start + gap; i < end; i += gap)
	{
		int temp = elements[i];

		for (j = i - gap; j >= start && elements[j] > temp; j -= gap)
		{
			elements[j + gap] = elements[j];
		}

		elements[j + gap] = temp;
	}
}

void MySort::ShellSort(vector<int>& elements)
{
	for (size_t gap = elements.size() / 2; gap > 0; gap /= 2)
	{
		if (gap % 2 == 0)
		{
			++gap;
		}

		for (size_t i = 0; i < gap; ++i)
		{
			GapInsertionSort(elements, i, elements.size(), gap);
		}
	}
}

void MySort::MergeSort(vector<int>& elements)
{
	if (elements.size() <= 1)
	{
		return;
	}

	size_t mid = elements.size() / 2;
	vector<int> left{ vector<int>(elements.begin(), elements.begin() + mid) };
	vector<int> right{ vector<int>(elements.begin() + mid, elements.end()) };

	MergeSort(left);
	MergeSort(right);

	elements = Merge(left, right);
}

vector<int> MySort::Merge(const vector<int>& left, const vector<int>& right)
{
	size_t leftIndex = 0, rightIndex = 0;
	vector<int> merged;

	merged.reserve(left.size() + right.size());

	while (leftIndex < left.size() && rightIndex < right.size())
	{
		if (left[leftIndex] <= right[rightIndex])
		{
			merged.push_back(left[leftIndex++]);

			if (leftIndex == left.size())
			{
				merged.insert(merged.end(), right.begin() + rightIndex, right.end());

				return merged;
			}
		}
		else if (left[leftIndex] > right[rightIndex])
		{
			merged.push_back(right[rightIndex++]);

			if (rightIndex == right.size())
			{
				merged.insert(merged.end(), left.begin() + leftIndex, left.end());

				return merged;
			}
		}
	}

	return merged;
}

size_t MySort::Partition(vector<int>& elements, const size_t start, const size_t end)
{
	// 아래 로직은 (end - start + 1) 만큼 메모리를 생성하고 partition 해서 메모리 효율이 좋지 않다.
	/*const int& pivot = elements[end];
	vector<int> partitioned_elements(end - start + 1);

	size_t left = 0;
	size_t right = (end - start);

	for (size_t i = start; i <= end; ++i)
	{
		if (elements[i] <= pivot)
		{
			partitioned_elements[left++] = elements[i];
		}
		else
		{
			partitioned_elements[right--] = elements[i];
		}
	}

	for (size_t i = start; i <= end; ++i)
	{
		elements[i] = partitioned_elements[i];
	}

	return right;*/
	// 메모리를 추가 생성하지 않고 in-place 방식으로 partition 한다.
	// A 지시자는 피벗과 비교하는 원소
	// B 지시자는 피벗보다 작으면 A 지시자와 swap할 원소
	const int& pivot = elements[end];
	size_t current_small_loc = start; // B 지시자

	// i가 A 지시자
	for (size_t i = start; i <= end; ++i)
	{
		if (elements[i] <= pivot)
		{
			swap(elements[i], elements[current_small_loc]);
			++current_small_loc;
		}
	}

	return current_small_loc - 1;
}

void MySort::QuickSortInternal(vector<int>& elements, const size_t start, const size_t end)
{
	if (start >= end)
	{
		return;
	}

	size_t pivot = Partition(elements, start, end);

	if (pivot > 0)
	{
		QuickSortInternal(elements, start, pivot - 1);
	}
	QuickSortInternal(elements, pivot + 1, end);
}

void MySort::QuickSort(vector<int>& elements)
{
	QuickSortInternal(elements, 0, elements.size() - 1);
}