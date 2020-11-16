#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum Commands { INSERT = 0, EXTRUCTMIN = 1, GETMIN = 2, SIZE = 3, CLEAR = 4 };

const int max_command_size = 12;
const int max_start_size = (int)2e6;
const int standart_size = 10;
const int comand_numbers = 5;

struct Heap {
  size_t size = 0;
  size_t capacity = 0;
  size_t increase_factor = 2;
  int* buffer = NULL;
};

int GetParent(int index) { return (index - 1) / 2; }

void Swap(int* numb1, int* numb2) {
  int tmp = *numb1;
  *numb1 = *numb2;
  *numb2 = tmp;
}

void Construct(Heap* heap, size_t size) {
  assert(size < max_start_size);
  heap->buffer = (int*)calloc(size, sizeof(int));
  heap->capacity = size;
  heap->size = 0;
}

void SiftUp(Heap* heap, int index) {
  if (index == 0) {
    return;
  }
  if (heap->buffer[index] < heap->buffer[GetParent(index)]) {
    Swap(heap->buffer + index, heap->buffer + GetParent(index));
    SiftUp(heap, GetParent(index));
  }
}

int GetMin(Heap* heap) { return (heap->buffer[0]); }

void SiftDown(Heap* heap, int index) {
  int min_index = index;

  if (index * 2 + 1 < heap->size) {
    if (heap->buffer[min_index] > heap->buffer[index * 2 + 1]) {
      min_index = index * 2 + 1;
    }
  }
  if (index * 2 + 2 < heap->size) {
    if (heap->buffer[min_index] > heap->buffer[index * 2 + 2]) {
      min_index = index * 2 + 2;
    }
  }
  if (min_index != index) {
    Swap(heap->buffer + min_index, heap->buffer + index);
    SiftDown(heap, min_index);
  }
}

void ExtractMin(Heap* heap) {
  Swap(heap->buffer, heap->buffer + heap->size - 1);
  heap->buffer[heap->size - 1] = 0;
  heap->size--;
  if (heap->size > 0) {
    SiftDown(heap, 0);
  }
}

void Resize(Heap* heap) {
  if (heap->size == heap->capacity - 2) {
    heap->buffer = (int*)realloc(
        heap->buffer, heap->capacity * sizeof(int) * heap->increase_factor);
    heap->capacity *= heap->increase_factor;
  }
}

void Push(Heap* heap, int value) {
  Resize(heap);
  heap->buffer[heap->size] = value;
  heap->size++;
  SiftUp(heap, heap->size - 1);
}

int GetSize(Heap* heap) { return heap->size; }

void Clear(Heap* heap) {
  heap->buffer = (int*)realloc(heap->buffer, standart_size * sizeof(int));
  heap->capacity = standart_size;
  heap->size = 0;
}

int FindComand(char* command) {
  const char* str_commands[] = {"insert", "extract_min", "get_min", "size",
                                "clear"};
  for (int i = 0; i < comand_numbers; ++i) {
    if (strcmp(str_commands[i], command) == 0) {
      return i;
    }
  }
  return -1;
}

void WorkWithCommand(char* command, Heap* heap) {
  int comand_number = FindComand(command);
  switch (comand_number) {
    case INSERT: {
      int numb = 0;
      scanf("%d", &numb);
      Push(heap, numb);
      printf("ok\n");
      break;
    }
    case EXTRUCTMIN: {
      if (GetSize(heap) == 0) {
        printf("error\n");
        break;
      }
      printf("%d\n", GetMin(heap));
      ExtractMin(heap);
      break;
    }
    case GETMIN: {
      if (GetSize(heap) == 0) {
        printf("error\n");
        break;
      }
      printf("%d\n", GetMin(heap));
      break;
    }
    case SIZE: {
      printf("%d\n", GetSize(heap));
      break;
    }
    case CLEAR: {
      Clear(heap);
      printf("ok\n");
      break;
    }
  }
}

void Destruct(Heap* heap) {
  free(heap->buffer);
  heap->buffer = NULL;
  heap->size = 0;
  heap->capacity = 0;
}

int main() {
  Heap heap = {};
  Construct(&heap, standart_size);
  int n = 0;
  scanf("%d", &n);
  for (int i = 0; i < n; ++i) {
    char* command = (char*)calloc(max_command_size, sizeof(char));
    scanf("%s", command);
    WorkWithCommand(command, &heap);
    free(command);
  }
  Destruct(&heap);

  return 0;
}