#include "aquant.h" 
#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <ctype.h>  

#define DATABASE_FILE "students.csv"
#define MAX_STUDENTS 100
#define MAX_ID_LENGTH 25
#define MAX_SUBJECT_NAME_LENGTH 30
#define MAX_SEMESTERS 4
#define MAX_SUBJECTS_PER_SEMESTER 5

typedef struct {
    string subject_name; 
    int mark;            
} SubjectMark;

typedef struct {
    int semester_number; 
    SubjectMark subjects[MAX_SUBJECTS_PER_SEMESTER];
    int num_subjects_taken;
} SemesterMarks;

typedef struct {
    string id;
    string name;
    int age;
    string major;
    SemesterMarks semesters_data[MAX_SEMESTERS];
    bool semester_active[MAX_SEMESTERS];
} Student;

Student students[MAX_STUDENTS];
int student_count = 0;

void display_menu(void);
void add_student(void);
void display_all_students(void);
void display_student_details(const Student *s, bool show_marks);
void search_student_menu(void);
void update_student(void);
void delete_student(void);
bool load_students_from_file(const char *filename);
bool save_students_to_file(const char *filename);
void free_all_student_memory(void);
void free_student_marks_memory(Student *s); 
int find_student_by_id(const string id);

void add_marks_for_student(Student *s);
void update_marks_for_student(Student *s);
void display_marks_for_student(const Student *s);
string format_marks_to_string(const Student *s);
bool parse_marks_from_string(Student *s, const string marks_str);
void initialize_student_marks(Student *s); 

void search_by_id_prefix_and_sort(void);
static int compare_students_by_id_desc(const void *a, const void *b);
void search_by_exact_id(void);
void search_by_subject_mark(void);


void print_student_table_header(bool with_marks_summary);
void print_student_row(const Student *s, bool with_marks_summary);
void print_student_table_footer(void);


int main(void) {
    initialize_random(); 

    if (load_students_from_file(DATABASE_FILE)) {
        printf("Loaded %d student(s) from %s\n", student_count, DATABASE_FILE);
    } else {
        printf("No existing database file found or error loading. Starting fresh.\n");
    }

    int choice;
    do {
        display_menu();
        choice = get_int_range("Enter your choice: ", 0, 6); 

        switch (choice) {
            case 1: add_student(); break;
            case 2: display_all_students(); break;
            case 3: search_student_menu(); break;
            case 4: update_student(); break;
            case 5: delete_student(); break;
            case 6:
                if (save_students_to_file(DATABASE_FILE)) {
                    printf("Data saved to %s successfully.\n", DATABASE_FILE);
                } else {
                    printf("Error saving data to %s.\n", DATABASE_FILE);
                }
                break;
            case 0: printf("Exiting program.\n"); break;
            default: printf("Invalid choice. Please try again.\n"); break;
        }
        if (choice != 0 && choice != 6) { 
            printf("Auto-saving data...\n");
            if (!save_students_to_file(DATABASE_FILE)) {
                fprintf(stderr, "Error: Auto-save failed!\n");
            }
        }
        printf("\n");
    } while (choice != 0);

    free_all_student_memory();
    return 0;
}

void display_menu(void) {
    printf("\n--- Student Database Management System ---\n");
    printf("1. Add Student (and Marks)\n");
    printf("2. Display All Students (Summary)\n");
    printf("3. Search Students / View Details\n");
    printf("4. Update Student / Marks\n");
    printf("5. Delete Student\n");
    printf("6. Save Data to File\n");
    printf("0. Exit\n");
    printf("----------------------------------------\n");
}

void initialize_student_marks(Student *s) {
    for (int i = 0; i < MAX_SEMESTERS; i++) {
        s->semesters_data[i].semester_number = i + 1;
        s->semesters_data[i].num_subjects_taken = 0;
        s->semester_active[i] = false;
        for (int j = 0; j < MAX_SUBJECTS_PER_SEMESTER; j++) {
            s->semesters_data[i].subjects[j].subject_name = NULL;
            s->semesters_data[i].subjects[j].mark = -1; 
        }
    }
}

void print_student_table_header(bool with_marks_summary) {
    printf("-----------------------------------------------------------------------------------\n");
    printf("| %-*s | %-25s | %-3s | %-20s |", MAX_ID_LENGTH, "ID", "Name", "Age", "Major");
    if (with_marks_summary) {
        printf(" %-10s |\n", "Marks Added");
    } else {
        printf("\n"); 
    }
    printf("-----------------------------------------------------------------------------------\n");
}

