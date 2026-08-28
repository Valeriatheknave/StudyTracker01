
#include <algorithm>
#include <cctype>
#include <cstdio>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

enum class Priority
{
    High = 1,
    Medium = 2,
    Low = 3
};

enum class TaskStatus
{
    Incomplete = 0,
    Completed = 1
};

std::string priorityToString(Priority priority)
{
    switch (priority)
    {
    case Priority::High:
        return "High";

    case Priority::Medium:
        return "Medium";

    case Priority::Low:
        return "Low";
    }

    return "Unknown";
}

bool priorityFromInt(int value, Priority& result)
{
    switch (value)
    {
    case 1:
        result = Priority::High;
        return true;

    case 2:
        result = Priority::Medium;
        return true;

    case 3:
        result = Priority::Low;
        return true;

    default:
        return false;
    }
}

int priorityToInt(Priority priority)
{
    return static_cast<int>(priority);
}

std::string statusToString(TaskStatus status)
{
    return status == TaskStatus::Completed
               ? "Completed"
               : "Incomplete";
}

class Date
{
private:
    int year;
    int month;
    int day;

    static bool isLeapYear(int value)
    {
        return (value % 400 == 0) ||
               (value % 4 == 0 && value % 100 != 0);
    }

    static int daysInMonth(int yearValue, int monthValue)
    {
        static const int days[] =
        {
            31, 28, 31, 30, 31, 30,
            31, 31, 30, 31, 30, 31
        };

        if (monthValue == 2 && isLeapYear(yearValue))
        {
            return 29;
        }

        return days[monthValue - 1];
    }

public:

    Date()
        : year(0),
          month(0),
          day(0)
    {
    }

    Date(
        int yearValue,
        int monthValue,
        int dayValue
    )
        : year(yearValue),
          month(monthValue),
          day(dayValue)
    {
    }

    bool isEmpty() const
    {
        return year == 0;
    }

    bool isValid() const
    {
        if (isEmpty())
        {
            return true;
        }

        if (year < 1 ||
            month < 1 ||
            month > 12)
        {
            return false;
        }

        return day >= 1 &&
               day <= daysInMonth(year, month);
    }

    std::string toString() const
    {
        if (isEmpty())
        {
            return "";
        }

        std::ostringstream output;

        output << std::setfill('0')
               << std::setw(4) << year
               << '-'
               << std::setw(2) << month
               << '-'
               << std::setw(2) << day;

        return output.str();
    }

    static bool tryParse(
        const std::string& text,
        Date& result
    )
    {
        if (text.empty())
        {
            result = Date();
            return true;
        }
        if (text.size() != 10 ||
            text[4] != '-' ||
            text[7] != '-')
        {
            return false;
        }

        for (std::size_t i = 0; i < text.size(); ++i)
        {
            if (i == 4 || i == 7)
            {
                continue;
            }

            if (!std::isdigit(
                    static_cast<unsigned char>(text[i])))
            {
                return false;
            }
        }

        try
        {
            const int parsedYear =
                std::stoi(text.substr(0, 4));

            const int parsedMonth =
                std::stoi(text.substr(5, 2));

            const int parsedDay =
                std::stoi(text.substr(8, 2));

            Date candidate(
                parsedYear,
                parsedMonth,
                parsedDay
            );

            if (!candidate.isValid())
            {
                return false;
            }

            result = candidate;
            return true;
        }
        catch (const std::exception&)
        {
            return false;
        }
    }
    
    bool operator<(const Date& other) const
    {
        if (isEmpty() || other.isEmpty())
        {
            return !isEmpty() && other.isEmpty();
        }

        if (year != other.year)
        {
            return year < other.year;
        }

        if (month != other.month)
        {
            return month < other.month;
        }

        return day < other.day;
    }
};

class Task
{
private:
    std::string title;
    Priority priority;
    TaskStatus status;
    Date deadline;

public:

    Task(
        std::string titleValue,
        Priority priorityValue,
        TaskStatus statusValue,
        Date deadlineValue
    )
        : title(std::move(titleValue)),
          priority(priorityValue),
          status(statusValue),
          deadline(deadlineValue)
    {
    }

