# Study Tracker

A command-line study and task manager written in **C++17**.

I made this project to practise building something closer to a real application instead of another collection of small exercises. It started with basic task CRUD, then grew to include persistent storage, input validation, date handling, searching, editing, and defensive file-error handling.

It is still a small project — and that's intentional. I wanted to be able to understand and build each part myself.

## Features

- Create tasks with title, priority, status and optional deadline
- Priorities: `High / Medium / Low`
- Mark tasks as `Completed / Incomplete` (toggle)
- Optional deadlines in `YYYY-MM-DD`
- View all tasks
- Search tasks (case-insensitive: title, priority, status, deadline)
- Edit tasks
- Delete tasks with confirmation
- Persistent storage between sessions
- Input validation and error recovery
- Safe saving via temporary file + atomic rename

## Demo

```
============================================================
                     STUDY TRACKER
============================================================
1. Show My Tasks
2. Search Tasks
3. Add a New Task
4. Delete a Task
5. Toggle Task Status
6. Edit a Task
7. Exit
============================================================
```

## How it is structured

Although the current version is contained in one `.cpp` file, responsibilities are separated instead of putting everything in `main()`:

```
Priority / TaskStatus
        │
        ▼
      Task ◄──── Date
        │
        ▼
   TaskManager
      /     \
     ▼       ▼
Repository  ConsoleUI
     │
     ▼
 tasks.txt
```

### `Date`
- Year, month, day + empty state (no deadline)
- Validation, leap-year handling, correct days-per-month
- Parsing `YYYY-MM-DD` and `toString()`
- Comparison operator

### `Task`
- Title, priority, status, deadline
- Encapsulated with getters/setters, `toggleStatus()`

### `TaskRepository`
- Handles `tasks.txt` persistence
- File starts with explicit version: `STUDY_TRACKER_V1`
- Validation on load: header, count, malformed tasks, invalid values, unreasonable counts
- Safe save: write to `tasks.txt.tmp` → `remove()` old → `rename()`

### `TaskManager`
- CRUD + search + load/save
- Keeps in-memory state consistent with disk: if save fails, previous state is restored

### `ConsoleUI`
- Menu, input reading, display
- Uses `TaskManager` instead of touching the vector directly
- Separation of UI vs. data logic

## Searching

Case-insensitive search over all visible fields.

Examples:
- `algorithm` → finds `Study Algorithms`
- `high` → finds high-priority tasks
- `2026-09-01` → finds tasks with that deadline
- `completed` → finds completed tasks

## Input validation

The program defensively checks:

- Invalid menu choices / numbers
- Invalid task indexes
- Empty titles
- Invalid priorities / statuses
- Invalid dates (`2026-02-30` rejected, `2028-02-29` accepted)
- Invalid save-file data / failed file ops / unexpected EOF

## Saving and loading

Tasks are stored in `tasks.txt` in the program's current working directory.

Simplified format:

```
STUDY_TRACKER_V1
2
Study Algorithms
1
0
2026-09-01
Read C++ Book
2
1

```

- Line 1: version identifier
- Line 2: task count
- Then per task: title, priority (1-3), status (0/1), deadline (empty = none)

If no save file exists, the tracker starts empty. No database or external service is required.

## Building

Requires a **C++17** compiler. Only the standard library is used.

### With g++

```bash
g++ -std=c++17 -Wall -Wextra -pedantic study_tracker_V01.cpp -o study_tracker
./study_tracker
```

### With Makefile

```bash
make
./study_tracker
```

### With CMake

```bash
cmake -B build -S .
cmake --build build
./build/study_tracker
```

On Windows:

```
study_tracker.exe
```

## Project structure

```
StudyTracker01/
├── study_tracker_V01.cpp   # Main source (single-file v01)
├── README.md               # This file
├── README.txt              # Plain-text version
├── Makefile                # Simple make build
├── CMakeLists.txt          # CMake build
├── .gitignore
└── LICENSE
```

`tasks.txt` is generated at runtime and gitignored.

## What I practised

- C++17, classes/objects, encapsulation, `enum class`
- `std::vector`, `std::string`, `std::fstream`, `std::stringstream`
- File parsing & persistence, atomic save
- Input validation, exception handling
- Searching, date validation, CRUD, separation of concerns, basic error recovery

## What I would improve next

- Split source into `.h` / `.cpp` files
- Add automated tests (Catch2 / GoogleTest)
- Sorting by priority / deadline, overdue handling
- Categories / tags, recurring tasks, subtasks, statistics
- Replace custom text format with SQLite / JSON
- Improve UI, eventually graphical version

## Why this project exists

A task class alone isn't interesting. A file reader alone isn't interesting. But once the program has to create an object, validate its data, store it, load it again, let the user modify it, handle failures, and keep everything consistent — design starts to matter.

That's what I wanted to learn.

---

**Valeria the Knave** — C++17 · CLI · OOP
