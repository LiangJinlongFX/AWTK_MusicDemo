#encoding:utf-8
import os
import sys
import glob

reload(sys)
sys.setdefaultencoding('utf-8')

DATA_DIR = "E:\\AWTK_MusicDemo_UI\\scripts"
LRC_DIR = "D:\\audiofile"

def joinPath(root, subdir):
    return os.path.normpath(os.path.join(root, subdir))

def output_res_c_source(filename, type, subtype, buff):
	'''
	生成AWTK特定的常量数组
	'''
	#生成数组名
	temp_str = filename.split('.')[0]
	array_name = temp_str.replace(' ','_')
	result = []
	result += ["const unsigned char %s_%s[] = {" % (type,array_name)]
	#生成数组asset_info信息
	
	print(result)


def lrc_to_array(lrcfilename, datafilename):
	'''
	将LRC文件转换成十六进制常量数组放于data文件中
	'''
	#存放生成的C源码
	result = []
	# 用于计算数组大小
	i = 0;
	lrcfile_path = lrcfilename
	lrc_file = open(lrcfile_path, 'r')
	lrc_lines = lrc_file.readlines()
	hex_array = []
	for line in lrc_lines:
		for item in line:
			hex_array.append("0x%s, " %item.encode('hex'))
			i = i+1
	lrc_file.close()
	
	temp_str = lrcfilename.split('.')[0]
	array_name = temp_str.replace(' ','_')
	result += ["const unsigned char %s[] = {" %array_name]
	result += hex_array
	result += ["0x00, 0x00, 0x00};"]

	return result



if __name__ == '__main__':
	'''
	os.chdir(LRC_DIR);
	lrc_files = glob.glob('*.lrc')
	datafile_path = joinPath(DATA_DIR, '123.data')
	with open(datafile_path,'w') as f:
		for item in lrc_files:
			result = lrc_to_array(item, '123.data')
			f.writelines(result)
			f.write("\n\n")
	f.close()
	'''
	output_res_c_source("brow you up", "lyric", 0, 0)

		
