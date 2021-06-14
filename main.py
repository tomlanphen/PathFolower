from PathFolower import instructiemaker, pad, uarthandeler
import utilities


def setup_pad():
    padarray = pad.Pad()
    padarray.set_vector(0, 30)
    padarray.set_vector(0, 35)
    padarray.set_vector(-90, 0)

    return padarray


if __name__ == '__main__':
    volgen = setup_pad()
    wielen = utilities.setup_wielen()
    uart = uarthandeler.Uarthandeler()

    instructies = instructiemaker.Instructiemaker()

    for i in range(len(volgen.vectoren)):
        instructies.rijinstructies.wielinstructies = []
        instructies.maak_instructie(wielen, volgen.get_vector())
        uart.stuur_instructie(instructies)

        # start de woordzoeker
