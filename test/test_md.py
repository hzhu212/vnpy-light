import sys
import csv
import datetime
import os
import tkinter as tk

from vnpy.event import EventEngine
from vnpy.trader.vtEngine import MainEngine
from vnpy.trader.vtEvent import EVENT_TICK
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


def on_tick_update(event):
    """处理行情事件"""
    tick = event.dict_['data']
    print('Tick data updated at %s: %s. [Tick]' % (datetime.datetime.now(), tick.vtSymbol))
    global me
    data = list(map(lambda obj: obj.__dict__, me.dataEngine.tickDict.values()))
    to_csv(data)


def to_csv(obj_list):
    """save tick data to csv file"""
    filename = 'data_' + __file__.split('.')[0] + '.csv'
    with open(filename, 'w') as f:
        header = sorted(obj_list[0].keys())
        writer = csv.DictWriter(f, header)
        writer.writeheader()
        for d in obj_list:
            writer.writerow(d)


def main():
    global me

    ee = EventEngine()
    me = MainEngine(ee)
    ee.register(EVENT_TICK, on_tick_update)

    me.addGateway(ctpGateway)
    me.getGateway('CTP').filePath = os.path.join(os.path.dirname(os.path.abspath(__file__)), 'CTP_connect.simnow.mock.json')
    me.connect('CTP')

    # 等待 1 秒是为了避免订阅时机 bug
    import time
    time.sleep(1)

    # vt_symbols = ['SR901C5300.CZCE', 'SR901C5200.CZCE']
    vt_symbols = ['ru1908.SHFE', 'rb1908.SHFE']
    symbols, exchanges = zip(*[vs.split('.') if '.' in vs else (vs, '') for vs in vt_symbols])
    for s, e in zip(symbols, exchanges):
        req = VtSubscribeReq()
        req.symbol, req.exchange = s, e
        me.subscribe(req, 'CTP')
    print('Futures subscribed.')


if __name__ == '__main__':
    try:
        main()
    except:
        me = globals().get('me')
        if me: me.exit()
        raise

    me = globals().get('me')
    if not me: sys.exit()
    wd = ControlWindow(me)
    wd.mainloop()
