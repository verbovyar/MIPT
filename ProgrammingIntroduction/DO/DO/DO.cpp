#define _CRT_SECURE_NO_WARNINGS

#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>

using std::vector;

const int factor = 4;

struct Tree {
  vector<long long> tree;
  vector<long long> add_value;
  int size = 0;
  int array_size = 0;
  int real_size = 0;
};

long long* ArrayConstruct(int size) {
  long long* army = (long long*)calloc(size, sizeof(long long));

  return army;
}

void FillTree(long long* army, Tree* seg_tree, int idx, int left, int right) {
  if (right - left == 1) {
    if (left < seg_tree->real_size) {
      seg_tree->tree[idx] = army[left];
    }
  } else {
    int median = (left + right) / 2;
    FillTree(army, seg_tree, idx * 2 + 1, left, median);
    FillTree(army, seg_tree, idx * 2 + 2, median, right);
    seg_tree->tree[idx] =
        seg_tree->tree[idx * 2 + 1] + seg_tree->tree[idx * 2 + 2];
  }
}

Tree* ConstructTree(int size) {
  Tree* seg_tree = new Tree;

  int new_size = 1;
  while (new_size < size) {
    new_size *= 2;
  }

  seg_tree->real_size = size;
  seg_tree->size = new_size * 2 - 1;
  seg_tree->array_size = new_size;

  auto it = seg_tree->tree.begin();
  for (int i = 0; i < seg_tree->size; ++i) {
    seg_tree->tree.push_back(0);
    seg_tree->add_value.push_back(0);
  }

  return seg_tree;
}

void Update(Tree* seg_tree, int idx, int tree_left, int tree_right, int left,
            int right, int add_value) {
  if (left >= right) {
    return;
  }

  if (left == tree_left && tree_right == right) {
    seg_tree->add_value[idx] += add_value;
  } else {
    int median = (tree_left + tree_right) / 2;
    Update(seg_tree, idx * 2 + 1, tree_left, median, left, fmin(right, median),
           add_value);
    Update(seg_tree, idx * 2 + 2, median, tree_right, fmax(left, median), right,
           add_value);
  }
}

long long Get(Tree* seg_tree, int idx, int tree_left, int tree_right, int pos) {
  if (abs(tree_left - tree_right) == 1) {
    return seg_tree->tree[idx] + seg_tree->add_value[idx];
  }

  int median = (tree_left + tree_right) / 2;

  if (pos <= median) {
    return seg_tree->add_value[idx] +
           Get(seg_tree, idx * 2 + 1, tree_left, median, pos);
  }

  return seg_tree->add_value[idx] +
         Get(seg_tree, idx * 2 + 2, median, tree_right, pos);
}

int main() {
  int size = 0;
  scanf("%d", &size);
  long long* army = ArrayConstruct(size);

  for (int i = 0; i < size; ++i) {
    scanf("%lld", &army[i]);
  }

  int request_count = 0;
  scanf("%d", &request_count);

  Tree* seg_tree = ConstructTree(size);
  FillTree(army, seg_tree, 0, 0, seg_tree->array_size);

  char command = 0;
  int value = 0;
  for (int i = 0; i < request_count; ++i) {
    scanf("\n");
    scanf("%c", &command);
    char flag = command;

    switch (flag) {
      case 'g': {
        int value = 0;
        scanf("%d", &value);
        long long answer = Get(seg_tree, 0, 0, seg_tree->array_size, value);
        printf("%lld\n", answer);

        break;
      }
      case 'a': {
        int left = 0;
        int right = 0;
        int add = 0;
        scanf("%d %d %d", &left, &right, &add);
        Update(seg_tree, 0, 0, seg_tree->array_size, left - 1, right, add);
        break;
      }
    }
  }

  return 0;
}