void print_student_row(const Student *s, bool with_marks_summary) {
    printf("| %-*s | %-25s | %-3d | %-20s |",
           MAX_ID_LENGTH, s->id ? s->id : "N/A",
           s->name ? s->name : "N/A",
           s->age,
           s->major ? s->major : "N/A");
    if (with_marks_summary) {
        bool has_marks = false;
        for(int i=0; i < MAX_SEMESTERS; ++i) {
            if(s->semester_active[i] && s->semesters_data[i].num_subjects_taken > 0) {
                has_marks = true;
                break;
            }
        }
        printf(" %-10s |\n", has_marks ? "Yes" : "No");
    } else {
        printf("\n"); 
    }
}

void print_student_table_footer(void) {
    printf("-----------------------------------------------------------------------------------\n");
}


void add_student(void) {
    if (student_count >= MAX_STUDENTS) {
        printf("Database is full. Cannot add more students.\n");
        return;
    }

    printf("\n--- Add New Student ---\n");
    Student new_student;
    initialize_student_marks(&new_student); 

    new_student.id = NULL; new_student.name = NULL; new_student.major = NULL;

    bool id_ok = false;
    string temp_id = NULL;
    do {
        if(temp_id) free_string(temp_id); 
        temp_id = get_string_non_empty("Enter Student ID (digits only): ");
        if (!string_is_digit(temp_id)) {
            printf("Error: Student ID must contain only digits.\n");
        } else if (strlen(temp_id) == 0) { 
            printf("Error: Student ID cannot be empty.\n");
        } else if (strlen(temp_id) > MAX_ID_LENGTH) {
             printf("Error: Student ID is too long (max %d digits).\n", MAX_ID_LENGTH);
        } else if (find_student_by_id(temp_id) != -1) {
            printf("Error: Student ID '%s' already exists.\n", temp_id);
        } else {
            new_student.id = string_copy(temp_id);
            if (!new_student.id) { fprintf(stderr, "Memory error copying ID.\n"); free_string(temp_id); return;}
            id_ok = true;
        }
    } while (!id_ok);
    if(temp_id) free_string(temp_id);

    new_student.name = get_string_non_empty("Enter Student Name: ");
    new_student.age = get_int_range("Enter Student Age: ", 5, 100);
    new_student.major = get_string_non_empty("Enter Student Major: ");

    printf("\n--- Add Marks for Student %s ---\n", new_student.name);
    add_marks_for_student(&new_student);

    students[student_count] = new_student; 
    student_count++;
    printf("Student %s (ID: %s) added successfully!\n", new_student.name, new_student.id);
}

void add_marks_for_student(Student *s) {
    char choice_char; 
    for (int i = 0; i < MAX_SEMESTERS; i++) {
        printf("Add marks for Semester %d? (y/n): ", i + 1);
        choice_char = get_char(NULL); 
        if (choice_char != 'y' && choice_char != 'Y') {
            s->semester_active[i] = false;
            continue;
        }
        s->semester_active[i] = true;
        s->semesters_data[i].semester_number = i + 1;
        s->semesters_data[i].num_subjects_taken = 0;

        printf("--- Semester %d ---\n", i + 1);
        for (int j = 0; j < MAX_SUBJECTS_PER_SEMESTER; j++) {
            printf("Add subject %d for Semester %d? (y/n, or 'q' to finish semester): ",
                   s->semesters_data[i].num_subjects_taken + 1, i + 1);
            choice_char = get_char(NULL);
            if (choice_char == 'q' || choice_char == 'Q') break;
            if (choice_char != 'y' && choice_char != 'Y') continue;

            string sub_name_temp = NULL;
            do {
                if(sub_name_temp) free_string(sub_name_temp);
                sub_name_temp = get_string_non_empty("Enter Subject Name: ");
                if(strlen(sub_name_temp) > MAX_SUBJECT_NAME_LENGTH) {
                    printf("Subject name too long (max %d chars).\n", MAX_SUBJECT_NAME_LENGTH);
                    free_string(sub_name_temp); 
                    sub_name_temp = NULL; 
                }
            } while (sub_name_temp == NULL); 

            int mark = get_int_range("Enter Mark (0-100): ", 0, 100);

            int current_sub_idx = s->semesters_data[i].num_subjects_taken;
            s->semesters_data[i].subjects[current_sub_idx].subject_name = sub_name_temp; 
            s->semesters_data[i].subjects[current_sub_idx].mark = mark;
            s->semesters_data[i].num_subjects_taken++;
        }
    }
}


