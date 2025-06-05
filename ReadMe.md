# Database Management System Implementation Guide

## Description
This project implements a simple DBMS that lets you execute a limited set of SQL commands to create and manage tables. Its main purpose is to help you understand object-oriented programming concepts.

## Requirements
- C++ compiler compatible with the C++17 standard (e.g., `g++` or Visual Studio)
- Windows or Linux operating system
- `comenziCreate.txt` and `comenziAltele.txt` must exist in the `input` directory

## Build Steps
1. Open a console inside the project directory.
2. Compile the sources using a C++17 compiler. Example for `g++`:
   ```bash
   g++ main.cpp Classes/*.cpp utils/utils.cpp -IHeaders -std=c++17 -o dbms
   ```
3. Run the application while specifying the command files:
   ```bash
   ./dbms input/comenziCreate.txt input/comenziAltele.txt
   ```

## Command Examples
After launching the program, you can run commands such as:
- `CREATE TABLE STUDENT((Height, integer, 5, 190), (Faculty, text, 25, CSIE))`
- `INSERT INTO STUDENT VALUES (172, MAN)`
- `DELETE FROM STUDENT WHERE HEIGHT = 172`
- `UPDATE STUDENT SET HEIGHT = 180 WHERE HEIGHT = 172`
- `--showdatabase`

### Remarks
1. If compilation errors occur, try disabling SDL checks (they may be triggered by `strcpy`).
2. The project is still under development and may contain bugs. Its role is mainly educational.

## Supported Data Types
- Text
- Integer
- Float

## Supported SQL Commands

1. **CREATE TABLE**
   ```
   CREATE TABLE table_name ((Column_Name, type, dimension, default_value), ...)
   ```
   **Example:**
   ```
   CREATE TABLE STUDENT((Height, integer, 5, 190), (Faculty, text, 25, CSIE))
   ```
   **Observations:**
   - `default_value` shouldn't contain spaces (e.g., `John_Smith`).
   - `dimension` defines the maximum length for `type`. For `integer` it is the number of digits; for `text` it is the maximum number of characters.
   - Tables with duplicate names are not allowed.
   - The command creates a binary file named `TABEL_table_name`.

2. **DROP TABLE**
   ```
   DROP TABLE table_name
   ```
   Removes the specified table and its corresponding binary file.

3. **DISPLAY TABLE**
   ```
   DISPLAY TABLE table_name
   ```
   Currently equivalent to `SELECT ALL FROM table_name`.

   **Observation:** This command creates a text file `SELECT_x` containing the console output.

4. **INSERT INTO**
   ```
   INSERT INTO table_name VALUES(...)
   ```
   **Example:**
   ```
   INSERT INTO STUDENT VALUES (172, MAN)
   ```
   **Observation:** Make sure the value types follow the columns' order and type.

5. **DELETE FROM**
   ```
   DELETE FROM table_name WHERE Column_Name = Value
   ```
   **Example:**
   ```
   DELETE FROM STUDENT WHERE HEIGHT = 172
   ```

6. **UPDATE**
   ```
   UPDATE table_name SET Column_Name = VALUE WHERE Column_Name = Value
   ```
   **Example:**
   ```
   UPDATE STUDENT SET HEIGHT = 180 WHERE HEIGHT = 172
   ```

7. **--showdatabase**
   Shows all existing tables.

8. **IMPORT**
   ```
   Import table_name csvfile.csv
   ```
   **Observations:**
   - Only the comma (`,`) separator is accepted.
   - The following characters are disallowed: `:/.;[]()*&^%$#@!`?<>-_+=`.
   - The number of values must be a multiple of the table's column count.
