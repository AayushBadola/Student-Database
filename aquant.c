#include "aquant.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include <float.h>
#include <errno.h>
#include <math.h>
#include <stdint.h>
#include <time.h>

// Define a small epsilon for float/double comparisons
#define FLOAT_EPSILON 1e-6f
#define DOUBLE_EPSILON 1e-9

// Assuming string is char*
typedef char *string;


// --- Original Input Functions ---
// ... (code for get_string, get_char, etc. - unchanged) ...
string get_string(const char *prompt)
{
    if (prompt != NULL)
    {
        printf("%s", prompt);
        fflush(stdout);
    }

    char *buffer = NULL;
    size_t capacity = 0;
    size_t size = 0;
    int c;

    while ((c = fgetc(stdin)) != '\n' && c != EOF)
    {
        if (size + 1 > capacity)
        {
            size_t new_capacity = (capacity == 0) ? 32 : capacity + (capacity / 2);
            if (new_capacity < size + 1) new_capacity = size + 1;
            if (new_capacity == capacity) new_capacity++;

            char *temp = realloc(buffer, new_capacity);
            if (temp == NULL)
            {
                if (buffer != NULL) {
                   buffer[size] = '\0';
                }
                return buffer; // Return NULL if nothing read, or partial string on realloc fail
            }
            buffer = temp;
            capacity = new_capacity;
        }
        buffer[size++] = (char) c;
    }

    if (size == 0 && (c == '\n' || c == EOF))
    {
        char *empty_buffer = realloc(buffer, 1);
        if (empty_buffer == NULL) {
            free(buffer);
            return NULL;
        }
        empty_buffer[0] = '\0';
        return empty_buffer; // Return empty string "" or NULL on malloc fail
    }

    char *final_buffer = realloc(buffer, size + 1);
    if (final_buffer == NULL) {
        if (buffer != NULL) {
            buffer[size] = '\0';
        }
        return buffer; // Return partial string or NULL on realloc fail
    }

    final_buffer[size] = '\0';
    return final_buffer; // Caller must free
}


char get_char(const char *prompt)
{
    const char *current_prompt = prompt;
    while (1)
    {
        string line = get_string(current_prompt);
        if (line == NULL)
        {
             fprintf(stderr, "\nInput error or EOF. Please try again.\n");
             current_prompt = "Retry: ";
             continue;
        }

        if (line[0] != '\0' && line[1] == '\0')
        {
            char c = line[0];
            free(line);
            return c;
        }

        free(line);
        printf("Invalid input. Please enter exactly one character.\n");
        current_prompt = "Retry: ";
    }
}


int get_int(const char *prompt)
{
    const char *current_prompt = prompt;
    while (1)
    {
        string line = get_string(current_prompt);
        if (line == NULL) {
             fprintf(stderr, "\nInput error or EOF. Please try again.\n");
             current_prompt = "Retry: ";
             continue;
        }

        char *endptr;
        errno = 0;
        long n = strtol(line, &endptr, 10);

        if (endptr == line || errno == ERANGE || n < INT_MIN || n > INT_MAX) {
             free(line);
             printf("Invalid input or out of range. Please enter an integer.\n");
             current_prompt = "Retry: ";
             continue;
        }

        char *check_ptr = endptr;
        while (isspace((unsigned char)*check_ptr)) check_ptr++;
        if (*check_ptr != '\0') {
             free(line);
             printf("Invalid input. Please enter only an integer.\n");
             current_prompt = "Retry: ";
             continue;
        }

        free(line);
        return (int) n;
    }
}


long get_long(const char *prompt)
{
    const char *current_prompt = prompt;
    while (1)
    {
        string line = get_string(current_prompt);
         if (line == NULL) {
             fprintf(stderr, "\nInput error or EOF. Please try again.\n");
             current_prompt = "Retry: ";
             continue;
        }

        char *endptr;
        errno = 0;
        long n = strtol(line, &endptr, 10);

        if (endptr == line || errno == ERANGE) {
            free(line);
             printf("Invalid input or out of range. Please enter a long integer.\n");
             current_prompt = "Retry: ";
             continue;
        }

        char *check_ptr = endptr;
        while (isspace((unsigned char)*check_ptr)) check_ptr++;
        if (*check_ptr != '\0') {
             free(line);
             printf("Invalid input. Please enter only a long integer.\n");
             current_prompt = "Retry: ";
             continue;
        }

        free(line);
        return n;
    }
}


float get_float(const char *prompt)
{
    const char *current_prompt = prompt;
    while (1)
    {
        string line = get_string(current_prompt);
        if (line == NULL) {
             fprintf(stderr, "\nInput error or EOF. Please try again.\n");
             current_prompt = "Retry: ";
             continue;
        }

        char *endptr;
        errno = 0;
        float f = strtof(line, &endptr);

        if (endptr == line || (errno == ERANGE && (f == HUGE_VALF || f == -HUGE_VALF || f == 0))) {
            free(line);
             printf("Invalid input or out of range. Please enter a floating-point number.\n");
             current_prompt = "Retry: ";
             continue;
        }

        char *check_ptr = endptr;
        while (isspace((unsigned char)*check_ptr)) check_ptr++;
        if (*check_ptr != '\0') {
             free(line);
             printf("Invalid input. Please enter only a floating-point number.\n");
             current_prompt = "Retry: ";
             continue;
        }

        free(line);
        return f;
    }
}


