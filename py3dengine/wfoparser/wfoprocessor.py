from typing import List
from py3dengine.wfostatement import WfoStatement


class WfoProcessor:
    def __init__(self):
        self._material_files = []

    def process_statements(self, statements: List[WfoStatement]):
        for statement in statements:
            self._process_statement(statement)

    def get_material_files(self):
        return tuple(self._material_files)

    def _process_statement(self, statement: WfoStatement):
        if statement.type == WfoStatement.Types.MATERIAL_LIB:
            self._material_files.append(statement.data)
