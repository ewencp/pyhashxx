pyhashxx
========

Python wrapper of xxhash that supports concurrency and is packaged
nicely for installation with pip/distribute.

The existing wrapper I could find used a static global hash context
and wasn't packaged nicely for installation with Python package
managers. This version fixes both those issues. The naming
rearrangement is to avoid conflicts with the existing wrapper in
python.

See http://code.google.com/p/xxhash/ for the original xxHash code.

Examples
--------

The easiest way to use the hash function is the convenience function
`hashxx` that returns the hash of the bytes of its parameters, and
which will traverse tuples (even recursively).

    from pyhashxx import hashxx
    hashxx(b'Hello World!')
    hashxx(b'Hello', b' ', b'World!')
    hashxx((b'Hello', b' ', b'World!'))
    hashxx((b'Hello', b' '), (b'World!',))
    # All return 198612872
    hashxx(b'Hello World!', seed=1)
    # Changing the seed changes the result to 2212595744

You can also use the `Hashxx` class to compute the hash incrementally,
and extract intermediate digest values:

    from pyhashxx import Hashxx
    hasher = Hashxx(seed=0) # seed is optional
    hasher.update(b'Hello')
    hasher.update(b' ')
    print(hasher.digest()) # Prints 1401757748
    hasher.update(b'World!')
    print(hasher.digest()) # Prints 198612872

See the `examples/` directory for more, including a script testing
performance.

Buildbot
--------
[![Build Status](https://secure.travis-ci.org/ewencp/pyhashxx.png)](http://travis-ci.org/ewencp/pyhashxx)
