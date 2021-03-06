#encoding:utf-8
import os
import sys
import glob
import shutil
import copy
import re
import platform

reload(sys)
sys.setdefaultencoding('utf-8')

TOOTS_DIR = "E:\\awtk\\bin\\"
DST_DIR = "..\\res\\assets\\inc"
SRC_DIR = "..\\res\\assets\\raw"


def check_python_version():
	print("python version: " + platform.python_version())
	major_version = sys.version_info[0]
	if major_version > 2:
		print("The python version is %d.%d. But python2.x is required.(Version 2.7 is well tested!)" %(major_version, sys.version_info[1]))
		return True
	return False

def removeDir(path):
	'''
	移除目录
	'''
	if os.path.isdir(path):
		print('rmdir:' + path);
		shutil.rmtree(path)

def toExePath(root, name):
	'''
	获取生成工具程序路径
	@params root 根目录
	@params name 工具程序名称
	'''
	if platform.system() == 'Windows':
		return joinPath(root, name + '.exe')
	else:
		return joinPath(root, name)

def format_filename(path):
	'''
	格式化目录下的所有文件名
	将文件名的',' ';' ' ' 统统替换成'_'
	'''
	filenames = os.walk(path)
	for item in filenames:
		for i in item[2]:
			b = re.sub(r'[ ,;-]', '_', i)
			print(b)

def generate(tools_dir, tool_name, src_dir, src_sub_dir, src_suffix, dst_dir, dst_sub_dir, dst_suffix, option, is_remake_dir):
	'''
	调用exe工具生成资源文件
	@params: tools_dir 工具集目录
	@params: tool_name 生成工具名称
	@params: src_dir   源文件目录
	@params: src_sub_dir 源文件子资源目录
	@params: src_suffix 生成工具名称
	@params: src_suffix 源文件后缀
	@params: dst_dir 目标生成资源目录
	@params: dst_sub_dir 目标生成资源子目录
	@params: dst_suffix 目标生成资源文件后缀
	@params: option 	附加选项
	@params: is_remake_dir 是否要重建目录 
	'''
	tool_path = toExePath(tools_dir, tool_name)
	src_dir = joinPath(src_dir, src_sub_dir)
	dst_dir = joinPath(dst_dir, dst_sub_dir)

	if not os.path.exists(tool_path) :
		print(tool_path + ' not exist')
		return
	if not os.path.exists(src_dir) :
		print(src_dir + ' not exist')
		return
	# Delete History
	if is_remake_dir :
		removeDir(dst_dir)
		os.makedirs(dst_dir);

	print(tool_path + '\n' + src_dir + '\n' + dst_dir)
	print("Start Generate...")

	# Generate
	# 判断对象是否是一个已知的类型
	if isinstance(src_suffix, list) :
		for f in glob.glob(joinPath(src_dir, '*.*')):
			raw=copy.copy(f);
			if dst_suffix == '':
				inc=''
			else :
				inc=copy.copy(f);
				inc=inc.replace(src_dir, dst_dir)
				for suffix in src_suffix : 
					inc=inc.replace(suffix, dst_suffix)
			print(tool_path + ' ' + joinPath(src_dir, raw) + ' ' + joinPath(dst_dir, inc) + ' ' + option)
			os.system(tool_path + ' ' + joinPath(src_dir, raw) + ' ' + joinPath(dst_dir, inc) + ' ' + option)
	else :
		for f in glob.glob(joinPath(src_dir, '*' + src_suffix)):
			print(f)
			raw=copy.copy(f);
			if dst_suffix == '':
				inc=''
			else :
				inc=copy.copy(f);
				inc=inc.replace(src_dir, dst_dir)
				inc=inc.replace(src_suffix, dst_suffix)
			print(tool_path + ' ' + joinPath(src_dir, raw) + ' ' + joinPath(dst_dir, inc) + ' ' + option)
			os.system(tool_path + ' ' + joinPath(src_dir, raw) + ' ' + joinPath(dst_dir, inc) + ' ' + option)

def joinPath(root, subdir):
	'''
	@method 拼接路径
	@params root 根目录
	@params subdir 要拼接的相对目录
	@return 合成的路径
	'''
	return os.path.normpath(os.path.join(root, subdir))

def output_res_c_source(filename, type, subtype, buff):
	'''
	生成AWTK特定的常量数组
	@params: filename LRC文件名
	@params: type 	  数据类型
	@params: subtype  数据子类型
	@params: buff 	  已转换的十六进制数据

	'''
	if type != "ASSET_TYPE_DATA":
		print("please set type \"ASSET_TYPE_DATA\"!")
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
	if check_python_version():
		exit()
	'''
	if len(sys.argv) <= 3 :
		print('Usage: assets_gen.py tools_dir src_dir dst_dir')
		exit()
	'''
	tools_dir = os.path.abspath(TOOTS_DIR)
	src_dir = os.path.abspath(SRC_DIR)
	dst_dir = os.path.abspath(DST_DIR)
	format_filename("D:\\audiofile\\")
	#generate(tools_dir, "resgen", src_dir, "data", ".lrc", dst_dir, "data", ".data", '', 1)
	#
	#

pyftsubset C:/Windows/Fonts/msyh.ttf --text-file=./res/assets/raw/fonts/text.txt --output-file=./res/assets/raw/fonts/default.mini.ttf
