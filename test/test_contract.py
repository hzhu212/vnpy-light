import os
import sys
import csv
import datetime
import pandas as pd
import tkinter as tk

from vnpy.event import EventEngine
from vnpy.trader.vtEngine import MainEngine
from vnpy.trader.vtEvent import EVENT_CONTRACT
from vnpy.trader.gateway import ctpGateway
from vnpy.trader.vtObject import VtSubscribeReq


class ControlWindow(tk.Frame):
    def __init__(self, target, master=None):
        tk.Frame.__init__(self, master)
        # 待控制的拥有 exit 方法的对象
        self.target = target
        self.master.title('ControlWindow')
        self.master.geometry('300x100+100+100')
        self.pack()
        self.create_widgets()

    def create_widgets(self):
        tk.Button(self, width=10, text='Quit', command=self.exit).pack()

    def exit(self):
        print('Exit button clicked.')
        self.target.exit()
        self.quit()


def on_contract(event):
    """处理合约事件"""
    if not event.dict_['isLast']:
        return

    # 等待所有合约加载完成后
    print('-'*20 + 'All contracts acquired.' + '-'*20)
    global me
    data = list(map(lambda obj: obj.__dict__, me.getAllContracts()))
    data = pd.DataFrame(data)

    filename = 'data_' + __file__.split('.')[0] + '.csv'
    data.to_csv(filename, index=False, encoding='gbk')


def main():
    global me

    ee = EventEngine()
    me = MainEngine(ee)
    ee.register(EVENT_CONTRACT, on_contract)

    me.addGateway(ctpGateway)
    me.getGateway('CTP').filePath = os.path.join(os.path.dirname(os.path.abspath(__file__)), 'CTP_connect.simnow.mock.json')
    me.connect('CTP')


if __name__ == '__main__':
    main()
    global me
    wd = ControlWindow(me)
    wd.mainloop()
