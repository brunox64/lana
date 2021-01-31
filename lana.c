#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

typedef struct {
    int _size;
    void **data;
    int length;
} List;

List* List_init(int size);
void List_free(List *list);
void List_push(List *list, void* item);
int List_indexOf(List *list, void* item);
int List_lastIndexOf(List *list, void* item);
void List_sort(List *list, int(*comp)(const void *a, const void *b));

// typedef struct {
//     regex_t regex;
// } Regex;

// int Regex_init();
// int Regex_test();
// int Regex_exec();
// int Regex_free();

int main(int argc, char** argv) {

    char *a = "abcdefghdksskakskakskskskskdlalks";
    char *b = "ab";
    int c = 100;

    printf("sizeof item: %d %d %d %d\n", (int)sizeof(a), (int)sizeof(b), (int)sizeof(c), (int)sizeof(void));
    printf("sizeof item: %d %d %d %d\n", (int)sizeof(*a), (int)sizeof(*b), (int)sizeof(c), (int)sizeof(void*));
    return 0;

    // printf("sizeof void* %d\n", (int) sizeof(void**));

    // int value = 20;

    // void **ptr = malloc(2 * sizeof(void*));
    // ptr[0] = "abc";
    // ptr[1] = &value;
    // printf("index: %d, value: %s\n", 0, (char*)ptr[0]);
    // printf("index: %d, value: %d\n", 1, (int)(*(int*)(ptr[1])));

    // void **ptr2 = malloc(4 * sizeof(void*));
    // memcpy(ptr2, ptr, 2 * sizeof(void*));
    // ptr2[2] = "xyz";
    // printf("index: %d, value: %s\n", 0, (char*)ptr2[0]);
    // printf("index: %d, value: %d\n", 1, (int)(*(int*)(ptr2[1])));
    // printf("index: %d, value: %s\n", 2, (char*)ptr2[2]);

    // List *list = List_init(2);

    // List_push(list, "ab");
    // List_push(list, "cd");
    // List_push(list, "ef");
    
    // printf("list size: %d, length: %d\n", list->_size, list->length);

    // for (int index = 0; index < list->length; index++) {
    //     // printf("item %d\n", index);
    //     printf("item %s\n", (char*) list->data[index]);
    // }

    // printf("indexOf a: %d\n", List_indexOf(list, "cd"));

    // List_free(list);

    // int v1 = 1;
    // int v2 = 2;
    // int v3 = 2;
    // List *l = List_init(2);
    // List_push(l, &v1);
    // List_push(l, &v2);

    // printf("index %d\n", List_indexOf(l, &v3));

    // List_free(l);

    List *searchText = List_init(1);
    List *regexMatches = List_init(1);
    List *regexNotMatches = List_init(1);
    List *regexGroups = List_init(1);
    List *regexGroupsNumber = List_init(1);
    int printLines = 0;
    int completeGaps = 0;

    for (int index = 1; index < argc; index++) {
        char *name = argv[index];

        if (strcmp(name, "-pl") == 0) {
            printLines = 1;
        } else if (strcmp(name, "-cg") == 0) {
            completeGaps = 1;
        } else if (strcmp(name, "-t") == 0) {
            List_push(searchText, argv[index+1]);
        } else if (strcmp(name, "-m") == 0) {
            List_push(regexMatches, argv[index+1]);
        } else if (strcmp(name, "-M") == 0) {
            List_push(regexNotMatches, argv[index+1]);
        }
    }

    return 0;
}

List* List_init(int size) {
    List* list = malloc(sizeof(List));
    list->data = malloc(size * sizeof(void*));
    list->length = 0;
    list->_size = size;
    return list;
}

void List_push(List *list, void* item) {
    if (list->_size <= list->length) {
        int plusSize = 10;
        void **data = list->data;
        list->data = malloc((list->_size+plusSize) * sizeof(void*));
        list->data = memcpy(list->data, data, list->_size * sizeof(void*));
        list->_size = list->_size+plusSize;
        free(data);
    }

    list->data[list->length] = item;
    list->length = list->length + 1;
}

int List_indexOf(List *list, void* item) {
    int sizeItem = sizeof(*item);

    int sizeOther;
    void* other;
    for (int index = 0; index < list->length; index++) {
        other = list->data[index];
        sizeOther = sizeof(*other);
        
        if (sizeOther == sizeItem && memcmp(other, item, sizeItem) == 0) {
            return index;
        }
    }

    return -1;
}

int List_lastIndexOf(List *list, void* item) {
    int sizeItem = sizeof(*item);

    int last = -1;
    int sizeOther;
    void* other;
    for (int index = 0; index < list->length; index++) {
        other = list->data[index];
        sizeOther = sizeof(*other);
        
        if (sizeOther == sizeItem && memcmp(other, item, sizeItem) == 0) {
            last = index;
        }
    }

    return last;
}

void List_sort(List *list, int(*comp)(const void *a, const void *b)) {
    qsort(list, list->length, sizeof(void*), comp);
}

void List_free(List *list) {
    free(list->data);
    free(list);
}
