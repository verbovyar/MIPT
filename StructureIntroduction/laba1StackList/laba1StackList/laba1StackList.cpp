#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

struct Node {
    int value_ = 0;
    Node* next_ = NULL;
};

struct ForwardList {
    int size_ = 0;
    Node* head_ = NULL;
};

//
void construct(ForwardList* list)
{
    assert(list != NULL);

    Node* node = (Node*)calloc(1, sizeof(Node));
    
    list->size_ = 1; 
    list->head_ = node;
}

void push(ForwardList* list, int value)
{
    assert(list != NULL);

    Node* temp_node = (Node*)calloc(1, sizeof(Node));
    if (temp_node == NULL)
    {
        return;
    }

    Node* node = temp_node;
    node->value_ = value;
    node->next_ = list->head_;
    list->head_ = node;
    ++list->size_;
}

void pop(ForwardList* list)
{ 
    assert(list != NULL);

    if (list->size_ == NULL)
    {
        printf("WARNING! LIST IS EMPTY!");

        return;
    }

    struct Node* temp_node = list->head_;
    list->head_ = list->head_->next_;
    --list->size_;

    free(temp_node);
}

int top(ForwardList* list) 
{
    assert(list != NULL);

    return (list->head_->value_);
}

void destroy(ForwardList* list)
{
    assert(list != NULL);

    int size = list->size_;
    for (int index = 0; index < size; ++index)
    {
        free(list->head_);
        list->head_ = list->head_->next_;
    }
}

int main()
{
    ForwardList my_list;
    construct(&my_list);

    push(&my_list, 4);
    push(&my_list, 7);

    int value = top(&my_list);
    printf("%d ", value);

    pop(&my_list);

    value = top(&my_list);
    printf("%d ", value);

    return 0;
}