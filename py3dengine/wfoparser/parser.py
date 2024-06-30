from typing import Sequence

from py3dengine.wfostatement import WfoStatement
from py3dengine.wfotoken import WFOToken

_token_statement_type_mapping = {
    WFOToken.Types.COMMENT: WfoStatement.Types.COMMENT,
    WFOToken.Types.MATERIAL_LIB: WfoStatement.Types.MATERIAL_LIB,
    WFOToken.Types.OBJECT: WfoStatement.Types.OBJECT,
    WFOToken.Types.VERTEX: WfoStatement.Types.VERTEX,
    WFOToken.Types.NORMAL: WfoStatement.Types.NORMAL,
    WFOToken.Types.TEX_COORD: WfoStatement.Types.TEX_COORD,
    WFOToken.Types.USE_MATERIAL: WfoStatement.Types.USE_MATERIAL,
    WFOToken.Types.FACE: WfoStatement.Types.FACE,
    WFOToken.Types.SMOOTH_SHADING: WfoStatement.Types.SMOOTH_SHADING,
}

_seperator_token_types = [
    WFOToken.Types.SEPERATOR
]

_punctuation_token_types = [
    WFOToken.Types.LINE_BREAK
]


class ParserError(Exception):
    pass


class Parser:
    def __init__(self):
        self.statement_type = None
        self.statement_data = []
        self.statement_line_num = 0
        self.statement_char_num = 0
        self.num_parsed_tokens = 0
        self._reset_state()

        def number_statement_gen(x):
            return lambda t: self._parse_number_statement(t, x)

        self.statement_buffer = []
        self.parse_statement_mapping = {
            WfoStatement.Types.COMMENT: self._parse_comment_statement,
            WfoStatement.Types.MATERIAL_LIB: self._parse_single_string_statement,
            WfoStatement.Types.OBJECT: self._parse_single_string_statement,
            WfoStatement.Types.VERTEX: number_statement_gen(3),
            WfoStatement.Types.NORMAL: number_statement_gen(3),
            WfoStatement.Types.TEX_COORD: number_statement_gen(2),
            WfoStatement.Types.USE_MATERIAL: self._parse_single_string_statement,
            WfoStatement.Types.FACE: self._parse_face_statement,
            WfoStatement.Types.SMOOTH_SHADING: number_statement_gen(1),
        }

    def parse_tokens(self, tokens: Sequence[WFOToken]):
        for token in tokens:
            self._process_token(token)

        return self.statement_buffer

    def _process_token(self, token):
        if self.statement_type is None:
            self._handle_expecting_header_state(token)
        else:
            self.parse_statement_mapping[self.statement_type](token)

    def _handle_expecting_header_state(self, token: WFOToken):
        if token.type in [WFOToken.Types.SEPERATOR, WFOToken.Types.LINE_BREAK]:
            return  # separators and line breaks between statements are ignored

        if token.type not in _token_statement_type_mapping.keys():
            self._raise_unexpected_token_error(token, 'expected statement start')

        self.statement_type = _token_statement_type_mapping[token.type]
        self.statement_data = token.data
        self.statement_line_num = token.line_number
        self.statement_char_num = token.char_number
        self.num_parsed_tokens = 1

        if token.type == WFOToken.Types.COMMENT:
            self.state = WFOToken.Types.LINE_BREAK
        else:
            self.state = WFOToken.Types.SEPERATOR

    def _parse_comment_statement(self, token: WFOToken):
        if token.type != WFOToken.Types.LINE_BREAK:
            self._raise_unexpected_token_error(token)

        self._discard_token()  # technically correct but unnecessary
        self.statement_buffer.append(self._extract_statement_from_state())

    def _parse_single_string_statement(self, token: WFOToken):
        if self.state == WFOToken.Types.SEPERATOR and token.type == WFOToken.Types.SEPERATOR:
            self.state = WFOToken.Types.STRING
            self._discard_token()
        elif self.state == WFOToken.Types.STRING and token.type == WFOToken.Types.STRING:
            self.statement_data = token.data
            self.state = WFOToken.Types.LINE_BREAK
            self._discard_token()
        elif self.state == WFOToken.Types.LINE_BREAK and token.type == WFOToken.Types.LINE_BREAK:
            self.statement_buffer.append(self._extract_statement_from_state())
        else:
            self._raise_unexpected_token_error(token)

    def _parse_number_statement(self, token: WFOToken, expected_number_count):
        tokens_until_linebreak = 1 + (expected_number_count * 2)

        if self.state == WFOToken.Types.SEPERATOR and token.type == WFOToken.Types.SEPERATOR:
            self.state = WFOToken.Types.NUMBER
            self._discard_token()
        elif self.state == WFOToken.Types.NUMBER and token.type == WFOToken.Types.NUMBER:
            if self.statement_data is None:
                self.statement_data = (token.data,)
            else:
                self.statement_data = (*self.statement_data, token.data)
            self._discard_token()

            if self.num_parsed_tokens >= tokens_until_linebreak:
                self.state = WFOToken.Types.LINE_BREAK
            else:
                self.state = WFOToken.Types.SEPERATOR
        elif self.state == WFOToken.Types.LINE_BREAK and token.type == WFOToken.Types.LINE_BREAK:
            if len(self.statement_data) == 1:
                self.statement_data = self.statement_data[0]
            self.statement_buffer.append(self._extract_statement_from_state())
        else:
            self._raise_unexpected_token_error(token)

    def _parse_face_statement(self, token: WFOToken):
        if self._is_expected_token(token, WFOToken.Types.SEPERATOR):
            self.state = WFOToken.Types.POLYGON
            self._discard_token()
        elif self._is_expected_token(token, WFOToken.Types.POLYGON):
            if self.statement_data is None:
                self.statement_data = [token.data]
            else:
                self.statement_data.append(token.data)
            self._discard_token()

            if self.num_parsed_tokens >= 7:
                self.state = WFOToken.Types.LINE_BREAK
            else:
                self.state = WFOToken.Types.SEPERATOR
        elif self._is_expected_token(token, WFOToken.Types.LINE_BREAK):
            self.statement_data = tuple(self.statement_data)
            self.statement_buffer.append(self._extract_statement_from_state())
        else:
            self._raise_unexpected_token_error(token)

    def _is_expected_token(self, token: WFOToken, token_type: WFOToken.Types):
        return self.state == token_type and token.type == token_type

    def _discard_token(self):
        self.num_parsed_tokens += 1

    def _extract_statement_from_state(self):
        new_statement = WfoStatement(
            self.statement_type,
            self.statement_data,
            self.statement_line_num,
            self.statement_char_num
        )
        self._reset_state()

        return new_statement

    def _reset_state(self):
        self.statement_type = None
        self.statement_data = None
        self.statement_line_num = 0
        self.statement_char_num = 0
        self.num_parsed_tokens = 0

    @staticmethod
    def _raise_unexpected_token_error(token, msg=''):
        sep = '' if msg == '' else ', '

        raise ParserError(
            'Unexpected token: {} on line: {} at: {}{}{}'.format(
                token.type.name, token.line_number, token.char_number, sep, msg
            )
        )