double get_double(const char *prompt)
{
    const char *current_prompt = prompt;
     while (1)
    {
        string line = get_string(current_prompt);
        if (line == NULL) {
             fprintf(stderr, "\nInput error or EOF. Please try again.\n");
             current_prompt = "Retry: ";
             continue;
        }

        char *endptr;
        errno = 0;
        double d = strtod(line, &endptr);

        if (endptr == line || (errno == ERANGE && (d == HUGE_VAL || d == -HUGE_VAL || d == 0))) {
            free(line);
            printf("Invalid input or out of range. Please enter a double-precision number.\n");
            current_prompt = "Retry: ";
             continue;
        }

        char *check_ptr = endptr;
        while (isspace((unsigned char)*check_ptr)) check_ptr++;
         if (*check_ptr != '\0') {
             free(line);
             printf("Invalid input. Please enter only a double-precision number.\n");
             current_prompt = "Retry: ";
             continue;
        }

        free(line);
        return d;
    }
}


// --- Original Integer Array Functions ---
// ... (array_max, array_min, array_sum, etc. - unchanged) ...
bool array_max(const int *arr, size_t size, int *max_val) {
    if (arr == NULL || size == 0 || max_val == NULL) return false;
    *max_val = arr[0];
    for (size_t i = 1; i < size; ++i) {
        if (arr[i] > *max_val) *max_val = arr[i];
    }
    return true;
}

bool array_min(const int *arr, size_t size, int *min_val) {
    if (arr == NULL || size == 0 || min_val == NULL) return false;
    *min_val = arr[0];
    for (size_t i = 1; i < size; ++i) {
        if (arr[i] < *min_val) *min_val = arr[i];
    }
    return true;
}

bool array_sum(const int *arr, size_t size, long long *sum) {
    if (sum == NULL) return false;
    if (arr == NULL || size == 0) { *sum = 0; return true; }
    *sum = 0;
    for (size_t i = 0; i < size; ++i) { *sum += arr[i]; }
    return true;
}

bool array_contains_int(const int *arr, size_t size, int value) {
    if (arr == NULL || size == 0) return false;
    for (size_t i = 0; i < size; ++i) {
        if (arr[i] == value) return true;
    }
    return false;
}

int array_index_of_int(const int *arr, size_t size, int value) {
    if (arr == NULL || size == 0) return -1;
    for (size_t i = 0; i < size; ++i) {
        if (arr[i] == value) return (i > INT_MAX) ? -1 : (int)i; // Check index fits int
    }
    return -1;
}

double array_average(const int *arr, size_t size) {
    if (arr == NULL || size == 0) return NAN;
    long long sum;
    if (!array_sum(arr, size, &sum)) return NAN;
    return (double)sum / size;
}

size_t array_count_occurrence(const int *arr, size_t size, int value) {
    if (arr == NULL || size == 0) return 0;
    size_t count = 0;
    for (size_t i = 0; i < size; ++i) {
        if (arr[i] == value) count++;
    }
    return count;
}

int* array_copy_int(const int *arr, size_t size) {
    if (arr == NULL || size == 0) return NULL;
    int *copy = malloc(size * sizeof(int));
    if (copy == NULL) return NULL; // Allocation failed
    memcpy(copy, arr, size * sizeof(int));
    return copy; // Caller must free
}


// --- Hash Table Helper Functions ---
// ... (ht_create, ht_destroy, ht_search, ht_insert, ht_contains_key - unchanged) ...
typedef struct HashNode {
    int key;
    int count;
    struct HashNode *next;
} HashNode;

typedef struct HashTable {
    HashNode **buckets;
    size_t table_size;
} HashTable;

static size_t hash_func(int key, size_t table_size) {
    return (size_t)llabs((long long)key) % table_size;
}

static HashTable* ht_create(size_t initial_size) {
    if (initial_size < 7) initial_size = 101;
    HashTable *ht = malloc(sizeof(HashTable));
    if (!ht) return NULL;
    ht->buckets = calloc(initial_size, sizeof(HashNode*));
    if (!ht->buckets) { free(ht); return NULL; }
    ht->table_size = initial_size;
    return ht;
}

static void ht_destroy(HashTable *ht) {
    if (!ht) return;
    for (size_t i = 0; i < ht->table_size; ++i) {
        HashNode *current = ht->buckets[i];
        while (current) {
            HashNode *temp = current;
            current = current->next;
            free(temp);
        }
    }
    free(ht->buckets);
    free(ht);
}

static HashNode* ht_search(HashTable *ht, int key) {
    if (!ht) return NULL;
    size_t index = hash_func(key, ht->table_size);
    HashNode *current = ht->buckets[index];
    while (current) {
        if (current->key == key) return current;
        current = current->next;
    }
    return NULL;
}

static bool ht_insert(HashTable *ht, int key) {
    if (!ht) return false;
    size_t index = hash_func(key, ht->table_size);
    HashNode *current = ht->buckets[index];
    while (current) {
        if (current->key == key) { current->count++; return true; }
        current = current->next;
    }
    HashNode *new_node = malloc(sizeof(HashNode));
    if (!new_node) return false;
    new_node->key = key;
    new_node->count = 1;
    new_node->next = ht->buckets[index];
    ht->buckets[index] = new_node;
    return true;
}

static bool ht_contains_key(HashTable *ht, int key) {
    return ht_search(ht, key) != NULL;
}


// --- Pair Functions ---
// ... (array_has_pair_sum, array_has_pair_product, array_has_pair_difference - unchanged) ...
bool array_has_pair_sum(const int *arr, size_t size, int target) {
    if (arr == NULL || size < 2) return false;
    HashTable *ht = ht_create(size);
    if (!ht) return false;
    bool found = false;
    for (size_t i = 0; i < size; ++i) {
        long long complement_ll = (long long)target - arr[i];
        if (complement_ll >= INT_MIN && complement_ll <= INT_MAX) {
             if (ht_contains_key(ht, (int)complement_ll)) {
                  found = true;
                  break;
             }
        }
        ht_insert(ht, arr[i]);
    }
    ht_destroy(ht);
    return found;
}


