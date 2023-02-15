#pragma once
#include <string>
#include <vector>
#include <iostream>
#include "PhyUnit.h"

namespace Utils {
	void VISIONPX_API stringcopy(std::string* dst,const char* src);
	
	template<typename T>
	long long int VISIONPX_API findAt(std::vector<T> *list, T element);

	template<typename T>
	long long int findAt(std::vector<T>* list, T element)
	{
		size_t index = 0;
		while (index<list->size()) {
			if ((*list)[index] == element) {
				return index;
			}
			index++;
		}
		return -1;
	}
};
