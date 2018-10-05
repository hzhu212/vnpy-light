This project is designed to provide an easier way to install [vnpy](http://www.vnpy.org); main codes are shipped from [vnpy/vnpy](https://github.com/vnpy/vnpy).

# To vnpy users

> **Please do not clone or download this project if you just want to try vnpy!** This project is for developers to build vnpy APIs.

No Anaconda, No build, no PyQT or GUI, vnpy-light is a minimal but ready to use version of vnpy. Installing is as simple as just one command (assuming you have installed Python):

```sh
pip install vnpy-light
```

Then you can use it totally the same as using vnpy, say:

```python
>>> from vnpy.event import EventEngine
>>> from vnpy.trader.gateway import ctpGateway
>>>
```

Currently, vnpy-light works on **Windows** and supports **Python35, Python36 and Python37, both 32 and 64 bits**. **Python27 is NOT supported**. Linux supporting is under consideration.

The supports for APIs is very poor now, **only CTP API at the moment**. It's because writing `CMakeLists.txt` for each API is a lot of work. More APIs will be supported inch by inch if necessary.

Note:

- the CTP API of vnpy-light(v6.3.8_20171101) is newer than vnpy, because I can't find old versions of CTP API dll for Windows-x64.
- vnpy-light doesn't require GUI or PyQT by default, but it's still OK if you want a GUI; just install it manually:
    ```sh
    pip install pyqt5 qtpy
    ```

# To vnpy-light developers

The purpose of this project is to build `.pyd` files from the vnpy APIs, which are wrapped with Boost.python.

The build system of vnpy-light is totally based on `cmake`, and the most common compiler on Windows is of course Visual Studio. If you want to contribute to this project, you need the following toolchains:

1. [CMake](https://cmake.org/)
2. [Visual Studio](https://visualstudio.microsoft.com)
3. [Boost C++ Libraries](https://www.boost.org/)
4. [Python](https://www.python.org/) (version is according to your needs)

After getting all these tools ready, you need to build some Boost libraries. Generally, vnpy-light requires the following libraries:

```txt
boost_python, boost_locale, boost_thread, boost_system, boost_atomic, boost_date_time, boost_chrono
```

Next, edit `./config-build.bat` to set your tools' paths and `./cmake/config-*.cmake` to set python environment. Don't worry, there are examples in the config files.

Finally, you will build the whole project by only one command, without opening Visual Studio.

For example, this command build APIs for Python35(32bits):

```sh
./build.bat config-win-py35-x32 32
```

Target files will be found in `./build_32/config-win-py35-x32`, and also in `vnpy/api/<api_name>`(copied automatically).

Enjoy it.
