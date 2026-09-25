Study Tracker

A command-line study and task manager written in C++17.

I made this project as a way to practise building something a little closer to a real application instead of another collection of small programming exercises. The project started with the basic idea of creating and managing tasks, then grew to include persistent storage, input validation, date handling, searching, editing, and some defensive handling of file errors.

It is still a small project, and that's intentional. I wanted to be able to understand and build each part myself.

Features

Study Tracker currently supports:

- Creating tasks
- Setting task priorities
- Marking tasks as completed or incomplete
- Adding optional deadlines
- Viewing all tasks
- Searching tasks
- Editing tasks
- Deleting tasks
- Saving tasks between sessions
- Loading previously saved tasks
- Input validation and error handling
- Confirmation before deleting tasks

Each task contains a title, priority, completion status, and optional deadline.

Priorities are:

High
Medium
Low

Dates use the format:

YYYY-MM-DD

A deadline can also be left empty.

The menu

When the program starts, it presents a simple command-line interface:

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

The application loads saved tasks before entering the main loop, so the task list survives between executions.

How it is structured

Although the current version is contained in one ".cpp" file, I tried to keep the different responsibilities separate instead of putting everything inside "main()".

The main structure is:

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

"Date"

"Date" is a small class responsible for representing and validating deadlines.

It supports:

- Year, month, and day
- Empty dates
- Date validation
- Leap-year calculation
- Correct days for each month
- Parsing "YYYY-MM-DD"
- Converting dates back to strings
- Comparing dates

An empty "Date" represents a task without a deadline.

One reason I made this a separate class was to avoid scattering date-related logic throughout the rest of the program.

"Task"

"Task" represents the actual unit of work.

A task contains:

Title
Priority
Status
Deadline

The task's data is kept private and accessed through member functions.

For example, changing its status is handled by the task itself:

task.toggleStatus();

rather than having unrelated parts of the program modify the internal state directly.

"TaskRepository"

"TaskRepository" is responsible for persistence.

It handles reading and writing the task data to:

tasks.txt

The saved file starts with a version identifier:

STUDY_TRACKER_V1

This gives the file format an explicit version rather than treating every text file as automatically valid.

The repository also performs validation when loading data. Invalid file headers, malformed tasks, invalid values, and unreasonable task counts are rejected instead of blindly being loaded into the application.

When saving, the program writes to a temporary file before replacing the existing task file. This is intended to reduce the chance of leaving behind a partially-written save file if writing fails.

"TaskManager"

"TaskManager" is where most of the actual task operations live.

It handles:

- Adding tasks
- Deleting tasks
- Editing tasks
- Toggling task status
- Searching tasks
- Loading tasks
- Saving changes

It also takes care of keeping the in-memory state consistent with the saved state.

For example, when an operation modifies the task list, the manager attempts to save the new state. If saving fails, the previous state can be restored rather than leaving the application in a state that was never successfully persisted.

"ConsoleUI"

"ConsoleUI" handles interaction with the user.

It is responsible for:

- Displaying the menu
- Reading input
- Asking for task information
- Displaying tasks
- Asking for deletion confirmation
- Showing success and error messages

The UI uses "TaskManager" to perform the actual operations instead of directly managing the task collection.

This was one of the main design ideas I wanted to practise with the project: the code that talks to the user doesn't have to be the code that manages the data.

Searching

The search system accepts a text query and performs a case-insensitive search.

It can match information such as:

- Task titles
- Priorities
- Completion status
- Deadlines

For example, searching for:

algorithm

can find:

Study Algorithms

while:

high

can find tasks with high priority.

The search returns the positions of matching tasks, which are then used by the UI to display the results.

Input validation

A fair amount of the program is dedicated to dealing with bad input.

The program checks for things such as:

- Invalid menu choices
- Invalid numbers
- Invalid task indexes
- Empty task titles
- Invalid priorities
- Invalid task statuses
- Invalid dates
- Invalid save-file data
- Failed file operations
- Unexpected end-of-input

For example, a deadline such as:

2026-02-30

is rejected because February does not have 30 days.

Leap years are handled as well, so dates such as:

2028-02-29

can be considered valid.

The goal isn't to make the program impossible to break, but to avoid assuming that every input or file will always be correct.

Saving and loading

Tasks are stored locally in:

tasks.txt

The file is created in the program's current working directory.

A simplified example of the format is:

STUDY_TRACKER_V1
2
Study Algorithms
1
0
2026-09-01
Read C++ Book
2
1


The program loads this file when it starts and saves changes after task operations.

If no save file exists, Study Tracker simply starts with an empty task list.

There is no database or external service involved.

Building

You need a C++17-compatible compiler.

With GCC:

g++ -std=c++17 -Wall -Wextra -pedantic study_tracker_V01.cpp -o study_tracker

Then:

./study_tracker

On Windows:

study_tracker.exe

The project only uses the C++ standard library, so there are no external dependencies.

Project structure

Currently the repository is intentionally simple:

StudyTracker01/
│
├── study_tracker_V01.cpp
└── README.md

"tasks.txt" is generated when the application runs.

The source file is currently kept as one file because this project is still at the stage where I am experimenting with the design. If I continue developing it, separating the classes into header and implementation files would be one of the next changes I'd make.

What I wanted to practise

This project was mainly about getting comfortable with the parts of C++ that become important once a program gets larger than a few functions.

Things I worked with include:

- C++17
- Classes and objects
- Encapsulation
- "enum class"
- "std::vector"
- "std::string"
- "std::fstream"
- "std::stringstream" / "std::ostringstream"
- File parsing
- File persistence
- Input validation
- Exception handling
- Searching
- Date validation
- CRUD-style operations
- Separation of responsibilities
- Basic error recovery

I also wanted to practise thinking about what should happen when something goes wrong, rather than only implementing the successful path.

Things I would improve

Study Tracker is not intended to be a finished production application.

There are several directions I could take it from here:

- Split the source into ".h" and ".cpp" files
- Add automated tests
- Improve the search and filtering system
- Add sorting by priority or deadline
- Add categories or tags
- Add overdue-task handling
- Add recurring tasks
- Add subtasks
- Add study statistics
- Replace the custom text format with a database
- Improve the UI
- Eventually build a graphical version

I would also like to revisit some of the design decisions as I become more comfortable with C++ and software architecture.

Why this project exists

I wanted to build something where the individual pieces actually had to work together.

A task class by itself isn't particularly interesting. A file reader by itself isn't particularly interesting either. But once the program has to create an object, validate its data, store it, load it again, let the user modify it, handle failures, and keep everything consistent, the design starts becoming more important.

That's what I wanted to learn from this project.

It is a small application, but it is also a snapshot of where I am in learning C++ and software design.

---

Valeria the Knave

C++17 · Command Line · Object-Oriented Programming