bool array_has_pair_product(const int *arr, size_t size, int target) {
    if (arr == NULL || size < (target == 0 ? 1 : 2)) return false;
    if (target != 0 && size < 2) return false; // Ensure size >= 2 for non-zero target

    HashTable *ht = ht_create(size);
    if (!ht) return false;

    bool found = false;
    for (size_t i = 0; i < size; ++i) {
         int current_val = arr[i];

         if (target == 0) {
             if (current_val == 0) {
                 // Need another element (zero or non-zero) if target is 0. Size check handles this.
                 if (size > 1) { found = true; break; }
             } else if (ht_contains_key(ht, 0)) { // If non-zero found a zero previously
                  found = true; break;
             }
         } else { // target != 0, current_val must be non-zero
              if (current_val != 0 && target % current_val == 0) {
                 long long needed_ll = (long long)target / current_val;
                 if (needed_ll >= INT_MIN && needed_ll <= INT_MAX) {
                     if (ht_contains_key(ht, (int)needed_ll)) {
                          found = true;
                          break;
                     }
                 }
              }
         }
         ht_insert(ht, current_val);
    }

    ht_destroy(ht);
    return found;
}

bool array_has_pair_difference(const int *arr, size_t size, int target) {
    if (arr == NULL || size < 1) return false;
    int abs_target = (target < 0) ? -target : target;

    // Need size >= 2 for non-zero difference with distinct indices
    if (abs_target != 0 && size < 2) return false;

    HashTable *ht = ht_create(size);
    if (!ht) return false;
    bool found = false;

    for (size_t i = 0; i < size; ++i) {
        int current_val = arr[i];
        long long needed1_ll = (long long)current_val - abs_target;
        long long needed2_ll = (long long)current_val + abs_target;

        if ((needed1_ll >= INT_MIN && needed1_ll <= INT_MAX && ht_contains_key(ht, (int)needed1_ll)) ||
            (needed2_ll >= INT_MIN && needed2_ll <= INT_MAX && ht_contains_key(ht, (int)needed2_ll))) {
             found = true;
             break;
        }
        ht_insert(ht, current_val);
    }

    ht_destroy(ht);
    return found;
}


// --- Sort and Print ---
// ... (compare_int, sort_array, print_array - unchanged) ...
static int compare_int(const void *a, const void *b) {
    return (*(const int*)a - *(const int*)b);
}

// O(n log n) time complexity
void sort_array(int arr[], size_t size) {
    if (arr == NULL || size < 2) return;
    qsort(arr, size, sizeof(int), compare_int);
}

void print_array(const int arr[], size_t size) {
     if (arr == NULL) { printf("[]\n"); return; }
     printf("[");
     for (size_t i = 0; i < size; ++i) {
         printf("%d", arr[i]);
         if (i < size - 1) printf(", ");
     }
     printf("]\n");
}


// --- Original String Functions ---
// ... (find_string, string_copy, string_equals, string_trim, string_is_int, free_string_array - unchanged) ...
int find_string(const string names[], size_t size, const string target_name) {
    if (names == NULL || target_name == NULL) return -1;
    for (size_t i = 0; i < size; ++i) {
        if (string_equals(names[i], target_name)) return (i > INT_MAX) ? -1 : (int)i;
    }
    return -1;
}

// O(L) time. Caller must free.
string string_copy(const string s) {
    if (s == NULL) return NULL;
    size_t len = strlen(s);
    string copy = malloc(len + 1);
    if (copy == NULL) return NULL;
    memcpy(copy, s, len + 1);
    return copy;
}

// O(L) time.
bool string_equals(const string s1, const string s2) {
    if (s1 == s2) return true;
    if (s1 == NULL || s2 == NULL) return false;
    return strcmp(s1, s2) == 0;
}

// O(L) time. Caller must free.
string string_trim(const string s) {
    if (s == NULL) return NULL;
    const char *start = s;
    while (*start != '\0' && isspace((unsigned char)*start)) start++;
    if (*start == '\0') { string empty_s = malloc(1); if(empty_s) *empty_s = '\0'; return empty_s;}
    const char *end = s + strlen(s) - 1;
    while (end > start && isspace((unsigned char)*end)) end--;
    size_t trimmed_len = end - start + 1;
    string trimmed_s = malloc(trimmed_len + 1);
    if (trimmed_s == NULL) return NULL;
    memcpy(trimmed_s, start, trimmed_len);
    trimmed_s[trimmed_len] = '\0';
    return trimmed_s;
}

// O(L) time.
bool string_is_int(const string s) {
    if (s == NULL || *s == '\0') return false;
    char *endptr;
    errno = 0;
    (void)strtol(s, &endptr, 10);
    if (endptr == s) return false; // No digits found
    while (isspace((unsigned char)*endptr)) endptr++;
    return *endptr == '\0'; // Only whitespace or null remaining
}

// O(n * L) time (deep free). Caller must free array AND strings.
void free_string_array(string *arr, size_t size) {
    if (arr == NULL) return;
    for (size_t i = 0; i < size; ++i) free(arr[i]); // Free each string
    free(arr); // Free the array of pointers
}


// ----------------------------------------------------------------------------
//                         NEW FUNCTIONS START HERE
// ----------------------------------------------------------------------------

// --- Array Functions for Float ---
// ... (all float array functions - unchanged) ...
bool array_max_float(const float *arr, size_t size, float *max_val) {
    if (arr == NULL || size == 0 || max_val == NULL) return false;
    *max_val = arr[0];
    for (size_t i = 1; i < size; ++i) if (arr[i] > *max_val) *max_val = arr[i];
    return true;
}

bool array_min_float(const float *arr, size_t size, float *min_val) {
    if (arr == NULL || size == 0 || min_val == NULL) return false;
    *min_val = arr[0];
    for (size_t i = 1; i < size; ++i) if (arr[i] < *min_val) *min_val = arr[i];
    return true;
}

