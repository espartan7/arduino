#
# Adapted by Edgar Casanova Cardoz - Diciembre 2014
# ecardoz@idun.com.mx
# http://idun.com.mx
#
# Genera un STREAM para el puerto Serial con datos de
# monitoreo del sistema.

# Comunicacion con el puerto serial
# Se envian datos a la placa Arduino
from time import sleep
import serial
import psutil

puerto = serial.Serial('/dev/ttyACM0',9600)



class FreeMemLinux(object):

    """
    Non-cross platform way to get free memory on Linux. Note that this code 
    uses the key word as, which is conditionally Python 2.5 compatible!
    If for some reason you still have Python 2.5 on your system add in the head
    of your code, before all imports:
    from __future__ import with_statement
    """

    def __init__(self, unit='kB'):

        with open('/proc/meminfo', 'r') as mem:
            lines = mem.readlines()

        self._tot = int(lines[0].split()[1])
        self._free = int(lines[1].split()[1])
        self._buff = int(lines[2].split()[1])
        self._cached = int(lines[3].split()[1])
        self._shared = int(lines[20].split()[1])
        self._swapt = int(lines[14].split()[1])
        self._swapf = int(lines[15].split()[1])
        self._swapu = self._swapt - self._swapf

        self.unit = unit
        self._convert = self._faktor()

    def _faktor(self):
        """determine the convertion factor"""
        if self.unit == 'kB':
            return 1
        if self.unit == 'k':
            return 1024.0
        if self.unit == 'MB':
            return 1/1024.0
        if self.unit == 'GB':
            return 1/1024.0/1024.0
        if self.unit == '%':
            return 1.0/self._tot
        else:
            raise Exception("Unit not understood")

    @property
    def total(self):
        return self._convert * self._tot

    @property
    def used(self):
        return self._convert * (self._tot - self._free)

    @property
    def used_real(self):
        """memory used which is not cache or buffers"""
        return self._convert * (self._tot - self._free - self._buff - self._cached)

    @property
    def shared(self):
        return self._convert * (self._tot - self._free)

    @property
    def buffers(self):
        return self._convert * (self._buff)

    @property
    def cached(self):
        return self._convert * self._cached

    @property
    def user_free(self):
        """This is the free memory available for the user"""
        return self._convert *(self._free + self._buff + self._cached)

    @property
    def swap(self):
        return self._convert * self._swapt

    @property
    def swap_free(self):
        return self._convert * self._swapf

    @property
    def swap_used(self):
        return self._convert * self._swapu

print "Iniciando... "
sleep(2)
print "En 3..."
sleep(1)
print "En 2..."
sleep(1)
print "En 1..."
sleep(1)
a = True

while a:
	f = FreeMemLinux()
	stream = ""
	cpus = ""

	usedRam = str(int(round(f.used_real))/1024)
	freeRam = str(int(round(f.total-f.used))/1024)
	perCPU  = psutil.cpu_percent(interval=1, percpu=True)	#Array

	for i in range(len(perCPU)):
		stream += "CPU " + str(i) + ":"
		if perCPU[i]<100:
			if perCPU[i]<10:
				stream += "  "
			else:
				stream += " "
		stream += str(int(round(perCPU[i]))) + "%\n"

	stream += "uRAM: "
	if int(usedRam) < 1000:
		if int(usedRam) < 100:
			if int(usedRam) < 10:
		 		stream += "   "
		 	else:
		 		stream += "  "
		else:
		 	stream += " "
	stream += usedRam + "MB\n"

	stream += "fRAM: "
	if int(freeRam) < 1000:
		if int(freeRam) < 100:
			if int(freeRam) < 10:
		 		stream += "   "
		 	else:
		 		stream += "  "
		else:
		 	stream += " "
	stream += freeRam + "MB" + "\0"
	puerto.write(stream)
