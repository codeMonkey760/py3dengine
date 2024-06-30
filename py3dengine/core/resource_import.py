_importers = {}


class ResourceImportError(Exception):
    pass


def register_importer(_func=None, obj_type=None):
    if _func is None or obj_type is None:
        return _func

    if obj_type in _importers.keys():
        raise ValueError(f'An importer for type \'{obj_type.__name__}\' has already been registered')

    _importers[obj_type] = _func

    return _func


def get_importer(obj_type):
    if obj_type not in _importers.keys():
        raise ValueError(f'An importer for type \'{obj_type.__name__}\' is not registered')
