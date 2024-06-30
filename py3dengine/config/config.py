import json

_DEFAULT_FILE_NAME = 'config.json'

_config = {}

class ConfigError(Exception):
    pass

def parse_config():
    parse_config_from_file(_DEFAULT_FILE_NAME)

def parse_config_from_file(file_name):
    try:
        config_file = open(file_name, "r")
    except OSError as e:
        raise ConfigError('Could not open config file for reading', e)

    try:
        parse_config_from_stream(config_file)
    finally:
        config_file.close()

def parse_config_from_stream(stream):
    global _config

    try:
        new_values = json.load(stream)
    except json.JSONDecodeError as e:
        raise ConfigError('Could not parse config source as JSON', e)

    _config = _config | new_values

def append_config_values(additional_values):
    global _config

    _config = _config | additional_values

def get_config_value_or_fail(name):
    global _config

    if name not in _config.keys():
        raise ConfigError('Value named "{}" is not configured'.format(name))

    return _config[name]

def get_config_value_or_default(name, default_value=None):
    global _config

    if name not in _config.keys():
        return default_value

    return _config[name]
