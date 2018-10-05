# -*- coding: utf-8 -*-

"""
使用 jinja2 的模板语法将生成的 .h 和 .cpp 片段整合成完整的程序
"""

import os

from jinja2 import Environment, FileSystemLoader


in_dir = 'generate'
template_dir = 'template'
out_dir = 'final'
os.makedirs(os.path.join(out_dir, 'vnctpmd'), exist_ok=True)
os.makedirs(os.path.join(out_dir, 'vnctptd'), exist_ok=True)


env = Environment(
    loader=FileSystemLoader(searchpath='./template'),
    block_start_string='<%',
    block_end_string='%>',
    variable_start_string='<[',
    variable_end_string=']>',
    )

template_md_h = env.get_template('vnctpmd.template.h')
template_md_cpp = env.get_template('vnctpmd.template.cpp')
template_td_h = env.get_template('vnctptd.template.h')
template_td_cpp = env.get_template('vnctptd.template.cpp')


# vnctpmd.h
with open(os.path.join(out_dir, 'vnctpmd', 'vnctpmd.h'), 'w', encoding='gbk') as f_md_h,\
        open(os.path.join(in_dir, 'ctp_md_header_define.h'), 'r') as f_md_h_define,\
        open(os.path.join(in_dir, 'ctp_md_header_function.h'), 'r') as f_md_h_function,\
        open(os.path.join(in_dir, 'ctp_md_header_on.h'), 'r') as f_md_h_on,\
        open(os.path.join(in_dir, 'ctp_md_header_on_orig.h'), 'r') as f_md_h_on_orig,\
        open(os.path.join(in_dir, 'ctp_md_header_process.h'), 'r') as f_md_h_process:
    data = {
        'ctp_md_header_define': f_md_h_define.read(),
        'ctp_md_header_function': f_md_h_function.read(),
        'ctp_md_header_on': f_md_h_on.read(),
        'ctp_md_header_on_orig': f_md_h_on_orig.read(),
        'ctp_md_header_process': f_md_h_process.read(),
    }
    res = template_md_h.render(render_data=data)
    f_md_h.write(res)


# vnctpmd.cpp
with open(os.path.join(out_dir, 'vnctpmd', 'vnctpmd.cpp'), 'w', encoding='gbk') as f_md,\
        open(os.path.join(in_dir, 'ctp_md_function.cpp'), 'r') as f_md_function,\
        open(os.path.join(in_dir, 'ctp_md_process.cpp'), 'r') as f_md_process,\
        open(os.path.join(in_dir, 'ctp_md_switch.cpp'), 'r') as f_md_switch,\
        open(os.path.join(in_dir, 'ctp_md_task.cpp'), 'r') as f_md_task,\
        open(os.path.join(in_dir, 'ctp_md_wrap.cpp'), 'r') as f_md_wrap,\
        open(os.path.join(in_dir, 'ctp_md_python_module.cpp'), 'r') as f_md_python_module:
    data = {
        'ctp_md_function': f_md_function.read(),
        'ctp_md_process': f_md_process.read(),
        'ctp_md_switch': f_md_switch.read(),
        'ctp_md_task': f_md_task.read(),
        'ctp_md_wrap': f_md_wrap.read(),
        'ctp_md_python_module': f_md_python_module.read(),
    }
    res = template_md_cpp.render(render_data=data)
    f_md.write(res)


# vnctptd.h
with open(os.path.join(out_dir, 'vnctptd', 'vnctptd.h'), 'w', encoding='gbk') as f_td_h,\
        open(os.path.join(in_dir, 'ctp_td_header_define.h'), 'r') as f_td_h_define,\
        open(os.path.join(in_dir, 'ctp_td_header_function.h'), 'r') as f_td_h_function,\
        open(os.path.join(in_dir, 'ctp_td_header_on.h'), 'r') as f_td_h_on,\
        open(os.path.join(in_dir, 'ctp_td_header_on_orig.h'), 'r') as f_td_h_on_orig,\
        open(os.path.join(in_dir, 'ctp_td_header_process.h'), 'r') as f_td_h_process:
    data = {
        'ctp_td_header_define': f_td_h_define.read(),
        'ctp_td_header_function': f_td_h_function.read(),
        'ctp_td_header_on': f_td_h_on.read(),
        'ctp_td_header_on_orig': f_td_h_on_orig.read(),
        'ctp_td_header_process': f_td_h_process.read(),
    }
    res = template_td_h.render(render_data=data)
    f_td_h.write(res)


# vnctpmd.cpp
with open(os.path.join(out_dir, 'vnctptd', 'vnctptd.cpp'), 'w', encoding='gbk') as f_td,\
        open(os.path.join(in_dir, 'ctp_td_function.cpp'), 'r') as f_td_function,\
        open(os.path.join(in_dir, 'ctp_td_process.cpp'), 'r') as f_td_process,\
        open(os.path.join(in_dir, 'ctp_td_switch.cpp'), 'r') as f_td_switch,\
        open(os.path.join(in_dir, 'ctp_td_task.cpp'), 'r') as f_td_task,\
        open(os.path.join(in_dir, 'ctp_td_wrap.cpp'), 'r') as f_td_wrap,\
        open(os.path.join(in_dir, 'ctp_td_python_module.cpp'), 'r') as f_td_python_module:
    data = {
        'ctp_td_function': f_td_function.read(),
        'ctp_td_process': f_td_process.read(),
        'ctp_td_switch': f_td_switch.read(),
        'ctp_td_task': f_td_task.read(),
        'ctp_td_wrap': f_td_wrap.read(),
        'ctp_td_python_module': f_td_python_module.read(),
    }
    res = template_td_cpp.render(render_data=data)
    f_td.write(res)


# copy ctp_data_type.py to final directory for using
import shutil
shutil.copy('ctp_data_type.py', 'final')
