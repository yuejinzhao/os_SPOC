
data = []
block = []
disk = []

f = open("data", "r")
while(True):
    line = f.readline()
    if(line == ""):
        break;
    line = line.split()
    for i in line:
        i = "0x" + i
        i = eval(i)
        data.append(i)
    block.append(data)
    data = []
f.close()
f = open("disk", "r")
while(True):
    line = f.readline()
    if(line == ""):
        break;
    line = line.split()
    for i in line:
        i = "0x" + i
        i = eval(i)
        data.append(i)
    disk.append(data)
    data = []
f.close()


def convert(str):
    print "Virtual Address " + str + ":"
    global block
    number = eval(str)
    offset = number & 0x1f
    pdeNo = number >> 10
    pteNo = number >> 5 & 0x1f
    pde = block[0x6c][pdeNo]
    pdeValid = pde >> 7
    print " --> pde index:0x%x  pde contents:(valid %d pfn 0x%x)" % (pdeNo, pdeValid, pde & 0x7f)

    if(pdeValid == 0):
        print '\t\tFault (page directory entry not valid)'
        return

    pte = block[pde & 0x7f][pteNo]
    pteValid = pte >> 7
    print "    --> pte index:0x%x  pte contents:(valid %d pfn 0x%x)" % (pteNo, pte >> 7, pte & 0x7f)

    if (pteValid == 0) and (pte & 0x7f) == 0x1f:
        print "      --> nothing"
        return

    Address = offset + (pte % 128)*32
    if(pteValid == 0):
        print '     --> To Disk Sector Address 0x%x --> Value: %x ' % (Address, disk[pte & 0x7f][offset])
        return
    value = block[pte & 0x7f][offset]
    print "      --> Translates to Physical Address 0x%x --> Value: %x" % (Address, value)

convert("0x1e6f")
convert("0x0330")


