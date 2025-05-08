#ifndef AQUANT_H
#define AQUANT_H

#include <stdbool.h>
#include <stddef.h>
#include <math.h>

// Define string type
typedef char *string;

// Define Epsilon values for float/double comparisons
#define FLOAT_EPSILON 1e-6f
#define DOUBLE_EPSILON 1e-9

// --- Input Functions ---
string get_string(const char *prompt); // Caller must free result
char get_char(const char *prompt);
int get_int(const char *prompt);
long get_long(const char *prompt);
float get_float(const char *prompt);
double get_double(const char *prompt);
int get_int_range(const char *prompt, int min, int max);
string get_string_non_empty(const char *prompt); // Caller must free result

// --- Integer Array Functions ---
bool array_max(const int *arr, size_t size, int *max_val);
bool array_min(const int *arr, size_t size, int *min_val);
bool array_sum(const int *arr, size_t size, long long *sum);
bool array_contains_int(const int *arr, size_t size, int value);
int array_index_of_int(const int *arr, size_t size, int value);
double array_average(const int *arr, size_t size);
size_t array_count_occurrence(const int *arr, size_t size, int value);
int* array_copy_int(const int *arr, size_t size); // Caller must free result
bool array_has_pair_sum(const int *arr, size_t size, int target); // O(n) average
bool array_has_pair_product(const int *arr, size_t size, int target); // O(n) average
bool array_has_pair_difference(const int *arr, size_t size, int target); // O(n) average
void sort_array(int arr[], size_t size); // O(n log n)
void print_array(const int arr[], size_t size);
void array_reverse_int(int arr[], size_t size);
void array_shuffle_int(int arr[], size_t size); // Call initialize_random() once first
int* array_unique_int(const int *arr, size_t size, size_t *new_size); // O(n) average, caller must free result
int* array_concat_int(const int *arr1, size_t size1, const int *arr2, size_t size2, size_t *new_size); // Caller must free result

// --- Float Array Functions ---
bool array_max_float(const float *arr, size_t size, float *max_val);
bool array_min_float(const float *arr, size_t size, float *min_val);
bool array_sum_float(const float *arr, size_t size, double *sum);
double array_average_float(const float *arr, size_t size);
void sort_array_float(float arr[], size_t size); // O(n log n)
bool array_contains_float(const float *arr, size_t size, float value);
int array_index_of_float(const float *arr, size_t size, float value);
size_t array_count_occurrence_float(const float *arr, size_t size, float value);
float* array_copy_float(const float *arr, size_t size); // Caller must free result
void print_float_array(const float arr[], size_t size);
void array_reverse_float(float arr[], size_t size);
void array_shuffle_float(float arr[], size_t size); // Call initialize_random() once first
float* array_concat_float(const float *arr1, size_t size1, const float *arr2, size_t size2, size_t *new_size); // Caller must free result

// --- Double Array Functions ---
bool array_max_double(const double *arr, size_t size, double *max_val);
bool array_min_double(const double *arr, size_t size, double *min_val);
bool array_sum_double(const double *arr, size_t size, double *sum);
double array_average_double(const double *arr, size_t size);
void sort_array_double(double arr[], size_t size); // O(n log n)
bool array_contains_double(const double *arr, size_t size, double value);
int array_index_of_double(const double *arr, size_t size, double value);
size_t array_count_occurrence_double(const double *arr, size_t size, double value);
double* array_copy_double(const double *arr, size_t size); // Caller must free result
void print_double_array(const double arr[], size_t size);
void array_reverse_double(double arr[], size_t size);
void array_shuffle_double(double arr[], size_t size); // Call initialize_random() once first
double* array_concat_double(const double *arr1, size_t size1, const double *arr2, size_t size2, size_t *new_size); // Caller must free result

// --- String Array Functions ---
bool array_max_string(const string *arr, size_t size, string *max_val); // Returns pointer within arr
bool array_min_string(const string *arr, size_t size, string *min_val); // Returns pointer within arr
void sort_array_string(string arr[], size_t size); // O(n log n * L)
bool array_contains_string(const string *arr, size_t size, const string value);
int find_string(const string names[], size_t size, const string target_name); // Alias for index_of_string
size_t array_count_occurrence_string(const string *arr, size_t size, const string value);
string* array_copy_string_array(const string *arr, size_t size); // Deep copy. Caller must free using free_string_array.
void print_string_array(const string arr[], size_t size);
void array_reverse_string(string arr[], size_t size);
void array_shuffle_string(string arr[], size_t size); // Call initialize_random() once first
string* array_concat_string(const string *arr1, size_t size1, const string *arr2, size_t size2, size_t *new_size); // Deep copy. Caller must free using free_string_array.

// --- String Manipulation Functions ---
string string_copy(const string s); // Caller must free result
bool string_equals(const string s1, const string s2);
string string_trim(const string s); // Caller must free result
bool string_is_int(const string s);
bool string_is_alpha(const string s);
bool string_is_digit(const string s);
bool string_is_alnum(const string s);
bool string_is_space(const string s);
bool string_is_empty(const string s);
string string_concat(const string s1, const string s2); // Caller must free result
string string_substring(const string s, size_t start, size_t length); // Caller must free result
int string_find_char(const string s, char c);
int string_find_substring(const string haystack, const string needle);
string string_replace_char(const string s, char old_char, char new_char); // Caller must free result
string string_to_lower(const string s); // Caller must free result
string string_to_upper(const string s); // Caller must free result
string* string_split(const string s, char delimiter, size_t *num_tokens); // Caller must free using free_string_array.
string string_join(const string *arr, size_t size, const string separator); // Caller must free result
bool string_starts_with(const string s, const string prefix);
bool string_ends_with(const string s, const string suffix);
float string_to_float(const string s, bool *success);
double string_to_double(const string s, bool *success);

// --- Memory Management Helpers ---
void free_string(string s); // Frees string allocated by aquant functions
void free_string_array(string *arr, size_t size); // Frees array of strings allocated by aquant functions

// --- Utility Functions ---
void initialize_random(); // Call ONCE at program start if using random functions
int get_random_int(int min, int max);
float get_random_float(float min, float max);
double get_random_double(double min, double max);
void start_timer();
double stop_timer(); // Returns elapsed seconds since start_timer()

#endif // AQUANT_H
