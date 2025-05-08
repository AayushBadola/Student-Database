

<p align="center">
  ![Static Badge](https://img.shields.io/badge/Author%20%3A%20Aayush)
  </a>
</p>

<h1 align="center">
  Advanced Student Database Management System in C
</h1>

<p align="center">
  A comprehensive, console-based Student Database Management System built with C, featuring robust data handling, dynamic memory management, and persistent storage using CSV files. This project leverages the custom <code>aquant.h</code> library for enhanced input/output and string manipulation.
</p>

<p align="center">
  <img src="https://img.shields.io/badge/Language-C-A8B9CC.svg?style=for-the-badge&logo=c" alt="Language: C"/>
  <img src="https://img.shields.io/badge/C%20Standard-C11%2B%20Recommended-orange?style=for-the-badge" alt="C Standard: C11+ Recommended"/>
  <img src="https://img.shields.io/badge/Platform-Cross--Platform-lightgrey?style=for-the-badge&logo=linux&logoColor=white" alt="Platform: Cross-Platform"/>
  <a href="https://github.com/AayushBadola/Student-Database/blob/main/LICENSE">
    <img src="https://img.shields.io/github/license/AayushBadola/Student-Database?style=for-the-badge&color=blue" alt="License"/>
  </a>
</p>

---

## 📜 Table of Contents

*   [🌟 Features](#-features)
*   [🛠️ Tech Stack](#️-tech-stack)
*   [⚙️ Prerequisites](#️-prerequisites)
*   [🚀 Getting Started](#-getting-started)
    *   [Cloning the Repository](#cloning-the-repository)
    *   [Directory Structure](#directory-structure)
    *   [Compilation](#compilation)
    *   [Running the Application](#running-the-application)
*   [📖 How to Use](#-how-to-use)
    *   [Main Menu](#main-menu)
    *   [Adding a Student](#adding-a-student)
    *   [Displaying All Students](#displaying-all-students)
    *   [Searching Students](#searching-students)
    *   [Updating Student Information](#updating-student-information)
    *   [Deleting a Student](#deleting-a-student)
    *   [Saving Data](#saving-data)
*   [💾 Data Persistence](#-data-persistence)
*   [🏗️ Code Structure & Design](#️-code-structure--design)
    *   [`studentdb.c`](#studentdbc)
    *   [`aquant.h` & `aquant.c`](#aquanth--aquantc)
    *   [Data Structures](#data-structures)
*   [💡 Contributing](#-contributing)
*   [📝 License](#-license)
*   [📞 Contact](#-contact)

---

## 🌟 Features

*   **CRUD Operations**: Full Create, Read, Update, and Delete functionality for student records.
*   **Detailed Student Information**: Stores ID (string), Name, Age, and Major.
*   **Semester-wise Marks Management**:
    *   Add/Update marks for multiple subjects across multiple semesters (up to 4 semesters, 5 subjects per semester).
    *   Marks are stored out of 100.
*   **Robust Input Validation**: Ensures data integrity for all fields, including ID format, age range, marks range, etc.
*   **Advanced Search Capabilities**:
    *   Search by exact Student ID.
    *   Search by Student ID prefix (results sorted in descending order).
    *   Search by mark in a specific subject for a given semester.
*   **Persistent Storage**: Student data, including marks, is saved to and loaded from a CSV file (`students.csv`).
*   **Automatic Save**: Data is auto-saved after most operations to prevent data loss.
*   **Dynamic Memory Management**: Utilizes `malloc`, `realloc`, and `free` (via `aquant.h` wrappers) for string data.
*   **Custom Utility Library (`aquant.h`)**: Leverages a custom library for safer and more convenient input, string operations, and other utilities.
*   **Console-Based Interface**: Clear and interactive command-line menu.

---

## 🛠️ Tech Stack

*   **Language**: C (C11 standard or newer recommended for `snprintf` and other features. Tested with GCC).
*   **Compiler**: GCC (GNU Compiler Collection) is recommended. Clang should also work.
*   **Core Libraries**: Standard C Library (`stdio.h`, `stdlib.h`, `string.h`, `ctype.h`).
*   **Custom Library**: `aquant.h` – A custom C utility library. The `aquant.h` header and `aquant.c` implementation files are included in this repository for convenience. The original library can be found at its own repository: <a href="https://github.com/AayushBadola/Aquant" target="_blank">AayushBadola/Aquant</a>.

---

## ⚙️ Prerequisites

Before you begin, ensure you have the following installed on your system:

1.  **C Compiler**:
    *   **Linux/macOS**: GCC is usually pre-installed or can be installed via package managers (e.g., `sudo apt-get install build-essential` on Debian/Ubuntu, `xcode-select --install` on macOS).
    *   **Windows**:
        *   MinGW-w64 (provides GCC for Windows).
        *   MSYS2 (provides a Unix-like environment with GCC).
        *   Windows Subsystem for Linux (WSL) with GCC installed within the Linux environment.
2.  **`make` (Optional but Recommended)**: For easier compilation, though a direct GCC command is also provided.
3.  **Git**: For cloning the repository.

---

## 🚀 Getting Started

Follow these steps to get the project up and running on your local machine.

### Cloning the Repository

Open your terminal or command prompt and run the following command:

```bash
git clone https://github.com/AayushBadola/Student-Database.git
cd Student-Database
```

This will download the project files into a directory named `Student-Database` and navigate you into it. The necessary `aquant.h` and `aquant.c` files are included directly in this repository.

### Directory Structure

After cloning, your project directory will look something like this:

```
Student-Database/
├── License        
├── Readme.md           
├── aquant.c              # My person C library Check it out - https://github.com/AayushBadola/Aquant
├── aquant.h        
├── studentdb.c             
└── students.csv          # Data file (will be created on first run/save)
```

### Compilation

The project consists of two main C source files (`studentdb.c` and `aquant.c`) that need to be compiled and linked together.

**Using GCC (Recommended)**:

Open your terminal in the `Student-Database` directory and execute the following command:

```bash
gcc studentdb.c aquant.c -o studentdb -lm
```

Let's break down this command:
*   `gcc`: The GNU C Compiler.
*   `studentdb.c aquant.c`: The source files to compile.
*   `-o studentdb`: Specifies the output executable file name as `studentdb` (or `studentdb.exe` on Windows).
*   `-lm`: Links the math library.

**If you encounter errors related to `snprintf` or other C99/C11 features, ensure your compiler supports these standards. You might explicitly specify the standard:**

```bash
gcc -std=c11 studentdb.c aquant.c -o studentdb -lm
```

### Running the Application

Once the compilation is successful, an executable file named `studentdb` (or `studentdb.exe`) will be created in the project directory.

To run the application, execute the following command in your terminal:

*   **Linux/macOS**:
    ```bash
    ./studentdb
    ```
*   **Windows (Command Prompt/PowerShell)**:
    ```bash
    .\studentdb.exe
    ```
    or
    ```bash
    studentdb.exe
    ```

Upon running, the application will first attempt to load data from `students.csv`. If the file doesn't exist (e.g., on the first run), it will inform you and start with an empty database. The `students.csv` file will be created in the same directory when you save data or when auto-save is triggered.

---

## 📖 How to Use

The application presents a menu-driven interface. Enter the number corresponding to the desired action.

### Main Menu

```
--- Student Database Management System ---
1. Add Student (and Marks)
2. Display All Students (Summary)
3. Search Students / View Details
4. Update Student / Marks
5. Delete Student
6. Save Data to File
0. Exit
----------------------------------------
Enter your choice:
```

### Adding a Student

*   Select option `1`.
*   You will be prompted to enter:
    1.  **Student ID**: A string of digits only (e.g., `05817702121`). Must be unique and within `MAX_ID_LENGTH`.
    2.  **Student Name**: Full name of the student.
    3.  **Student Age**: Integer between 5 and 100.
    4.  **Student Major**: The student's major (e.g., "Computer Science").
*   **Adding Marks**:
    *   After basic details, the system will ask if you want to add marks for each semester (1 to `MAX_SEMESTERS`).
    *   For each active semester, you can add multiple subjects (up to `MAX_SUBJECTS_PER_SEMESTER`).
    *   For each subject, enter the **Subject Name** (e.g., "Calculus I") and the **Mark** (0-100).
    *   You can enter 'q' to finish adding subjects for the current semester.

### Displaying All Students

*   Select option `2`.
*   This displays a table summarizing all students, including their ID, Name, Age, Major, and an indicator (`Yes`/`No`) if marks data has been added for them.

### Searching Students

*   Select option `3`.
*   A sub-menu for search options will appear:
    ```
    --- Search Students / View Details ---
    1. Search by ID Prefix (sorted descending, summary)
    2. Search by Exact ID (view full details with marks)
    3. Search by Mark in a Subject
    0. Back to Main Menu
    ```
    *   **Search by ID Prefix**: Enter a starting part of an ID (e.g., `058`). Results will be displayed in a summary table, sorted by the full ID in descending order.
    *   **Search by Exact ID**: Enter the complete, exact student ID. If found, full student details including all recorded marks will be displayed.
    *   **Search by Mark in a Subject**: Prompts for semester number, subject name, and a minimum mark. It then lists students who achieved at least that minimum mark in the specified subject and semester.

### Updating Student Information

*   Select option `4`.
*   Enter the ID of the student you wish to update.
*   If the student is found, a sub-menu appears:
    ```
    What do you want to update?
    1. Name (current: ...)
    2. Age (current: ...)
    3. Major (current: ...)
    4. Marks
    0. Cancel
    ```
    *   Select the field to update and enter the new value.
    *   **Updating Marks**:
        *   Displays current marks for the student.
        *   Prompts for the semester number to update.
        *   If the semester was not previously active, you can activate it.
        *   Provides options to:
            *   Add a new subject and mark to the semester.
            *   Update the mark for an existing subject in that semester.

### Deleting a Student

*   Select option `5`.
*   Enter the ID of the student to delete.
*   A confirmation prompt will appear. Enter `y` to confirm deletion or `n` to cancel.

### Saving Data

*   Select option `6` to manually save all current student data to `students.csv`.
*   The application also **auto-saves** data after most operations (add, update, delete) to minimize data loss.

---

## 💾 Data Persistence

*   All student information, including detailed semester-wise marks, is stored in a CSV (Comma Separated Values) file named `students.csv`.
*   This file is created in the same directory as the executable.
*   **CSV Format**:
    ```csv
    ID,Name,Age,Major,MarksData
    05817702121,Alice Wonderland,20,Computer Science,S1:Math=85,Physics=78;S2:Chemistry=90,Programming=82
    05300202020,Bob The Builder,22,Civil Engineering,S1:Statics=70,Drawing=88
    ...
    ```
    *   The `MarksData` column uses a specific format:
        *   Semester blocks are separated by a semicolon (`;`).
        *   Each semester block starts with `S<semester_number>:` (e.g., `S1:`).
        *   Within a semester block, subject-mark pairs are `SubjectName=Mark`, separated by commas (`,`).
*   The application loads from this file on startup and saves to it manually or automatically.

---

## 🏗️ Code Structure & Design

The project is primarily structured around two C files:

### `studentdb.c`

*   Contains the main application logic:
    *   `main()` function: Program entry point, main loop, menu handling.
    *   Student data management functions (`add_student`, `display_all_students`, `find_student_by_id`, `update_student`, `delete_student`).
    *   Search functions (`search_by_id_prefix_and_sort`, `search_by_exact_id`, `search_by_subject_mark`).
    *   Marks management functions (`add_marks_for_student`, `update_marks_for_student`, `display_marks_for_student`).
    *   File I/O functions (`load_students_from_file`, `save_students_to_file`).
    *   Marks data serialization/deserialization (`format_marks_to_string`, `parse_marks_from_string`).
    *   Memory management helpers (`free_student_marks_memory`, `free_all_student_memory`).
    *   UI display helpers (`print_student_table_header`, `print_student_row`, etc.).

### `aquant.h` & `aquant.c`

*   This is a custom utility library designed to provide more convenient and often safer alternatives to standard C functions. The `aquant.h` and `aquant.c` files for this library are included in this repository for ease of use.
*   The standalone Aquant library project can be found at: <a href="https://github.com/AayushBadola/Aquant" target="_blank">**AayushBadola/Aquant**</a>.
*   **`aquant.h`**: The header file, containing function declarations and type definitions (like `string` as `char*`).
*   **`aquant.c`**: The implementation file for the functions declared in `aquant.h`.
*   **Key functionalities provided by `aquant.h` used in this project**:
    *   **Input Functions**: `get_string()`, `get_string_non_empty()`, `get_int()`, `get_int_range()`, `get_float()`, `get_char()`. These typically handle input validation, re-prompting, and dynamic memory allocation for strings.
    *   **String Manipulation**: `string_copy()`, `string_concat()`, `string_split()`, `string_equals()`, `string_starts_with()`, `string_to_lower()`, `string_is_digit()`, `string_is_empty()`, `free_string()`, `free_string_array()`, etc.
    *   **Memory Management Wrappers**: `free_string` is essentially `free`.
    *   **Utility**: `initialize_random()`.

### Data Structures

*   **`Student` Struct**:
    ```c
    typedef struct {
        string id;
        string name;
        int age;
        string major;
        SemesterMarks semesters_data[MAX_SEMESTERS];
        bool semester_active[MAX_SEMESTERS];
    } Student;
    ```
*   **`SemesterMarks` Struct**:
    ```c
    typedef struct {
        int semester_number;
        SubjectMark subjects[MAX_SUBJECTS_PER_SEMESTER];
        int num_subjects_taken;
    } SemesterMarks;
    ```
*   **`SubjectMark` Struct**:
    ```c
    typedef struct {
        string subject_name;
        int mark;
    } SubjectMark;
    ```
*   **Global Array**: `Student students[MAX_STUDENTS];` stores all student records. `student_count` tracks the current number of students.

**Design Principles**:

*   **Modularity**: Separation of concerns between core database logic (`studentdb.c`) and utility functions (`aquant.c`).
*   **Data Integrity**: Emphasis on input validation and consistent data handling.
*   **Memory Safety**: Careful use of dynamic memory allocation and deallocation, primarily managed through `aquant.h` string functions.
*   **User Experience**: A clear, interactive console menu, though basic.
*   **Persistence**: Simple CSV-based storage for ease of access and human readability (though less robust than binary formats or databases for complex data).

---

## 💡 Contributing

Contributions are welcome! If you have ideas for improvements, new features, or bug fixes, please feel free to:

1.  **Fork the repository**: Click the 'Fork' button at the top right of the [repository page](https://github.com/AayushBadola/Student-Database).
2.  **Create a new branch**:
    ```bash
    git checkout -b feature/your-feature-name
    ```
    or
    ```bash
    git checkout -b bugfix/issue-description
    ```
3.  **Make your changes**: Implement your feature or fix the bug.
4.  **Commit your changes**:
    ```bash
    git commit -m "feat: Describe your new feature"
    ```
    or
    ```bash
    git commit -m "fix: Describe your bug fix"
    ```
5.  **Push to your branch**:
    ```bash
    git push origin feature/your-feature-name
    ```
6.  **Open a Pull Request**: Go to the original repository on GitHub and click 'New pull request'. Provide a clear title and description of your changes.

**Possible areas for future development**:

*   Dynamic resizing of the `students` array instead of `MAX_STUDENTS`.
*   More sophisticated error handling and logging.
*   Advanced reporting features (e.g., class averages, student rankings).
*   More robust CSV parsing (e.g., handling commas within quoted fields, though current design avoids this).
*   Unit tests.
*   GUI implementation (e.g., using GTK, Qt, or a web interface).
*   Transition to a more robust database backend (e.g., SQLite).

---

## 📝 License

This project is licensed under the **MIT License**. See the [LICENSE](https://github.com/AayushBadola/Student-Database/blob/main/LICENSE) file for details.

---

## 📞 Contact

Aayush Badola

*   **Email**: <a href="mailto:aayush.badola2@gmail.com">aayush.badola2@gmail.com</a>
*   **LinkedIn**: <a href="https://www.linkedin.com/in/aayush-badola-0a7b2b343/" target="_blank">linkedin.com/in/aayush-badola</a>
*   **GitHub**: <a href="https://github.com/AayushBadola" target="_blank">github.com/AayushBadola</a>
*   **Project Repository**: <a href="https://github.com/AayushBadola/Student-Database.git" target="_blank">Student-Database.git</a>

<br/>
<p align="center">
  <img src="https://img.shields.io/badge/Made%20with-Love%20%E2%9D%A4%EF%B8%8F-ff69b4?style=for-the-badge" alt="Made with Love"/>
  <img src="https://img.shields.io/badge/Powered%20by-Coffee%20%E2%98%95-lightgrey?style=for-the-badge&logo=buy-me-a-coffee&logoColor=brown" alt="Powered by Coffee"/>
</p>