void display_all_students(void) {
    printf("\n--- All Students (Summary) --- (%d total)\n", student_count);
    if (student_count == 0) {
        printf("No students in the database.\n");
        return;
    }
    print_student_table_header(true); 
    for (int i = 0; i < student_count; i++) {
        print_student_row(&students[i], true);
    }
    print_student_table_footer();
    printf("To view detailed marks, use the Search option.\n");
}

void display_student_details(const Student *s, bool show_marks_details) {
    if (!s) return;
    printf("\n--- Student Details ---\n");
    printf("ID    : %s\n", s->id ? s->id : "N/A");
    printf("Name  : %s\n", s->name ? s->name : "N/A");
    printf("Age   : %d\n", s->age);
    printf("Major : %s\n", s->major ? s->major : "N/A");

    if (show_marks_details) {
        display_marks_for_student(s);
    }
    printf("-----------------------\n");
}

void display_marks_for_student(const Student *s) {
    printf("--- Marks ---\n");
    bool any_marks_found = false;
    for (int i = 0; i < MAX_SEMESTERS; i++) {
        if (s->semester_active[i] && s->semesters_data[i].num_subjects_taken > 0) {
            any_marks_found = true;
            printf("Semester %d:\n", s->semesters_data[i].semester_number);
            for (int j = 0; j < s->semesters_data[i].num_subjects_taken; j++) {
                printf("  %-*s: %3d\n",
                       MAX_SUBJECT_NAME_LENGTH,
                       s->semesters_data[i].subjects[j].subject_name ? s->semesters_data[i].subjects[j].subject_name : "N/A",
                       s->semesters_data[i].subjects[j].mark);
            }
        } else if (s->semester_active[i]) {
            any_marks_found = true; 
            printf("Semester %d: No subjects recorded.\n", s->semesters_data[i].semester_number);
        }
    }
    if (!any_marks_found) {
        printf("No marks data (or no active semesters with marks) recorded for this student.\n");
    }
}

int find_student_by_id(const string id) {
    if (id == NULL) return -1;
    for (int i = 0; i < student_count; i++) {
        if (students[i].id && string_equals(students[i].id, id)) {
            return i;
        }
    }
    return -1;
}

static int compare_students_by_id_desc(const void *a, const void *b) {
    const Student *student_a = *(const Student **)a;
    const Student *student_b = *(const Student **)b;
    if (student_a->id == NULL && student_b->id == NULL) return 0;
    if (student_a->id == NULL) return 1;
    if (student_b->id == NULL) return -1;
    return strcmp(student_b->id, student_a->id);
}

void search_by_id_prefix_and_sort(void) {
    string prefix_query_raw = get_string_non_empty("Enter ID prefix to search: ");
    if (!string_is_digit(prefix_query_raw) || string_is_empty(prefix_query_raw)) {
        printf("Invalid ID prefix format or empty.\n");
        free_string(prefix_query_raw);
        return;
    }

    Student* matched_students_ptrs[MAX_STUDENTS];
    int match_count = 0;
    for (int i = 0; i < student_count; i++) {
        if (students[i].id && string_starts_with(students[i].id, prefix_query_raw)) {
            if (match_count < MAX_STUDENTS) { 
                 matched_students_ptrs[match_count++] = &students[i];
            } else {
                fprintf(stderr, "Error: Too many matches for temporary buffer.\n"); 
                break;
            }
        }
    }

    if (match_count == 0) {
        printf("No students found with ID starting with '%s'.\n", prefix_query_raw);
    } else {
        printf("\nStudents with ID starting with '%s' (%d found, sorted descending by ID):\n", prefix_query_raw, match_count);
        qsort(matched_students_ptrs, match_count, sizeof(Student*), compare_students_by_id_desc);
        print_student_table_header(true); 
        for (int i = 0; i < match_count; i++) {
            print_student_row(matched_students_ptrs[i], true);
        }
        print_student_table_footer();
    }
    free_string(prefix_query_raw);
}

void search_by_exact_id(void) {
    string id_query = get_string_non_empty("Enter exact Student ID to search: ");
    if (!string_is_digit(id_query)) {
        printf("Invalid ID format: Must contain only digits.\n");
        free_string(id_query);
        return;
    }
    int index = find_student_by_id(id_query);
    if (index != -1) {
        display_student_details(&students[index], true);
    } else {
        printf("Student with ID '%s' not found.\n", id_query);
    }
    free_string(id_query);
}

