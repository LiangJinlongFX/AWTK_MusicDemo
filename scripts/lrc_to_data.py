import os
import sys

filename = "123.lrc"

if __name__ == '__main__':
	file = open(filename,"r")
	result = []
	result += ["const unsigned char 123[] = {"]
	list = file.readlines()
	hex_array = []
	for line in list:
		for item in line:
			hex_array.append("0x%s, " %item.encode('hex'))
		print(hex_array)
	result += hex_array
	result += ["};"]
	with open("123.data",'w') as f:
		f.writelines(result)

		
