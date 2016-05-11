s = "0000,"

for i in range(1,10000):
	s = s + str(i).zfill(4) + ","

print s
