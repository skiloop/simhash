#!/usr/bin/env python
# -*- coding:utf8 -*-
#
# created by shibaofeng@ipin.com 2018/8/7
#


import collections
import hashlib
import time
from pysimhash import SimHash


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
        if hashfunc is None:
            def _hashfunc(x):
                return int(hashlib.md5(x).hexdigest(), 16)

            self.hashfunc = _hashfunc
        else:
            self.hashfunc = hashfunc

        if isinstance(value, Simhash):
            self.value = value.value
        elif isinstance(value, basestring):
            self.build_by_text(unicode(value))
        elif isinstance(value, collections.Iterable):
            self.build_by_features(value)
        elif isinstance(value, long):
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
                self.part.append(long(0))
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

    def similar(self, another, cnt=2, distance=6):
        assert self.f == another.f
        for i, part in enumerate(self.part):
            if part == another.part[i]:
                cnt -= 1
            if cnt == 0:
                return self.distance(another) <= distance
        return False


def load_hashes():
    with open('simhash.txt') as f:
        return [line.strip() for line in f]


def simhash_compare():
    t = time.time()
    hashes = [Simhash(long(s)) for s in load_hashes()]
    cnt = 0
    for i, v1 in enumerate(hashes):
        for j, v2 in enumerate(hashes):
            if i == j:
                continue
            if v1.similar(v2):
                cnt += 1
    return time.time() - t, cnt


def pysimhash_compare():
    t = time.time()
    hashes = [SimHash(s, 128, 16) for s in load_hashes()]
    cnt = 0
    for i, v1 in enumerate(hashes):
        for j, v2 in enumerate(hashes):
            if i == j:
                continue
            if v1.similar(v2, 2, 6):
                cnt += 1
    return time.time() - t, cnt


def benchmark():
    st, sn = simhash_compare()
    print "simhash: {} ns, {}".format(st, sn)
    st, sn = pysimhash_compare()
    print "pysimhash: {} ns, {}".format(st, sn)


if __name__ == '__main__':
    benchmark()
