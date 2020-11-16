#include <stdio.h>
#include <stdlib.h>

const int insertion_size = 15;
const int last_size = 2;

void Swap(void* ptr1, void* ptr2, int size);

void InsertionSort(void* begin, int size, int size_elem,
                   int (*cmp)(const void*, const void*)) {
  for (int i = 1; i < size; ++i) {
    int j = i - 1;
    while (j >= 0 && cmp((char*)begin + (j + 1) * size_elem,
                         (char*)begin + j * size_elem) < 0) {
      Swap((char*)begin + (j + 1) * size_elem, (char*)begin + j * size_elem,
           size_elem);
      j--;
    }
  }
}

void Swap(void* ptr1, void* ptr2, int size) {
  char tmp = '\0';
  for (int i = 0; i < size; ++i) {
    tmp = ((char*)ptr1)[i];
    ((char*)ptr1)[i] = ((char*)ptr2)[i];
    ((char*)ptr2)[i] = tmp;
  }
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

void QuickSort(void* begin, int size, int size_elem,
               int (*cmp)(const void*, const void*)) {
  if (size <= 1) {
    return;
  }
  if (size == last_size) {
    if (cmp((char*)begin, (char*)begin + 1 * size_elem) <= 0) {
      return;
    }
    Swap((char*)begin, (char*)begin + 1 * size_elem, size_elem);
    return;
  }
  if (size < insertion_size) {
    InsertionSort(begin, size, size_elem, cmp);
    return;
  }
  int first_pivot = 0;
  int second_pivot = 0;
  Partition(begin, size, size_elem, cmp, &first_pivot, &second_pivot);

  QuickSort((char*)begin, first_pivot, size_elem, cmp);
  QuickSort((char*)begin + (second_pivot + 1) * size_elem,
            size - second_pivot - 1, size_elem, cmp);
}

int Intcmp(const void* arg1, const void* arg2) {
  return *(const int*)arg1 - *(const int*)arg2;
}

int main() {
  int n = 0;
  scanf("%d", &n);
  int* array = (int*)calloc(n, sizeof(int));

  for (int i = 0; i < n; ++i) {
    scanf("%d", array + i);
  }
  QuickSort(array, n, sizeof(int), Intcmp);

  for (int i = 0; i < n; ++i) {
    printf("%d ", array[i]);
  }
  return 0;
}