    const std::string& getTitle() const
    {
        return title;
    }

    Priority getPriority() const
    {
        return priority;
    }

    TaskStatus getStatus() const
    {
        return status;
    }

    const Date& getDeadline() const
    {
        return deadline;
    }


    void setTitle(std::string newTitle)
    {
        title = std::move(newTitle);
    }

    void setPriority(Priority newPriority)
    {
        priority = newPriority;
    }

    void setStatus(TaskStatus newStatus)
    {
        status = newStatus;
    }

    void setDeadline(Date newDeadline)
    {
        deadline = newDeadline;
    }


    void toggleStatus()
    {
        status =
            status == TaskStatus::Completed
                ? TaskStatus::Incomplete
                : TaskStatus::Completed;
    }
};

class TaskRepository
{
private:
    std::string filename;

    static bool parseNonNegativeInt(
        const std::string& text,
        std::size_t& result
    )
    {
        if (text.empty())
        {
            return false;
        }

        for (char character : text)
        {
            if (!std::isdigit(
                    static_cast<unsigned char>(character)))
            {
                return false;
            }
        }

        try
        {
            std::size_t consumed = 0;

            const unsigned long long value =
                std::stoull(text, &consumed);

            if (consumed != text.size() ||
                value >
                    std::numeric_limits<std::size_t>::max())
            {
                return false;
            }

            result =
                static_cast<std::size_t>(value);

            return true;
        }
        catch (const std::exception&)
        {
            return false;
        }
    }


    static bool parseTask(
        std::ifstream& input,
        Task& task
    )
    {
        std::string title;
        std::string priorityLine;
        std::string statusLine;
        std::string deadlineLine;

        if (!std::getline(input, title) ||
            !std::getline(input, priorityLine) ||
            !std::getline(input, statusLine) ||
            !std::getline(input, deadlineLine))
        {
            return false;
        }

        if (title.empty() ||
            title.find('\r') != std::string::npos ||
            title.find('\n') != std::string::npos)
        {
            return false;
        }

        int priorityValue = 0;

        try
        {
            std::size_t consumed = 0;

            priorityValue =
                std::stoi(priorityLine, &consumed);

            if (consumed != priorityLine.size())
            {
                return false;
            }
        }
        catch (const std::exception&)
        {
            return false;
        }

        Priority priority;

        if (!priorityFromInt(
                priorityValue,
                priority))
        {
            return false;
        }
        
        int statusValue = 0;

        try
        {
            std::size_t consumed = 0;

            statusValue =
                std::stoi(statusLine, &consumed);

            if (consumed != statusLine.size() ||
                (statusValue != 0 &&
                 statusValue != 1))
            {
                return false;
            }
        }
        catch (const std::exception&)
        {
            return false;
        }

        Date deadline;

        if (!Date::tryParse(
                deadlineLine,
                deadline))
        {
            return false;
        }

        task = Task(
            title,
            priority,
            statusValue == 1
                ? TaskStatus::Completed
                : TaskStatus::Incomplete,
            deadline
        );

        return true;
    }

public:

    explicit TaskRepository(
        std::string filenameValue
    )
        : filename(std::move(filenameValue))
    {
    }

    bool save(
        const std::vector<Task>& tasks
    ) const
    {
        const std::string temporaryFilename =
            filename + ".tmp";

        {
            std::ofstream output(
                temporaryFilename,
                std::ios::trunc
            );

            if (!output)
            {
                return false;
            }

            output << "STUDY_TRACKER_V1\n";
            output << tasks.size() << '\n';

            for (const Task& task : tasks)
            {
                output << task.getTitle() << '\n';

                output
                    << priorityToInt(
                           task.getPriority())
                    << '\n';

                output
                    << (
                        task.getStatus()
                            == TaskStatus::Completed
                            ? 1
                            : 0
                    )
                    << '\n';

                output
                    << task.getDeadline().toString()
                    << '\n';
            }

            if (!output)
            {
                output.close();
                std::remove(
                    temporaryFilename.c_str()
                );

                return false;
            }
        }

        std::remove(filename.c_str());

        if (std::rename(
                temporaryFilename.c_str(),
                filename.c_str()) != 0)
        {
            std::remove(
                temporaryFilename.c_str()
            );

            return false;
        }

        return true;
    }

