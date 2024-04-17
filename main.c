#include <stdio.h>
#include <assert.h>
#include <malloc.h>
#include <stdlib.h>
#include <memory.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>


void inputArray_(int a[], const size_t n) {
    for (size_t i = 0; i < n; i++)
        scanf("%d", a + i);
}


void outputArray_(const int a[], const size_t n) {
    for (size_t i = 0; i < n; i++)
        printf("%d ", *(a + i));
    printf("\n");
}


size_t linearSearch_(const int a[], const size_t n, int x) {
    for (size_t i = 0; i < n; i++)
        if (a[i] == x)
            return i;
    return n;
}


size_t binarySearch_(const int a[], const size_t n, int x) {
    if (a[0] > x || n == 0)
        return n;

    size_t left = 0;
    size_t right = n - 1;

    while (left <= right) {
        size_t middle = left + (right - left) / 2;

        if (a[middle] < x)
            left = middle + 1;
        else if (a[middle] > x)
            right = middle - 1;
        else
            return middle;
    }

    return n;
}


size_t binarySearchMoreOrEqual_(const int a[], const size_t n, int x) {
    if (a[0] >= x)
        return 0;

    size_t left = 0;
    size_t right = n;

    while (right - left > 1) {
        size_t middle = left + (right - left) / 2;

        if (a[middle] < x)
            left = middle;
        else
            right = middle;
    }

    return right;
}




void append_(int a[], size_t* n, int value) {
    a[*n] = value;
    (*n)++;
}


void deleteByPosSaveOrder_(int a[], size_t* n, size_t pos) {
    for (size_t i = pos; i < *n; i++)
        a[i] = a[i + 1];
    (*n)--;
}


void deleteByPosUnsaveOrder_(int a[], size_t* n, size_t pos) {
    a[pos] = a[*n - 1];
    (*n)--;
}


int all_(const int a[], size_t n, int (*predicate)(int)) {
    for (size_t i = 0; i < n; i++)
        if (!predicate(a[i]))
            return 0;
    return 1;
}


int any_(const int a[], size_t n, int (*predicate)(int)) {
    for (size_t i = 0; i < n; i++)
        if (predicate(a[i]))
            return 1;
    return 0;
}


void forEach_(const int source[], int dest[], size_t n, int(*predicate)(int)) {
    for (size_t i = 0; i < n; i++)
        dest[i] = predicate(source[i]);
}


int countIf_(const int a[], size_t n, int(*predicate)(int)) {
    int res = 0;

    for (size_t i = 0; i < n; i++)
        if (predicate(a[i]))
            res++;

    return res;
}


void deleteIf_(int a[], size_t* n, int(*deletePredicate)(int)) {
    size_t i_read = 0;
    while (i_read < *n && !deletePredicate(a[i_read]))
        i_read++;

    size_t i_write = i_read;
    while (i_read < *n) {
        if (!deletePredicate(a[i_read])) {
            a[i_write] = a[i_read];
            i_write++;
        }

        i_read++;
    }

    (*n) = i_write;
}





typedef struct unordered_array_set {
    int* data;
    size_t size;
    size_t capacity;
} unordered_array_set;


static int compare_ints(const void* a, const void* b) {
    return *(int* ) a - *(int* ) b;
}


unordered_array_set unordered_array_set_create(size_t capacity) {
    return (unordered_array_set) {malloc(sizeof(int) * capacity), 0, capacity};
}


void unordered_array_set_isAbleAppend(unordered_array_set *set) {
    assert(set -> size < set -> capacity);
}


size_t unordered_array_set_in(unordered_array_set* set, int value) {
    return linearSearch_(set -> data, set -> size, value);
}


void unordered_array_set_insert(unordered_array_set* set, int value) {
    if (unordered_array_set_in(set, value) == set -> size) {
        unordered_array_set_isAbleAppend(set);
        append_(set -> data, &set -> size, value);
    }
}


bool unordered_array_set_isEqual(unordered_array_set set1, unordered_array_set set2) {
    if (set1.size != set2.size)
        return 0;

    qsort(set1.data, set1.size, sizeof(int), compare_ints);
    qsort(set2.data, set2.size, sizeof(int), compare_ints);

    return memcmp(set1.data, set2.data, sizeof(int) * set1.size) == 0;
}


static void unordered_array_set_shrinkToFit(unordered_array_set* a) {
    if (a -> size != a -> capacity) {
        a -> data = (int*)realloc(a -> data, sizeof(int) * a -> size);
        a -> capacity = a -> size;
    }
}


