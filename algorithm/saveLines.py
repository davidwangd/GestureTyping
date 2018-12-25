# -*- coding: utf-8 -*-
board_h=[1,2,2,1,0, 1,1,1,0,1,1, 1,2,2,0,0, 0,0,1,0,0,2, 0,2,0,2]; #012
board_w=[0,4,2,2,2, 3,4,5,7,6,7, 8,6,5,8,9, 0,3,1,4,6,3, 1,1,5,0];
#		 a b c d e  f g h i j k  l m n o p  q r s t u v  w x y z
fileIn=open('词库.txt', mode='r', encoding='utf-8')
fileOut=open('out.txt', mode='w', encoding='utf-8')

for line in fileIn:
	word=line.split(' ')
	print("\n"+word[0])
	fileOut.write(word[0]+"\n")
	wordList=[]
	lineLen=[]
	allLen=0
	h=[]
	w=[]
	wordLen=len(word[0])
	wordList.append(ord(word[0][0])-ord('a'))
	h.append(board_h[wordList[0]])
	w.append(board_w[wordList[0]])
	for i in range(1,wordLen):
		chara=ord(word[0][i])-ord('a')
		h.append(board_h[chara])
		w.append(board_w[chara])
		wordList.append(chara)
		lineLen.append(float((h[i]-h[i-1])**2+(w[i]-w[i-1])**2) ** 0.5)
		allLen+=lineLen[i-1]
		#print(lineLen[i-1])
		#fileOut.write(str(lineLen[i-1])+"\n")
	lineLen.append(0)
	unit=allLen/49
	allLen=0
	#fileOut.write("unit "+str(unit)+"\n")

	ansH=[]
	ansW=[]
	left=0
	flag=0
	ansH.append(h[0])
	ansW.append(w[0])
	fileOut.write(str(ansH[flag])+" "+str(ansW[flag])+"\n")
	flag+=1
	for i in range(1,wordLen):
		if lineLen[i-1]==0:
			continue
		unit_h=unit/lineLen[i-1]*(h[i]-h[i-1])
		unit_w=unit/lineLen[i-1]*(w[i]-w[i-1])
		base_h=h[i-1]+(unit-left)/lineLen[i-1]*(h[i]-h[i-1])
		base_w=w[i-1]+(unit-left)/lineLen[i-1]*(w[i]-w[i-1])
		lineLen[i-1]+=left;
		#fileOut.write("left "+str(left)+"\n")
		while(lineLen[i-1]>=unit):
			#fileOut.write(str(lineLen[i-1])+"\n")
			lineLen[i-1]-=unit
			ansH.append(base_h)
			ansW.append(base_w)
			base_w+=unit_w
			base_h+=unit_h
			fileOut.write(str(ansH[flag])+" "+str(ansW[flag])+"\n")
			flag+=1
		left=lineLen[i-1]
	if len(ansW)!=50:
		if len(ansW)==49:
			fileOut.write(str(h[-1])+" "+str(w[-1])+"\n")
		elif len(ansW)==1:
			for i in range(1,50):
				fileOut.write(str(ansH[0])+" "+str(ansW[0])+"\n")
		#fileOut.write("~~~\n")
	fileOut.write("\n")
fileIn.close()
fileOut.close()