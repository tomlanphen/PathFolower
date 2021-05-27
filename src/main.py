import instructiemaker, wiel, pad, uarthandeler


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
    padarray.set_vector(90, 10)
    #padarray.set_vector(-90, 10)

    return padarray


if __name__ == '__main__':
    volgen = setup_pad()
    wielen = setup_wielen()
    uart = uarthandeler.Uarthandeler()

    instructies = instructiemaker.Instructiemaker()
    for i in range(len(volgen.vectoren)):
        instructies.rijinstructies.wielinstructies = []
        instructies.maak_instructie(wielen, volgen.get_vector())
        uart.stuur_instructie(instructies)
