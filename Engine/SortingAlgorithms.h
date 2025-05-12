#pragma once
#include <vector>

class SortingAlgorithms
{
public:
	template<typename T>
	static void QuickSort(std::vector<T>& data, const int low, const int high)
	{
		if (low < high)
		{
			int partitionIndex = Partition(data, low, high);
			QuickSort(data, low, partitionIndex - 1);
			QuickSort(data, partitionIndex + 1, high);
		}
	}

	template<typename T>
	int Partition(const std::vector<T>& data, const int low, const int high)
	{
		//forces the user to pass a class that has a member function
		static_assert(std::is_member_function_pointer<decltype(&T::getComparisonValue)>::value,
			"T must have a member function getComparisonValue()");

		auto getFactor = [](const T& item) -> float {
			if constexpr (std::is_pointer<T>::value)
				return item->GetComparingFactor();  // T is a pointer
			else
				return item.GetComparingFactor();   // T is a value/reference
			};

		float pivot = getFactor(data[high]);
		int i = low - 1;
		for (int j = low; j < high; j++) {
			if (getFactor(data[j]) < pivot) {
				i++;
				Swap(i, j);
			}
		}
		Swap(i + 1, high);
		return i + 1;
	}

	template<typename T>
	void Swap(std::vector<T>& data, const int firstPos, const int secondPos)
	{
		T temp = data[firstPos];
		data[firstPos] = data[secondPos];
		data[secondPos] = temp;
	}
};