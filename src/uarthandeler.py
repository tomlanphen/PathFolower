import serial, time
import RPi.GPIO as GPIO

class Uarthandeler:
    def __init__(self):
        GPIO.setmode(GPIO.BCM)
        GPIO.setup(17, GPIO.IN)

        self.ser = serial.Serial('/dev/ttyS0', baudrate=9600,
                            parity=serial.PARITY_NONE,
                            stopbits=serial.STOPBITS_ONE,
                            bytesize=serial.EIGHTBITS
                            )

    def stuur_instructie(self, instructie):
        for rijinstructie in instructie.rijinstructies.wielinstructies:
            print("niew")
            while GPIO.input(17):
                niks = False

            for wielinstructie in rijinstructie:
                self.ser.write(wielinstructie.instructie)

            self.ser.write(bytes("sssssss", 'utf-8'))

            time.sleep(0.1)
