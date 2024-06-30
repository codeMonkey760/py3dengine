from lexer import Lexer
from parser import Parser
from wfoprocessor import WfoProcessor


class ParseError(Exception):
    pass


def import_from_file(file_name):
    try:
        wfo_file = open(file_name, mode='r', newline='')
    except OSError as err:
        raise ParseError(err)

    lexer = Lexer(wfo_file)
    tokens = lexer.lex_tokens()

    wfo_file.close()

    parser = Parser()
    statements = parser.parse_tokens(tokens)

    processor = WfoProcessor()
    data = processor.process_statements(statements)

    return data
