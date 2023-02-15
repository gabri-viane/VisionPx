#include "pch.h"
#include "utils.h"

namespace Utils {
	void stringcopy(std::string* dst,const char* src) {
		for (int i = 0;; i++) {
			if (src[i] == '\0') {
				break;
			}
			*dst += src[i];
		}
	}

}