from enum import Enum


class WfoStatement:
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

    def __init__(self, statement_type, data, line_num, start_at_char):
        self.type = statement_type
        self.data = data
        self.line_num = line_num
        self.start_at_char = start_at_char
