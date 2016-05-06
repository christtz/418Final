import sys
import time
from random import randint
from random import shuffle


CMDS = ["ins", "rem", "fin"]

def main():
	args = sys.argv

	for x in range(0, len(args)):
		if (args[x] == '-r'): 
			maxnum = int(args[x+1])
		elif (args[x] == '-n'): 
			cmdsize = int(args[x+1])
		elif (args[x] == '-b'): 
			batchsize = int(args[x+1])
		else: 
			pass
	# consecutive script
	if (args[1] == "consec"): 
		consecScript(batchsize, cmdsize)
	elif (args[1] == "rand"):
		randScript(batchsize, cmdsize, maxnum)
        elif (args[1] == "insrand"):
                insRandScript(cmdsize, maxnum)
        elif (args[1] == "insconsec"):
                insConsecScript(cmdsize)
        else:
		pass

def insConsecScript(cmdsize):
	print('Creating new text file')
	name = 'ins' + repr(time.time()) + '.txt' # Name of file depends of time created

	try:
		file = open(name, 'a')
		file.write(str(0) + "\n")
                val = 0
		for x in range(0, cmdsize):
		    file.write("ins " + str(val) + "\n")
                    val += 1
		file.close()
		print('Closed file!')

	except:
		print('Error with opening file!')
		sys.exit(0) # quit Python	


def insRandScript(cmdsize, maxnum):
	print('Creating new text file')
	name = 'ins' + repr(time.time()) + '.txt' # Name of file depends of time created

	try:
		file = open(name, 'a')
		file.write(str(0) + "\n")
		for x in range(0, cmdsize):
                    val = randint(0, maxnum)
		    file.write("ins " + str(val) + "\n")
		file.close()
		print('Closed file!')

	except:
		print('Error with opening file!')
		sys.exit(0) # quit Python	


def consecScript(batchsize, cmdsize):
	print('Creating new text file')
	name = 'consec' + repr(time.time()) + '.txt' # Name of file depends of time created

	# Important info includes:
	# (1) batchsize (2) cmdsize (3) num inserts (4) num removes

	try:
		file = open(name, 'a')
		file.write(str(batchsize) + "\n")
		shouldInsert = True
		val = 0
		for x in range(0, cmdsize, batchsize):
			for y in range(0, min(batchsize, cmdsize-x)):
				cmd = "rem"
				if (shouldInsert): cmd = "ins"
				file.write(cmd + " " + str(val) + "\n")
				val += 1;
			val = 0
			shouldInsert = not shouldInsert
		file.close()
		print('Closed file!')

	except:
		print('Error with opening file!')
		sys.exit(0) # quit Python	

def randScript(batchsize, cmdsize, maxnum):
	print('Creating new text file')
	name = 'rand' + repr(time.time()) + '.txt' # Name of file depends of time created

	pastInserted = []
	currInserted = []
	try:
		file = open(name, 'a')
		file.write(str(batchsize) + "\n")
		for x in range(0, cmdsize, batchsize):
			for y in range(0, min(batchsize, cmdsize-x)):
				cmd = CMDS[randint(0, 1)]
				if (cmd == "rem" and len(pastInserted) > 0):
					shuffle(pastInserted)
					val = pastInserted[0]
					pastInserted.pop(0)
				else:
					cmd = "ins"
					val = randint(0, maxnum)
					currInserted.append(val)
				file.write(cmd + " " + str(val) + "\n")
			pastInserted += currInserted
			currInserted = []
		
		file.close()
		print('Closed file!')

	except:
		print('Error with opening file!')
		sys.exit(0) # quit Python


# def writeScript():
# 	print('Creating new text file')
# 	name = repr(time.time()) + '.txt' # Name of file depends of time created
# 	maxnum = 5

# 	# Important info includes:
# 	# (1) batchsize (2) cmdsize (3) num inserts (4) num removes
# 	batchsize = 3
# 	cmdsize = 12

# 	inserts = 0
# 	removes = 0
# 	finds = 0

# 	pastInserted = []
# 	currInserted = []
# 	try:
# 		file = open(name, 'a')
# 		file.write(str(cmdsize) + "\n")
# 		for x in range(0, cmdsize, batchsize):
# 			for y in range(0, min(batchsize, cmdsize-x)):
# 				cmd = CMDS[randint(0, 2)]
# 				if (cmd == "ins"):
# 					val = randint(0, maxnum)
# 					currInserted.append(val)
# 				elif (cmd == "rem" and len(pastInserted) > 0):
# 					shuffle(pastInserted)
# 					val = pastInserted[0]
# 					pastInserted.pop(0)
# 				else:
# 					# If trying to remove when nothing to delete,
# 					# immediately change command to find
# 					cmd = "find"
# 					val = randint(0, maxnum)
# 				file.write(cmd + " " + str(val) + "\n")
# 			pastInserted += currInserted
# 			currInserted = []
		
# 		file.close()
# 		print('Closed file!')

# 	except:
# 		print('Error with opening file!')
# 		sys.exit(0) # quit Python

# randScript()
# consecScript()
main()