void search_by_subject_mark(void) {
    if (student_count == 0) { printf("No students to search.\n"); return; }
    int sem_num = get_int_range("Enter Semester Number (1-4): ", 1, MAX_SEMESTERS);
    string subject_query = get_string_non_empty("Enter Subject Name to search: ");
    int min_mark = get_int_range("Enter minimum mark for this subject (0-100): ", 0, 100);

    printf("\nStudents with >= %d in '%s' (Semester %d):\n", min_mark, subject_query, sem_num);
    print_student_table_header(false); 
    bool found = false;
    for(int i = 0; i < student_count; ++i) {
        if (students[i].semester_active[sem_num - 1]) {
            SemesterMarks *sm = &students[i].semesters_data[sem_num - 1];
            for (int j = 0; j < sm->num_subjects_taken; ++j) {
                if (sm->subjects[j].subject_name && string_equals(sm->subjects[j].subject_name, subject_query) && sm->subjects[j].mark >= min_mark) {
                    print_student_row(&students[i], false);
                    found = true;
                    break; 
                }
            }
        }
    }
    if (!found) {
        printf("| No students found matching this criteria.                                        |\n");
    }
    print_student_table_footer();
    free_string(subject_query);
}


void search_student_menu(void) {
    if (student_count == 0) {
        printf("No students in the database to search.\n");
        return;
    }
    printf("\n--- Search Students / View Details ---\n");
    printf("1. Search by ID Prefix (sorted descending, summary)\n");
    printf("2. Search by Exact ID (view full details with marks)\n");
    printf("3. Search by Mark in a Subject\n");
    printf("0. Back to Main Menu\n");

    int search_choice = get_int_range("Enter search type: ", 0, 3);

    switch (search_choice) {
        case 1: search_by_id_prefix_and_sort(); break;
        case 2: search_by_exact_id(); break;
        case 3: search_by_subject_mark(); break;
        case 0: return;
        default: printf("Invalid search choice.\n"); break;
    }
}

void update_marks_for_student(Student *s) {
    printf("\n--- Update Marks for %s (ID: %s) ---\n", s->name, s->id);
    display_marks_for_student(s);

    int sem_choice = get_int_range("Enter Semester number to update (1-4, or 0 to cancel): ", 0, MAX_SEMESTERS);
    if (sem_choice == 0) {
        printf("Mark update cancelled.\n");
        return;
    }
    int sem_idx = sem_choice - 1;

    if (!s->semester_active[sem_idx]) {
        printf("Semester %d was not previously active. Do you want to add marks now? (y/n): ", sem_choice);
        char activate_choice = get_char(NULL);
        if (activate_choice == 'y' || activate_choice == 'Y') {
            s->semester_active[sem_idx] = true;
            s->semesters_data[sem_idx].semester_number = sem_choice; 
            s->semesters_data[sem_idx].num_subjects_taken = 0; 
        } else {
            printf("Mark update for Semester %d cancelled.\n", sem_choice);
            return;
        }
    }

    SemesterMarks *current_sem = &s->semesters_data[sem_idx];
    printf("--- Updating Semester %d ---\n", sem_choice);

    if (current_sem->num_subjects_taken > 0) {
        printf("Current subjects in Semester %d:\n", sem_choice);
        for (int i = 0; i < current_sem->num_subjects_taken; i++) {
            printf("%d. %s (Mark: %d)\n", i + 1, 
                   current_sem->subjects[i].subject_name ? current_sem->subjects[i].subject_name : "N/A", 
                   current_sem->subjects[i].mark);
        }
    } else {
        printf("No subjects currently recorded for Semester %d.\n", sem_choice);
    }

    printf("Options:\n1. Add a new subject and mark\n2. Update mark for an existing subject\n0. Finish updating this semester\n");
    int action = get_int_range("Choose action: ", 0, 2);

    if (action == 1) { 
        if (current_sem->num_subjects_taken >= MAX_SUBJECTS_PER_SEMESTER) {
            printf("Cannot add more subjects to Semester %d (limit: %d).\n", sem_choice, MAX_SUBJECTS_PER_SEMESTER);
        } else {
            string sub_name_temp = NULL;
             do {
                if(sub_name_temp) free_string(sub_name_temp);
                sub_name_temp = get_string_non_empty("Enter New Subject Name: ");
                 if(strlen(sub_name_temp) > MAX_SUBJECT_NAME_LENGTH) {
                    printf("Subject name too long.\n"); free_string(sub_name_temp); sub_name_temp = NULL;
                }
            } while(sub_name_temp == NULL);

            int mark = get_int_range("Enter Mark (0-100): ", 0, 100);
            current_sem->subjects[current_sem->num_subjects_taken].subject_name = sub_name_temp;
            current_sem->subjects[current_sem->num_subjects_taken].mark = mark;
            current_sem->num_subjects_taken++;
            printf("Subject '%s' added to Semester %d.\n", sub_name_temp, sem_choice);
        }
    } else if (action == 2) { 
        if (current_sem->num_subjects_taken == 0) {
            printf("No subjects to update in Semester %d.\n", sem_choice);
        } else {
            string sub_to_update = get_string_non_empty("Enter name of subject to update mark for: ");
            int sub_found_idx = -1;
            for (int i = 0; i < current_sem->num_subjects_taken; i++) {
                if (current_sem->subjects[i].subject_name && string_equals(current_sem->subjects[i].subject_name, sub_to_update)) {
                    sub_found_idx = i;
                    break;
                }
            }
            if (sub_found_idx != -1) {
                int new_mark = get_int_range("Enter new Mark (0-100): ", 0, 100);
                current_sem->subjects[sub_found_idx].mark = new_mark;
                printf("Mark for '%s' in Semester %d updated to %d.\n", sub_to_update, sem_choice, new_mark);
            } else {
                printf("Subject '%s' not found in Semester %d.\n", sub_to_update, sem_choice);
            }
            free_string(sub_to_update);
        }
    }
    printf("Finished updating marks for Semester %d.\n", sem_choice);
}


