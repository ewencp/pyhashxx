from __future__ import print_function, unicode_literals
from pyhashxx import Hashxx, hashxx

def hash_one_value(val):
    '''
    Use the hashxx function to initialize, add data, and compute the
    digest in one go. Add the seed parameter if you need to control
    the initial state of the digest.
    '''
    print("Hash of %s = %d" % (repr(val), hashxx(val, seed=0)))

def hash_multiple_values(vals):
    '''
    Same as hash_one_value but iterates over a list and uses the
    Hashxx class so it can call update() multiple times, passing in
    additional data on each call. This could also be useful for
    streaming data, and also allows you to get the current (partial)
    digest and continue adding data.
    '''
    hasher = Hashxx(seed=0)
    for idx, val in enumerate(vals):
        hasher.update(val)
        print(" Intermediate hash up to %s = %d" % (repr(vals[:idx+1]), hasher.digest()))
    print("Hash of %s = %d" % (repr(vals), hasher.digest()))

if __name__ == "__main__":
    # Normally you'll use byte arrays (strings in Python2)
    hash_one_value(b'Hello')
    hash_one_value(b'Hello World!')
    # Tuples can be passed in as a single value and will be treated as
    # if you just concatenated them
    hash_one_value((b'Hello', b' ', b'World!'))
    # Lists need to be iterated over manually (they are not normally
    # hashable in Python). Ultimately, the hash is the same as the
    # previous tuple.
    hash_multiple_values([b'Hello', b' ', b'World!'])
    # Note that you *cannot* use Unicode values because their hash
    # value would depend on the encoding. You need to convert them to
    # bytes (or strings in Python2) before passing them for hashing to
    # ensure you get the proper (and consistent) hash value.
    try:
        hash_one_value('Hello World!')
    except TypeError as exc:
        print('Trying to hash a unicode literal raised %s: "%s"' % (type(exc), exc,))
