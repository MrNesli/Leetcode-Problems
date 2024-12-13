/*
https://leetcode.com/problems/maximum-score-from-performing-multiplication-operations/description/?envType=problem-list-v2&envId=array& 
*/

#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_PERMUTATIONS 1000

int compare(const void *a, const void *b) {
  int a_val = *((int *) a);
  int b_val = *((int *) b);

  if (a_val < b_val) return -1;
  if (a_val == b_val) return 0;
  if (a_val > b_val) return 1;

  return 0;
}
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

void append_item(Array *arr, int item) {
  // Appending the new element
  *(arr->elems + arr->length) = item;

  // Updating the length
  arr->length++;

  // Reallocating the memory for the next element
  arr->elems = (int *) realloc(arr->elems, sizeof(int) * arr->length);

}

void append_array(Array2D *arr2d, Array arr) {
  // TODO: Fix the error with reallocation and see why it's not copying one array into another.
  
  // Appending the array
  // *(arr2d->elems + arr2d->length) = (Array) { (int *) malloc(sizeof(int)), 0 };
  // *(arr2d->elems + arr2d->length) = arr;

  memcpy(arr2d->elems + arr2d->length, &arr, sizeof(int) * arr.length);

  printf("First array length: %ld\n", arr2d->elems[0].length);
  printf("2D Arr length: %ld\n", arr2d->length);
  printf("Arr length: %ld\n", arr.length);

  // array->elems = (int *) malloc((arr.length + 1) * sizeof(int));

  // memcpy(array->elems, arr.elems, arr.length * sizeof(int));

  // Updating the length
  arr2d->length++;

  // // Reallocating the memory for the next array
  arr2d->elems = (Array *) realloc(arr2d->elems, sizeof(Array) * arr2d->length);
}

int get_item(Array *arr, int index) {

}

int* get_array(Array2D *arr2d, int index) {

}

void shift_arr(int *arr, int start, int num_of_last_ops) {

}

void save_permutation(int *permutation, int **permutations) {

}

int** get_permutations(int num_of_last_ops, int arr_length) {
  int **result = (int **) malloc(MAX_PERMUTATIONS * sizeof(int *));
  int *state_array = (int *) calloc(arr_length, sizeof(int));

  // Initializing last operations
  for (int i = 0; i <= num_of_last_ops; i++) {
    state_array[i] = 1;
  }

  int *initial_array;
  memcpy(initial_array, state_array, arr_length * sizeof(int));

  printf("Initial array:\n");
  for (int i = 0; i < arr_length; i++) {
    printf("%d ", initial_array[i]);
  }
  printf("\n");

  int start = 0;
  int init_element_index = num_of_last_ops - 1;
  int current_element_index = init_element_index;
  int offset = 1;

  while (start + num_of_last_ops < arr_length) {


  }
}

void print_arr(Array arr) {
  printf("Array elements: ");
  for (int i = 0; i < arr.length; i++) {
    printf("%d ", arr.elems[i]);
  }
  printf("\n");
}

void print_2d_arr(Array2D arr) {
  printf("2D Array elements:\n");
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

  Array test = { (int *) malloc(sizeof(int)), 0 };
  append_item(&test, 24);
  append_item(&test, 50);

  Array2D test2d = { (Array *) malloc(sizeof(Array)), 0 };
  append_array(&test2d, test);
  append_array(&test2d, test);
  append_array(&test2d, test);

  // print_arr(test);

  print_2d_arr(test2d);

  // int **result = (int **) malloc(MAX_PERMUTATIONS * sizeof(int *));
  // printf("2D Array size: %d\n", (int) sizeof(*(result + 1)));

  return 0;
}
