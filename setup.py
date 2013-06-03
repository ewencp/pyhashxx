from setuptools import find_packages, setup, Extension

pyhashxx = Extension('pyhashxx',
                      sources = [ 'pyhashxx/xxhash.c', 'pyhashxx/pyhashxx.c' ])

setup(
    name = "pyhashxx",
    version = "0.1",
    description = "Python wrapper for xxHash algorithm",
    author = "Ewen Cheslack-Postava",
    author_email = 'me@ewencp.org',
    platforms=["any"],
    license="BSD",
    url = "http://github.com/ewencp/pyhashxx",
    packages = find_packages(),
    ext_modules = [ pyhashxx ],
    test_suite = "tests",
)
