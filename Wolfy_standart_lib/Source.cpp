


extern "C" {
	int is_equal(const char* str1, const char* str2) {
		int index = 0;
		while (str1[index]!='\0')
		{
			if (str1[index] != str2[index]) {
				return -1;
			}
			index++;
		}

		return 1;
	}

	int save_is_equal(const char* str1, const char* str2, int size) {
		for (int i = 0; i < size; i++) {
			if (str1[i] != str2[i]) {
				return -1;
			}
		}
		return 1;
	}
}