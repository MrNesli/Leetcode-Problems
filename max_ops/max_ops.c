/*
https://leetcode.com/problems/maximum-score-from-performing-multiplication-operations/description/?envType=problem-list-v2&envId=array& 
*/

#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <strings.h>
#include <stdbool.h>

/*
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
void remove_item_at(Array *arr, int index);
int get_item(Array *arr, int index);
int* get_array(Array2D *arr2d, int index);
void swap(Array *arr, int index, int swap_index);
int shift_arr(Array *arr, int start, int num_of_last_ops);
void save_permutation(Array permutation, Array2D *permutations);
Array2D get_permutations(int num_of_last_ops, int arr_length);
void print_arr(Array arr);
void print_2d_arr(Array2D arr);
void copy_arr(Array *dest, Array *src);
void concat2d(Array2D *arr, Array2D *arr_to_concat);
Array convert_pointer_to_arr(int *arr, int arr_length);

void copy_arr(Array *dest, Array *src) {
    *dest = (Array) {0};

    for (int i = 0; i < src->length; i++) {
        append_item(dest, src->elems[i]);
    }
}

Array convert_pointer_to_arr(int *arr, int arr_length) {
    Array new_arr = {0};

    for (int i = 0; i < arr_length; i++) {
        append_item(&new_arr, *(arr + i));
    }

    return new_arr;
}

void remove_item_at(Array *arr, int index) {
    Array new_arr = {0};

    if (index > arr->length) {
        printf("Index overflow removing item... Arr length: %d. Index: %d\n", (int) arr->length, index);
        exit(1);
    }
    else if (index < 0) {
        printf("Index underflow removing item... Arr length: %d. Index: %d\n", (int) arr->length, index);
        exit(1);
    }

    for (int i = 0; i < index; i++) {
        append_item(&new_arr, arr->elems[i]);
    }

    if (index + 1 < arr->length) {
        for (int i = index + 1; i < arr->length; i++) {
            append_item(&new_arr, arr->elems[i]);
        }
    }

    memcpy(arr, &new_arr, sizeof(Array));
}

void concat2d(Array2D *arr, Array2D *arr_to_concat) {
    for (int i = 0; i < arr_to_concat->length; i++) {
        append_array(arr, arr_to_concat->elems[i]);
    }
}

Array init_array(int length, int init_value) {
    Array arr = {0};

    for (int i = 0; i < length; i++) {
        append_item(&arr, init_value);
    }

    return arr;
}

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
    // Updating the length
    arr2d->length++;

    // Reallocating the memory for the next array
    arr2d->elems = (Array *) realloc(arr2d->elems, sizeof(Array) * arr2d->length);

    // Last array index
    unsigned int last_index = arr2d->length - 1;

    // Copying the array into the 2D array
    copy_arr(arr2d->elems + last_index, &arr);
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

    if (num_of_last_ops == 0) {
        append_array(&permutations, state_array);
        return permutations;
    }

    // Initializing last operations
    set_items(&state_array, 0, num_of_last_ops - 1, 1);

    copy_arr(&initial_array, &state_array);

    int start = 0;
    int current_index = start + num_of_last_ops - 1;
    int offset = 1;

    // Saving first permutation 
    save_permutation(state_array, &permutations);

    // Getting all possible permutations... Maybe there's a more elegant way to do this using recursion but I don't know yet how to do this
    while (start + num_of_last_ops < arr_length) {
        if (current_index != start) {
            int index = current_index + offset;

            if (index < arr_length && get_item(&state_array, index) != 1) {
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

int max(int a, int b) {
    return (a > b) ? a : b;
}

int get_max_sum_from_perms(Array2D perms, Array numbers, Array multipliers) {
    bool initialized = false;
    int max_sum;

    for (int i = 0; i < perms.length; i++) {
        Array current_arr;
        Array current_numbers;
        copy_arr(&current_arr, perms.elems + i);
        copy_arr(&current_numbers, &numbers);
        int current_sum = 0;

        printf("Combination %d:\n", i);
        print_arr(current_arr);
        print_arr(current_numbers);
        for (int j = 0; j < multipliers.length; j++) {
            if (get_item(&current_arr, j) == 0) {
                // printf("First: %d * %d = %d\n", get_item(&current_numbers, 0), get_item(&multipliers, j), get_item(&current_numbers, 0) * get_item(&multipliers, j));

                current_sum += (get_item(&current_numbers, 0) * get_item(&multipliers, j));
                remove_item_at(&current_numbers, 0);
            }
            else if (get_item(&current_arr, j) == 1) {
                // printf("Last: %d * %d = %d\n", get_item(&current_numbers, current_numbers.length - 1), get_item(&multipliers, j), get_item(&current_numbers, current_numbers.length - 1) * get_item(&multipliers, j));

                current_sum += (get_item(&current_numbers, current_numbers.length - 1) * get_item(&multipliers, j));
                remove_item_at(&current_numbers, current_numbers.length - 1);
            }
        }

        printf("Sum: %d\n\n", current_sum);

        if (!initialized) {
            max_sum = current_sum;
            initialized = true;
        }
        else if (current_sum > max_sum) {
            max_sum = current_sum;
        }
    }

    return max_sum;
}

int calc_score(int *nums, int numsSize, int *mults, int multsSize, int start, int end) {
    // if (start > multsSize) {

    // }

}

int maximumScore(int* nums, int numsSize, int* multipliers, int multipliersSize) {
    // Recursive way to approach this problem.
    // Say we want to start calculating the sum starting from the first and the last element at the same time
    // on each recursive iteration we calculate the sum of both sides. And if I understand recursion correctly it should get through every
    // single possible permutation there
    // 40 % 40 = 0
    // 40 - 1 = 39; 40 % 39 = 1
    // score(0 + 1); score(numsSize - 1)
    // 0, 1, 1, 1, 0
    // (5 - 1) - (0 - 0) = 4
    // (5 - 1) - (1 - 0) = 3

    // Array numbers = convert_pointer_to_arr(nums, numsSize);
    // Array mults = convert_pointer_to_arr(multipliers, multipliersSize);

    // Array2D perms = {0};

    // for (int i = 0; i <= multipliersSize; i++) {
    //     Array2D current_perms = get_permutations(i, multipliersSize);
    //     concat2d(&perms, &current_perms);
    // }

    // printf("Permutations: %d\n", (int) perms.length);

    // // Getting the result max sum
    // // int result = get_max_sum_from_perms(perms, numbers, mults);
    // int result = 0;

    // printf("Result: %d\n", result);

    // return result;
}

int main(void)
{
    // int n, m;
    // scanf("%d %d", &n, &m);

    // Array numbers = {0};
    // Array multipliers = {0};

    // for (int i = 0; i < n; i++) {
    //     int num;
    //     scanf("%d", &num);

    //     append_item(&numbers, num);
    // }

    // for (int i = 0; i < m; i++) {
    //     int num;
    //     scanf("%d", &num);

    //     append_item(&multipliers, num);
    // }

    // // Calculating all possible permutations
    // Array2D perms = {0};

    // for (int i = 0; i <= m; i++) {
    //     Array2D current_perms = get_permutations(i, m);
    //     concat2d(&perms, &current_perms);
    // }

    // // Getting the result max sum
    // int result = get_max_sum_from_perms(perms, numbers, multipliers);

    // printf("Result: %d\n", result);
    int numbers[148] = {830,388,289,14,-221,610,163,444,-750,-217,-235,-882,746,-907,67,-71,-990,400,-54,-114,-100,233,667,612,-812,-471,-639,-306,-95,524,-198,-520,-652,704,-697,-43,-539,-105,-160,838,499,-109,-977,975,658,-250,593,154,-777,496,747,307,-340,403,-749,-185,721,327,-851,-112,770,-14,754,-242,-220,-776,-66,348,-707,-693,9,-354,741,129,234,638,404,-408,-30,422,-234,818,-627,706,-752,-699,-773,-786,241,432,273,272,-57,878,-149,-547,491,519,870,700,476,-99,902,-878,-237,-549,445,-372,277,-486,872,-569,-687,339,653,-564,862,898,-962,306,668,-143,344,-312,108,-536,-453,-52,627,-225,-28,403,-422,367,133,970,-575,353,347,275,-876,337,594,441,-498,-875,-934,133 };
                
    int multipliers[98] = { -844,-363,797,480,141,733,-935,842,160,-928,787,-895,-742,-963,889,-713,-264,-400,117,163,68,-286,-810,-365,180,-690,-558,-409,290,51,-272,-454,-110,850,578,131,-913,675,817,380,410,860,-441,56,-80,-650,-474,858,269, -844,-363,797,480,141,733,-935,842,160,-928,787,-895,-742,-963,889,-713,-264,-400,117,163,68,-286,-810,-365,180,-690,-558,-409,290,51,-272,-454,-110,850,578,131,-913,675,817,380,410,860,-441,56,-80,-650,-474,858,269 };

    maximumScore(numbers, 148, multipliers, 98);

    return 0;
}
