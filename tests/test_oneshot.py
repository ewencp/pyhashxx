from pyhashxx import hashxx, Hashxx
import unittest

class TestOneShot(unittest.TestCase):
    # The shorthand should be equivalent to this simple function:
    def hash_value(self, val, seed=0):
        h = Hashxx(seed=seed)
        h.update(val)
        return h.digest()

    def test_empty_string(self):
        self.assertEqual(hashxx(''), self.hash_value(''))

    def test_string(self):
        self.assertEqual(hashxx('hello'), self.hash_value('hello'))

    def test_seeds(self):
        self.assertNotEqual(hashxx('hello', seed=0), hashxx('hello', seed=1))

        self.assertEqual(hashxx('hello', seed=0), self.hash_value('hello', seed=0))
        self.assertEqual(hashxx('hello', seed=1), self.hash_value('hello', seed=1))
        self.assertEqual(hashxx('hello', seed=2), self.hash_value('hello', seed=2))

    def test_bad_arg(self):
        with self.assertRaises(TypeError):
            hashxx([1, 2, 3])

    def test_bad_seed(self):
        with self.assertRaises(TypeError):
            hashxx(seed="badseed")