bool array_sum_float(const float *arr, size_t size, double *sum) {
    if (sum == NULL) return false;
    if (arr == NULL || size == 0) { *sum = 0.0; return true; }
    *sum = 0.0;
    for (size_t i = 0; i < size; ++i) *sum += arr[i];
    return true;
}

double array_average_float(const float *arr, size_t size) {
    if (arr == NULL || size == 0) return NAN;
    double sum;
    if (!array_sum_float(arr, size, &sum)) return NAN;
    return sum / size;
}

static int compare_float(const void *a, const void *b) {
    float fa = *(const float*)a; float fb = *(const float*)b;
    if (fa < fb) return -1; if (fa > fb) return 1; return 0;
}
// O(n log n) time
void sort_array_float(float arr[], size_t size) {
    if (arr == NULL || size < 2) return;
    qsort(arr, size, sizeof(float), compare_float);
}

// Uses FLOAT_EPSILON
bool array_contains_float(const float *arr, size_t size, float value) {
    if (arr == NULL || size == 0) return false;
    for (size_t i = 0; i < size; ++i) if (fabs(arr[i] - value) < FLOAT_EPSILON) return true;
    return false;
}

// Uses FLOAT_EPSILON. Returns -1 or index (if fits int).
int array_index_of_float(const float *arr, size_t size, float value) {
    if (arr == NULL || size == 0) return -1;
    for (size_t i = 0; i < size; ++i) if (fabs(arr[i] - value) < FLOAT_EPSILON) return (i > INT_MAX) ? -1 : (int)i;
    return -1;
}

// Uses FLOAT_EPSILON
size_t array_count_occurrence_float(const float *arr, size_t size, float value) {
    if (arr == NULL || size == 0) return 0;
    size_t count = 0;
    for (size_t i = 0; i < size; ++i) if (fabs(arr[i] - value) < FLOAT_EPSILON) count++;
    return count;
}

// Caller must free.
float* array_copy_float(const float *arr, size_t size) {
    if (arr == NULL || size == 0) return NULL;
    float *copy = malloc(size * sizeof(float));
    if (copy == NULL) return NULL;
    memcpy(copy, arr, size * sizeof(float));
    return copy;
}


// --- Array Functions for Double ---
// ... (all double array functions - unchanged) ...
bool array_max_double(const double *arr, size_t size, double *max_val) {
    if (arr == NULL || size == 0 || max_val == NULL) return false;
    *max_val = arr[0];
    for (size_t i = 1; i < size; ++i) if (arr[i] > *max_val) *max_val = arr[i];
    return true;
}

bool array_min_double(const double *arr, size_t size, double *min_val) {
    if (arr == NULL || size == 0 || min_val == NULL) return false;
    *min_val = arr[0];
    for (size_t i = 1; i < size; ++i) if (arr[i] < *min_val) *min_val = arr[i];
    return true;
}

bool array_sum_double(const double *arr, size_t size, double *sum) {
    if (sum == NULL) return false;
    if (arr == NULL || size == 0) { *sum = 0.0; return true; }
    *sum = 0.0;
    for (size_t i = 0; i < size; ++i) *sum += arr[i];
    return true;
}

double array_average_double(const double *arr, size_t size) {
    if (arr == NULL || size == 0) return NAN;
    double sum;
    if (!array_sum_double(arr, size, &sum)) return NAN;
    return sum / size;
}

static int compare_double(const void *a, const void *b) {
    double da = *(const double*)a; double db = *(const double*)b;
    if (da < db) return -1; if (da > db) return 1; return 0;
}
// O(n log n) time
void sort_array_double(double arr[], size_t size) {
     if (arr == NULL || size < 2) return;
    qsort(arr, size, sizeof(double), compare_double);
}

// Uses DOUBLE_EPSILON
bool array_contains_double(const double *arr, size_t size, double value) {
    if (arr == NULL || size == 0) return false;
    for (size_t i = 0; i < size; ++i) if (fabs(arr[i] - value) < DOUBLE_EPSILON) return true;
    return false;
}

// Uses DOUBLE_EPSILON. Returns -1 or index (if fits int).
int array_index_of_double(const double *arr, size_t size, double value) {
    if (arr == NULL || size == 0) return -1;
    for (size_t i = 0; i < size; ++i) if (fabs(arr[i] - value) < DOUBLE_EPSILON) return (i > INT_MAX) ? -1 : (int)i;
    return -1;
}

// Uses DOUBLE_EPSILON
size_t array_count_occurrence_double(const double *arr, size_t size, double value) {
    if (arr == NULL || size == 0) return 0;
    size_t count = 0;
    for (size_t i = 0; i < size; ++i) if (fabs(arr[i] - value) < DOUBLE_EPSILON) count++;
    return count;
}

// Caller must free.
double* array_copy_double(const double *arr, size_t size) {
    if (arr == NULL || size == 0) return NULL;
    double *copy = malloc(size * sizeof(double));
    if (copy == NULL) return NULL;
    memcpy(copy, arr, size * sizeof(double));
    return copy;
}


// --- Array Functions for String ---
// ... (array_max_string, array_min_string, compare_string, sort_array_string, array_contains_string - unchanged) ...
// *** ADDED MISSING DEFINITION HERE ***
// O(n * L) time, where L is string length.
size_t array_count_occurrence_string(const string *arr, size_t size, const string value) {
    if (arr == NULL || size == 0) {
        return 0;
    }
    size_t count = 0;
    for (size_t i = 0; i < size; ++i) {
         if (string_equals(arr[i], value)) { // Handles NULLs correctly
            count++;
        }
    }
    return count;
}
// ... (array_copy_string_array, print_string_array, etc. - unchanged) ...
bool array_max_string(const string *arr, size_t size, string *max_val) {
    if (arr == NULL || size == 0 || max_val == NULL) return false;
    *max_val = NULL; bool found_first = false;
    for (size_t i = 0; i < size; ++i) {
        if (arr[i] != NULL) {
            if (!found_first) { *max_val = arr[i]; found_first = true;}
            else if (strcmp(arr[i], *max_val) > 0) *max_val = arr[i];
        }
    } return true; // Returns true even if all were NULL (max_val will be NULL)
}

