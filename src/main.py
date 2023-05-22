import curses
import sqlite3
import sys
import os
from typing import List

import data_structs
import database
import time_24h

# PSA: all the magic numbers here are mostly padding. Too tired to label all. Tread carefully. goodluck.

os.chdir(os.path.dirname(sys.executable))           # sets the cwd to path to executable
db: sqlite3.Connection = database.get_database_connection("journal.db")
super_task_list: List[data_structs.SuperTask] = database.read_super_tasks(db.cursor())
week_list: List[data_structs.WeekView] = database.read_all_tasks_from_date(db.cursor(), time_24h.get_date())


def _print_super_task():
    """
    Prints super tasks in the three super task windows
    :return: nothing
    side effect: prints to CLI
                 updates global variable super_task_list
    """
    super_task_list_in: List[data_structs.SuperTask] = database.read_super_tasks(db.cursor())
    for index, item in enumerate(super_task_windows):
        item.erase()

        if len(super_task_list_in) <= index:
            item.addstr(super_task_height // 2, (super_task_width // 2) - 2, "Empty")
        else:
            item.addstr(1, 2, super_task_list_in[index].__str__())
        item.box()
        item.refresh()

    global super_task_list
    super_task_list = super_task_list_in


def _print_weekview():
    """
    Prints ongoing tasks in the ongoing tasks window
    :return: nothing
    side effect: prints to CLI
                 updates global variable week_list
    """
    ongoing_task_win.erase()
    global week_list
    week_list = database.read_all_tasks_from_date(db.cursor(), time_24h.get_date())

    ongoing_time = time_24h.get_total_time_from_list(week_list)

    ongoing_task_win.addstr(1, 1, "Today" + " " * 42 + "Time today: " + "{:04}".format(ongoing_time))
    for index, day in enumerate(week_list):
        ongoing_task_win.addstr(2 + index, 3, day.__str__())

    ongoing_task_win.box()
    ongoing_task_win.refresh()


def _exit_curses():
    """
    Returns terminal variables to normal and exits out of the program with success signal
    """
    curses.nocbreak()
    curses.echo()
    curses.endwin()
    sys.exit(0)


def _task_id_from_user(st_index, t_index) -> int:
    """
    calculates the real task ID from apparent task ID as shown in terminal
    :param st_index: apparent super task index
    :param t_index: apparent task index
    :return: real index of the task as it is in the database
    """
    return int(super_task_list[int(st_index) - 1].tasks[int(t_index) - 1].identity)


def _create_command_processor(command_list):
    """
    Helper function for processing CREATE commands given by the user
    :param command_list: command from the user split by " "
    :return: response to user
    """
    if command_list[1].lower() == "supertask":
        title = " ".join(command_list[2:])
        database.create_super_task(db, title)
        _print_super_task()
        return f"Created super task: {title}"

    elif command_list[1].lower() == "task":
        super_task_id = int(command_list[2])
        title = " ".join(command_list[3:])
        database.create_task(db, super_task_id, title)
        _print_super_task()
        return f"Created task: {title} under id: {super_task_id}"

    elif command_list[1].lower() == "ongoing":
        task_id = _task_id_from_user(command_list[2], command_list[3])
        database.create_ongoing_task(db, task_id)
        _print_weekview()
        return f"Created new ongoing task with task id: {task_id}"
    else:
        raise IndexError  # invalid command


def _delete_command_processor(command_list):
    """
    Helper function for processing DELETE commands given by the user
    :param command_list: command from the user split by " "
    :return: response to user
    """
    if command_list[1].lower() == "supertask":
        index = super_task_list[int(command_list[2]) - 1].identity
        database.delete_super_task(db, index)
        _print_super_task()
        return f"Deleted super task index: {index}"

    elif command_list[1].lower() == "task":
        index = _task_id_from_user(command_list[2], command_list[3])
        database.delete_task(db, index)
        _print_super_task()
        return f"Deleted task index: {index}"

    elif command_list[1].lower() == "ongoing":
        index = int(command_list[2]) - 1
        database.delete_ongoing_task(db, week_list[index].task_id, week_list[index].start_time,
                                     time_24h.get_date())
        _print_weekview()
        return f"Deleted ongoing task with task id: {index}"
    else:
        raise IndexError  # invalid command


def _update_command_processor(command_list):
    """
    Helper function for processing UPDATE commands given by the user
    :param command_list: command from the user split by " "
    :return: response to user
    """
    if command_list[1].lower() == "supertask":
        index = super_task_list[int(command_list[2]) - 1].identity
        title = " ".join(command_list[3:])
        database.update_super_task_title(db, index, title)
        _print_super_task()
        return f"Renamed super task with index: {index} to {title}"

    elif command_list[1].lower() == "task":
        index = _task_id_from_user(command_list[2], command_list[3])
        title = " ".join(command_list[4:])
        database.update_task_title(db, index, title)
        _print_super_task()
        return f"Renamed task with index: {index} to {title}"

    elif command_list[1].lower() == "toggle":
        index = _task_id_from_user(command_list[2], command_list[3])
        database.update_task_checkmark_toggle(db, index)
        _print_super_task()
        return f"Toggled task with index: {index}"

    elif command_list[1].lower() == "ongoing":
        if command_list[2].lower() == "end":
            index = int(command_list[3]) - 1
            database.update_ongoing_task(db, week_list[index].task_id, week_list[index].start_time,
                                         new_end_time=time_24h.get_time())
            _print_weekview()
            return f"Added end time to ongoing task with task id: {index}"

        elif command_list[2].lower() == "se":
            index = int(command_list[3]) - 1
            new_start = int(command_list[4])
            new_end = int(command_list[5])
            database.update_ongoing_task(db, week_list[index].task_id, week_list[index].start_time,
                                         new_start_time=new_start, new_end_time=new_end)
            _print_weekview()
            return f"Edited ongoing task times with task id: {index}"
    else:
        raise IndexError  # invalid command



def _process_command(cmd: str) -> str:
    """
    Processes command given by the user
    :param cmd: command from the user
    :return: helpful message response from command
    """
    command_list = cmd.split(' ')

    try:
        if command_list[0].lower() == "create":
            return _create_command_processor(command_list)
        elif command_list[0].lower() == "delete":
            return _delete_command_processor(command_list)
        elif command_list[0].lower() == "update":
            return _update_command_processor(command_list)
        elif command_list[0].lower() == "exit":
            _exit_curses()
    except IndexError:
        return "Error in command. Try again."
    except ValueError:
        return "Expected number!"
    else:
        return "Unknown command."


if __name__ == '__main__':
    stdscr = curses.initscr()
    curses.noecho()
    curses.cbreak()

    title_height = 1
    title_width = curses.COLS
    ongoing_task_height = 25
    super_task_height = curses.LINES - 3 - ongoing_task_height
    super_task_width = curses.COLS // 3
    ongoing_task_width = curses.COLS
    command_height = 1
    command_width = curses.COLS
    title_y = 0
    title_x = 0
    super_task_1_y = title_height
    super_task_1_x = 0
    super_task_2_y = title_height
    super_task_2_x = super_task_width
    super_task_3_y = title_height
    super_task_3_x = super_task_width * 2
    ongoing_task_y = title_height + super_task_height
    ongoing_task_x = 0
    command_y = ongoing_task_y + ongoing_task_height
    command_x = 0

    title_win = curses.newwin(title_height, title_width, title_y, title_x)
    super_task_win_1 = curses.newwin(super_task_height, super_task_width, super_task_1_y, super_task_1_x)
    super_task_win_2 = curses.newwin(super_task_height, super_task_width, super_task_2_y, super_task_2_x)
    super_task_win_3 = curses.newwin(super_task_height, super_task_width, super_task_3_y, super_task_3_x)
    ongoing_task_win = curses.newwin(ongoing_task_height, ongoing_task_width, ongoing_task_y, ongoing_task_x)
    command_win = curses.newwin(command_height, command_width, command_y, command_x)

    title_win.bkgd(curses.A_NORMAL)
    super_task_win_1.bkgd(curses.A_NORMAL)

    super_task_win_2.bkgd(curses.A_NORMAL)
    super_task_win_3.bkgd(curses.A_NORMAL)
    ongoing_task_win.bkgd(curses.A_NORMAL)
    ongoing_task_win.box()
    command_win.bkgd(curses.A_NORMAL)

    title_win.addstr(0, 2, time_24h.get_weekname())
    command_win.addstr(0, 2, "> ")

    super_task_windows = [super_task_win_1, super_task_win_2, super_task_win_3]
    _print_super_task()
    _print_weekview()

    super_task_win_1.box()
    super_task_win_2.box()
    super_task_win_3.box()

    title_win.refresh()
    super_task_win_1.refresh()
    super_task_win_2.refresh()
    super_task_win_3.refresh()
    ongoing_task_win.refresh()
    command_win.refresh()

    while True:
        command_win.move(0, 4)
        command = ""
        while True:
            ch = command_win.getch()
            if ch == ord("\n"):
                break
            elif ch == curses.KEY_BACKSPACE or ch == ord("\b") or ch == ord("\x7f"):
                command = command[:-1]
                command_win.delch(0, len(command) + 4)
            else:
                command += chr(ch)
                command_win.addch(ch)

        message = _process_command(command)

        command_win.clear()
        command_win.addstr(0, 2, "> " + " " * (curses.COLS - len(message) - 10) + message)

        command_win.refresh()
