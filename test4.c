#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
void reverse_string(char* string,int low ,int high) {
	int tmp = 0;
	if (low < high) {
		tmp = *(string + low);
		*(string + low) = *(string + high);
		*(string + high) = tmp;
		reverse_string(string,(low + 1),( high - 1));
	}
}

int main() {
	char arr[] = "abcdefg";
	reverse_string(arr, 0, 6);
	printf("%s", arr);
	return 0;

	system("pause");
}