// O(n * L) time. Returns pointer within the array (can be NULL).
bool array_min_string(const string *arr, size_t size, string *min_val) {
    if (arr == NULL || size == 0 || min_val == NULL) return false;
    *min_val = NULL; bool found_first = false;
     for (size_t i = 0; i < size; ++i) {
         if (arr[i] == NULL) { *min_val = NULL; found_first = true;} // NULL is the minimum
         else if (!found_first) { *min_val = arr[i]; found_first = true; }
         else if (*min_val != NULL && strcmp(arr[i], *min_val) < 0) *min_val = arr[i];
     } return true;
}

static int compare_string(const void *a, const void *b) {
    const string *sa = (const string*)a; const string *sb = (const string*)b;
    if (*sa == NULL && *sb == NULL) return 0;
    if (*sa == NULL) return -1; if (*sb == NULL) return 1;
    return strcmp(*sa, *sb);
}
// O(n log n * L) time
void sort_array_string(string arr[], size_t size) {
     if (arr == NULL || size < 2) return;
    qsort(arr, size, sizeof(string), compare_string);
}

// O(n * L) time. Handles NULLs.
bool array_contains_string(const string *arr, size_t size, const string value) {
    if (arr == NULL || size == 0) return false;
    for (size_t i = 0; i < size; ++i) if (string_equals(arr[i], value)) return true;
    return false;
}

// O(n * L) time (deep copy). Caller must free using free_string_array.
string* array_copy_string_array(const string *arr, size_t size) {
    if (arr == NULL || size == 0) return NULL;
    string *copy = malloc(size * sizeof(string));
    if (copy == NULL) return NULL; // Allocation failed
    for (size_t i = 0; i < size; ++i) {
        copy[i] = string_copy(arr ? arr[i] : NULL); // Deep copy string (handles NULL source)
        if (copy[i] == NULL && (arr ? arr[i] : NULL) != NULL) { // Failed copy of non-NULL
             for(size_t j=0; j<i; ++j) free(copy[j]); free(copy); return NULL; // Cleanup and fail
        }
    } return copy;
}


// --- More Array Manipulation Functions ---
// ... (array_reverse_int, array_shuffle_int, array_unique_int, array_concat_int, etc. - unchanged) ...
void array_reverse_int(int arr[], size_t size) {
    if (arr == NULL || size < 2) return;
    size_t left = 0; size_t right = size - 1;
    while (left < right) {
        int temp = arr[left]; arr[left] = arr[right]; arr[right] = temp;
        left++; right--;
    }
}
void array_reverse_float(float arr[], size_t size) {
    if (arr == NULL || size < 2) return;
    size_t left = 0; size_t right = size - 1;
    while (left < right) {
        float temp = arr[left]; arr[left] = arr[right]; arr[right] = temp;
        left++; right--;
    }
}
void array_reverse_double(double arr[], size_t size) {
     if (arr == NULL || size < 2) return;
    size_t left = 0; size_t right = size - 1;
    while (left < right) {
        double temp = arr[left]; arr[left] = arr[right]; arr[right] = temp;
        left++; right--;
    }
}
void array_reverse_string(string arr[], size_t size) {
     if (arr == NULL || size < 2) return;
    size_t left = 0; size_t right = size - 1;
    while (left < right) {
        string temp = arr[left]; arr[left] = arr[right]; arr[right] = temp;
        left++; right--;
    }
}

// Needs srand() called once at program start. O(n) time.
void array_shuffle_int(int arr[], size_t size) {
    if (arr == NULL || size < 2) return;
    for (size_t i = size - 1; i > 0; --i) {
        size_t j = (size_t)rand() % (i + 1); // Random index from 0 to i
        int temp = arr[i]; arr[i] = arr[j]; arr[j] = temp; // Swap
    }
}
// Needs srand() called once at program start. O(n) time.
void array_shuffle_float(float arr[], size_t size) {
    if (arr == NULL || size < 2) return;
    for (size_t i = size - 1; i > 0; --i) { size_t j = (size_t)rand() % (i + 1); float temp = arr[i]; arr[i] = arr[j]; arr[j] = temp;}
}
// Needs srand() called once at program start. O(n) time.
void array_shuffle_double(double arr[], size_t size) {
     if (arr == NULL || size < 2) return;
    for (size_t i = size - 1; i > 0; --i) { size_t j = (size_t)rand() % (i + 1); double temp = arr[i]; arr[i] = arr[j]; arr[j] = temp;}
}
// Needs srand() called once at program start. O(n) time.
void array_shuffle_string(string arr[], size_t size) {
     if (arr == NULL || size < 2) return;
    for (size_t i = size - 1; i > 0; --i) { size_t j = (size_t)rand() % (i + 1); string temp = arr[i]; arr[i] = arr[j]; arr[j] = temp;}
}