unordered_array_set unordered_array_set_create_from_array(const int* a, size_t size) {
    unordered_array_set set = unordered_array_set_create(size);
    for (size_t i = 0; i < size; i++)
        unordered_array_set_insert(&set, a[i]);
    unordered_array_set_shrinkToFit(&set);

    return set;
}


bool unordered_array_set_isSubset(unordered_array_set subset, unordered_array_set set) {
    for (size_t i = 0; i < subset.size; i++) {
        bool found = false;

        for (size_t j = 0; j < set.size; j++)
            if (subset.data[i] == set.data[j]) {
                found = true;
                break;
            }

        if (!found)
            return false;
    }

    return true;
}


void unordered_array_set_deleteElement(unordered_array_set* set, int value) {
    size_t index_value = unordered_array_set_in(set, value);

    if (index_value < set -> size) {
        set->data[index_value] = set->data[set->size - 1];
        (set->size)--;
    }
}


unordered_array_set unordered_array_set_union(unordered_array_set set1, unordered_array_set set2) {
    size_t new_capacity = set1.size + set2.size;
    unordered_array_set set = unordered_array_set_create(new_capacity);

    for (size_t i = 0; i < set1.size; i++) {
        set.data[i] = set1.data[i];
        set.size++;
    }

    for (size_t i = 0; i < set2.size; i++)
        unordered_array_set_insert(&set, set2.data[i]);

    unordered_array_set_shrinkToFit(&set);

    return set;
}


unordered_array_set unordered_array_set_intersection(unordered_array_set set1, unordered_array_set set2) {
    size_t new_capacity = set1.size < set2.size ? set1.size : set2.size;
    unordered_array_set set = unordered_array_set_create(new_capacity);

    for (size_t i = 0; i < set1.size; i++)
        if (unordered_array_set_in(&set2, set1.data[i]) != set2.size)
            unordered_array_set_insert(&set, set1.data[i]);

    return set;
}


unordered_array_set unordered_array_set_difference(unordered_array_set set1, unordered_array_set set2) {
    size_t new_capacity = set1.size;
    unordered_array_set set = unordered_array_set_create(new_capacity);

    for (size_t i = 0; i < set1.size; i++)
        if (unordered_array_set_in(&set2, set1.data[i]) == set2.size)
            unordered_array_set_insert(&set, set1.data[i]);

    return set;
}


unordered_array_set unordered_array_set_complement(unordered_array_set set, unordered_array_set universumSet) {
    size_t new_capacity = universumSet.size;
    unordered_array_set new_set = unordered_array_set_create(new_capacity);

    for (size_t i = 0; i < universumSet.size; i++)
        if (unordered_array_set_in(&set, universumSet.data[i]) == set.size)
            unordered_array_set_insert(&new_set, universumSet.data[i]);

    assert(unordered_array_set_isSubset(new_set, universumSet));

    return new_set;
}


void unordered_array_set_delete(unordered_array_set* set) {
    free(set -> data);
    set -> data = NULL;

    set -> size = 0;
    set -> capacity = 0;
}


unordered_array_set unordered_array_set_symmetricDifference(unordered_array_set set1, unordered_array_set set2) {
    unordered_array_set universum = unordered_array_set_union(set1, set2);
    unordered_array_set intersection = unordered_array_set_intersection(set1, set2);

    unordered_array_set symmetric = unordered_array_set_complement(intersection, universum);

    unordered_array_set_delete(&intersection);
    unordered_array_set_delete(&universum);

    return symmetric;
}


void unordered_array_set_print(unordered_array_set set) {
    printf("{");
    int is_empty = 1;

    for (size_t i = 0; i < set.size; i++) {
        printf("%d, ", *(set.data + i));
        is_empty = 0;
    }
    if (is_empty)
        printf("}\n");
    else
        printf("\b\b}\n");
}


typedef struct ordered_array_set {
    int* data;
    size_t size;
    size_t capacity;
} ordered_array_set;

ordered_array_set ordered_array_set_create(size_t capacity) {
    return (ordered_array_set) {malloc(sizeof(int) * capacity), 0, capacity};
}


void ordered_array_set_isAbleAppend(ordered_array_set *set) {
    assert(set->size < set->capacity);
}


size_t ordered_array_set_in(ordered_array_set *set, int value) {
    return binarySearch_(set->data, set->size, value);
}


