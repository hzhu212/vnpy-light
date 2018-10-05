# -*- coding: utf-8 -*-

"""
generate methods of BOOST_PYTHON_MODULE according to ctp_header_function.cpp and ctp_header_on.cpp
"""

import re
import os


in_dir = 'generate'
out_dir = 'generate'


module_cls = {
    'md': 'MdApiWrap',
    'td': 'TdApiWrap',
}


re_req_method = re.compile(r'^int (\w+)\([\w\s\,]+\);$')
re_on_method = re.compile(r'^virtual void (\w+)\([\w\s\,]+\)[\s\{\}]*;$')


def handle_req_method(line, module):
    """
    handle ctp_header_function, `line` should be like this:
        `int reqUserLogin(dict req, int nRequestID);`
    """
    cls_name = module_cls[module]
    m = re_req_method.match(line)
    assert m, 'Unresolvable ctp_header_function: %s' % line
    method_name = m.group(1)
    # return value should be like this: `.def("release", &TdApiWrap::release)`
    return '.def("{mn}", &{cn}::{mn})'.format(mn=method_name, cn=cls_name)


def handle_on_method(line, module):
    """
    handle ctp_header_on, `line` should be like this:
        `virtual void onRspUserLogin(dict data, dict error, int id, bool last) {};`
    """
    cls_name = module_cls[module]
    m = re_on_method.match(line)
    assert m, 'Unresolvable ctp_header_on: %s' % line
    method_name = m.group(1)
    # return value should be like this: `.def("release", &TdApiWrap::release)`
    return '.def("{mn}", pure_virtual(&{cn}::{mn}))'.format(mn=method_name, cn=cls_name)


# generate ctp_md_python_module.cpp
with open(os.path.join(out_dir, 'ctp_md_python_module.cpp'), 'w', encoding='utf8') as fo:
    with open(os.path.join(in_dir, 'ctp_md_header_function.h'), 'r') as fi:
        for line in fi:
            line = line.strip()
            if line.startswith('int req'):
                s = handle_req_method(line, module='md')
                fo.write(s + '\n')

    fo.write('\n')

    with open(os.path.join(in_dir, 'ctp_md_header_on.h'), 'r') as fi:
        for line in fi:
            line = line.strip()
            if line.startswith('virtual void on'):
                s = handle_on_method(line, module='md')
                fo.write(s + '\n')


# generate ctp_td_python_module.cpp
with open(os.path.join(out_dir, 'ctp_td_python_module.cpp'), 'w', encoding='utf8') as fo:
    with open(os.path.join(in_dir, 'ctp_td_header_function.h'), 'r') as fi:
        for line in fi:
            line = line.strip()
            if line.startswith('int req'):
                s = handle_req_method(line, module='td')
                fo.write(s + '\n')

    fo.write('\n')

    with open(os.path.join(in_dir, 'ctp_td_header_on.h'), 'r') as fi:
        for line in fi:
            line = line.strip()
            if line.startswith('virtual void on'):
                s = handle_on_method(line, module='td')
                fo.write(s + '\n')
