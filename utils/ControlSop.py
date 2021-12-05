
if __name__ == "__main__":
	fp = open("ControlTable.txt", "r") 
	lines = fp.readlines()
	rows =[]
	for line in lines:
		rows.append(line.split())
	fp.close()
	inputBits = {}
	controlBits = {}
	Sop = {}
	for i in range(7) :
		inputBits[rows[0][i]] = [rows[j][i] for j in range(1, len(rows))]
	for i in range(8,len(rows[0])) :
		controlBits[rows[0][i]] = [rows[j][i] for j in range(1, len(rows))]
		Sop[rows[0][i]] = ""
	for i in range(len(controlBits.keys())):
		key = list(controlBits.keys())[i]
		print(key,"* =",sep="",end=" ")
		for j in range(len(controlBits[key])):
			if(controlBits[key][j] == '1'):
				newStr = ""
				factors = []
				for k in inputBits:
					if inputBits[k][j] != 'X':
						factors.append((k,inputBits[k][j]))
				iter = 0
				for i in range(len(factors)):
					if iter > 0:
						if(factors[i][1] == '0'):
							print(",not_gate(",factors[i][0],")",sep="",end= "")
						else:
							print(",",factors[i][0],sep="",end = "")
					else:
						if(factors[i][1] == '0'):
							print("and_gate(not_gate(",factors[i][0],")",end = "",sep="")
						else:
							print("and_gate(",factors[i][0],end = "",sep="")
					iter+=1
				print(") ||")

