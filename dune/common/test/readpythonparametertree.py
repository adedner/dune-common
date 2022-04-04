foo = {'str': 'test',
       'subTree': {'a': 1, 'b':2},
       'value': 11,
       'bool': True,
       'vectorStr': "1 127 2.5",
       'array': [2, 12, 5.2],
       'tuple': (3, 13, 6.2)
       }
try:
    import numpy as np
    foo['haveNumpy'] = True
    foo['numpy'] = np.array([2,4,6], dtype=np.float64)
    foo['numpyInt'] = np.array([3,5,7])
except ImportError:
    foo['haveNumpy'] = False
    print("WARNING: failed to test numpy arrays, as numy is not installed")