void ordered_array_set_insert(ordered_array_set *set, int value) {
    size_t index = ordered_array_set_in(set, value);

    if (index == set->size) {
        ordered_array_set_isAbleAppend(set);

        size_t i;
        for (i = set->size; (i > 0 && set->data[i - 1] > value); i--)
            set->data[i] = set->data[i - 1];

        set->data[i] = value;
        set->size++;
    }
}


bool ordered_array_set_isEqual(ordered_array_set set1, ordered_array_set set2) {
    if (set1.size != set2.size)
        return 0;

    return memcmp(set1.data, set2.data, sizeof(int) * set1.size) == 0;
}


void ordered_array_set_shrinkToFit(ordered_array_set *a) {
    if (a->size != a->capacity) {
        a->data = (int *) realloc(a->data, sizeof(int) * a->size);
        a->capacity = a->size;
    }
}


ordered_array_set ordered_array_set_create_from_array(const int *a, size_t size) {
    ordered_array_set set = ordered_array_set_create(size);
    for (size_t i = 0; i < size; i++)
        ordered_array_set_insert(&set, *(a + i));
    ordered_array_set_shrinkToFit(&set);

    return set;
}


bool ordered_array_set_isSubset(ordered_array_set subset, ordered_array_set set) {
    for (size_t i = 0; i < subset.size; i++) {
        bool found = false;

        for (size_t j = 0; j < set.size; j++)
            if (subset.data[i] == set.data[j]) {
                found = true;
                break;
            }

        if (!found)
            return false;
    }

    return true;
}


void ordered_array_set_deleteElement(ordered_array_set *set, int value) {
    size_t index = ordered_array_set_in(set, value);

    if (index != set->size)
        deleteByPosSaveOrder_(set->data, &set->size, index);
}


ordered_array_set ordered_array_set_union(ordered_array_set set1, ordered_array_set set2) {
    size_t new_capacity = set1.size + set2.size;
    ordered_array_set set = ordered_array_set_create(new_capacity);


    size_t i = 0;
    size_t j = 0;
    while (i < set1.size && j < set2.size) {
        if (j == set2.size || set1.data[i] < set2.data[j]) {
            set.data[set.size] = set1.data[i];
            set.size++;
            i++;
        } else if (i == set1.size || set1.data[i] > set2.data[j]) {
            set.data[set.size] = set2.data[j];
            set.size++;
            j++;
        } else {
            set.data[set.size] = set1.data[i];
            set.size++;
            i++;
            j++;
        }
    }

    while (i < set1.size) {
        set.data[set.size] = set1.data[i];
        set.size++;
        i++;
    }

    while (j < set2.size) {
        set.data[set.size] = set2.data[j];
        set.size++;
        j++;
    }

    ordered_array_set_shrinkToFit(&set);

    return set;
}


ordered_array_set ordered_array_set_intersection(ordered_array_set set1, ordered_array_set set2) {
    size_t new_capacity = set1.size < set2.size ? set1.size : set2.size;
    ordered_array_set set = ordered_array_set_create(new_capacity);

    size_t i = 0;
    size_t j = 0;

    while (i != set1.size && j != set2.size) {
        if (set1.data[i] < set2.data[j])
            i++;
        else if (set1.data[i] > set2.data[j])
            j++;
        else {
            set.data[set.size] = set1.data[i];
            set.size++;
            i++;
            j++;
        }
    }

    ordered_array_set_shrinkToFit(&set);

    return set;
}


ordered_array_set ordered_array_set_difference(ordered_array_set set1, ordered_array_set set2) {
    size_t new_capacity = set1.size;
    ordered_array_set set = ordered_array_set_create(new_capacity);

    size_t i = 0;
    size_t j = 0;

    while (i < set1.size) {
        if (j == set2.size || set1.data[i] < set2.data[j]) {
            set.data[set.size] = set1.data[i];
            set.size++;
            i++;
        } else if (set1.data[i] > set2.data[j])
            j++;
        else
            i++;
    }

    ordered_array_set_shrinkToFit(&set);

    return set;
}


ordered_array_set ordered_array_set_complement(ordered_array_set set, ordered_array_set universumSet) {
    size_t new_capacity = universumSet.size;
    ordered_array_set new_set = ordered_array_set_create(new_capacity);

    size_t i = 0, j = 0;
    while (i < universumSet.size) {
        if (j < set.size && universumSet.data[i] == set.data[j]) {
            i++;
            j++;
        } else {
            new_set.data[new_set.size] = universumSet.data[i];
            new_set.size++;
            i++;
        }
    }

    ordered_array_set_shrinkToFit(&new_set);

    assert(ordered_array_set_isSubset(new_set, universumSet));

    return new_set;
}


