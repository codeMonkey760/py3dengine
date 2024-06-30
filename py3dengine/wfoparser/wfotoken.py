from enum import Enum


class WFOToken:
    class Types(Enum):
        COMMENT = 1
        MATERIAL_LIB = 2
        OBJECT = 3
        VERTEX = 4
        NORMAL = 5
        TEX_COORD = 6
        USE_MATERIAL = 7
        FACE = 8
        SMOOTH_SHADING = 9
        NUMBER = 10
        STRING = 11
        POLYGON = 12
        SEPERATOR = 13
        LINE_BREAK = 14

    token_type_mapping = {
        'mtllib': Types.MATERIAL_LIB,
        'o': Types.OBJECT,
        'v': Types.VERTEX,
        'vn': Types.NORMAL,
        'vt': Types.TEX_COORD,
        'usemtl': Types.USE_MATERIAL,
        'f': Types.FACE,
        's': Types.SMOOTH_SHADING,
    }

    def __init__(self, token_type, line_number, char_number, data=None):
        self.type = token_type
        self.data = data
        self.line_number = line_number
        self.char_number = char_number
