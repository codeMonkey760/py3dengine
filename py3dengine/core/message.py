from copy import copy


class Message:
    """Message: A message representing an event in the game engine

    Create a new message and then supply it to a message propagation function
    """

    def __init__(self, name, data=None, propagate=True):
        self._name = str(name)
        self._data = copy(data)
        self._propagate = bool(propagate)

    @property
    def name(self):
        return self._name

    @property
    def data(self):
        return copy(self._data)

    @property
    def propagate(self):
        return self._propagate
