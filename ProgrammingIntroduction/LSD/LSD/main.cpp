#define _CRT_SECURE_NO_WARNINGS

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

struct MyVector {
  uint64_t* ptr = NULL;
  uint64_t capacity = 0;
  uint64_t size = 0;
  uint64_t increase_factor = 0;
};

const uint64_t mask = 0xff;
const int bite_count = 8;
const int numbers_count = 256;
const int size = 2;

uint64_t GetBite(uint64_t value, uint64_t position) {
  return ((value >> (bite_count * position)) & mask);
}

void Construct(MyVector* vector, uint64_t start_size) {
  vector->ptr = (uint64_t*)calloc(start_size, sizeof(uint64_t));
  vector->increase_factor = size;
  vector->size = 0;
  vector->capacity = start_size;
}

void Resize(MyVector* vector) {
  if (vector->size == vector->capacity) {
    vector->ptr = (uint64_t*)realloc(
        vector->ptr,
        vector->capacity * vector->increase_factor * sizeof(uint64_t));
    vector->capacity *= vector->increase_factor;
  }
}

void Push(MyVector* vector, uint64_t value) {
  Resize(vector);

  vector->ptr[vector->size] = value;
  ++vector->size;
}

uint64_t* Lsd(MyVector** numbers, uint64_t* nums, int count) {
  for (int i = 0; i < bite_count; ++i) {
    for (int j = 0; j < numbers_count; ++j) {
      numbers[j]->size = 0;
    }

    for (int j = 0; j < count; ++j) {
      uint64_t temp = GetBite(nums[j], i);
      Push(numbers[temp], nums[j]);
    }

    int k = 0;
    for (int j = 0; j < numbers_count; ++j) {
      for (int p = 0; p < numbers[j]->size; ++p) {
        nums[k] = numbers[j]->ptr[p];
        ++k;
      }
    }
  }

  return nums;
}

const int start = 500;
int main() {
  MyVector** numbers = (MyVector**)calloc(numbers_count, sizeof(MyVector*));

  int count = 0;
  scanf("%d", &count);

  uint64_t* nums = (uint64_t*)calloc(count, sizeof(uint64_t));
  for (int i = 0; i < count; ++i) {
    scanf("%llu", &nums[i]);
  }

  for (int i = 0; i < numbers_count; ++i) {
    numbers[i] = (MyVector*)calloc(1, sizeof(MyVector));
    Construct(numbers[i], start);
  }

  nums = Lsd(numbers, nums, count);

  for (int i = 0; i < count; ++i) {
    printf("%llu\n", nums[i]);
  }

  return 0;
}