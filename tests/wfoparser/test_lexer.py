import unittest
from io import StringIO
from py3dengine.lexer import Lexer
from py3dengine.wfotoken import WFOToken


class LexerTests(unittest.TestCase):
    def test_lex_single_token(self):
        for data_set in [
            ('# This is a comment', WFOToken.Types.COMMENT, '# This is a comment'),
            ('mtllib', WFOToken.Types.MATERIAL_LIB, None),
            ('o', WFOToken.Types.OBJECT, None),
            ('v', WFOToken.Types.VERTEX, None),
            ('vn', WFOToken.Types.NORMAL, None),
            ('vt', WFOToken.Types.TEX_COORD, None),
            ('usemtl', WFOToken.Types.USE_MATERIAL, None),
            ('f', WFOToken.Types.FACE, None),
            ('s', WFOToken.Types.SMOOTH_SHADING, None),
            ('1.00', WFOToken.Types.NUMBER, 1.0),
            ('asdf', WFOToken.Types.STRING, 'asdf'),
            ('1/2/3', WFOToken.Types.POLYGON, (1, 2, 3)),
            ('1//2', WFOToken.Types.POLYGON, (1, None, 2)),
            ('1/3/', WFOToken.Types.POLYGON, (1, 3, None)),
            (' ', WFOToken.Types.SEPERATOR, None),
            ('    ', WFOToken.Types.SEPERATOR, None),
            ('\n', WFOToken.Types.LINE_BREAK, '\n'),
            ('\r', WFOToken.Types.LINE_BREAK, '\r'),
            ('\n\r', WFOToken.Types.LINE_BREAK, '\n\r'),
        ]:
            with self.subTest(data_set=data_set):
                lexer = Lexer(StringIO(data_set[0]))
                tokens = lexer.lex_tokens()

                self.assertEqual(len(tokens), 1, 'Lexer returns 1 result')
                actual_token = tokens[0]
                self._assert_token(actual_token, data_set[1], data_set[2], 1, 1)

    def test_multiple_line_endings(self):
        test_data = '\r\n\r\n\n\r\n\n\r\r'  # very unlikely but it should handle it

        lexer = Lexer(StringIO(test_data))
        tokens = lexer.lex_tokens()

        self.assertEqual(len(tokens), 7, 'Lexer returns all results')
        self._assert_token(tokens[0], WFOToken.Types.LINE_BREAK, '\r', 1, 1)
        self._assert_token(tokens[1], WFOToken.Types.LINE_BREAK, '\n\r', 2, 1)
        self._assert_token(tokens[2], WFOToken.Types.LINE_BREAK, '\n', 3, 1)
        self._assert_token(tokens[3], WFOToken.Types.LINE_BREAK, '\n\r', 4, 1)
        self._assert_token(tokens[4], WFOToken.Types.LINE_BREAK, '\n', 5, 1)
        self._assert_token(tokens[5], WFOToken.Types.LINE_BREAK, '\n\r', 6, 1)
        self._assert_token(tokens[6], WFOToken.Types.LINE_BREAK, '\r', 7, 1)

    def test_lex_multiple_tokens_same_line(self):
        test_data = 'v 0.00 1.00 2.00\n'

        lexer = Lexer(StringIO(test_data))
        tokens = lexer.lex_tokens()

        self.assertEqual(len(tokens), 8, 'Lexer returns all results')
        self._assert_token(tokens[0], WFOToken.Types.VERTEX, None, 1, 1)
        self._assert_token(tokens[1], WFOToken.Types.SEPERATOR, None, 1, 2)
        self._assert_token(tokens[2], WFOToken.Types.NUMBER, 0.0, 1, 3)
        self._assert_token(tokens[3], WFOToken.Types.SEPERATOR, None, 1, 7)
        self._assert_token(tokens[4], WFOToken.Types.NUMBER, 1.0, 1, 8)
        self._assert_token(tokens[5], WFOToken.Types.SEPERATOR, None, 1, 12)
        self._assert_token(tokens[6], WFOToken.Types.NUMBER, 2.0, 1, 13)
        self._assert_token(tokens[7], WFOToken.Types.LINE_BREAK, '\n', 1, 17)

    def test_lex_multiple_tokens_multiple_lines(self):
        test_data = '# First line comment\nv 0.00 1.00 2.00\nusemtl some-material\n\ns 1\n'

        lexer = Lexer(StringIO(test_data))
        tokens = lexer.lex_tokens()

        self.assertEqual(len(tokens), 19, 'Lexer returns all results')
        self._assert_token(tokens[0], WFOToken.Types.COMMENT, '# First line comment', 1, 1)
        self._assert_token(tokens[1], WFOToken.Types.LINE_BREAK, '\n', 1, 21)

        self._assert_token(tokens[2], WFOToken.Types.VERTEX, None, 2, 1)
        self._assert_token(tokens[3], WFOToken.Types.SEPERATOR, None, 2, 2)
        self._assert_token(tokens[4], WFOToken.Types.NUMBER, 0.0, 2, 3)
        self._assert_token(tokens[5], WFOToken.Types.SEPERATOR, None, 2, 7)
        self._assert_token(tokens[6], WFOToken.Types.NUMBER, 1.0, 2, 8)
        self._assert_token(tokens[7], WFOToken.Types.SEPERATOR, None, 2, 12)
        self._assert_token(tokens[8], WFOToken.Types.NUMBER, 2.0, 2, 13)
        self._assert_token(tokens[9], WFOToken.Types.LINE_BREAK, '\n', 2, 17)

        self._assert_token(tokens[10], WFOToken.Types.USE_MATERIAL, None, 3, 1)
        self._assert_token(tokens[11], WFOToken.Types.SEPERATOR, None, 3, 7)
        self._assert_token(tokens[12], WFOToken.Types.STRING, 'some-material', 3, 8)
        self._assert_token(tokens[13], WFOToken.Types.LINE_BREAK, '\n', 3, 21)

        self._assert_token(tokens[14], WFOToken.Types.LINE_BREAK, '\n', 4, 1)

        self._assert_token(tokens[15], WFOToken.Types.SMOOTH_SHADING, None, 5, 1)
        self._assert_token(tokens[16], WFOToken.Types.SEPERATOR, None, 5, 2)
        self._assert_token(tokens[17], WFOToken.Types.NUMBER, 1.0, 5, 3)
        self._assert_token(tokens[18], WFOToken.Types.LINE_BREAK, '\n', 5, 4)

    def _assert_token(self, actual_token, token_type, data, line_number=None, char_number=None):
        self.assertIsInstance(actual_token, WFOToken, 'Lexer returns token')

        self.assertEqual(token_type, actual_token.type, 'Lexer returns correct token type')

        self.assertEqual(data, actual_token.data, 'Token has correct data')

        if line_number is not None:
            self.assertEqual(line_number, actual_token.line_number, 'Token has correct line number')

        if char_number is not None:
            self.assertEqual(char_number, actual_token.char_number, 'Token has correct char position')
