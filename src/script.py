import sys
import time
from random import randint

CMDS = ["ins", "rem", "fin"]

def writeScript():
	print('Creating new text file')
	name = repr(time.time()) + '.txt' # Name of file depends of time created
	maxnum = 100

	# Important info includes:
	# (1) batchsize (2) cmdsize (3) num inserts (4) num removes

	cmdsize = 5

	inserts = 0
	removes = 0
	finds = 0

	# inserted = {}
	# len(inserted)

	try:
		file = open(name, 'a')
		file.write(str(cmdsize) + "\n")
		for x in range(0, cmdsize):
			cmd = CMDS[randint(0, 2)]
			val = randint(0, maxnum)
			file.write(cmd + " " + str(val) + "\n")
		file.close()
		print('Closed file!')

	except:
		print('Error with opening file!')
		sys.exit(0) # quit Python

writeScript()