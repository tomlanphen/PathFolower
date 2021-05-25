from src import instructiemaker, wiel, pad, uarthandeler

if __name__ == '__main__':
        volgen = pad.Pad()
        # volgen.set_vector(90, 10)
        volgen.set_vector(10, 10)
        # volgen.set_vector(0, 18.7867)

        wiel1 = wiel.Wiel()
        wiel1.set_wiel("lv")
        wiel2 = wiel.Wiel()
        wiel2.set_wiel("la")
        wiel3 = wiel.Wiel()
        wiel3.set_wiel("rv")
        wiel4 = wiel.Wiel()
        wiel4.set_wiel("ra")
        wielen = [wiel1, wiel2, wiel3, wiel4]

        instructies = instructiemaker.Instructiemaker()
        instructies.maak_instructie(wielen, volgen.get_vector())
        uart = uarthandeler.Uarthandeler()

        uart.stuur_instructie(instructies)