    bool load(
        std::vector<Task>& tasks,
        std::string& errorMessage
    ) const
    {
        std::ifstream input(filename);

        if (!input)
        {
            return true;
        }

        std::string header;

        if (!std::getline(input, header))
        {
            errorMessage =
                "The task file is empty.";

            return false;
        }

        if (header != "STUDY_TRACKER_V1")
        {
            errorMessage =
                "Unknown or incompatible task-file format.";

            return false;
        }

        std::string countLine;

        if (!std::getline(input, countLine))
        {
            errorMessage =
                "The task count is missing.";

            return false;
        }

        std::size_t taskCount = 0;

        if (!parseNonNegativeInt(
                countLine,
                taskCount))
        {
            errorMessage =
                "The task count is invalid.";

            return false;
        }

        constexpr std::size_t maxTasks = 100000;

        if (taskCount > maxTasks)
        {
            errorMessage =
                "The task count is unreasonably large.";

            return false;
        }

        std::vector<Task> loadedTasks;
        loadedTasks.reserve(taskCount);

        for (std::size_t i = 0;
             i < taskCount;
             ++i)
        {
            Task task(
                "",
                Priority::Low,
                TaskStatus::Incomplete,
                Date()
            );

            if (!parseTask(input, task))
            {
                errorMessage =
                    "Task " +
                    std::to_string(i + 1) +
                    " is corrupted.";

                return false;
            }

            loadedTasks.push_back(
                std::move(task)
            );
        }

        tasks = std::move(loadedTasks);

        return true;
    }
};

class TaskManager
{
private:
    std::vector<Task> tasks;
    TaskRepository repository;

    bool saveCurrentState(
        std::string& errorMessage
    ) const
    {
        if (!repository.save(tasks))
        {
            errorMessage =
                "Could not save tasks to disk.";

            return false;
        }

        return true;
    }

    static std::string toLower(
        std::string text
    )
    {
        std::transform(
            text.begin(),
            text.end(),
            text.begin(),
            [](unsigned char character)
            {
                return static_cast<char>(
                    std::tolower(character)
                );
            }
        );

        return text;
    }

public:

    explicit TaskManager(
        TaskRepository repositoryValue
    )
        : repository(
              std::move(repositoryValue))
    {
    }

    bool load(
        std::string& errorMessage
    )
    {
        return repository.load(
            tasks,
            errorMessage
        );
    }

    const std::vector<Task>& getTasks() const
    {
        return tasks;
    }

    bool addTask(
        std::string title,
        Priority priority,
        TaskStatus status,
        Date deadline,
        std::string& errorMessage
    )
    {
        if (title.empty())
        {
            errorMessage =
                "Task title cannot be empty.";

            return false;
        }

        if (!deadline.isEmpty() &&
            !deadline.isValid())
        {
            errorMessage =
                "The deadline is invalid.";

            return false;
        }

        tasks.emplace_back(
            std::move(title),
            priority,
            status,
            deadline
        );

        if (!saveCurrentState(errorMessage))
        {
            tasks.pop_back();
            return false;
        }

        return true;
    }

    bool deleteTask(
        std::size_t index,
        std::string& errorMessage
    )
    {
        if (index >= tasks.size())
        {
            errorMessage =
                "Invalid task number.";

            return false;
        }

        Task backup = tasks[index];

        tasks.erase(
            tasks.begin() +
            static_cast<std::ptrdiff_t>(index)
        );

        if (!saveCurrentState(errorMessage))
        {
            tasks.insert(
                tasks.begin() +
                    static_cast<std::ptrdiff_t>(index),
                std::move(backup)
            );

            return false;
        }

        return true;
    }

    bool toggleTask(
        std::size_t index,
        std::string& errorMessage
    )
    {
        if (index >= tasks.size())
        {
            errorMessage =
                "Invalid task number.";

            return false;
        }

        tasks[index].toggleStatus();

        if (!saveCurrentState(errorMessage))
        {
            tasks[index].toggleStatus();
            return false;
        }

        return true;
    }

