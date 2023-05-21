import sqlite3
from typing import List, Tuple
from data_structs import SuperTask, Task, WeekView
from time_24h import get_time, get_date


def _create_tables(conn: sqlite3.Connection) -> None:
    """
    Creates the tables if they do not already exist for the database file
    :param conn: sqlite connection to the database file to create tables in
    :return: nothing
    side effect: creates tables
    """
    cursor: sqlite3.Cursor = conn.cursor()
    cursor.execute("CREATE TABLE IF NOT EXISTS tasks (id INTEGER PRIMARY KEY, title TEXT, super_task_id INTEGER, "
                   "checked INTEGER)")
    cursor.execute("CREATE TABLE IF NOT EXISTS super_tasks (id INTEGER PRIMARY KEY, title TEXT)")
    cursor.execute("CREATE TABLE IF NOT EXISTS weeks (task_id INTEGER, start_time INTEGER, end_time INTEGER, "
                   "date INTEGER)")
    conn.commit()


def _get_all_tasks_by_super_task_id(cursor: sqlite3.Cursor, super_task_id: int) -> List[Task]:
    """
    Fetches every task associated with given super task id as a list of tasks
    :param cursor: cursor object to a database connection
    :param super_task_id: the id o the supertask to pull tasks from
    :return: Python list of Task objects for given super task (by id)
    """
    task_sql: str = "SELECT * FROM tasks WHERE super_task_id = {}".format(super_task_id)
    tasks_iterator: List[Tuple[int, str, int, int]] = cursor.execute(task_sql).fetchall()
    tasks: List[Task] = list()

    for task in tasks_iterator:
        tasks.append(Task(task[0], task[1], task[2], task[3] == 1))

    return tasks


def get_database_connection(filename: str) -> sqlite3.Connection:
    """
    Establises connection to a sqlite database and returns the connection
    :param filename: name of the database file
    :return: connection to sqlite database with given name
    """
    conn: sqlite3.Connection = sqlite3.connect(filename)

    _create_tables(conn)

    return conn


def create_super_task(conn: sqlite3.Connection, title: str) -> None:                         # CREATE super task
    """
    Creates a super task in the sqlite file
    :param conn: connection to the sqlite file
    :param title: name of the super task to create
    :return: nothing
    side effect: creates a super task in the database
    """
    cursor: sqlite3.Cursor = conn.cursor()
    sql: str = "INSERT INTO super_tasks (title) VALUES (\'{}\')".format(title)
    cursor.execute(sql)
    conn.commit()


def create_task(conn: sqlite3.Connection, super_task_id: int, title: str) -> None:           # CREATE task
    """
    Creates a task in the sqlite file
    :param conn: connection to the sqlite file
    :param super_task_id: id of the super task this task belongs to
    :param title: title of the task
    :return: nothing
    side effect: creates a task in the database
    """
    cursor: sqlite3.Cursor = conn.cursor()
    sql: str = "INSERT INTO tasks (title, super_task_id, checked) VALUES(?, ?, ?)"
    params: tuple[str, int, int] = (title, super_task_id, 0)
    cursor.execute(sql, params)
    conn.commit()


def create_ongoing_task(conn: sqlite3.Connection, task_id: int) -> None:                     # CREATE ongoing task
    """
    Creates an ongoing_task, a weekview object representation in the sqlite file
    :param conn: connection to the sqlite file
    :param task_id: id of this task
    :return: nothing
    side effect: creates an ongoing task in the database
    """
    cursor: sqlite3.Cursor = conn.cursor()
    start_time: int = get_time()
    end_time: int = 0  # representing 0000 default time
    date = get_date()
    sql: str = "INSERT INTO weeks (task_id, start_time, end_time, date) VALUES(?, ?, ?, ?)"
    params: tuple[int, int, int, int] = (task_id, start_time, end_time, date)
    cursor.execute(sql, params)
    conn.commit()


def read_super_tasks(cursor: sqlite3.Cursor) -> List[SuperTask]:                             # READ super task
    """
    Reads from the sqlite file
    :param cursor: cursor to a connection to the sqlite file
    :return: Python list of SuperTask objects read from the database
    """
    sql: str = "SELECT * FROM super_tasks"
    super_tasks_iterator: List[Tuple[int, str]] = cursor.execute(sql).fetchall()
    super_tasks: List[SuperTask] = list()

    for super_task in super_tasks_iterator:
        temp_super_task: SuperTask = SuperTask(super_task[0], super_task[1], [])
        temp_super_task.tasks = _get_all_tasks_by_super_task_id(cursor, super_task[0])
        super_tasks.append(temp_super_task)

    return super_tasks


def read_all_tasks_from_date(cursor: sqlite3.Cursor, date: int) -> List[WeekView]:           # READ ongoing task
    """
    Reads all ongoing tasks from a given date
    :param cursor: cursor to a connection to the sqlite file
    :param date: day for which to read in format yyyymmDDd {DD: last monday date} {d: days since last monday}
    :return: Python list of WeekView objects read from the database
    """
    sql: str = "SELECT * FROM weeks WHERE date = {}".format(date)
    weekly_tasks: List[Tuple[int, int, int]] = cursor.execute(sql).fetchall()
    weekview_list: List[WeekView] = list()

    for wt in weekly_tasks:
        if wt[0] == -1:  # associated super task/task deleted
            weekview_list.append(WeekView(wt[0], "DELETED TASK", wt[1], wt[2]))
        else:
            sql_title_query: str = "SELECT (title) FROM tasks WHERE id = {}".format(wt[0])
            tuple_with_title: Tuple[str] = cursor.execute(sql_title_query).fetchone()
            weekview_list.append(WeekView(wt[0], tuple_with_title[0], wt[1], wt[2]))

    return weekview_list


