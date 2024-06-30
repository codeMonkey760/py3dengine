from abc import ABC, abstractmethod


class MessageHandler(ABC):
    def __init__(self):
        self._message_type_white_list = []
        self._message_type_black_list = []
        self._message_type_accept_by_default = True

    @property
    def accept_messages_by_default(self):
        return self._message_type_accept_by_default

    @accept_messages_by_default.setter
    def accept_messages_by_default(self, new_value):
        self._message_type_accept_by_default = bool(new_value)

    @abstractmethod
    def bubble_message(self, message):
        pass

    @abstractmethod
    def receive_message(self, message):
        pass

    @abstractmethod
    def accept_message(self, message):
        pass

    def add_message_filter(self, message_type, white_list):
        filter_list = self._message_type_white_list if white_list else self._message_type_black_list

        if message_type in filter_list:
            raise ValueError('Cannot add a filter that is already registered')

        filter_list.append(message_type)

    def remove_message_filter(self, message_type, white_list):
        filter_list = self._message_type_white_list if white_list else self._message_type_black_list

        if message_type not in filter_list:
            raise ValueError('Cannot remove a filter that is not registered')

        filter_list.remove(message_type)