    bool editTask(
        std::size_t index,
        const std::string& newTitle,
        Priority newPriority,
        TaskStatus newStatus,
        Date newDeadline,
        std::string& errorMessage
    )
    {
        if (index >= tasks.size())
        {
            errorMessage =
                "Invalid task number.";

            return false;
        }

        if (newTitle.empty())
        {
            errorMessage =
                "Task title cannot be empty.";

            return false;
        }

        if (!newDeadline.isEmpty() &&
            !newDeadline.isValid())
        {
            errorMessage =
                "The deadline is invalid.";

            return false;
        }

        Task backup = tasks[index];

        tasks[index].setTitle(newTitle);
        tasks[index].setPriority(newPriority);
        tasks[index].setStatus(newStatus);
        tasks[index].setDeadline(newDeadline);

        if (!saveCurrentState(errorMessage))
        {
            tasks[index] = std::move(backup);
            return false;
        }

        return true;
    }

    std::vector<std::size_t> searchTasks(
        const std::string& query
    ) const
    {
        std::vector<std::size_t> results;

        const std::string searchTerm =
            toLower(query);

        if (searchTerm.empty())
        {
            return results;
        }

        for (std::size_t i = 0;
             i < tasks.size();
             ++i)
        {
            const Task& task = tasks[i];

            const std::string title =
                toLower(task.getTitle());

            const std::string priority =
                toLower(
                    priorityToString(
                        task.getPriority()
                    )
                );

            const std::string status =
                toLower(
                    statusToString(
                        task.getStatus()
                    )
                );

            const std::string deadline =
                toLower(
                    task.getDeadline().toString()
                );

            const bool matches =
                title.find(searchTerm) !=
                    std::string::npos ||
                priority.find(searchTerm) !=
                    std::string::npos ||
                status.find(searchTerm) !=
                    std::string::npos ||
                deadline.find(searchTerm) !=
                    std::string::npos;

            if (matches)
            {
                results.push_back(i);
            }
        }

        return results;
    }
};

class ConsoleUI
{
private:
    TaskManager& manager;

    static bool isBlank(
        const std::string& text
    )
    {
        if (text.empty())
        {
            return true;
        }

        for (char character : text)
        {
            if (!std::isspace(
                    static_cast<unsigned char>(character)))
            {
                return false;
            }
        }

        return true;
    }

    static std::string getLine(
        const std::string& prompt
    )
    {
        std::cout << prompt;

        std::string input;

        if (!std::getline(
                std::cin,
                input))
        {
            return "";
        }

        if (!input.empty() &&
            input.back() == '\r')
        {
            input.pop_back();
        }

        return input;
    }

    static int getInt(
        const std::string& prompt
    )
    {
        while (true)
        {
            const std::string input =
                getLine(prompt);

            std::istringstream stream(input);

            int value = 0;
            char extra = '\0';

            if (stream >> value &&
                !(stream >> extra))
            {
                return value;
            }

            if (std::cin.eof())
            {
                return 0;
            }

            std::cout
                << "Invalid input. "
                << "Please enter a number.\n";
        }
    }

    static Priority getPriority()
    {
        while (true)
        {
            const int value = getInt(
                "Priority "
                "(1 = High, 2 = Medium, 3 = Low): "
            );

            Priority priority;

            if (priorityFromInt(
                    value,
                    priority))
            {
                return priority;
            }

            std::cout
                << "Please enter only "
                << "1, 2, or 3.\n";
        }
    }

    static TaskStatus getStatus()
    {
        while (true)
        {
            const int value = getInt(
                "Status "
                "(1 = Completed, 0 = Incomplete): "
            );

            if (value == 1)
            {
                return TaskStatus::Completed;
            }

            if (value == 0)
            {
                return TaskStatus::Incomplete;
            }

            std::cout
                << "Please enter only "
                << "1 or 0.\n";
        }
    }

    static Date getDeadline()
    {
        while (true)
        {
            const std::string input =
                getLine(
                    "Deadline "
                    "(YYYY-MM-DD, Enter for none): "
                );

            Date deadline;

            if (Date::tryParse(
                    input,
                    deadline))
            {
                return deadline;
            }

            std::cout
                << "Invalid date. "
                << "Use a real date such as "
                << "2026-09-01, "
                << "or press Enter for none.\n";
        }
    }

