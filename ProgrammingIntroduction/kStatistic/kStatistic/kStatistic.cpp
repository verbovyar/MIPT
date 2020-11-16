#include <stdio.h>
#include <stdlib.h>

const int factor1 = 123;
const int factor2 = 45;
const int divisior = 10004321;

void Swap(void* ptr1, void* ptr2, int size) {
  char temp = '\0';
  for (int i = 0; i < size; ++i) {
    temp = ((char*)ptr1)[i];
    ((char*)ptr1)[i] = ((char*)ptr2)[i];
    ((char*)ptr2)[i] = temp;
  }
}

int* NewArray(int n) {
  int* array = (int*)calloc(n, sizeof(int));
  scanf("%d %d", array, array + 1);
  for (int i = 2; i < n; ++i) {
    array[i] = (array[i - 1] * factor1 + array[i - 2] * factor2) % divisior;
  }
  return array;
}

int MyCmp(const void* arg1, const void* arg2) {
  return *(const int*)arg1 - *(const int*)arg2;
}

void Partition(void* begin, int size, int size_elem,
               int (*cmp)(const void*, const void*), int* first_pivot,
               int* second_pivot) {
  int i = 1;
  for (int j = 1; j < size; ++j) {
    if (cmp((char*)begin, (char*)begin + j * size_elem) > 0) {
      Swap((char*)begin + j * size_elem, (char*)begin + i * size_elem,
           size_elem);
      ++i;
    }
  }
  *first_pivot = i - 1;
  for (int j = i; j < size; ++j) {
    if (cmp((char*)begin, (char*)begin + j * size_elem) == 0) {
      Swap((char*)begin + j * size_elem, (char*)begin + i * size_elem,
           size_elem);
      ++i;
    }
  }
  Swap((char*)begin, (char*)begin + *first_pivot * size_elem, size_elem);
  *second_pivot = i - 1;
}

int KStatistic(int* array, int size, int k_statistic,
               int (*cmp)(const void*, const void*)) {
  int first_pivot = 0;
  int second_pivot = 0;
  Partition(array, size, sizeof(int), cmp, &first_pivot, &second_pivot);
  if (first_pivot <= k_statistic and k_statistic <= second_pivot) {
    return array[first_pivot];
  }
  if (k_statistic < first_pivot) {
    return KStatistic(array, first_pivot, k_statistic, cmp);
  }
  return KStatistic(array + second_pivot + 1, size - second_pivot - 1,
                    k_statistic - second_pivot - 1, cmp);
}

int main() {
  int count = 0;
  int k = 0;
  scanf("%d %d", &count, &k);
  int* array = NewArray(count);
  int answer = KStatistic(array, count, k - 1, MyCmp);
  printf("%d\n", answer);
  return 0;
}
