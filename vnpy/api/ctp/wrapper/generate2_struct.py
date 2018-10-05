# encoding: UTF-8

__author__ = 'CHENXY'

import os

from ctp_data_type import *


def main():
    """主函数"""
    in_dir = 'ctp_header'
    out_dir = '.'

    with open(os.path.join(in_dir, 'ThostFtdcUserApiStruct.h'), 'r', encoding='gbk') as fcpp,\
            open(os.path.join(out_dir, 'ctp_struct.py'), 'w', encoding='utf8') as fpy:

        fpy.write('# encoding: UTF-8\n')
        fpy.write('\n')
        fpy.write('structDict = {}\n')
        fpy.write('\n')

        for no, line in enumerate(fcpp):

            # 结构体申明注释
            if '///' in line and '\t' not in line:
                py_line = '#' + line[3:]

            # 结构体变量注释
            elif '\t///' in line:
                py_line = '#' + line[4:]

            # 结构体申明
            elif 'struct ' in line:
                content = line.split(' ')
                name = content[1].replace('\n','')
                py_line = '%s = {}\n' % name

            # 结构体变量
            elif '\t' in line and '///' not in line:
                content = line.split('\t')
                typedef = content[1]
                type_ = typedefDict[typedef]
                variable = content[2].replace(';\n', "")
                py_line = '%s["%s"] = "%s"\n' % (name, variable, type_)

            # 结构体结束
            elif '}' in line:
                py_line = "structDict['%s'] = %s\n\n" % (name, name)

            # 结构体开始
            elif '{' in line:
                py_line = ''

            # 其他
            else:
                py_line = '\n'

            fpy.write(py_line)


if __name__ == '__main__':
    main()