    std::size_t getTaskIndex(
        const std::string& prompt
    ) const
    {
        const auto& tasks =
            manager.getTasks();

        while (true)
        {
            const int number =
                getInt(prompt);

            if (number == 0)
            {
                return tasks.size();
            }

            if (number >= 1 &&
                static_cast<std::size_t>(
                    number) <= tasks.size())
            {
                return static_cast<std::size_t>(
                    number - 1
                );
            }

            std::cout
                << "Invalid task number. "
                << "Choose a number from 1 to "
                << tasks.size()
                << ", or 0 to cancel.\n";
        }
    }

    static void printTask(
        const Task& task,
        std::size_t number
    )
    {
        std::cout
            << '\n'
            << number
            << ". "
            << task.getTitle()
            << '\n';

        std::cout
            << "   Priority: "
            << priorityToString(
                   task.getPriority())
            << '\n';

        std::cout
            << "   Status: "
            << statusToString(
                   task.getStatus())
            << '\n';

        std::cout
            << "   Deadline: "
            << (
                task.getDeadline().isEmpty()
                    ? "None"
                    : task.getDeadline().toString()
            )
            << '\n';

        std::cout
            << "------------------------------------------------------------\n";
    }

    void showTasks() const
    {
        const auto& tasks =
            manager.getTasks();

        std::cout
            << "\n============================================================\n"
            << "                       YOUR TASKS\n"
            << "============================================================\n";

        if (tasks.empty())
        {
            std::cout
                << "Your task list is empty.\n";

            return;
        }

        for (std::size_t i = 0;
             i < tasks.size();
             ++i)
        {
            printTask(
                tasks[i],
                i + 1
            );
        }
    }

    void addTask()
    {
        std::cout
            << "\n---------------------- ADD TASK ----------------------------\n";

        std::string title;

        while (true)
        {
            title =
                getLine("Task title: ");

            if (!isBlank(title))
            {
                break;
            }

            std::cout
                << "Task title cannot be empty.\n";
        }

        const Priority priority =
            getPriority();

        const TaskStatus status =
            getStatus();

        const Date deadline =
            getDeadline();

        std::string error;

        if (manager.addTask(
                title,
                priority,
                status,
                deadline,
                error))
        {
            std::cout
                << "Task added successfully.\n";
        }
        else
        {
            std::cout
                << "Could not add task: "
                << error
                << '\n';
        }
    }

    void searchTasks() const
    {
        if (manager.getTasks().empty())
        {
            std::cout
                << "\nNo tasks to search.\n";

            return;
        }

        std::cout
            << "\n---------------------- SEARCH -----------------------------\n";

        const std::string query =
            getLine("Search for: ");

        if (isBlank(query))
        {
            std::cout
                << "Search query cannot be empty.\n";

            return;
        }

        const std::vector<std::size_t> results =
            manager.searchTasks(query);

        std::cout
            << "\n============================================================\n"
            << "                     SEARCH RESULTS\n"
            << "============================================================\n";

        if (results.empty())
        {
            std::cout
                << "No tasks matched \""
                << query
                << "\".\n";

            return;
        }

        std::cout
            << results.size()
            << " task(s) found for \""
            << query
            << "\".\n";

        for (std::size_t index : results)
        {
            printTask(
                manager.getTasks()[index],
                index + 1
            );
        }
    }

    void deleteTask()
    {
        if (manager.getTasks().empty())
        {
            std::cout
                << "\nNo tasks to delete.\n";

            return;
        }

        showTasks();

        const std::size_t index =
            getTaskIndex(
                "Task number to delete "
                "(0 = cancel): "
            );

        if (index >= manager.getTasks().size())
        {
            std::cout
                << "Deletion cancelled.\n";

            return;
        }

        const std::string title =
            manager.getTasks()[index]
                .getTitle();

        const std::string confirmation =
            getLine(
                "Delete \"" +
                title +
                "\"? (y/n): "
            );

        if (confirmation != "y" &&
            confirmation != "Y")
        {
            std::cout
                << "Deletion cancelled.\n";

            return;
        }

        std::string error;

        if (manager.deleteTask(
                index,
                error))
        {
            std::cout
                << "Task deleted successfully.\n";
        }
        else
        {
            std::cout
                << "Could not delete task: "
                << error
                << '\n';
        }
    }

