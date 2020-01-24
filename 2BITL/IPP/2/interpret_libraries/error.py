from __future__ import print_function
import sys

#Print error to stderr
def err_print(*args, **kwargs):
    print(*args, file=sys.stderr, **kwargs)

#Print error to stderr and exit program with specified code
def err_exit(code, *args, **kwargs):
    print(*args, file=sys.stderr, **kwargs)
    exit(code)