from __future__ import print_function
from pyhashxx import hashxx
import random, time

print("Generating random data")
chars = b'abcdefghijklmnopqrstuvwxyz'
short_size = 20
short = b''.join([bytes(random.choice(chars)) for i in range(short_size)])
long_size = 1024
long = b''.join([bytes(random.choice(chars)) for i in range(long_size)])
extra_long_size = 64*1024*1024
extra_long = long * 64

def time_trial(size, data, number=1000000):
    started = time.time()
    # Avoid loop overhead
    [hashxx(data) for i in range(number)]
    finished = time.time()
    duration = finished - started
    print("%d bytes %d times, %f s, %f ms/hash, %f hashes/s, %f MB/s" % (size, number, duration, duration/number*1000, number/duration, number*size/(duration*1024*1024)))

if __name__ == "__main__":
    time_trial(short_size, short, number=10000000)
    time_trial(long_size, long, number=250000)
    time_trial(extra_long_size, extra_long, number=10000)