void update_student(void) {
    if (student_count == 0) {
        printf("No students in the database to update.\n");
        return;
    }
    printf("\n--- Update Student Information ---\n");
    string id_to_update = get_string_non_empty("Enter ID of student to update: ");
    if(!string_is_digit(id_to_update)){
        printf("Invalid ID format. Update cancelled.\n");
        free_string(id_to_update);
        return;
    }
    int index = find_student_by_id(id_to_update);
    

    if (index == -1) {
        printf("Student with ID '%s' not found.\n", id_to_update);
        free_string(id_to_update); 
        return;
    }
    
    free_string(id_to_update); 


    Student *s_to_update = &students[index];
    printf("Student found: %s (ID: %s)\n", s_to_update->name, s_to_update->id);
    printf("What do you want to update?\n");
    printf("1. Name (current: %s)\n", s_to_update->name ? s_to_update->name : "N/A");
    printf("2. Age (current: %d)\n", s_to_update->age);
    printf("3. Major (current: %s)\n", s_to_update->major ? s_to_update->major : "N/A");
    printf("4. Marks\n");
    printf("0. Cancel\n");

    int field_choice = get_int_range("Enter field to update: ", 0, 4);
    switch (field_choice) {
        case 1: {
            string new_name = get_string_non_empty("Enter new Name: ");
            free_string(s_to_update->name); s_to_update->name = new_name; break;
        }
        case 2: s_to_update->age = get_int_range("Enter new Age: ", 5, 100); break;
        case 3: {
            string new_major = get_string_non_empty("Enter new Major: ");
            free_string(s_to_update->major); s_to_update->major = new_major; break;
        }
        case 4: update_marks_for_student(s_to_update); break;
        case 0: printf("Update cancelled.\n"); return;
    }
    if (field_choice != 0 && field_choice != 4) { 
        printf("Student information updated successfully!\n");
    }
}

void delete_student(void) {
    if (student_count == 0) {
        printf("No students in the database to delete.\n");
        return;
    }
    printf("\n--- Delete Student ---\n");
    string id_to_delete = get_string_non_empty("Enter ID of student to delete: ");
     if(!string_is_digit(id_to_delete)){
        printf("Invalid ID format. Deletion cancelled.\n");
        free_string(id_to_delete);
        return;
    }
    int index = find_student_by_id(id_to_delete);

    if (index == -1) {
        printf("Student with ID '%s' not found.\n", id_to_delete);
    } else {
        printf("Are you sure you want to delete student: %s (ID: %s)? ", students[index].name, students[index].id);
        char confirm = get_char("(y/n): ");
        if (confirm == 'y' || confirm == 'Y') {
            free_string(students[index].id);
            free_string(students[index].name);
            free_string(students[index].major);
            free_student_marks_memory(&students[index]);

            for (int i = index; i < student_count - 1; i++) {
                students[i] = students[i + 1];
            }
            if (student_count > 0) { 
                Student *last_student = &students[student_count - 1];
                last_student->id = NULL;
                last_student->name = NULL;
                last_student->major = NULL;
                initialize_student_marks(last_student); 
            }
            student_count--;
            printf("Student deleted successfully.\n");
        } else {
            printf("Deletion cancelled.\n");
        }
    }
    free_string(id_to_delete);
}


