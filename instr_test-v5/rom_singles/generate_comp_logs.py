import re
import sys

    
def main():
    if len(sys.argv)<2:
        print "Add a file name"
        sys.exit(0)

    myvector1 = []
    myvector2 = []

    INFILE = sys.argv[1]
    OUTFILE1 = INFILE.split('.')[0] + '_simple.log'
    OUTFILE2 = INFILE.split('.')[0] + '_pc.log'

    infile = open(INFILE,'r')

    for line in infile:
	    if line[0] == "A":
		    nline = re.sub(' +', ' ',line)
		    v1 = nline.split(" ")
		    myvector1.append(v1[0:6])
		    myvector2.append(v1[5])

    infile.close()

    outf1 = open(OUTFILE1, 'w')
    outf2 = open(OUTFILE2, 'w')

    for i in range(0,len(myvector1)):
	    outf1.write(" ".join(myvector1[i]) + "\n")
	    outf2.write(myvector2[i] + "\n")

    outf1.close()
    outf2.close()

if __name__ == '__main__':
    main()
