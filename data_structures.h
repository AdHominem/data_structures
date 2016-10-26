//#include <stdio.h>
//#include <stdlib.h>
//
//typedef struct node Node;
//
//struct node {
//    int value;
//    Node *next;
//};
//
//typedef struct list {
//    Node *head;
//} List;
//
//
//Node *create_node(int value) {
//
//    Node *result = malloc(sizeof(Node));
//    if (result == NULL) {
//        return NULL;
//    }
//
//    result->next = NULL;
//    result->value = value;
//
//    return result;
//}
//
//
//void destroy_node(Node *node) {
//    if (node != NULL) {
//        free(node);
//    }
//}
//
//
//List *create_list() {
//    return malloc(sizeof(List));
//}
//
//
//void print_list(List *list) {
//    Node *head = list->head;
//
//    while (head != NULL) {
//        printf("%d ", head->value);
//        head = head->next;
//    }
//
//    printf("\n");
//}
//
//
//size_t size_of_(List *list) {
//    Node *head = list->head;
//    size_t result = 0;
//
//    while (head != NULL) {
//        head = head->next;
//        ++result;
//    }
//
//    return result;
//}
//
//
//void insert_at_index(List *list, Node *node, size_t index) {
//
//    // Catch index out of range
//    if (index > size_of_(list) || index < 0) {
//        perror("List index out of range!");
//        exit(1);
//    }
//
//    Node *head = list->head;
//
//    // Catch append to front case
//    if (index == 0) {
//        node->next = head;
//        list->head = node;
//        return;
//    }
//
//    Node *before = head;
//
//    for (int i = 0; i < index; ++i) {
//        before = head;
//        head = head->next;
//    }
//
//    before->next = node;
//    node->next = head;
//}
//
//
//void insert_at_end(List *list, Node *node) {
//    size_t size = size_of_(list);
//    insert_at_index(list, node, size == 0 ? 0 : size - 1);
//}