// O(n) average time, O(n) space. Caller must free returned array.
int* array_unique_int(const int *arr, size_t size, size_t *new_size) {
    if (new_size == NULL) return NULL;
    if (arr == NULL || size == 0) { *new_size = 0; return NULL; }
    HashTable *ht = ht_create(size);
    if (!ht) { *new_size = 0; return NULL; } // Allocation failed
    for (size_t i = 0; i < size; ++i) ht_insert(ht, arr[i]); // Populate counts
    size_t unique_count = 0;
    for (size_t i = 0; i < ht->table_size; ++i) { HashNode *current = ht->buckets[i]; while(current) { unique_count++; current = current->next;}}
    *new_size = unique_count;
    if (unique_count == 0) { ht_destroy(ht); return NULL; }
    int *unique_arr = malloc(unique_count * sizeof(int));
    if (unique_arr == NULL) { ht_destroy(ht); *new_size = 0; return NULL; } // Allocation failed
    size_t k = 0;
    for (size_t i = 0; i < ht->table_size; ++i) {
        HashNode *current = ht->buckets[i];
        while(current) { unique_arr[k++] = current->key; current = current->next;}
    }
    ht_destroy(ht);
    return unique_arr; // Caller must free
}

// O(size1 + size2) time. Caller must free.
int* array_concat_int(const int *arr1, size_t size1, const int *arr2, size_t size2, size_t *new_size) {
    if (new_size == NULL) return NULL;
    size_t total_size = size1 + size2; *new_size = total_size;
    if (total_size == 0) return NULL; // Cannot create empty array
    int *concat_arr = malloc(total_size * sizeof(int));
    if (concat_arr == NULL) { *new_size = 0; return NULL;} // Allocation failed
    if (arr1 && size1 > 0) memcpy(concat_arr, arr1, size1 * sizeof(int));
    if (arr2 && size2 > 0) memcpy(concat_arr + size1, arr2, size2 * sizeof(int));
    return concat_arr; // Caller must free
}

// O(size1 + size2) time. Caller must free.
float* array_concat_float(const float *arr1, size_t size1, const float *arr2, size_t size2, size_t *new_size) {
    if (new_size == NULL) return NULL;
    size_t total_size = size1 + size2; *new_size = total_size;
    if (total_size == 0) return NULL;
    float *concat_arr = malloc(total_size * sizeof(float));
    if (concat_arr == NULL) { *new_size = 0; return NULL;}
    if (arr1 && size1 > 0) memcpy(concat_arr, arr1, size1 * sizeof(float));
    if (arr2 && size2 > 0) memcpy(concat_arr + size1, arr2, size2 * sizeof(float));
    return concat_arr; // Caller must free
}

// O(size1 + size2) time. Caller must free.
double* array_concat_double(const double *arr1, size_t size1, const double *arr2, size_t size2, size_t *new_size) {
    if (new_size == NULL) return NULL;
    size_t total_size = size1 + size2; *new_size = total_size;
    if (total_size == 0) return NULL;
    double *concat_arr = malloc(total_size * sizeof(double));
    if (concat_arr == NULL) { *new_size = 0; return NULL;}
    if (arr1 && size1 > 0) memcpy(concat_arr, arr1, size1 * sizeof(double));
    if (arr2 && size2 > 0) memcpy(concat_arr + size1, arr2, size2 * sizeof(double));
    return concat_arr; // Caller must free
}

// O((size1 + size2) * L) time (deep copy). Caller must free using free_string_array.
string* array_concat_string(const string *arr1, size_t size1, const string *arr2, size_t size2, size_t *new_size) {
    if (new_size == NULL) return NULL;
    size_t total_size = size1 + size2; *new_size = total_size;
    if (total_size == 0) return NULL;
    string *concat_arr = malloc(total_size * sizeof(string));
    if (concat_arr == NULL) { *new_size = 0; return NULL;}
    for(size_t i=0; i < size1; ++i) { concat_arr[i] = string_copy(arr1 ? arr1[i] : NULL); if (concat_arr[i] == NULL && (arr1 ? arr1[i] : NULL) != NULL) { for(size_t k=0; k<i; ++k) free(concat_arr[k]); free(concat_arr); *new_size=0; return NULL;} } // Copy arr1, handle fail
    for(size_t i=0; i < size2; ++i) { concat_arr[size1 + i] = string_copy(arr2 ? arr2[i] : NULL); if (concat_arr[size1+i] == NULL && (arr2 ? arr2[i] : NULL) != NULL) { for(size_t k=0; k<(size1+i); ++k) free(concat_arr[k]); free(concat_arr); *new_size=0; return NULL;} } // Copy arr2, handle fail
    return concat_arr; // Caller must free using free_string_array
}


// --- More String Manipulation Functions ---
// ... (string_concat, string_substring, string_find_char, etc. - unchanged) ...
// O(L1 + L2) time. Caller must free.
string string_concat(const string s1, const string s2) {
    if (s1 == NULL && s2 == NULL) return NULL;
    size_t len1 = (s1 == NULL) ? 0 : strlen(s1);
    size_t len2 = (s2 == NULL) ? 0 : strlen(s2);
    size_t total_len = len1 + len2;
    string new_s = malloc(total_len + 1);
    if (new_s == NULL) return NULL; // Allocation failed
    if (s1 != NULL) memcpy(new_s, s1, len1);
    if (s2 != NULL) memcpy(new_s + len1, s2, len2 + 1);
    else new_s[total_len] = '\0';
    return new_s; // Caller must free
}

// O(L) time. Caller must free.
string string_substring(const string s, size_t start, size_t length) {
    if (s == NULL) return NULL; size_t s_len = strlen(s);
    if (start > s_len) start = s_len; if (start + length > s_len) length = s_len - start;
    if (length == 0) { string empty_s = malloc(1); if (empty_s) *empty_s = '\0'; return empty_s; } // Return "" or NULL
    string sub_s = malloc(length + 1);
    if (sub_s == NULL) return NULL; // Allocation failed
    memcpy(sub_s, s + start, length); sub_s[length] = '\0';
    return sub_s; // Caller must free
}

