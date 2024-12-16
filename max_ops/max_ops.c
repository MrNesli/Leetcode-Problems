/*
https://leetcode.com/problems/maximum-score-from-performing-multiplication-operations/description/?envType=problem-list-v2&envId=array& 
*/

#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <strings.h>

// #define MAX_PERMUTATIONS 1000

// int compare(const void *a, const void *b) {
//   int a_val = *((int *) a);
//   int b_val = *((int *) b);

//   if (a_val < b_val) return -1;
//   if (a_val == b_val) return 0;
//   if (a_val > b_val) return 1;

//   return 0;
// }
// (0, 0, 0, 0, 0, 0)
// (1, 0, 0, 0, 0, 0)
// (1, 1, 0, 0, 0, 0)
// (1, 1, 1, 0, 0, 0)
// (1, 1, 1, 1, 0, 0)
// (1, 1, 1, 1, 1, 0)
// (1, 1, 1, 1, 1, 1)

// (1, 0, 0, 0, 0, 0)
// (0, 1, 0, 0, 0, 0)
// (0, 0, 1, 0, 0, 0)
// (0, 0, 0, 1, 0, 0)
// (0, 0, 0, 0, 1, 0)
// (0, 0, 0, 0, 0, 1)

// (1, 1, 0, 0, 0, 0)
// (1, 0, 1, 0, 0, 0)
// (1, 0, 0, 1, 0, 0)
// (1, 0, 0, 0, 1, 0)
// (1, 0, 0, 0, 0, 1)
// (0, 1, 1, 0, 0, 0)
// (0, 1, 0, 1, 0, 0)
// (0, 1, 0, 0, 1, 0)
// (0, 1, 0, 0, 0, 1)
// ...
// (0, 0, 0, 0, 1, 1)

// (1, 1, 0, 0)
// (1, 0, 1, 0)
// (1, 0, 0, 1)
// shift_arr()
// (0, 1, 1, 0)
// (0, 1, 0, 1)
// (0, 0, 1, 1)

// (1, 1, 1, 0)
// (1, 1, 0, 1)
// (1, 0, 1, 1)
// (0, 1, 1, 1)

// (0, 0, 0, 0, 0, 0)

/*
start + num_of_last_ops < arr_length

Algorithm:
1. Move current "last" number to the end of the array
2. Get the next "last" number, check if it's not "start", then move it up until another "last" number
3. If it's "start", then you need to shift the initial array's elements to the right and reset the state array to this newly shifted one
   Also, don't forget to reset current "last" number index
4. If we can't move the "last" number to the right anymore (we've reached array's end), then we break out of the loop and return all permutations.

*/

typedef struct Array {
    int *elems;
    size_t length;
} Array;

typedef struct Array2D {
    Array *elems;
    size_t length;
} Array2D;

Array init_array_with(int length, int init_value);
void set_item(Array *arr, int index, int item);
void set_items(Array *arr, int start_index, int end_index, int item);
void append_item(Array *arr, int item);
void append_array(Array2D *arr2d, Array arr);
int get_item(Array *arr, int index);
int* get_array(Array2D *arr2d, int index);
void swap(Array *arr, int index, int swap_index);
int shift_arr(Array *arr, int start, int num_of_last_ops);
void save_permutation(Array permutation, Array2D *permutations);
Array2D get_permutations(int num_of_last_ops, int arr_length);
// void copy(Array *from, Array *into);
void print_arr(Array arr);
void print_2d_arr(Array2D arr);
void copy_arr(Array *dest, Array *src);

void copy_arr(Array *dest, Array *src) {
    *dest = (Array) {0};

    for (int i = 0; i < src->length; i++) {
        append_item(dest, src->elems[i]);
    }
}

Array init_array(int length, int init_value) {
    Array arr = {0};

    for (int i = 0; i < length; i++) {
        append_item(&arr, init_value);
    }

    return arr;
}

// void copy(Array *from, Array *into) {
//     into->elems = (int *) realloc(into->elems, from->length * sizeof(int));

//     memcpy(into->elems, from->elems, sizeof(int *));
// }

void set_items(Array *arr, int start_index, int end_index, int item) {
    if (start_index < 0 || end_index < 0) {
        printf("Index underflow in swap... Indexes: %d, %d... Arr length: %d\n", start_index, end_index, (int) arr->length);
        exit(1);
    }
    else if (start_index >= arr->length || end_index >= arr->length) {
        printf("Index overflow in swap... Indexes: %d, %d... Arr length: %d\n", start_index, end_index, (int) arr->length);
        exit(1);
    }

    if (start_index > end_index) {
        printf("Start index must be lesser than end index");
        exit(1);
    }

    for (int i = start_index; i <= end_index; i++) {
        set_item(arr, i, item);
    }
}

void set_item(Array *arr, int index, int item) {
    if (index >= arr->length) {
        printf("Failed to set item. Array overflow with index %d\n", index);
        exit(1);
    }
    else if (index < 0) {
        printf("Failed to set item. Array underflow with index %d\n", index);
        exit(1);
    }

    arr->elems[index] = item;
}

void append_item(Array *arr, int item) {
    // Updating the length
    arr->length++;

    // Reallocating the memory for the next element
    arr->elems = (int *) realloc(arr->elems, sizeof(int) * arr->length);

    // Appending the new element
    *(arr->elems + (arr->length - 1)) = item;
}