def update_super_task_title(conn: sqlite3.Connection, super_task_id: int, new_super_task_title: str) -> None:
    """                                                                                      # UPDATE super task
    Updates super task item's title
    :param conn: connection to the sqlite database
    :param super_task_id: id of the super task to update
    :param new_super_task_title: new title of the super task to update to
    :return: nothing
    side effect: changes super task in the table
    """
    cursor: sqlite3.Cursor = conn.cursor()
    sql: str = "UPDATE super_tasks SET title = \"{}\" WHERE id = {}".format(new_super_task_title, super_task_id)
    cursor.execute(sql)
    conn.commit()


def update_task_title(conn: sqlite3.Connection, task_id: int, new_task_title: str) -> None:  # UPDATE task
    """
    Updates task item's title
    :param conn: connection to the sqlite database
    :param task_id: id of the task to update
    :param new_task_title: new title of the task to update to
    :return: nothing
    side effect: changes task in the table
    """
    cursor: sqlite3.Cursor = conn.cursor()
    sql: str = "UPDATE tasks SET title = \"{}\" WHERE id = {}".format(new_task_title, task_id)
    cursor.execute(sql)
    conn.commit()


def update_task_checkmark_toggle(conn: sqlite3.Connection, task_id: int) -> bool:            # UPDATE task checkmark
    """
    Updates task item's checkmark status. Toggles to False if True and to True if False
    :param conn: connection to the sqlite database
    :param task_id: id of the task to update
    :return: the new state of the checkmark of the task
    side effect: changes the task checkmark status in the table
    """
    cursor: sqlite3.Cursor = conn.cursor()
    sql: str = "SELECT (checked) FROM tasks WHERE id = {}".format(task_id)
    cursor_state: Tuple[int] = cursor.execute(sql).fetchone()
    new_state: bool = cursor_state[0] != 1
    sql: str = "UPDATE tasks SET checked = {} WHERE id = {}".format(1 if new_state else 0, task_id)
    cursor.execute(sql)
    conn.commit()
    return new_state


def update_ongoing_task(conn: sqlite3.Connection, task_id: int, old_start_time: int, date: int = get_date(),
                        new_start_time: int = None, new_end_time: int = get_time()) -> None:
    """                                                                                      # UPDATE ongoing task
    Updates ongoing task's start time and end time
    :param conn: connection to the sqlite database
    :param task_id: id of the associated task to the ongoing task
    :param old_start_time: previous start time of ongoing task
    :param date: (optional) date this ongoing task was started
                 default: today's date
    :param new_start_time: (optional) new start time for this task
                 default: previous ongoing task's start time (remains unchanged)
    :param new_end_time: (optional) new end time for this task
                 default: time when this function is invoked
    :return: nothing
    side effect: updates weekview item in the database
    """
    cursor: sqlite3.Cursor = conn.cursor()
    change_start_time: bool = new_start_time is not None

    sql1: str = "UPDATE weeks SET (start_time, end_time) = ({}, {})". \
        format(new_start_time if change_start_time else old_start_time, new_end_time)
    sql2: str = " WHERE task_id = {} AND start_time = {} AND date = {}".format(task_id, old_start_time, date)
    cursor.execute(sql1 + sql2)
    conn.commit()


def delete_task(conn: sqlite3.Connection, task_id: int) -> None:                             # DELETE task
    """
    Deletes a task in the given sqlite database
    :param conn: connection to the sqlite database
    :param task_id: id of the task to delete
    :return: nothing
    side effect: deletes a task item from the database
    """
    cursor: sqlite3.Cursor = conn.cursor()
    sql: str = "UPDATE weeks SET task_id = -1 WHERE task_id = {}".format(task_id)
    cursor.execute(sql)
    sql = "DELETE FROM tasks WHERE id = {}".format(task_id)
    cursor.execute(sql)
    conn.commit()


def delete_super_task(conn: sqlite3.Connection, super_task_id: int) -> None:                 # DELETE super task
    """
    Deletes a super task AND all its associated tasks from given sqlite database
    :param conn: connection to the sqlite database
    :param super_task_id: id of the super task to delete
    :return: nothing
    side effect: deletes a super task item and all its task items from database
    """
    cursor: sqlite3.Cursor = conn.cursor()
    tasks = _get_all_tasks_by_super_task_id(cursor, super_task_id)

    for task in tasks:
        delete_task(conn, task.identity)

    sql: str = "DELETE FROM super_tasks WHERE id = {}".format(super_task_id)
    cursor.execute(sql)
    conn.commit()


def delete_ongoing_task(conn: sqlite3.Connection, task_id: int, start_time: int, date: int) -> None:
    """                                                                                      # DELETE ongoing task
    Deletes an ongoing task from the given sqlite database
    :param conn: connection to the sqlite database file
    :param task_id: id of the task to delete
    :param start_time: start time of the task to delete
    :param date: date of the task to delete
    :return: nothing
    side effect: deletes an ongoing task with given identifiers from the database
    """
    cursor: sqlite3.Cursor = conn.cursor()
    sql: str = "DELETE FROM weeks WHERE task_id = {} AND start_time = {} AND date = {}". \
        format(task_id, start_time, date)
    cursor.execute(sql)
    conn.commit()
