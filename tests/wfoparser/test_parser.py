import unittest
from py3dengine.parser import Parser
from py3dengine.wfotoken import WFOToken
from py3dengine.wfostatement import WfoStatement


class ParserTests(unittest.TestCase):
    def test_parse_single_statement(self):
        # data_set[0] is token_list
        # data_set[1] is expected statement type
        # data_set[2] is expected statement data
        for data_set in [
            # "# HI"
            (
                [
                    WFOToken(WFOToken.Types.COMMENT, 1, 1, '# HI'),
                    WFOToken(WFOToken.Types.LINE_BREAK, 1, 5, '\n')
                ],
                WfoStatement.Types.COMMENT,
                '# HI',
            ),

            # "mtllib something.mtl
            (
                [
                    WFOToken(WFOToken.Types.MATERIAL_LIB, 1, 1),
                    WFOToken(WFOToken.Types.SEPERATOR, 1, 7),
                    WFOToken(WFOToken.Types.STRING, 1, 8, "something.mtl"),
                    WFOToken(WFOToken.Types.LINE_BREAK, 1, 21, '\n'),
                ],
                WfoStatement.Types.MATERIAL_LIB,
                'something.mtl',
            ),

            # "o object_name"
            (
                [
                    WFOToken(WFOToken.Types.OBJECT, 1, 1),
                    WFOToken(WFOToken.Types.SEPERATOR, 1, 2),
                    WFOToken(WFOToken.Types.STRING, 1, 3, 'object_name'),
                    WFOToken(WFOToken.Types.LINE_BREAK, 1, 14, '\n')
                ],
                WfoStatement.Types.OBJECT,
                'object_name',
            ),

            # "v 1.0 2.0 3.0"
            (
                [
                    WFOToken(WFOToken.Types.VERTEX, 1, 1),
                    WFOToken(WFOToken.Types.SEPERATOR, 1, 2),
                    WFOToken(WFOToken.Types.NUMBER, 1, 3, 1.0),
                    WFOToken(WFOToken.Types.SEPERATOR, 1, 6),
                    WFOToken(WFOToken.Types.NUMBER, 1, 7, 2.0),
                    WFOToken(WFOToken.Types.SEPERATOR, 1, 10),
                    WFOToken(WFOToken.Types.NUMBER, 1, 11, 3.0),
                    WFOToken(WFOToken.Types.LINE_BREAK, 1, 14, '\n'),
                ],
                WfoStatement.Types.VERTEX,
                (1.0, 2.0, 3.0),
            ),

            # "vn 1.0 2.0 3.0"
            (
                [
                    WFOToken(WFOToken.Types.NORMAL, 1, 1),
                    WFOToken(WFOToken.Types.SEPERATOR, 1, 2),
                    WFOToken(WFOToken.Types.NUMBER, 1, 3, 1.0),
                    WFOToken(WFOToken.Types.SEPERATOR, 1, 6),
                    WFOToken(WFOToken.Types.NUMBER, 1, 7, 2.0),
                    WFOToken(WFOToken.Types.SEPERATOR, 1, 10),
                    WFOToken(WFOToken.Types.NUMBER, 1, 11, 3.0),
                    WFOToken(WFOToken.Types.LINE_BREAK, 1, 14, '\n'),
                ],
                WfoStatement.Types.NORMAL,
                (1.0, 2.0, 3.0),
            ),

            # "vt 1.0 2.0"
            (
                [
                    WFOToken(WFOToken.Types.TEX_COORD, 1, 1),
                    WFOToken(WFOToken.Types.SEPERATOR, 1, 2),
                    WFOToken(WFOToken.Types.NUMBER, 1, 3, 1.0),
                    WFOToken(WFOToken.Types.SEPERATOR, 1, 6),
                    WFOToken(WFOToken.Types.NUMBER, 1, 7, 2.0),
                    WFOToken(WFOToken.Types.LINE_BREAK, 1, 14, '\n'),
                ],
                WfoStatement.Types.TEX_COORD,
                (1.0, 2.0),
            ),

            # "usemtl name"
            (
                [
                    WFOToken(WFOToken.Types.USE_MATERIAL, 1, 1),
                    WFOToken(WFOToken.Types.SEPERATOR, 1, 7),
                    WFOToken(WFOToken.Types.STRING, 1, 8, 'name'),
                    WFOToken(WFOToken.Types.LINE_BREAK, 1, 12, '\n'),
                ],
                WfoStatement.Types.USE_MATERIAL,
                'name',
            ),

            # "f 1/2/3 4/5/6 7/8/9"
            (
                [
                    WFOToken(WFOToken.Types.FACE, 1, 1),
                    WFOToken(WFOToken.Types.SEPERATOR, 1, 2),
                    WFOToken(WFOToken.Types.POLYGON, 1, 3, (1, 2, 3)),
                    WFOToken(WFOToken.Types.SEPERATOR, 1, 8),
                    WFOToken(WFOToken.Types.POLYGON, 1, 9, (4, 5, 6)),
                    WFOToken(WFOToken.Types.SEPERATOR, 1, 14),
                    WFOToken(WFOToken.Types.POLYGON, 1, 15, (7, 8, 9)),
                    WFOToken(WFOToken.Types.LINE_BREAK, 1, 20, '\n'),
                ],
                WfoStatement.Types.FACE,
                ((1, 2, 3), (4, 5, 6), (7, 8, 9)),
            ),

            # "s 1"
            (
                [
                    WFOToken(WFOToken.Types.SMOOTH_SHADING, 1, 1),
                    WFOToken(WFOToken.Types.SEPERATOR, 1, 2),
                    WFOToken(WFOToken.Types.NUMBER, 1, 3, 1.0),
                    WFOToken(WFOToken.Types.LINE_BREAK, 1, 4, '\n'),
                ],
                WfoStatement.Types.SMOOTH_SHADING,
                1.0,
            )
        ]:
            with self.subTest(dataset=data_set):
                parser = Parser()
                statements = parser.parse_tokens(data_set[0])

                self.assertEqual(len(statements), 1, 'Parser returns 1 result')
                self._assert_statement(statements[0], data_set[1], data_set[2])

    def _assert_statement(self, statement: WfoStatement, statement_type, data):
        self.assertIsInstance(statement, WfoStatement, 'Parser returns statement')
        self.assertEqual(statement_type, statement.type, 'Parser returns correct statement type')
        self.assertEqual(data, statement.data)
