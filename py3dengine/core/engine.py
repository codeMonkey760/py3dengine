from typing import Dict

from core import get_importer
from py3dengine.gameobject import GameObject
from py3dengine.message import Message

_scenes: Dict[str, GameObject] = {}
_active_scene: GameObject | None = None


class SceneError(Exception):
    pass


def register_event_generator():
    pass


def load_scene(path):
    global _scenes

    importer = get_importer(GameObject)

    try:
        descriptor_file = open(path)
    except OSError as err:
        raise SceneError(err)

    try:
        root_go: GameObject = importer(descriptor_file)
    except OSError as err:
        raise SceneError(err)
    except ValueError as err:
        raise SceneError(err)
    except TypeError as err:
        raise SceneError(err)
    except KeyError as err:
        raise SceneError(err)
    descriptor_file.close()

    if root_go.name in _scenes.keys():
        raise SceneError(f'Cannot load scene "{root_go.name}", scene already loaded')
    _scenes[root_go.name] = root_go

    root_go.receive_message(Message('loading'))


def unload_scene(name):
    global _scenes

    if name not in _scenes.keys():
        raise SceneError(f'Cannot unload scene "{name}", scene is not loaded')

    _scenes[name].receive_message(Message('unloading'))
    del _scenes[name]


def activate_scene(scene: GameObject):
    global _active_scene

    if _active_scene is not None:
        _active_scene.receive_message(Message('deactivating'))

    scene.receive_message(Message('activating'))
    _active_scene = scene


def run():
    pass
