from typing import List


class Task:
    def __init__(self, identity: int, title: str, super_task_id: int, checked: bool) -> None:
        """
        Task object, belongs to a supertask and is either done or not done
        :param identity: id of the task (aka task_id)
        :param title: title of the task
        :param super_task_id: id of the associated super task
        :param checked: whether the task is done or not
        """
        self.identity = identity
        self.title = title
        self.super_task_id = super_task_id
        self.checked = checked

    def __str__(self) -> str:
        """
        :return: string representation of the task object
        """
        return "[{}] {}".format("X" if self.checked else " ", self.title)


class SuperTask:
    def __init__(self, identity: int, title: str, tasks: List[Task]) -> None:
        """
        Contains many tasks which are either done or not done
        :param identity: id of the super task (super_task_id)
        :param title: title of the supertask
        :param tasks: Python list of Task objects associated with this super task
        """
        self.identity = identity
        self.title = title
        self.tasks = tasks

    def __str__(self) -> str:
        """
        :return: string representation of the super task object
        """
        return_string: str = "{}".format(self.title)
        for task in self.tasks:
            return_string += "\n  " + str(task)
        return return_string


class WeekView:
    def __init__(self, task_id: int, title: str, start_time: int, end_time: int) -> None:
        """
        Alternative task object which is like a pointer to a task belonging to a supertask. Has additional fields
        like the time this task was started and the time it ended. Also called ongoing task
        :param task_id: id of the task that's about to start
        :param title: title of the task
        :param start_time: start time of the task
        :param end_time: end time of the task
        """
        self.task_id = task_id
        self.title = title
        self.start_time = start_time
        self.end_time = end_time

    def __str__(self) -> str:
        """
        :return: string representation of week view object
        """
        len_title: int = len(self.title)
        padding: int = 50
        return self.title + " " * abs(padding - len_title) + "{:04} - {:04}" \
            .format(self.start_time, "----" if self.end_time == 0 else self.end_time)
