#define _CRT_SECURE_NO_WARNINGS

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

struct SparseTable {
  uint64_t** table = nullptr;
  uint64_t size = 0;
  uint64_t height = 0;
};

SparseTable* ConstructSparseTable(uint64_t count) {
  SparseTable* sparse = (SparseTable*)calloc(1, sizeof(SparseTable));

  sparse->size = count;
  sparse->height = log2(count);
  sparse->table = (uint64_t**)calloc(sparse->height, sizeof(uint64_t*));

  for (uint64_t i = 0; i < sparse->height + 1; ++i) {
    sparse->table[i] = (uint64_t*)calloc(sparse->size, sizeof(uint64_t));
  }

  return sparse;
}

void FillSparse(SparseTable* sparse, uint64_t* army) {
  for (uint64_t i = 0; i < sparse->size; ++i) {
    sparse->table[0][i] = army[i];
  }

  for (uint64_t i = 1; i <= sparse->height; ++i) {
    for (uint64_t j = 0; (j + (1 << i)) <= sparse->size; ++j) {
      sparse->table[i][j] = fmin(sparse->table[i - 1][j],
                                 sparse->table[i - 1][j + (1 << (i - 1))]);
    }
  }
}

uint64_t Query(SparseTable* sparse, uint64_t left, uint64_t right) {
  uint64_t i = log2(right - left + 1);

  return fmin(sparse->table[i][left], sparse->table[i][right + 1 - (1 << i)]);
}

int main() {
  uint64_t array_size = 0;
  uint64_t requests_count = 0;
  uint64_t first_element = 0;

  scanf("%d %d %d", &array_size, &requests_count, &first_element);

  uint64_t* army = (uint64_t*)calloc(array_size, sizeof(uint64_t));
  army[0] = first_element;
  for (uint64_t i = 0; i < array_size - 1; ++i) {
    army[i + 1] = (23 * army[i] + 21563) % 16714589;
  }

  uint64_t left = 0;
  uint64_t right = 0;
  uint64_t answer = 0;

  scanf("%d %d", &left, &right);

  SparseTable* sparse = ConstructSparseTable(array_size);
  FillSparse(sparse, army);

  uint64_t* log_array = (uint64_t*)calloc(array_size, sizeof(uint64_t));

  answer = Query(sparse, fmin(left, right) - 1, fmax(left, right) - 1);

  for (uint64_t i = 1; i < requests_count; ++i) {
    left = (17 * left + 751 + answer + 2 * i) % array_size + 1;
    right = (13 * right + 593 + answer + 5 * i) % array_size + 1;

    answer = Query(sparse, fmin(left, right) - 1, fmax(left, right) - 1);
  }

  printf("%llu %llu %llu\n", left, right, answer);

  return 0;
}
