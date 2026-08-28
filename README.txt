# Study Tracker

A lightweight command-line task manager built with **C++17** and designed around object-oriented programming principles.

Study Tracker helps users manage study tasks with priorities, completion status, and optional deadlines. Tasks are persisted locally in a text file, allowing them to survive between program executions.

---

## Features

* Create and manage study tasks
* Assign **High**, **Medium**, or **Low** priority
* Track task completion status
* Add optional deadlines
* Edit existing tasks
* Delete tasks with confirmation
* Toggle completion status
* Persistent local storage using `tasks.txt`
* Input validation and error handling
* Basic protection against corrupted task data
* Simple and interactive console interface

---

## Demo

```text
============================================================
                     STUDY TRACKER
============================================================
1. Show My Tasks
2. Add a New Task
3. Delete a Task
4. Toggle Task Status
5. Edit a Task
6. Exit
============================================================
Your choice:
```

Example task:

```text
1. Study Algorithms
   Priority: High
   Status: Incomplete
   Deadline: 2026-09-01
------------------------------------------------------------
```

---

## Tech Stack

| Technology    | Purpose                       |
| ------------- | ----------------------------- |
| **C++17**     | Application development       |
| **STL**       | Data structures and utilities |
| `std::vector` | In-memory task storage        |
| `fstream`     | Persistent file storage       |
| `regex`       | Deadline format validation    |
| `enum class`  | Type-safe task priorities     |

---

## Project Architecture

The application is organized around two primary classes:

```text
┌──────────────────────┐
│      Priority        │
│   HIGH / MED / LOW   │
└──────────┬───────────┘
           │
           ▼
┌──────────────────────┐
│        Task          │
│                      │
│ - title              │
│ - priority           │
│ - completed          │
│ - deadline           │
└──────────┬───────────┘
           │
           ▼
┌──────────────────────┐
│     TaskManager      │
│                      │
│ - task collection    │
│ - user interaction   │
│ - validation         │
│ - CRUD operations    │
│ - persistence        │
└──────────┬───────────┘
           │
           ▼
      ┌───────────┐
      │ tasks.txt │
      └───────────┘
```

### `Priority`

An `enum class` representing the three available priority levels:

```cpp
enum class Priority
{
    HIGH = 1,
    MEDIUM = 2,
    LOW = 3
};
```

### `Task`

Represents an individual study task.

A task contains:

* `title`
* `priority`
* `completed`
* `deadline`

The attributes are private and accessed through public member functions, providing encapsulation.

### `TaskManager`

Responsible for managing the application as a whole.

It handles:

* Task collection management
* Menu interaction
* Input validation
* Adding tasks
* Deleting tasks
* Editing tasks
* Completion status changes
* Saving and loading tasks

The application starts by loading saved tasks and continues running until the user chooses to exit.

---

## Task Data

Each task consists of four pieces of information:

| Field    | Type       | Description                     |
| -------- | ---------- | ------------------------------- |
| Title    | `string`   | Name or description of the task |
| Priority | `Priority` | High, Medium, or Low            |
| Status   | `bool`     | Completed or Incomplete         |
| Deadline | `string`   | Optional `YYYY-MM-DD` deadline  |

Example:

```text
Title:      Study Algorithms
Priority:   High
Status:     Incomplete
Deadline:   2026-09-01
```

---

## Input Validation

The program validates user input at several levels.

### Numeric Input

Invalid numeric input is rejected and the user is prompted again.

### Task Titles

Task titles cannot be empty or contain only whitespace.

### Priority

Only the following values are accepted:

```text
1 → High
2 → Medium
3 → Low
```

### Completion Status

Only:

```text
1 → Completed
0 → Incomplete
```

are accepted.

### Deadlines

Deadlines use the following format:

```text
YYYY-MM-DD
```

An empty input is also accepted when no deadline is required.

The current implementation validates the structure and numeric ranges of the date, but does not perform complete calendar validation.

---

## Persistence

Tasks are stored locally in:

```text
tasks.txt
```

The file contains the number of tasks followed by the data for each task.

Conceptually, the format is:

```text
<number of tasks>

<title>
<priority>
<completion status>
<deadline>

<title>
<priority>
<completion status>
<deadline>
```

For example:

```text
2
Study Algorithms
1
0
2026-09-01
Read C++ Book
2
1

```

The application automatically loads existing tasks when it starts and saves changes when tasks are added, deleted, edited, or updated.

