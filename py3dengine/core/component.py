from py3dengine.message_handler import MessageHandler


class Component(MessageHandler):
    def __init__(self, name):
        super().__init__()
        self._name = str(name)
        self._owner = None
        self._message_type_white_list = []
        self._message_type_black_list = []
        self._message_type_accept_by_default = True

    @property
    def name(self):
        return self._name

    def bubble_message(self, message):
        """Convenience function for bubbling the message up to the owning GameObject"""
        if self._owner is None:
            return

        self._owner.bubble_message(message)

    def accept_message(self, message):
        try:
            handler = getattr(self, message.name)
        except AttributeError:
            return False

        if not callable(handler):
            return False

        if message.name in self._message_type_white_list:
            return True

        if message.name in self._message_type_black_list:
            return False

        return self._message_type_accept_by_default

    def receive_message(self, message):
        if not self.accept_message(message):
            return

        if message.data is None:
            args = ()
        else:
            try:
                args = (*message.data,)
            except TypeError:
                args = (message.data,)

        getattr(self, message.name)(*args)
