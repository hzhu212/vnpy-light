# encoding: UTF-8

'''
vnpy-light provides an easier way to install vnpy.

vn.py - By Traders, For Traders.

The vn.py project is an open-source quantitative trading framework
that is developed by traders, for traders.

The project is mainly written in Python and uses C++ for low-layer
and performance sensitive infrastructure.

Using the vn.py project, institutional investors and professional
traders, such as hedge funds, prop trading firms and investment banks,
can easily develop complex trading strategies with the Event Engine
Strategy Module, and automatically route their orders to the most
desired destinations, including equity, commodity, forex and many
other financial markets.
'''


import os
import setuptools

import vnpy


# Build Platform Wheel (Not Pure Python Wheel)
try:
    # Play a trick to cheat wheel that the distribution is not pure python
    from wheel.bdist_wheel import bdist_wheel
    _get_tag = bdist_wheel.get_tag
    def _patch_get_tag(self, *args, **kw):
        self.root_is_pure = False
        return _get_tag(self, *args, **kw)
    bdist_wheel.get_tag = _patch_get_tag
except ImportError:
    print('Please install wheel if you want to build a wheel distribution')
    pass


setuptools.setup(
    name=vnpy.__vnpy_light_name__,
    version=vnpy.__vnpy_light_version__,
    description='An easier way to install vnpy',
    long_description = __doc__,
    author=vnpy.__vnpy_light_author__,
    author_email='zhuhe212@163.com',
    url='https://github.com/hzhu212/vnpy-light',
    packages=setuptools.find_packages(),
    package_data={
        '': ['LICENSE', 'README.md'],
        'vnpy.api': ['ctp/*.pyd', 'ctp/*.dll', 'ctp/*.so'],
        'vnpy.trader': ['*.json', 'ico/*.ico'],
    },
    keywords='vnpy quant quantitative investment trading algotrading',
    install_requires=['pymongo'],
    python_requires='>=3.5',
    license='MIT',
    classifiers=[
        'Development Status :: 3 - Alpha',
        'Intended Audience :: Developers',
        'Natural Language :: English',
        'License :: OSI Approved :: MIT License',
        'Operating System :: Microsoft :: Windows',
        'Programming Language :: Python :: 3',
        'Programming Language :: Python :: 3.5',
        'Programming Language :: Python :: 3.6',
        'Programming Language :: Python :: 3.7',
        'Programming Language :: Python :: Implementation :: CPython',
        'Topic :: Office/Business :: Financial :: Investment',
    ],
)
