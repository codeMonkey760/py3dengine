from string import whitespace
from enum import Enum
from py3dengine.wfotoken import WFOToken
from queue import deque


class State(Enum):
    TOKEN = 1,
    LINE_BREAK = 2,
    SEPERATOR = 3,
    COMMENT = 4


class Lexer:
    def __init__(self, stream):
        self.tokens = []
        self.char_buffer = ''
        self.char_pos = 0
        self.line_num = 1
        self.stream = stream
        self.state = None

    def lex_tokens(self):
        while True:
            cur_char = self._advance_char()
            if cur_char is None:
                self._append_new_token(self._process_char_buffer())
                break

            self._process_char(cur_char)

        return self.tokens

    def _advance_char(self):
        cur_char = self.stream.read(1)

        if len(cur_char) == 0:
            return None

        return cur_char

    def _advance_line(self):
        self.char_pos = 0
        self.line_num += 1

    def _process_char(self, cur_char):
        is_n_line_ending = cur_char == '\n'
        is_line_ending = cur_char in ['\n', '\r', '\n\r']
        is_white_space = cur_char in whitespace and not is_line_ending
        is_comment = cur_char in ['#']
        is_normal = not (is_line_ending or is_white_space or is_comment)
        has_nr_line_ending = self.char_buffer == '\n\r'

        if is_line_ending and self.state != State.LINE_BREAK:
            self._append_new_token(self._process_char_buffer())
            self.state = State.LINE_BREAK
        elif is_n_line_ending and self.state == State.LINE_BREAK:
            self._append_new_token(self._process_char_buffer())
        elif is_line_ending and has_nr_line_ending:
            self._append_new_token(self._process_char_buffer())
        elif is_line_ending and self.state == State.COMMENT:
            self._append_new_token(self._process_char_buffer())
            self.state = State.LINE_BREAK
        elif is_white_space and self.state != State.SEPERATOR and self.state != State.COMMENT:
            self._append_new_token(self._process_char_buffer())
            self.state = State.SEPERATOR
        elif is_comment and self.state != State.COMMENT:
            self._append_new_token(self._process_char_buffer())
            self.state = State.COMMENT
        elif is_normal and self.state != State.TOKEN and self.state != State.COMMENT:
            self._append_new_token(self._process_char_buffer())
            self.state = State.TOKEN

        self.char_buffer += cur_char
        self.char_pos += 1

    def _process_char_buffer(self):
        if len(self.char_buffer) == 0:
            return

        char_buffer = self.char_buffer
        self.char_buffer = ''

        char_pos = self.char_pos - len(char_buffer) + 1

        if self.state == State.COMMENT:
            return WFOToken(WFOToken.Types.COMMENT, self.line_num, char_pos, char_buffer)

        if self.state == State.LINE_BREAK:
            new_token = WFOToken(WFOToken.Types.LINE_BREAK, self.line_num, char_pos, char_buffer)
            self._advance_line()

            return new_token

        if self.state == State.SEPERATOR:
            return WFOToken(WFOToken.Types.SEPERATOR, self.line_num, char_pos)

        try:
            new_token_type = WFOToken.token_type_mapping[char_buffer]

            return WFOToken(new_token_type, self.line_num, char_pos)
        except KeyError:
            pass

        try:
            float_data = float(char_buffer)

            return WFOToken(WFOToken.Types.NUMBER, self.line_num, char_pos, float_data)
        except ValueError:
            pass

        try:
            polygon_data = self._lex_polygon(char_buffer)

            return WFOToken(WFOToken.Types.POLYGON, self.line_num, char_pos, polygon_data)
        except ValueError:
            pass

        return WFOToken(WFOToken.Types.STRING, self.line_num, char_pos, char_buffer)

    @staticmethod
    def _lex_polygon(chars):
        q = deque(chars)
        buffer = ''
        data = []
        divider_count = 0

        try:
            while len(q) > 0:
                c = q.popleft()
                if c != '/':
                    buffer += c
                else:
                    if divider_count == 2:
                        raise ValueError('Too many indices')
                    divider_count += 1
                    if buffer == '':
                        data.append(None)
                    else:
                        data.append(int(buffer))
                        buffer = ''
            if buffer == '':
                data.append(None)
            else:
                data.append(int(buffer))
        except ValueError:
            raise ValueError('Invalid polygon format')

        return (*data,)

    def _append_new_token(self, token):
        if token is None:
            return

        self.tokens.append(token)
