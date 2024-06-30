import json
from py3dengine.resource_import import register_importer, get_importer
from py3dengine.transform import Transform
from py3dengine.json_util import fetch_property, fetch_optional_property
from py3dengine.message_handler import MessageHandler
from py3dengine.nameable import Nameable


class GameObject(Nameable, MessageHandler):
    """GameObject: A representation of a single object in a 3d environment

    GameObjects serve two primary purposes:
    1) To serve as a node in a hierarchically organized data storage structure for scenes. Scenes are a single k tree
    containing all objects in a 3d environment. GameObjects are nodes in that k tree.
    2) To provide a message passing and propagation interface so that nodes in the scene can communicate
    with each other.
    """

    def __init__(self, name):
        Nameable.__init__(self, name)
        MessageHandler.__init__(self)
        self._parent = None
        self._transform = None
        self._children = []
        self._components = []

    @property
    def parent(self):
        return self._parent

    @property
    def get_transform(self):
        return self._transform

    def bubble_message(self, message):
        """Pass a message up to the root GameObject and then to the entire scene graph"""
        if self.parent is not None:
            self.parent.bubble_message(message)
        else:
            self.receive_message(message)

    def receive_message(self, message):
        """Process a message, then propagate based on message settings"""
        if not self.accept_message(message):
            return

        for component in self._components:
            component.receive_message(message)

        if message.propagate is False:
            return

        for child in self._children:
            child.receive_message(child)

    def accept_message(self, message):
        if message.name in self._message_type_white_list:
            return True

        if message.name in self._message_type_black_list:
            return False

        return self._message_type_accept_by_default

    def attach_child(self, new_child):
        if new_child in self._children:
            raise ValueError('Child is already attached')

        self._children.append(new_child)
        new_child._parent = self
        # TODO: emit event?

    def detach_child(self, child):
        if child not in self._children:
            raise ValueError('Child is not attached and cannot be detached')

        self._children.remove(child)
        child._parent = None

    def get_child_by_index(self, index):
        return self._children[index]

    def get_child_by_name(self, name, search_children=True):
        for c in self._children:
            if c.name == name:
                return c

        if not search_children:
            return None

        for c in self._children:
            target = c.get_child_by_name(name)
            if target is not None:
                return target

        return None

    def get_child_count(self):
        return len(self._children)

    def attach_component(self, new_component):
        if new_component in self._components:
            raise ValueError('Child is already attached')

        self._components.append(new_component)
        new_component.owner = new_component
        # TODO: emit event?

    def detach_component(self, component):
        if component not in self._components:
            raise ValueError('Component is not attached and cannot be detached')

        self._components.remove(component)
        component.owner = None

    def get_component_by_type(self, component_type: type):
        for c in self._components:
            if type(c) is component_type:
                return c

        return None

    def get_component_by_index(self, index):
        try:
            return self._components[index]
        except IndexError:
            return None

    def get_component_by_name(self, name):
        for c in self._components:
            if c.name == name:
                return c

        return None

    def get_component_count(self):
        return len(self._components)


def import_game_object(descriptor):
    description = json.load(descriptor)
    file_path = description['file_path']

    resource_file = open(file_path)
    game_object_json = json.load(resource_file)
    resource_file.close()

    return _do_import_game_object(game_object_json)


def _do_import_game_object(game_object_json):
    name = fetch_property(game_object_json, 'name', str)
    transform = fetch_property(game_object_json, 'transform', dict)
    components = fetch_property(game_object_json, 'components', list)
    children = fetch_property(game_object_json, 'children', list)
    white_list = fetch_optional_property(game_object_json, 'msg_white_list', list, [])
    black_list = fetch_optional_property(game_object_json, 'msg_black_list', list, [])
    accept = fetch_optional_property(game_object_json, 'accept_msgs', bool, True)

    new_go = GameObject(name)
    new_go._message_type_white_list = white_list
    new_go._message_type_black_list = black_list
    new_go._message_type_accept_by_default = accept

    import_transform = get_importer(Transform)
    new_go._transform = import_transform(transform)

    new_components = []
    for component_json in components:
        type_name = fetch_property(component_json, 'type', str)
        component_importer = get_importer(type_name)
        new_component = component_importer(component_json)
        new_component._owner = new_go
        new_components.append(new_component)
    new_go._components = new_components

    new_children = []
    for child_json in children:
        new_child = _do_import_game_object(child_json)
        new_child._parent = new_go
        new_children.append(new_child)
    new_go._children = new_children

    return new_go


register_importer(GameObject, import_game_object)