string format_marks_to_string(const Student *s) {
    string final_str = string_copy(""); 
    if (!final_str) return NULL;       

    char temp_buffer[256]; 

    for (int i = 0; i < MAX_SEMESTERS; i++) {
        if (s->semester_active[i] && s->semesters_data[i].num_subjects_taken > 0) {
            snprintf(temp_buffer, sizeof(temp_buffer), "S%d:", s->semesters_data[i].semester_number);
            string semester_header_str = string_copy(temp_buffer);
            if (!semester_header_str) {
                free_string(final_str); return NULL;
            }

            string subjects_combined_str = string_copy("");
            if (!subjects_combined_str) {
                free_string(final_str); free_string(semester_header_str); return NULL;
            }

            for (int j = 0; j < s->semesters_data[i].num_subjects_taken; j++) {
                snprintf(temp_buffer, sizeof(temp_buffer), "%s=%d",
                         s->semesters_data[i].subjects[j].subject_name ? s->semesters_data[i].subjects[j].subject_name : "N/A",
                         s->semesters_data[i].subjects[j].mark);
                string subject_entry_str = string_copy(temp_buffer);
                if (!subject_entry_str) {
                    free_string(final_str); free_string(semester_header_str);
                    free_string(subjects_combined_str); return NULL;
                }

                string old_subjects_combined_str = subjects_combined_str;
                if (j > 0) { 
                    subjects_combined_str = string_concat(subjects_combined_str, ",");
                    if (!subjects_combined_str) {
                        free_string(final_str); free_string(semester_header_str);
                        free_string(old_subjects_combined_str); free_string(subject_entry_str);
                        return NULL;
                    }
                    free_string(old_subjects_combined_str); 
                    old_subjects_combined_str = subjects_combined_str; 
                }

                subjects_combined_str = string_concat(subjects_combined_str, subject_entry_str);
                if (!subjects_combined_str) {
                    free_string(final_str); free_string(semester_header_str);
                    free_string(old_subjects_combined_str); free_string(subject_entry_str);
                    return NULL;
                }
                free_string(old_subjects_combined_str); 
                free_string(subject_entry_str);         
            }

            string old_final_str = final_str;
            if (strlen(old_final_str) > 0) { 
                final_str = string_concat(final_str, ";");
                if (!final_str) {
                    free_string(semester_header_str); free_string(subjects_combined_str);
                    free_string(old_final_str); return NULL;
                }
                free_string(old_final_str);
                old_final_str = final_str;
            }

            final_str = string_concat(final_str, semester_header_str);
            if (!final_str) {
                free_string(semester_header_str); free_string(subjects_combined_str);
                free_string(old_final_str); return NULL;
            }
            free_string(old_final_str);
            old_final_str = final_str;

            final_str = string_concat(final_str, subjects_combined_str);
            if (!final_str) {
                free_string(semester_header_str); free_string(subjects_combined_str);
                free_string(old_final_str); return NULL;
            }
            free_string(old_final_str);

            free_string(semester_header_str);
            free_string(subjects_combined_str);
        }
    }
    return final_str;
}


