from resource_import import register_importer
from py3dengine.math import Vector3, Quaternion, Matrix4x4
from py3dengine.json_util import fetch_property


class Transform:
    def __init__(self, position=Vector3(), orientation=Quaternion(), scale=Vector3(1, 1, 1)):
        self._position = Vector3(position)
        self._orientation = Quaternion(orientation)
        self._scale = Vector3(scale)

        self._w_mtx = None
        self._wit_mtx = None
        self._refresh_matrix_cache()

    @property
    def position(self):
        return self._position

    @property
    def orientation(self):
        return self._orientation

    @property
    def scale(self):
        return self._scale

    @property
    def world_matrix(self):
        return self._w_mtx

    @property
    def wit_matrix(self):
        return self._wit_mtx

    def _refresh_matrix_cache(self):
        s = Matrix4x4.Scaling(self._scale)
        r = Matrix4x4.RotationQuaternion(self._orientation)
        t = Matrix4x4.Translation(self._position)

        # TODO: this is horribly inefficient and should be re-written in c math lib
        self._w_mtx = (s * r) * t
        self._wit_matrix = self._w_mtx.inverse()


@register_importer(Transform)
def import_transform(descriptor, transform_json):
    t = Transform()

    t._position = Vector3(fetch_property(transform_json, 'position', dict))
    t._orientation = Quaternion(fetch_property(transform_json, 'orientation', dict))
    t._scale = Vector3(fetch_property(transform_json, 'scale', dict))

    return t
