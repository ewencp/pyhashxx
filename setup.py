from setuptools import find_packages, setup, Extension

headers = [  'pyhashxx/xxhash.h',
             'pyhashxx/pycompat.h',
         ]
sources = [ 'pyhashxx/xxhash.c',
            'pyhashxx/pyhashxx.c',
        ]
pyhashxx = Extension('pyhashxx', sources=sources, depends=headers)

setup(
    name = "pyhashxx",
    version = "0.1.2",
    description = "Python wrapper for xxHash algorithm",
    author = "Ewen Cheslack-Postava",
    author_email = 'me@ewencp.org',
    platforms=["any"],
    license="BSD",
    url = "http://github.com/ewencp/pyhashxx",
    packages = find_packages(),
    ext_modules = [ pyhashxx ],
    test_suite = "tests",
    headers = headers,
)
