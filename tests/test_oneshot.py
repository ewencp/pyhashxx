from __future__ import unicode_literals
from pyhashxx import hashxx, Hashxx
import unittest

class TestOneShot(unittest.TestCase):
    # The shorthand should be equivalent to this simple function:
    def hash_value(self, val, seed=0):
        h = Hashxx(seed=seed)
        h.update(val)
        return h.digest()

    def test_empty_string(self):
        self.assertEqual(hashxx(b''), self.hash_value(b''))

    def test_string(self):
        self.assertEqual(hashxx(b'hello'), self.hash_value(b'hello'))

    def test_seeds(self):
        self.assertNotEqual(hashxx(b'hello', seed=0), hashxx(b'hello', seed=1))

        self.assertEqual(hashxx(b'hello', seed=0), self.hash_value(b'hello', seed=0))
        self.assertEqual(hashxx(b'hello', seed=1), self.hash_value(b'hello', seed=1))
        self.assertEqual(hashxx(b'hello', seed=2), self.hash_value(b'hello', seed=2))

    def test_bad_arg(self):
        self.assertRaises(TypeError, hashxx, [1, 2, 3])

    def test_bad_seed(self):
        self.assertRaises(TypeError, hashxx, seed="badseed")

    def test_no_args(self):
        self.assertRaises(TypeError, hashxx)

    def test_no_unicode(self):
        self.assertRaises(TypeError, hashxx, 'hello')
