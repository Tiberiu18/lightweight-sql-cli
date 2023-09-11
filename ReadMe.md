# Database Management Implementation Guide

This implementation offers a basic solution for database management, enabling users to employ SQL commands to create and manage tables.

## Features:

- **Data Types Supported:** 
  - Text
  - Integer
  - Float

- **Project Files:** Within the `input` folder, ensure the presence of two files:
  - `comenziCreate.txt` (Accepts only `CREATE TABLE` commands)
  - `comenziAltele.txt` (Accepts all types of commands)

  To use these files, provide them as command-line arguments. 


### Disclaimers:

1. If compile errors arise, disable SDL checks. This might be due to the `strcpy` function.
2. The project is still in development and might contain bugs. Its primary purpose is to facilitate understanding of OOP concepts.

## Accepted SQL Commands:

1. **CREATE TABLE**

CREATE TABLE table_name ((Column1_name, type, dimension, default_value), ...)


**Example:** 
CREATE TABLE STUDENT((Height, integer, 5, 190), (Faculty, text, 25, CSIE))


**Observations:**
- `default_value` shouldn't have white spaces (e.g., John_Smith instead of John Smith).
- `dimension` defines the maximum length for `type`. For `integer`, it refers to the number of figures. For `text`, it indicates maximum characters.
- Tables with duplicate names are not allowed.
- The command creates a binary file named `TABEL_table_name`.

2. **DROP TABLE**

DROP TABLE table_name


This command removes the specified table and its corresponding binary file.

3. **DISPLAY TABLE**

DISPLAY TABLE table_name


Currently, this is analogous to `SELECT ALL FROM table_name`.

**Observation:** 
This command creates a text file named `SELECT_x`, where x is a number, containing console output.

4. **INSERT INTO**

INSERT INTO table_name VALUES(...)


**Example:** 
INSERT INTO STUDENT VALUES (172, MAN)


**Observations:** 
- Ensure that the values' types match the column's order and type.

5. **DELETE FROM**

DELETE FROM table_name WHERE Column_Name = Value


**Example:** 
DELETE FROM STUDENT WHERE HEIGHT = 172


6. **UPDATE**

UPDATE table_name SET Column_Name = VALUE WHERE Column_Name = Value


**Example:** 
UPDATE STUDENT SET HEIGHT = 180 WHERE HEIGHT = 172


7. **--showdatabase**

When executed, it displays all existing tables.

8. **IMPORT**

This option allows importing data into a table from a CSV file.

**Example:** 
Import student csvfile.csv


**Observations:** 
- Only the comma (`,`) separator is permitted.
- Certain characters are disallowed, such as: `:/.;[]()*&^%$#@!`?<>-_+=`.
- Ensure that the number of values is a multiple of the table's column count.
