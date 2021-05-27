from src import instructiemaker, wiel, pad, uarthandeler, kompas
from __future__ import print_function
import time, sys, signal, atexit, math
try:
    from upm import pyupm_bmm150 as sensorObj
except ImportError:
    print('Error: Please install python-mraa python-upm module.\r\n' 
          'See instruction here https://github.com/Seeed-Studio/pi_repo#mraa--upm-package-repository-for-raspberry-pi ')


def setup_wielen():
    wiel1 = wiel.Wiel()
    wiel1.set_wiel("lv")
    wiel2 = wiel.Wiel()
    wiel2.set_wiel("la")
    wiel3 = wiel.Wiel()
    wiel3.set_wiel("rv")
    wiel4 = wiel.Wiel()
    wiel4.set_wiel("ra")
    wielarray = [wiel1, wiel2, wiel3, wiel4]

    return wielarray


def setup_pad():
    padarray = pad.Pad()
    #padarray.set_vector(0, 10)
    padarray.set_vector(90, 0)
    padarray.set_vector(0, 10)
    #padarray.set_vector(-90, 10)

    return padarray


if __name__ == '__main__':
    volgen = setup_pad()
    extraDraai = pad.Pad()
    wielen = setup_wielen()
    uart = uarthandeler.Uarthandeler()
    richting = kompas.Kompas()

    instructies = instructiemaker.Instructiemaker()

    for i in range(len(volgen.vectoren)):
        beginRichting = richting.lees_richting()
        instructies.rijinstructies.wielinstructies = []
        instructies.maak_instructie(wielen, volgen.get_vector())
        uart.stuur_instructie(instructies)

        # als er gedraaid moet zijn en de draai is niet volledig draai dan extra
        while uart.draaicheck != 0:
            huidigeRichting = richting.lees_richting()
            verwachteRichting = richting.lees_richting() + uart.draaicheck
            if verwachteRichting > 360:
                verwachteRichting -= 360
            berekendeDraai = uart.draaicheck - (verwachteRichting - huidigeRichting)
            if -5 < berekendeDraai < 5:
                extraDraai.set_vector(int(berekendeDraai), 0)
                instructies.rijinstructies.wielinstructies = []
                instructies.maak_instructie(wielen, extraDraai.get_vector())
                uart.stuur_instructie(instructies)
            else:
                uart.draaicheck = 0
