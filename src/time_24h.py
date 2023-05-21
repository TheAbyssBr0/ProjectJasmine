import datetime
from typing import Tuple, List

from data_structs import WeekView


def _now_and_last_monday() -> Tuple[datetime.datetime, datetime.datetime]:
    """
    :return: returns date time of today (now) and last monday, two very useful times
    """
    time_now: datetime.datetime = datetime.datetime.now()
    last_monday: datetime.datetime = time_now - datetime.timedelta(days=time_now.weekday())
    return time_now, last_monday


def get_time() -> int:
    """
    :return: Time right now in 24-hour military time HHMM format
    """
    time_now, _ = _now_and_last_monday()
    time_formatted_24h: str = time_now.strftime("%H%M")
    return int(time_formatted_24h)


def get_date() -> int:
    """
    :return: Gets today's date in yyyymmDDd format (DD = last monday date) (d = days since monday)
    """
    time_now, last_monday = _now_and_last_monday()
    formatted_date_string = last_monday.strftime("%Y%m%d") + str(datetime.timedelta(days=time_now.weekday()).days)
    return int(formatted_date_string)


def get_weekname() -> str:
    """
    :return: Returns the week name in the format of Week of (monday date) (month) (year)
    """
    _, last_monday = _now_and_last_monday()
    return last_monday.strftime("Week of %d %b '%y")


def get_time_diff(weekview: WeekView) -> int:
    """
    Computes the amount of time spent in a task
    :param weekview: the task as a weekview object
    :return: amount of time in 24-hour military format HHMM
    """
    if weekview.end_time == 0:
        return 0

    hour_diff = (weekview.end_time // 100 - weekview.start_time // 100) * 100
    min_diff = weekview.end_time % 100 - weekview.start_time % 100

    if min_diff < 0:
        min_diff += 60
        hour_diff -= 100

    return hour_diff + min_diff


def _add_time(time1: int, time2: int) -> int:
    """
    Adds some amount of time to some other amount of time
    :param time1: some amount of time
    :param time2: some other amount of time
    :return: total amount of time between the two amounts of time
    """
    hours = (time1 // 100 + time2 // 100) * 100
    minutes = time1 % 100 + time2 % 100

    while minutes >= 60:
        minutes -= 60
        hours += 100

    return hours + minutes


def get_total_time_from_list(weekview_list: List[WeekView]) -> int:
    """
    Computes the total time spent from a list of tasks
    :param weekview_list: Python list of tasks as WeekView objects
    :return: total time spent in the tasks in 24-hour military time
    """
    total_time = 0
    for day in weekview_list:
        time_diff = get_time_diff(day)
        total_time = _add_time(time_diff, total_time)
    return total_time
