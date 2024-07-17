#!/usr/bin/env python
# -*- coding:utf8 -*-
#
# created by skiloop@gmail.com 2018/8/7
#
import re
from itertools import groupby

from six import PY3

if PY3:
    from collections import abc as collections
else:
    import collections
import unittest
import hashlib
import pysimhash

if PY3:
    basestring = (str, bytes)
    long = int
    unicode = str.encode


class Simhash(object):

    def __init__(self, value, f=128, reg=r'[\w\u4e00-\u9fcc]+', hashfunc=None):
        """
        `f` is the dimensions of fingerprints

        `reg` is meaningful only when `value` is basestring and describes
        what is considered to be a letter inside parsed string. Regexp
        object can also be specified (some attempt to handle any letters
        is to specify reg=re.compile(r'\w', re.UNICODE))

        `hashfunc` accepts a utf-8 encoded string and returns a unsigned
        integer in at least `f` bits.
        """
        self.hash_bit = 16
        self.f = f
        self.reg = reg
        self.value = None
        self.part = []
        self.hashfunc = hashfunc if hashfunc is not None else lambda x: int(hashlib.md5(x).hexdigest(), 16)

        if isinstance(value, Simhash):
            self.value = value.value
        elif isinstance(value, basestring):
            self.build_by_text(value if PY3 else unicode(value))
        elif isinstance(value, collections.Iterable):
            self.build_by_features(value)
        elif isinstance(value, (long, int)):
            self.value = value
        else:
            raise Exception('Bad parameter with type {}'.format(type(value)))
        self.split_part()

    def _slide(self, content, width=4):
        return [content[i:i + width] for i in range(max(len(content) - width + 1, 1))]

    def _tokenize(self, content):
        content = content.lower()
        content = ''.join(re.findall(self.reg, content))
        ans = self._slide(content)
        return ans

    def split_part(self):
        base = 2 ** self.hash_bit - 1
        for i in range(int(self.f / self.hash_bit)):
            if self.value == 0:
                self.part.append(int(0))
                continue
            base_ = base << (i * self.hash_bit)
            bhash = (self.value & base_) >> (i * self.hash_bit)
            self.part.append(bhash)

    def build_by_text(self, content):
        features = self._tokenize(content)
        features = {k: sum(1 for _ in g) for k, g in groupby(sorted(features))}
        return self.build_by_features(features)

    def build_by_features(self, features):
        """
        `features` might be a list of unweighted tokens (a weight of 1
                   will be assumed), a list of (token, weight) tuples or
                   a token -> weight dict.
        """
        v = [0] * self.f
        masks = [1 << i for i in range(self.f)]
        if isinstance(features, dict):
            features = features.items()
        for f in features:
            if isinstance(f, basestring):
                h = self.hashfunc(f.encode('utf-8'))
                w = 1
            else:
                assert isinstance(f, collections.Iterable)
                h = self.hashfunc(f[0].encode('utf-8'))
                w = f[1]
            for i in range(self.f):
                v[i] += w if h & masks[i] else -w
        ans = 0
        for i in range(self.f):
            if v[i] >= 0:
                ans |= masks[i]
        self.value = ans

    def distance(self, another):
        assert self.f == another.f
        x = (self.value ^ another.value) & ((1 << self.f) - 1)
        ans = 0
        while x:
            ans += 1
            x &= x - 1
        return ans


class Test(unittest.TestCase):

    def setUp(self):
        print(pysimhash.__file__)

    def test64Value(self):
        hs = "2479530857526804504"
        s1 = Simhash(int(hs), f=64)
        s2 = pysimhash.SimHash(hs, 64, 16, 10)
        self.assertEqual(str(s1.value), s2.value())
        self.assertEqual(s1.part, s2.parts())

    def testValue(self):
        hs = "2479530857526804504083961063697536543"
        s1 = Simhash(int(hs))
        s2 = pysimhash.SimHash(hs, 128, 16, 10)
        self.assertEqual(str(s1.value), s2.value())
        self.assertEqual(s1.part, s2.parts())

    def testDistance(self):
        h2 = "2479530857526804504083961063697536543"
        h1 = "16438062231610353799719944743129574075"
        s1 = Simhash(int(h1))
        s2 = Simhash(int(h2))
        d1 = s1.distance(s2)
        v1 = pysimhash.SimHash(h1, 128, 16, 10)
        v2 = pysimhash.SimHash(h2, 128, 16, 10)
        d2 = v1.distance(v2)
        self.assertEqual(d1, d2)

    def testBuild(self):
        document = "google.com hybridtheory.com youtube.com reddit.com"
        items = document.split(' ')
        s1 = Simhash(items)
        tokens = [hashlib.md5(s.encode('utf-8')).hexdigest() for s in items]
        s2 = pysimhash.SimHash(128, 16)
        s2.build(tokens, base=16)
        s2v = s2.value()
        self.assertEqual(s1.part, s2.parts())
        self.assertEqual(s2v, str(s1.value))

    def test64Build(self):
        document = "google.com hybridtheory.com youtube.com reddit.com"
        items = document.split(' ')
        s1 = Simhash(items, f=64)
        tokens = [hashlib.md5(s.encode('utf-8')).hexdigest() for s in items]
        s2 = pysimhash.SimHash(64, 16)
        s2.build(tokens, base=16)
        s2v = s2.value()
        self.assertEqual(s1.part, s2.parts())
        self.assertEqual(s2v, str(s1.value))


if __name__ == '__main__':
    unittest.main()