bool parse_marks_from_string(Student *s, const string marks_str) {
    if (string_is_empty(marks_str)) return true; 

    initialize_student_marks(s); 

    size_t num_sem_tokens;
    string *sem_tokens = string_split(marks_str, ';', &num_sem_tokens);
    if (!sem_tokens) return false; 

    for (size_t i = 0; i < num_sem_tokens; i++) {
        if (string_is_empty(sem_tokens[i])) continue;

        size_t num_header_parts;
        string *header_parts = string_split(sem_tokens[i], ':', &num_header_parts);
        if (!header_parts || num_header_parts != 2) {
            fprintf(stderr, "Warning: Malformed semester block in marks data: %s\n", sem_tokens[i]);
            if(header_parts) free_string_array(header_parts, num_header_parts);
            continue;
        }

        if (strlen(header_parts[0]) < 2 || header_parts[0][0] != 'S') {
             fprintf(stderr, "Warning: Malformed semester identifier in marks data: %s\n", header_parts[0]);
             free_string_array(header_parts, num_header_parts);
             continue;
        }
        bool sem_num_ok;
        
        string sem_num_str = string_substring(header_parts[0], 1, strlen(header_parts[0]) - 1);
        if (!sem_num_str) {
            fprintf(stderr, "Memory error parsing semester number string.\n");
            free_string_array(header_parts, num_header_parts);
            continue;
        }
        int sem_num = (int)string_to_double(sem_num_str, &sem_num_ok);
        free_string(sem_num_str);

        if (!sem_num_ok || sem_num < 1 || sem_num > MAX_SEMESTERS) {
            fprintf(stderr, "Warning: Invalid semester number in marks data: %s\n", header_parts[0]);
            free_string_array(header_parts, num_header_parts);
            continue;
        }
        int sem_idx = sem_num - 1;
        s->semester_active[sem_idx] = true;
        s->semesters_data[sem_idx].semester_number = sem_num;
        s->semesters_data[sem_idx].num_subjects_taken = 0;


        if (string_is_empty(header_parts[1])) { 
            free_string_array(header_parts, num_header_parts);
            continue;
        }

        size_t num_subject_tokens;
        string *subject_tokens = string_split(header_parts[1], ',', &num_subject_tokens);
        if (!subject_tokens) {
            free_string_array(header_parts, num_header_parts); 
            continue;
        }

        for (size_t j = 0; j < num_subject_tokens; j++) {
            if (s->semesters_data[sem_idx].num_subjects_taken >= MAX_SUBJECTS_PER_SEMESTER) {
                fprintf(stderr, "Warning: Too many subjects for semester %d in record. Some ignored.\n", sem_num);
                break; 
            }
            if (string_is_empty(subject_tokens[j])) continue;

            size_t num_mark_parts;
            string *mark_parts = string_split(subject_tokens[j], '=', &num_mark_parts);
            if (!mark_parts || num_mark_parts != 2) {
                fprintf(stderr, "Warning: Malformed subject-mark pair: %s\n", subject_tokens[j]);
                if(mark_parts) free_string_array(mark_parts, num_mark_parts);
                continue;
            }

            string sub_name = string_copy(mark_parts[0]);
            if (!sub_name || strlen(sub_name) > MAX_SUBJECT_NAME_LENGTH || string_is_empty(sub_name)) {
                 fprintf(stderr, "Warning: Invalid/long/empty subject name: %s\n", mark_parts[0]);
                 free_string(sub_name); free_string_array(mark_parts, num_mark_parts); continue;
            }

            bool mark_ok;
            int mark_val = (int)string_to_double(mark_parts[1], &mark_ok);
            if (!mark_ok || mark_val < 0 || mark_val > 100) {
                fprintf(stderr, "Warning: Invalid mark for subject %s: %s\n", sub_name, mark_parts[1]);
                free_string(sub_name); free_string_array(mark_parts, num_mark_parts); continue;
            }

            int current_sub_idx = s->semesters_data[sem_idx].num_subjects_taken;
            s->semesters_data[sem_idx].subjects[current_sub_idx].subject_name = sub_name;
            s->semesters_data[sem_idx].subjects[current_sub_idx].mark = mark_val;
            s->semesters_data[sem_idx].num_subjects_taken++;

            free_string_array(mark_parts, num_mark_parts);
        }
        free_string_array(subject_tokens, num_subject_tokens);
        free_string_array(header_parts, num_header_parts);
    }
    free_string_array(sem_tokens, num_sem_tokens);
    return true;
}