void ordered_array_set_delete(ordered_array_set* set) {
    free(set -> data);
    set -> data = NULL;

    set -> size = 0;
    set -> capacity = 0;
}


ordered_array_set ordered_array_set_symmetricDifference(ordered_array_set set1, ordered_array_set set2) {
    ordered_array_set universum = ordered_array_set_union(set1, set2);
    ordered_array_set intersection = ordered_array_set_intersection(set1, set2);

    ordered_array_set symmetric = ordered_array_set_complement(intersection, universum);

    ordered_array_set_delete(&intersection);
    ordered_array_set_delete(&universum);

    return symmetric;
}


void ordered_array_set_print(ordered_array_set set) {
    printf("{");
    int is_empty = 1;

    for (size_t i = 0; i < set.size; i++) {
        printf("%d, ", *(set.data + i));
        is_empty = 0;
    }
    if (is_empty)
        printf("}\n");
    else
        printf("\b\b}\n");
}

typedef struct bitset {
    uint32_t values;
    uint32_t max_value;
} bitset;


bitset bitset_create(unsigned max_value) {
    assert(max_value < 32);
    return (bitset) {0, max_value};
}

void bitset_insert(bitset* set, unsigned value) {
    set -> values = (set -> values) | (1 << value);
}


bitset bitset_create_from_array(const unsigned int a[], size_t size, unsigned max_value) {
    assert(size < 32);

    bitset set = bitset_create(max_value);

    for (size_t i = 0; i < size; i++)
        bitset_insert(&set, *(a + i));

    return set;
}


bool bitset_in(bitset set, unsigned value) {
    return set.values & (1 << value);
}

bool bitset_isEqual(bitset set1, bitset set2) {
    return set1.values == set2.values;
}

bool bitset_isSubset(bitset subset, bitset set) {
    return (set.values & subset.values) == subset.values;
}


void bitset_deleteElement(bitset* set, unsigned value) {
    set -> values = (set -> values) & ~(1 << value);
}


bitset bitset_union(bitset set1, bitset set2) {
    assert(set1.max_value == set2.max_value);
    return (bitset) {set1.values | set2.values, set1.max_value};
}


bitset bitset_intersection(bitset set1, bitset set2) {
    assert(set1.max_value == set2.max_value);
    return (bitset) {set1.values & set2.values};
}


bitset bitset_difference(bitset set1, bitset set2) {
    assert(set1.max_value == set2.max_value);
    return (bitset) {set1.values & ~set2.values};
}


bitset bitset_symmetricDifference(bitset set1, bitset set2) {
    assert(set1.max_value == set2.max_value);
    return (bitset) {set1.values ^ set2.values};
}


bitset bitset_complement(bitset set) {
    uint32_t universum = (1 << (set.max_value + 1)) - 1;
    return (bitset) {set.values ^ universum, set.max_value};
}


void bitset_print(bitset set) {
    printf ("{") ;
    int is_empty = 1;

    for (int i = 0; i <= set.max_value; i++) {
        if (bitset_in(set, i)) {
            printf("%d, ", i);
            is_empty = 0;
        }
    }

    if (is_empty)
        printf("}\n");
    else
        printf("\b\b}\n");
}



















int compare(const void * a, const void * b) {
    return ( *(int*)a - *(int*)b );
}

unordered_array_set create_set(char* str, int n) {
    unordered_array_set set;
    set.size = n;
    set.capacity = n;
    set.data = (int*) malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) {
        set.data[i] = str[i] - '0';
    }
    qsort(set.data, set.size, sizeof(int), compare);
    return set;
}

int main() {
    ordered_array_set A = ordered_array_set_create_from_array((int[]) {1, 3, 5, 7}, 4);
    ordered_array_set B = ordered_array_set_create_from_array((int[]) {2, 3, 6, 7}, 4);
    ordered_array_set C = ordered_array_set_create_from_array((int[]) {4, 5, 6, 7}, 4);

    ordered_array_set F = ordered_array_set_difference(A, B);

    ordered_array_set G = ordered_array_set_symmetricDifference(F, B);

    ordered_array_set K = ordered_array_set_difference(G, C);

    ordered_array_set_print(K);

    return 0;
}