    void toggleTask()
    {
        if (manager.getTasks().empty())
        {
            std::cout
                << "\nNo tasks to update.\n";

            return;
        }

        showTasks();

        const std::size_t index =
            getTaskIndex(
                "Task number to toggle "
                "(0 = cancel): "
            );

        if (index >= manager.getTasks().size())
        {
            std::cout
                << "Status update cancelled.\n";

            return;
        }

        std::string error;

        if (manager.toggleTask(
                index,
                error))
        {
            std::cout
                << "Task status changed to "
                << statusToString(
                       manager.getTasks()[index]
                           .getStatus())
                << ".\n";
        }
        else
        {
            std::cout
                << "Could not update task: "
                << error
                << '\n';
        }
    }

    void editTask()
    {
        if (manager.getTasks().empty())
        {
            std::cout
                << "\nNo tasks to edit.\n";

            return;
        }

        showTasks();

        const std::size_t index =
            getTaskIndex(
                "Task number to edit "
                "(0 = cancel): "
            );

        if (index >= manager.getTasks().size())
        {
            std::cout
                << "Editing cancelled.\n";

            return;
        }

        const Task original =
            manager.getTasks()[index];

        std::cout
            << "\nEditing: "
            << original.getTitle()
            << "\n";

        std::string title;

        while (true)
        {
            title =
                getLine("New title: ");

            if (!isBlank(title))
            {
                break;
            }

            std::cout
                << "Title cannot be empty.\n";
        }

        const Priority priority =
            getPriority();

        const TaskStatus status =
            getStatus();

        const Date deadline =
            getDeadline();

        std::string error;

        if (manager.editTask(
                index,
                title,
                priority,
                status,
                deadline,
                error))
        {
            std::cout
                << "Task updated successfully.\n";
        }
        else
        {
            std::cout
                << "Could not update task: "
                << error
                << '\n';
        }
    }

    static void printMenu()
    {
        std::cout
            << "\n============================================================\n"
            << "                     STUDY TRACKER\n"
            << "============================================================\n"
            << "1. Show My Tasks\n"
            << "2. Search Tasks\n"
            << "3. Add a New Task\n"
            << "4. Delete a Task\n"
            << "5. Toggle Task Status\n"
            << "6. Edit a Task\n"
            << "7. Exit\n"
            << "============================================================\n";
    }

public:

    explicit ConsoleUI(
        TaskManager& managerValue
    )
        : manager(managerValue)
    {
    }

    void run()
    {
        std::string error;

        if (!manager.load(error))
        {
            std::cout
                << "Warning: "
                << error
                << '\n'
                << "Starting with an empty task list.\n";
        }
        else if (!manager.getTasks().empty())
        {
            std::cout
                << manager.getTasks().size()
                << " task(s) loaded.\n";
        }

        while (true)
        {
            printMenu();

            const int choice =
                getInt("Your choice: ");

            switch (choice)
            {
            case 1:
                showTasks();
                break;

            case 2:
                searchTasks();
                break;

            case 3:
                addTask();
                break;

            case 4:
                deleteTask();
                break;

            case 5:
                toggleTask();
                break;

            case 6:
                editTask();
                break;

            case 7:
                std::cout
                    << "\nThank you for using "
                    << "Study Tracker!\n";

                return;

            default:
                std::cout
                    << "Please enter a number "
                    << "between 1 and 7.\n";
            }

            if (std::cin.eof())
            {
                std::cout
                    << "\nInput ended. Exiting.\n";

                return;
            }
        }
    }
};

int main()
{
    std::cout
        << "\n****************************************************\n"
        << "                  STUDY TRACKER\n"
        << "****************************************************\n"
        << "Welcome to the Study Tracker!\n";

    TaskRepository repository(
        "tasks.txt"
    );

    TaskManager manager(
        repository
    );

    ConsoleUI ui(
        manager
    );

    ui.run();

    return 0;
}
