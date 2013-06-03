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
