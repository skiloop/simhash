# simhash
![https://pypi.python.org/pypi/pysimhash](https://img.shields.io/pypi/v/pysimhash.svg)
![https://pypi.python.org/pypi/pysimhash](https://img.shields.io/pypi/pyversions/pysimhash.svg)
![https://github.com/skiloop/simhash/actions?query=workflow%3ACodeQL](https://github.com/skiloop/simhash/workflows/CodeQL/badge.svg)

simhash cpp module for python, a cpp implement of [simhash](https://github.com/leonsim/simhash), support for large
dimesion such as 128bit

# install

```shell
pip install pysimhash
```

or install from github.com

```shell
git clone https://github.com/skiloop/simhash
cd simhash
python setup.py install
```

# requirements

- boost-python


# how to use

example: 
```python
import pysimhash
import hashlib
document = "google.com hybridtheory.com youtube.com reddit.com"
tokens = [hashlib.md5(s.encode('utf-8')).hexdigest() for s in document.split(" ")]
s2 = pysimhash.SimHash(128, 16) # f=128, hash_bit=16
s2.build(tokens, base=16)
print(s2.hex())
```

# benchmark

With 10000 creating and 100,000 comparing(using [benchmark.py](./benchmark.py)) on the same linux, results
go as follow

| implement   | build time | comparison time |
|-------------|------------|-----------------|
| pure python | 1.73s      | 222.99s         |
| pysimhash   | 0.14s      | 49.89s          |
