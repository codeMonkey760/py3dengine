class ResourceManager:
    def __init__(self):
        self._store = {}

    def get_resource_by_name(self, name):
        return self._store[name]

    def store_resource(self, resource):
        name = resource.name

        if name in self._store.keys():
            raise ValueError(f'Resource with name "{name}" is already stored')

        self._store[name] = resource
