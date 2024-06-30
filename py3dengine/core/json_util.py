def fetch_property(json, key, expected_prop_type):
    prop = json[key]
    if type(prop) is not expected_prop_type:
        raise TypeError(f'Expected {str(expected_prop_type)} not {str(type(prop))}')

    return prop


def fetch_optional_property(json, key, expected_prop_type, default=None):
    try:
        prop = json[key]
    except KeyError:
        return default

    if type(prop) is not expected_prop_type:
        raise TypeError(f'Expected {str(expected_prop_type)} not {str(type(prop))}')

    return prop