The loading process also performs validation and reports corrupted or invalid task data instead of blindly accepting it.

---

## Getting Started

### Prerequisites

You need a C++17-compatible compiler.

For example:

* GCC
* Clang
* MSVC

### Clone the Repository

```bash
git clone <repository-url>
cd <repository-directory>
```

### Compile

Using `g++`:

```bash
g++ -std=c++17 -Wall -Wextra -pedantic main.cpp -o study_tracker
```

### Run

Linux/macOS:

```bash
./study_tracker
```

Windows:

```bash
study_tracker.exe
```

The program will create or use `tasks.txt` in its working directory for persistent storage.

---

## Usage

After launching the program, select an operation from the main menu.

### 1. Show My Tasks

Displays all currently stored tasks.

If there are no tasks:

```text
Your task list is empty.
```

### 2. Add a New Task

The program asks for:

1. Task title
2. Priority
3. Completion status
4. Optional deadline

The new task is then added to the task collection and saved.

### 3. Delete a Task

Select a task by its number.

The program asks for confirmation before permanently removing it.

```text
Are you sure? (y/n):
```

### 4. Toggle Task Status

Quickly switches a task between:

```text
Incomplete ↔ Completed
```

### 5. Edit a Task

An existing task can be modified through the editing menu:

```text
1. Change Title
2. Change Priority
3. Change Deadline
4. Change Completion Status
5. Finish Editing
```

Multiple properties can be changed during one editing session.

### 6. Exit

The current task list is saved before the application terminates.

---

## Error Handling

The program handles several common failure cases:

* Invalid numeric input
* Invalid menu selections
* Invalid task numbers
* Empty task titles
* Invalid priority values
* Invalid completion values
* Invalid deadline formats
* Missing task files
* Empty task files
* Corrupted task files
* File-writing failures

For example, if the task file cannot be opened for writing, the program reports the problem rather than claiming that the save succeeded.

---

## OOP Concepts Demonstrated

This project was built to practice practical object-oriented C++ rather than keeping all functionality inside `main()`.

### Encapsulation

Task data is kept private:

```cpp
class Task
{
private:
    string title;
    Priority priority;
    bool completed;
    string deadline;
};
```

Interaction with the data happens through public member functions.

### Abstraction

Operations such as:

```cpp
task.toggleStatus();
```

hide the implementation details from the code using the object.

### Separation of Responsibilities

`Task` represents an individual task, while `TaskManager` handles the application workflow and collection of tasks.

This keeps the basic domain object separate from the higher-level application logic.

---

## Project Structure

The current implementation is contained in a single C++ source file:

```text
.
├── main.cpp
├── tasks.txt        # Created automatically for persistence
└── README.md
```

The code can later be split into multiple source and header files as the project grows.

A possible future structure would be:

```text
StudyTracker/
├── include/
│   ├── Task.h
│   └── TaskManager.h
│
├── src/
│   ├── Task.cpp
│   ├── TaskManager.cpp
│   └── main.cpp
│
├── data/
│   └── tasks.txt
│
└── README.md
```

---

## Current Limitations

The project intentionally keeps its scope relatively small.

Current limitations include:

* No graphical interface
* No database
* No task search
* No task filtering
* No task sorting
* No categories or tags
* Deadline validation does not verify every possible calendar date
* Deadlines are stored as strings rather than a dedicated date type
* All application code currently resides in one source file

---

## Future Improvements

Possible future versions could introduce:

* Task search
* Filtering by priority or status
* Sorting by deadline or priority
* Overdue-task detection
* "Today's tasks" view
* Upcoming-deadline view
* Task categories and tags
* Subtasks
* Recurring tasks
* Study progress statistics
* A dedicated `Date` class
* Separate repository/persistence layer
* Database support
* Unit testing
* Multi-file project organization
* Graphical or web-based interface

---

## Learning Goals

This project was developed as a practical exercise in:

* C++17
* Object-oriented programming
* Encapsulation
* STL containers
* File I/O
* Input validation
* Regular expressions
* Error handling
* CRUD operations
* Persistent application state
* Designing a small console application

It represents a step toward building larger and more maintainable C++ applications.

---

## License

This project is available for educational and personal use.

If you reuse or modify the project, attribution is appreciated.

---

## Author

**Valeria the Knave**

Built with C++17 as a practical study project focused on object-oriented programming and application design.

