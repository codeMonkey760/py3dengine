import sys

_DEBUG_LEVEL = 'DEBUG'
_TRACE_LEVEL = 'TRACE'
_INFO_LEVEL = 'INFO'
_WARNING_LEVEL = 'WARNING'
_ERROR_LEVEL = 'ERROR'
_CRITICAL_LEVEL = 'CRITICAL'

_dst_mapping = {
    _DEBUG_LEVEL: sys.stdout,
    _TRACE_LEVEL: sys.stdout,
    _INFO_LEVEL: sys.stdout,
    _WARNING_LEVEL: sys.stderr,
    _ERROR_LEVEL: sys.stderr,
    _CRITICAL_LEVEL: sys.stderr
}

def debug(msg):
    _level_log(_DEBUG_LEVEL, msg)

def trace(msg):
    _level_log(_TRACE_LEVEL, msg)

def info(msg):
    _level_log(_INFO_LEVEL, msg)

def warning(msg):
    _level_log(_WARNING_LEVEL, msg)

def error(msg):
    _level_log(_ERROR_LEVEL, msg)

def critical(msg):
    _level_log(_CRITICAL_LEVEL, msg)

def _level_log(level, msg):
    global _dst_mapping
    if level not in _dst_mapping.keys() or _dst_mapping[level] is None:
        return

    log_msg = "[{}]: {}\n".format(level, msg)
    _dst_mapping[level].write(log_msg)

def set_debug_dst(new_dst):
    _set_dst(_DEBUG_LEVEL, new_dst)

def set_trace_dst(new_dst):
    _set_dst(_TRACE_LEVEL, new_dst)

def set_info_dst(new_dst):
    _set_dst(_INFO_LEVEL, new_dst)

def set_warning_dst(new_dst):
    _set_dst(_WARNING_LEVEL, new_dst)

def set_error_dst(new_dst):
    _set_dst(_ERROR_LEVEL, new_dst)

def set_critical_dst(new_dst):
    _set_dst(_CRITICAL_LEVEL, new_dst)

def _set_dst(level, new_dst):
    global _dst_mapping
    if level not in _dst_mapping.keys():
        return

    old_dst = _dst_mapping[level]
    _dst_mapping[level] = new_dst

    return old_dst
