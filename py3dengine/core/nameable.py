class Nameable:
    def __init__(self, name):
        self._name = str(name)

    @property
    def name(self):
        return self._name
