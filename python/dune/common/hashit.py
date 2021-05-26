import hashlib

def hashIt(typeName):
    """Return the MD5 hash of type name.
    """
    if hasattr(typeName, '__iter__'):
        return hashlib.md5("".join(t for t in typeName).encode('utf-8')).hexdigest()
    else:
        return hashlib.md5(typeName.encode('utf-8')).hexdigest()
