# Exam Management System

A terminal-based exam platform written in C++. Usese OOP and json file handling.

## Features

**Teachers**
- Create exams with multiple question types: multiple choice, short answer, and essay
- Auto-generated unique exam codes
- View grade lists per exam

**Students**
- Register for exams using an exam code
- Take exams and receive instant grading for objective questions
- Get a .txt report card file for each exam

**General**
- Signup and login system with password authentication
- Data persistence via JSON (no database required)

## Requirements

- C++17
- [nlohmann/json](https://github.com/nlohmann/json) for JSON serialization

## Project Structure

```
exam-management-system/
├── src/
│   ├── exam.h          # Class definitions: User, Teacher, Student, Exam, Question
│   ├── exam.cpp        # Method implementations
│   └── main.cpp        # Entry point and menu logic
└── data/               # Auto-generated at runtime
    ├── users.json
    └── exams.json
```

## Build & Run

```bash
g++ src/main.cpp src/exam.cpp -o exam_system
./exam_system
```