bool load_students_from_file(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) return false;

    char line_buffer[1024]; 
    student_count = 0;

    
    if (fgets(line_buffer, sizeof(line_buffer), file) == NULL) { 
        fclose(file); return feof(file); 
    }
    

    while (fgets(line_buffer, sizeof(line_buffer), file) != NULL && student_count < MAX_STUDENTS) {
        line_buffer[strcspn(line_buffer, "\r\n")] = 0; 
        if (string_is_empty(line_buffer)) continue;

        size_t num_tokens;
        string *tokens = string_split(line_buffer, ',', &num_tokens);

        if (tokens == NULL) { fprintf(stderr, "Error splitting line (memory): %s\n", line_buffer); continue; }

        if (num_tokens == 5) { 
            Student s;
            initialize_student_marks(&s); 
            s.id = NULL; s.name = NULL; s.major = NULL; 

            bool success_age_parse, success_age_format;

            
            if (string_is_empty(tokens[0]) || !string_is_digit(tokens[0]) || strlen(tokens[0]) > MAX_ID_LENGTH) {
                fprintf(stderr, "Warning: Invalid ID format/length in line: %s. Skipping.\n", line_buffer);
                free_string_array(tokens, num_tokens); continue;
            }
            s.id = string_copy(tokens[0]);
            if (!s.id) { fprintf(stderr, "Memory allocation failed for student ID: %s. Skipping.\n", line_buffer); free_string_array(tokens, num_tokens); continue;}
            if (find_student_by_id(s.id) != -1) { 
                 fprintf(stderr, "Warning: Duplicate Student ID '%s' found in file. Skipping record: %s\n", s.id, line_buffer);
                 free_string(s.id); free_string_array(tokens, num_tokens); continue;
            }

            
            s.name = string_copy(tokens[1]);
            if(!s.name && tokens[1] && !string_is_empty(tokens[1])) { 
                fprintf(stderr, "Memory allocation failed for student Name: %s. Skipping.\n", line_buffer);
                free_string(s.id); free_string_array(tokens, num_tokens); continue;
            }


            
            s.age = (int)string_to_double(tokens[2], &success_age_parse); 
            success_age_format = string_is_int(tokens[2]);
            if (!success_age_parse || !success_age_format || s.age <= 0) {
                fprintf(stderr, "Warning: Invalid Age format/value in line: %s. Skipping.\n", line_buffer);
                free_string(s.id); free_string(s.name);
                free_string_array(tokens, num_tokens);
                continue;
            }

            
            s.major = string_copy(tokens[3]);
             if(!s.major && tokens[3] && !string_is_empty(tokens[3])) {
                fprintf(stderr, "Memory allocation failed for student Major: %s. Skipping.\n", line_buffer);
                free_string(s.id); free_string(s.name); free_string_array(tokens, num_tokens); continue;
            }

            
            if (!parse_marks_from_string(&s, tokens[4])) {
                fprintf(stderr, "Warning: Error parsing marks for student %s. Marks may be incomplete or missing.\n", s.id);
                
                
                
            }

            students[student_count++] = s; 
        } else {
            fprintf(stderr, "Warning: Malformed line in %s (expected 5 fields, got %zu): %s. Skipping.\n",
                    DATABASE_FILE, num_tokens, line_buffer);
        }
        free_string_array(tokens, num_tokens);
    }
    fclose(file);
    return true;
}

bool save_students_to_file(const char *filename) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        fprintf(stderr, "Error: Could not open file %s for writing.\n", filename);
        return false;
    }
    fprintf(file, "ID,Name,Age,Major,MarksData\n"); 
    for (int i = 0; i < student_count; i++) {
        string marks_data_str = format_marks_to_string(&students[i]);
        if (!marks_data_str) {
            fprintf(stderr, "Error formatting marks for student %s (ID: %s). Data for this student might be lost or incomplete in save.\n", 
                    students[i].name ? students[i].name : "N/A", 
                    students[i].id ? students[i].id : "N/A");
            
            marks_data_str = string_copy(""); 
            if (!marks_data_str) { 
                fprintf(stderr, "CRITICAL: Memory allocation failed for fallback empty marks string. Aborting save for student.\n");
                continue; 
            }
        }

        fprintf(file, "%s,%s,%d,%s,%s\n",
                students[i].id ? students[i].id : "",
                students[i].name ? students[i].name : "",
                students[i].age,
                students[i].major ? students[i].major : "",
                marks_data_str ? marks_data_str : ""); 
        free_string(marks_data_str);
    }
    if (fclose(file) != 0) {
        fprintf(stderr, "Error: Could not properly close file %s after writing.\n", filename);
        return false; 
    }
    return true;
}

void free_student_marks_memory(Student *s) {
    for (int i = 0; i < MAX_SEMESTERS; i++) {
        for (int j = 0; j < s->semesters_data[i].num_subjects_taken; j++) {
            free_string(s->semesters_data[i].subjects[j].subject_name);
            s->semesters_data[i].subjects[j].subject_name = NULL;
        }
        s->semesters_data[i].num_subjects_taken = 0; 
        s->semester_active[i] = false; 
    }
}

void free_all_student_memory(void) {
    for (int i = 0; i < student_count; i++) {
        free_string(students[i].id); students[i].id = NULL;
        free_string(students[i].name); students[i].name = NULL;
        free_string(students[i].major); students[i].major = NULL;
        free_student_marks_memory(&students[i]);
    }
    student_count = 0;
}