// O(L) time. Returns index or -1.
int string_find_char(const string s, char c) {
    if (s == NULL) return -1;
    const char *ptr = strchr(s, c);
    if (ptr == NULL) return -1; // Not found
    size_t index = ptr - s; return (index > INT_MAX) ? -1 : (int)index; // Check index fits int
}

// O(L_haystack * L_needle) naive, typically optimized. Returns index or -1.
int string_find_substring(const string haystack, const string needle) {
    if (haystack == NULL || needle == NULL) return -1;
    if (*needle == '\0') return 0; // Empty needle found at start
    const char *ptr = strstr(haystack, needle);
    if (ptr == NULL) return -1; // Not found
    size_t index = ptr - haystack; return (index > INT_MAX) ? -1 : (int)index;
}

// O(L) time. Caller must free.
string string_replace_char(const string s, char old_char, char new_char) {
    if (s == NULL) return NULL;
    string new_s = string_copy(s); // Start with a copy
    if (new_s == NULL) return NULL;
    for (size_t i = 0; new_s[i] != '\0'; ++i) { if (new_s[i] == old_char) new_s[i] = new_char; }
    return new_s; // Caller must free
}

// O(L) time. Caller must free.
string string_to_lower(const string s) {
    if (s == NULL) return NULL; size_t len = strlen(s);
    string new_s = malloc(len + 1); if (new_s == NULL) return NULL;
    for (size_t i = 0; i <= len; ++i) new_s[i] = (char)tolower((unsigned char)s[i]);
    return new_s; // Caller must free
}

// O(L) time. Caller must free.
string string_to_upper(const string s) {
    if (s == NULL) return NULL; size_t len = strlen(s);
    string new_s = malloc(len + 1); if (new_s == NULL) return NULL;
    for (size_t i = 0; i <= len; ++i) new_s[i] = (char)toupper((unsigned char)s[i]);
    return new_s; // Caller must free
}

// O(L) time. Caller must free returned array AND strings using free_string_array.
string* string_split(const string s, char delimiter, size_t *num_tokens) {
    if (s == NULL || num_tokens == NULL) { if(num_tokens) *num_tokens = 0; return NULL; }
    size_t s_len = strlen(s); size_t estimated_tokens = 1;
    for (size_t i = 0; i < s_len; ++i) if (s[i] == delimiter) estimated_tokens++;

    string* tokens = malloc(estimated_tokens * sizeof(string));
    if (tokens == NULL) { *num_tokens = 0; return NULL; } // Allocation failed

    size_t token_count = 0; const char *current_pos = s; const char *next_delimiter;
    while ((next_delimiter = strchr(current_pos, delimiter)) != NULL) {
        size_t token_len = next_delimiter - current_pos;
        tokens[token_count] = malloc(token_len + 1);
        if (tokens[token_count] == NULL) { for(size_t i=0; i<token_count; ++i) free(tokens[i]); free(tokens); *num_tokens = 0; return NULL; } // Token malloc fail
        memcpy(tokens[token_count], current_pos, token_len); tokens[token_count][token_len] = '\0';
        token_count++; current_pos = next_delimiter + 1;
    }
    size_t last_token_len = strlen(current_pos);
    tokens[token_count] = malloc(last_token_len + 1);
     if (tokens[token_count] == NULL) { for(size_t i=0; i<token_count; ++i) free(tokens[i]); free(tokens); *num_tokens = 0; return NULL; } // Last token malloc fail
    memcpy(tokens[token_count], current_pos, last_token_len); tokens[token_count][last_token_len] = '\0';
    token_count++;

    *num_tokens = token_count;
    if (token_count < estimated_tokens) { string *temp = realloc(tokens, token_count * sizeof(string)); if(temp != NULL) tokens = temp;}
    return tokens; // Caller must free using free_string_array
}

// O(n * L + n*S) time (S is separator length). Caller must free.
string string_join(const string *arr, size_t size, const string separator) {
    if (arr == NULL || size == 0) { string empty_s = malloc(1); if(empty_s) *empty_s = '\0'; return empty_s; } // Return "" or NULL
    size_t total_len = 0; size_t sep_len = (separator == NULL) ? 0 : strlen(separator);
    for (size_t i = 0; i < size; ++i) { if (arr[i] != NULL) total_len += strlen(arr[i]); if (i < size - 1) total_len += sep_len;}
    string result_s = malloc(total_len + 1);
    if (result_s == NULL) return NULL; // Allocation failed

    size_t current_pos = 0;
    for (size_t i = 0; i < size; ++i) {
        if (arr[i] != NULL) { size_t len = strlen(arr[i]); memcpy(result_s + current_pos, arr[i], len); current_pos += len;}
        if (i < size - 1 && separator != NULL) { memcpy(result_s + current_pos, separator, sep_len); current_pos += sep_len;}
    } result_s[total_len] = '\0';
    return result_s; // Caller must free
}

// O(min(L, P)) time.
bool string_starts_with(const string s, const string prefix) {
    if (s == NULL || prefix == NULL) return false;
    if (*prefix == '\0') return true; // Starts with empty prefix
    return strncmp(s, prefix, strlen(prefix)) == 0;
}

// O(min(L, S)) time.
bool string_ends_with(const string s, const string suffix) {
    if (s == NULL || suffix == NULL) return false;
    size_t s_len = strlen(s); size_t suffix_len = strlen(suffix);
    if (suffix_len == 0) return true; // Ends with empty suffix
    if (suffix_len > s_len) return false;
    return strcmp(s + s_len - suffix_len, suffix) == 0;
}

// O(1) time.
bool string_is_empty(const string s) {
    return s == NULL || *s == '\0';
}

