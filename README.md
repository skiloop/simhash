# simhash
simhash cpp module for python, a cpp implement of [simhash](https://github.com/leonsim/simhash), support for large dimesion such as 128bit
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
- boost python

# benchmark
With 100,000 simhash value creating and comparing(using [benchmark.py](./benchmark.py)) on Intel(R) Core(TM) i5-4590 CPU @ 3.30GHz x86_64 ubuntu linux, results go as follow

implement| time
----|----
pure python|11148.62 s
pysimhash|2392.36 s