void append_array(Array2D *arr2d, Array arr) {
    // TODO: Fix the thing where it copies a pointer to the array instead of the array itself
    // Copy directly arr pointer's elements into the 2D array

    // Updating the length
    arr2d->length++;

    // Reallocating the memory for the next array
    arr2d->elems = (Array *) realloc(arr2d->elems, sizeof(Array) * arr2d->length);

    // Last array index
    unsigned int last_index = arr2d->length - 1;

    // Copying the array into the 2D array
    copy_arr(arr2d->elems + last_index, &arr);
    // *(arr2d->elems + last_index) = arr;
    // memcpy(arr2d->elems + last_index, &arr, sizeof(Array));

    // print_arr(*arr2d->elems);
}

int get_item(Array *arr, int index) {
    return *(arr->elems + index);
}

int* get_array(Array2D *arr2d, int index) {
    return (*(arr2d->elems + index)).elems;
}

void swap(Array *arr, int index, int swap_index) {
    if (index < 0 || swap_index < 0) {
        printf("Index underflow in swap... Indexes: %d, %d... Arr length: %d\n", index, swap_index, (int) arr->length);
        exit(1);
    }
    else if (index >= arr->length || swap_index >= arr->length) {
        printf("Index overflow in swap... Indexes: %d, %d... Arr length: %d\n", index, swap_index, (int) arr->length);
        exit(1);
    }

    // printf("Swap indexes: %d, %d\n", index, swap_index);
    // printf("Values: %d, %d\n", arr->elems[index], arr->elems[swap_index]);
    int tmp = arr->elems[index];
    arr->elems[index] = arr->elems[swap_index];
    arr->elems[swap_index] = tmp;


}

int shift_arr(Array *arr, int start, int num_of_last_ops) {  
    // If we can't shift the array completely we return 0, otherwise 1
    
    // Examples:
    // (1, 1, 0, 0) - start: 0; num_of_last_ops: 2
    // (0, 1, 1, 0) - start: 1; num_of_last_ops: 2

    if (start + num_of_last_ops >= arr->length) return 0;

    for (int i = start + num_of_last_ops - 1; i >= start; i--) {
        swap(arr, i, i + 1);
    }
    
    return 1;
}

void save_permutation(Array permutation, Array2D *permutations) {
    append_array(permutations, permutation);
}

void print_arr(Array arr) {
    printf("Elements: [");
    for (int i = 0; i < arr.length; i++) {
        if (i == 0) printf("%d", arr.elems[i]);
        else printf(", %d", arr.elems[i]);
    }
    printf("]\n");
}

void print_2d_arr(Array2D arr) {
    printf("2D Elements:\n");
    for (int i = 0; i < arr.length; i++) {
        printf("[");
        for (int j = 0; j < arr.elems[i].length; j++) {
            if (j == 0) printf("%d", arr.elems[i].elems[j]);
            else printf(", %d", arr.elems[i].elems[j]);
        }
        printf("]\n");
    }
    printf("\n");
}

Array2D get_permutations(int num_of_last_ops, int arr_length) {
    
    Array2D permutations = {0};
    Array state_array = init_array(arr_length, 0);
    Array initial_array;

    // Initializing last operations
    // for (int i = 0; i <= num_of_last_ops; i++) {
    //     set_item(&state_array, 0, 1);
    // }
    
    set_items(&state_array, 0, num_of_last_ops - 1, 1);

    // print_arr(state_array);

    copy_arr(&initial_array, &state_array);

    printf("Initial array:\n");
    print_arr(initial_array);
    printf("State array:\n");
    print_arr(state_array);
    printf("\n");

    int start = 0;
    int current_index = start + num_of_last_ops - 1;
    int offset = 1;

    // (1, 1, 0, 0)
    // (1, 0, 1, 0)
    // (1, 0, 0, 1)
    // shift_arr()
    // (0, 1, 1, 0)
    // (0, 1, 0, 1)
    // (0, 0, 1, 1)

    // (1, 1, 1, 0)
    // (1, 1, 0, 1)
    // (1, 0, 1, 1)
    // (0, 1, 1, 1)
    save_permutation(state_array, &permutations);
    while (start + num_of_last_ops < arr_length) {
        // printf("Saving permutation\n");
        // print_arr(state_array);

        if (current_index != start) {
            int index = current_index + offset;
            // printf("Index: %d\n", index);
            if (index < arr_length) {
                swap(&state_array, index - 1, index);
                save_permutation(state_array, &permutations);
                offset++;
            }
            else {
                current_index--;
                offset = 1;
            }
        }

        if(current_index == start) {
            if (!shift_arr(&initial_array, start, num_of_last_ops)) {
                return permutations;
            }
            start += 1;
            current_index = start + num_of_last_ops - 1;
            copy_arr(&state_array, &initial_array);
            save_permutation(state_array, &permutations);
        }
    }

    return permutations;
}

int main(void)
{
    // int n, m;
    // scanf("%d %d", &n, &m);

    // int numbers[n];
    // int multipliers[m];

    // for (int i = 0; i < n; i++) {
    //   scanf("%d", numbers + i);
    // }

    // for (int i = 0; i < m; i++) {
    //   scanf("%d", multipliers + i);
    // }

    // Array test = {0};
    // append_item(&test, 24);
    // append_item(&test, 50);

    // Array2D test2d = {0};
    // append_array(&test2d, &test);
    // append_array(&test2d, &test);
    // append_array(&test2d, &test);

    // print_arr(test);
    //
    Array2D perms = get_permutations(5, 10);

    print_2d_arr(perms);

    return 0;
}