// O(L) time. Uses local pointer copy.
bool string_is_alpha(const string s) {
    if (s == NULL || *s == '\0') return false;
    const char *p = s; // Use local pointer copy
    while (*p) {
        if (!isalpha((unsigned char)*p)) return false;
        p++; // Increment copy
    }
    return true;
}
// O(L) time. Uses local pointer copy.
bool string_is_digit(const string s) {
     if (s == NULL || *s == '\0') return false;
    const char *p = s; // Use local pointer copy
    while (*p) {
        if (!isdigit((unsigned char)*p)) return false;
        p++; // Increment copy
    }
    return true;
}
// O(L) time. Uses local pointer copy.
bool string_is_alnum(const string s) {
     if (s == NULL || *s == '\0') return false;
    const char *p = s; // Use local pointer copy
    while (*p) {
        if (!isalnum((unsigned char)*p)) return false;
        p++; // Increment copy
    }
    return true;
}
// O(L) time. Uses local pointer copy.
bool string_is_space(const string s) {
     if (s == NULL || *s == '\0') return false;
    const char *p = s; // Use local pointer copy
    while (*p) {
        if (!isspace((unsigned char)*p)) return false;
        p++; // Increment copy
    }
    return true;
}

// O(L) time. Use success flag to check conversion.
float string_to_float(const string s, bool *success) {
    if (s == NULL || *s == '\0') { if (success) *success = false; return 0.0f; }
    char *endptr; errno = 0; float f = strtof(s, &endptr);
    if (endptr == s || (errno == ERANGE && (f == HUGE_VALF || f == -HUGE_VALF || f == 0))) { if (success) *success = false; return 0.0f; }
    char *check_ptr = endptr; while (isspace((unsigned char)*check_ptr)) check_ptr++;
    if (*check_ptr != '\0') { if (success) *success = false; return 0.0f; }
    if (success) *success = true; return f;
}

// O(L) time. Use success flag to check conversion.
double string_to_double(const string s, bool *success) {
     if (s == NULL || *s == '\0') { if (success) *success = false; return 0.0; }
    char *endptr; errno = 0; double d = strtod(s, &endptr);
    if (endptr == s || (errno == ERANGE && (d == HUGE_VAL || d == -HUGE_VAL || d == 0))) { if (success) *success = false; return 0.0; }
    char *check_ptr = endptr; while (isspace((unsigned char)*check_ptr)) check_ptr++;
    if (*check_ptr != '\0') { if (success) *success = false; return 0.0; }
    if (success) *success = true; return d;
}


// --- More Input/Output Functions ---
// ... (get_int_range, get_string_non_empty, print_float_array, etc. - unchanged) ...
int get_int_range(const char *prompt, int min, int max) {
    const char *current_prompt = prompt;
    while(1) {
        int value = get_int(current_prompt);
        if (value >= min && value <= max) return value;
        printf("Input out of range. Please enter an integer between %d and %d.\n", min, max);
        current_prompt = "Retry: ";
    }
}

// O(retry_count * L) time. Loops until non-empty input. Caller must free.
string get_string_non_empty(const char *prompt) {
    const char *current_prompt = prompt;
    while(1) {
        string s = get_string(current_prompt);
         if (s == NULL) { fprintf(stderr, "\nInput error or EOF. Please try again.\n"); current_prompt = "Retry: "; continue; }
         if (*s != '\0') return s; // Return non-empty string
        free(s); printf("Input cannot be empty. Please enter text.\n"); current_prompt = "Retry: ";
    }
}

// O(n) time.
void print_float_array(const float arr[], size_t size) {
    if (arr == NULL) { printf("[]\n"); return; }
    printf("[");
    for (size_t i = 0; i < size; ++i) {
        printf("%.6f", arr[i]); // Default precision
        if (i < size - 1) printf(", ");
    }
    printf("]\n");
}

// O(n) time.
void print_double_array(const double arr[], size_t size) {
    if (arr == NULL) { printf("[]\n"); return; }
    printf("[");
    for (size_t i = 0; i < size; ++i) {
        printf("%.12f", arr[i]); // Default precision
        if (i < size - 1) printf(", ");
    }
    printf("]\n");
}

// O(n * L) time.
void print_string_array(const string arr[], size_t size) {
     if (arr == NULL) { printf("[null]\n"); return; }
     printf("[");
     for (size_t i = 0; i < size; ++i) {
         if (arr[i] == NULL) printf("null");
         else printf("\"%s\"", arr[i]); // Quoted strings
         if (i < size - 1) printf(", ");
     }
     printf("]\n");
}


// --- Memory Management Helpers ---
// ... (free_string, free_string_array - unchanged) ...
void free_string(string s) {
    free(s);
}


// --- Utility Functions ---
// ... (initialize_random, get_random_*, start/stop_timer - unchanged) ...
void initialize_random() {
    srand((unsigned int)time(NULL));
}

// O(1) time. Random integer in [min, max] (inclusive).
int get_random_int(int min, int max) {
    if (min > max) { int temp = min; min = max; max = temp; }
    return min + (rand() % (max - min + 1));
}
// O(1) time. Random float in [min, max].
float get_random_float(float min, float max) {
     if (min > max) { float temp = min; min = max; max = temp; }
    return min + ((float)rand() / (float)RAND_MAX) * (max - min);
}
// O(1) time. Random double in [min, max].
double get_random_double(double min, double max) {
    if (min > max) { double temp = min; min = max; max = temp; }
     return min + ((double)rand() / (double)RAND_MAX) * (max - min);
}


static clock_t aquant_timer_start_time; // Internal timer state

// O(1) time. Starts a timer.
void start_timer() {
    aquant_timer_start_time = clock();
}

// O(1) time. Stops timer and returns elapsed time in seconds.
double stop_timer() {
    clock_t end_time = clock();
    return (double)(end_time - aquant_timer_start_time) / CLOCKS_PER_SEC;
}
