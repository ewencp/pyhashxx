from pyhashxx import Hashxx
import unittest

class TestHashBytes(unittest.TestCase):

    def test_empty_string(self):
        h = Hashxx()
        h.update(b'')
        self.assertEqual(h.digest(), 46947589)

    def test_one_string(self):
        h = Hashxx()
        h.update(b'hello')
        self.assertEqual(h.digest(), 4211111929)

        h = Hashxx()
        h.update(b'goodbye')
        self.assertEqual(h.digest(), 2269043192)


    def test_multiple_strings(self):
        h = Hashxx()
        h.update(b'hello')
        h.update(b'goodbye')
        self.assertEqual(h.digest(), 4110974955)

    def test_tuple(self):
        # Tuples shouldn't affect the hash, they should be equivalent to hashing
        # each part in a separate update
        h = Hashxx()
        h.update((b'hello',b'goodbye'))
        self.assertEqual(h.digest(), 4110974955)

    def test_seeds(self):
        h = Hashxx(seed=0)
        h.update(b'hello')
        self.assertEqual(h.digest(), 4211111929)

        h = Hashxx(seed=1)
        h.update(b'hello')
        self.assertEqual(h.digest(), 4244634537)

        h = Hashxx(seed=2)
        h.update(b'hello')
        self.assertEqual(h.digest(), 4191738725)

    def hash_value(self, val, seed=0):
        h = Hashxx(seed=seed)
        h.update(val)
        return h.digest()

    def test_incremental(self):
        # Make sure incrementally computed results match those
        # computed all at once
        hello_hash = self.hash_value(b'hello')
        hello_world_hash = self.hash_value(b'helloworld')

        h = Hashxx()
        h.update(b'hello')
        self.assertEqual(h.digest(), hello_hash)
        h.update(b'world')
        self.assertEqual(h.digest(), hello_world_hash)


    def test_simultaneous(self):
        # Ensure that interleaved updates still give same results as
        # independent
        h1 = Hashxx()
        h2 = Hashxx()

        h1.update(b'he')
        h2.update(b'goo')
        h1.update(b'll')
        h2.update(b'db')
        h1.update(b'o')
        h2.update(b'ye')

        self.assertEqual(h1.digest(), self.hash_value(b'hello'))
        self.assertEqual(h2.digest(), self.hash_value(b'goodbye'))


    def test_bad_seed(self):
        self.assertRaises(TypeError, Hashxx, seed="badseed")

    def test_bad_arg(self):
        h = Hashxx()
        self.assertRaises(TypeError, h.update, [1,2,3])

    def test_no_args(self):
        h = Hashxx()
        self.assertRaises(TypeError, h.update)

    def test_no_unicode(self):
        h = Hashxx()
        self.assertRaises(TypeError, h.update, u'hello')
