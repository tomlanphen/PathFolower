import rijinstructie

class Uarthandeler:
    def __init__(self):
        help = 0

    def stuur_instructie(self, instructie):
        for i in instructie.rijinstructies.wielinstructies:
            print("new message:")
            for j in i:
                print(j.instructie)
                print(ord(j.instructie[2]), ord(j.instructie[3]))
                if ord(j.instructie[3]) != 0:
                    stappen = ord(j.instructie[2]) * ord(j.instructie[3])
                    print(stappen)
                else:
                    stappen = ord(j.instructie[2])
                rotaties = stappen / j.wiel.volstap
                # print(rotaties)
                afstand = rotaties * j.wiel.omtrek
                print(afstand)
