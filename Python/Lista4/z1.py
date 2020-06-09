import datetime
import time

def dekorator(funk):
    def inna_funkcja():
        start = datetime.datetime.now()
        funk()
        end = datetime.datetime.now() - start
        print(end)
    return inna_funkcja

@dekorator
def funkcja():
    time.sleep(1)

funkcja()
