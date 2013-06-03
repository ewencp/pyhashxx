from pyhashxx import Hashxx
import unittest

class TestHashBytes(unittest.TestCase):

    def test_empty_string(self):
        h = Hashxx()
        h.update('')
        self.assertEquals(h.digest(), 46947589)

    def test_one_string(self):
        h = Hashxx()
        h.update('hello')
        self.assertEquals(h.digest(), 4211111929)

        h = Hashxx()
        h.update('goodbye')
        self.assertEquals(h.digest(), 2269043192)


    def test_multiple_strings(self):
        h = Hashxx()
        h.update('hello')
        h.update('goodbye')
        self.assertEquals(h.digest(), 4110974955)

    def test_tuple(self):
        # Tuples shouldn't affect the hash, they should be equivalent to hashing
        # each part in a separate update
        h = Hashxx()
        h.update(('hello','goodbye'))
        self.assertEquals(h.digest(), 4110974955)

    def test_seeds(self):
        h = Hashxx(seed=0)
        h.update('hello')
        self.assertEquals(h.digest(), 4211111929)

        h = Hashxx(seed=1)
        h.update('hello')
        self.assertEquals(h.digest(), 4244634537)

        h = Hashxx(seed=2)
        h.update('hello')
        self.assertEquals(h.digest(), 4191738725)

    def hash_value(self, val, seed=0):
        h = Hashxx(seed=seed)
        h.update(val)
        return h.digest()

    def test_incremental(self):
        # Make sure incrementally computed results match those
        # computed all at once
        hello_hash = self.hash_value('hello')
        hello_world_hash = self.hash_value('helloworld')

        h = Hashxx()
        h.update('hello')
        self.assertEquals(h.digest(), hello_hash)
        h.update('world')
        self.assertEquals(h.digest(), hello_world_hash)


    def test_simultaneous(self):
        # Ensure that interleaved updates still give same results as
        # independent
        h1 = Hashxx()
        h2 = Hashxx()

        h1.update('he')
        h2.update('goo')
        h1.update('ll')
        h2.update('db')
        h1.update('o')
        h2.update('ye')

        self.assertEquals(h1.digest(), self.hash_value('hello'))
        self.assertEquals(h2.digest(), self.hash_value('goodbye'))
