#!/usr/bin/python
#encoding:utf-8

import os
import sys
import copy
import glob
import shutil
import platform

###########################
ASSETS_ROOT_DIR = ""
ASSETS_RAW_DIR = ""
ASSETS_INC_DIR = ""
TOOLS_DIR = ""
###########################


################################# 通用函数 ##################################################################

def to_var_name(s):
    '''
    将字符串转换成由数字和字母组成的字符串,其他符号则转换成_
    '''
    out = ''
    for c in s:
        if(c.isalpha() or c.isdigit()):
            out += c
        else:
            out += '_'
    return out


def fix_output_file_name(name):
    '''
    规范化文件名
    '''
    filename, extname = os.path.splitext(name)
    basename = os.path.basename(filename)
    dirname = os.path.dirname(filename)
    newname = os.path.normpath(os.path.join(
        dirname, to_var_name(basename) + extname))
    return newname


def joinPath(root, subdir):
    '''
    @method 拼接路径
    @params root 根目录
    @params subdir 要拼接的相对目录
    @return 合成的路径
    '''
    return os.path.normpath(os.path.join(root, subdir))

def toExePath(root, name):
    if platform.system() == 'Windows':
        return joinPath(root, name + '.exe')
    else:
        return joinPath(root, name)


def buildAll():
    '''
    调用 scons 编译命令
    '''
    os.system('scons')


def removeDir(path):
    '''
    移除目录
    '''
    if os.path.isdir(path):
        print('rmdir:' + path)
        shutil.rmtree(path)


def prepareOutputDir(assets_inc_dir, name):
    '''
    检查输出目录是否存在, 没有则创建
    '''
    fullpath = joinPath(assets_inc_dir, name)
    if os.path.exists(fullpath):
        print(fullpath+" exist.")
    else:
        os.makedirs(fullpath)

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
			raw=copy.copy(f);
			if dst_suffix == '':
				inc=''
			else :
				inc=copy.copy(f)
				inc=inc.replace(src_dir, dst_dir)
				inc=inc.replace(src_suffix, dst_suffix)
			os.system(tool_path + ' ' + joinPath(src_dir, raw) + ' ' + joinPath(dst_dir, inc) + ' ' + option)


###################################### 资源生成方法 ##########################################################

def genTheme(tools_dir, src_dir, dst_dir):
    '''
    生成主题形式资源文件
    '''
    generate(tools_dir, 'themegen', src_dir, 'styles', '.xml', dst_dir, 'styles', '.data', '', 1)

def genTheme_bin(tools_dir, src_dir, dst_dir):
    '''
    生成主题形式资源二进制数据
    '''
    generate(tools_dir, 'themegen', src_dir, 'styles', '.xml', src_dir, 'styles', '.bin', 'bin', 0)


def genUI(tools_dir, src_dir, dst_dir):
    '''
    生成UI资源文件
    '''
    generate(tools_dir, 'xml_to_ui', src_dir, 'ui', '.xml', src_dir, 'ui', '.bin', 'bin', 0)

def genUI_bin(tools_dir, src_dir, dst_dir):
    '''
    生成UI资源二进制数据
    '''
    generate(tools_dir, 'xml_to_ui', src_dir, 'ui', '.xml', dst_dir, 'ui', '.data', '', 1)

def genString(tools_dir, src_dir, dst_dir):
    '''
    生成字符资源文件
    '''
    generate(tools_dir, 'strgen', src_dir, 'strings', '.xml', dst_dir, 'strings', '', '', 1)


def genString_bin(tools_dir, src_dir, dst_dir):
    '''
    生成字符资源二进制文件
    '''
    generate(tools_dir, 'strgen', src_dir, 'strings', '.xml', src_dir, 'strings', '.bin', 'bin', 0)


def genRes(tools_dir, src_dir, dst_dir):
    '''
    生成其他资源的资源文件
    '''
    generate(tools_dir, 'resgen', src_dir, 'data', '.lrc', dst_dir, 'data', '.data', '', 1)


def genFont(tools_dir, src_dir, dst_dir):
    '''
    生成字体资源文件
    '''
    generate(tools_dir, 'resgen', src_dir, 'fonts', '.ttf', dst_dir, 'fonts', '.res', '', 1)


def genImage_bin(tools_dir, src_dir, dst_dir, dpi):
    '''
    生成图像资源文件
    '''
    IMAGEGEN_OPTIONS = '\"bgra|bgr565\"'
    suffix = ['.png', '.jpg']
    generate(tools_dir, 'resgen', src_dir, 'images/' + dpi, suffix, dst_dir, 'images', '.res', '', 0)

def genImage(tools_dir, src_dir, dst_dir, dpi):
    '''
    生成图像资源文件
    '''
    IMAGEGEN_OPTIONS = '\"bgra|bgr565\"'
    suffix = ['.png', '.jpg']
    generate(tools_dir, 'imagegen', src_dir, 'images/' + dpi, suffix, dst_dir, 'images', '.data', IMAGEGEN_OPTIONS, 0)


########################################## 生成某类资源函数集合 ###############################################################


def gen_res_all_image():
    '''
    生成所有image资源文件
    '''
    genImage(TOOLS_DIR, ASSETS_RAW_DIR, ASSETS_INC_DIR, 'x1')
    genImage_bin(TOOLS_DIR, ASSETS_RAW_DIR, ASSETS_INC_DIR, 'x1')

def gen_res_all_style():
    '''
    生成所有style资源文件
    '''
    genTheme(TOOLS_DIR, ASSETS_RAW_DIR, ASSETS_INC_DIR)
    genTheme_bin(TOOLS_DIR, ASSETS_RAW_DIR, ASSETS_INC_DIR)

def gen_res_all_ui():
    '''
    生成所有ui资源文件
    '''
    genUI(TOOLS_DIR, ASSETS_RAW_DIR, ASSETS_INC_DIR)
    genUI_bin(TOOLS_DIR, ASSETS_RAW_DIR, ASSETS_INC_DIR)

def gen_res_all_data():
    '''
    生成所有data资源文件
    '''
    genRes(TOOLS_DIR, ASSETS_RAW_DIR, ASSETS_INC_DIR)

def gen_res_all_font():
    '''
    生成所有font资源文件
    '''
    genFont(TOOLS_DIR, ASSETS_RAW_DIR, ASSETS_INC_DIR) 

def gen_res_all_string():
    '''
    生成所有的字符串资源
    '''
    genString(TOOLS_DIR, ASSETS_RAW_DIR, ASSETS_INC_DIR) 
    genString_bin(TOOLS_DIR, ASSETS_RAW_DIR, ASSETS_INC_DIR) 

def cleanRes(assets_dir):
    '''
    清除编译生成的inc资源文件
    @params assets_dir 资源根目录
    '''
    assets_raw_dir = joinPath(assets_dir, "raw")
    assets_inc_dir = joinPath(assets_dir, "inc")
    print("==================================================================")
    resFiles = glob.glob(joinPath(assets_raw_dir, '*/*.bin')) + \
        glob.glob(joinPath(assets_raw_dir, '*/*/*.bin'))
    for f in resFiles:
        print("remove: " + f)
        os.remove(f)
    resFiles = glob.glob(joinPath(assets_raw_dir, '*/*.bin')) + \
        glob.glob(joinPath(assets_raw_dir, '*/*/*.bsvg'))
    for f in resFiles:
        print("remove: " + f)
        os.remove(f)
    removeDir(assets_inc_dir)
    print("==================================================================")


def gen_res_all():
    gen_res_all_string()
    gen_res_all_font()
    gen_res_all_image()
    gen_res_all_ui()
    gen_res_all_style()
    gen_res_all_data()


################################################## 生成资源包含 C Header文件方法 ###################################################################


def writeToFile(file_name, str):
    '''
    将内容写入文件
    '''
    fd = os.open(file_name, os.O_RDWR|os.O_CREAT|os.O_TRUNC)
    os.write(fd, str)
    os.close(fd)


def genIncludes(files, dir_name):
    '''
    生成#include C语句
    @params files 文件列表
    @params dir_name 文件目录路径
    '''
    str1 = ""
    for f in files:
        incf = copy.copy(f);
        incf=incf.replace(dir_name, "/inc");
        incf=incf.replace('\\', '/');
        str1 += '#include "'+incf+'"\n'
    return str1

def GetFileBaseName(file_name, root_dir_name, subdir_name, suffix):
    '''
    生成资源文件名中的数组名
    @params file_name 文件名
    @params root_dir_name 资源根目录
    @params subdir_name 子资源目录
    @params suffix 后缀名
    '''
    name = file_name.replace(root_dir_name, '');
    name = name.replace('\\', '/');
    name = name.replace('/' + subdir_name + '/', '');
    name = name.replace(suffix, '');
    return name;

def genAssetsManagerAdd(assets_inc_dir, filter, dir_name, name, suffix):
    '''
    生成"assets_manager_add"C语句,往资源管理器添加数据
    @params assets_inc_dir 资源inc目录路径
    @params filter 文件过滤器
    @params dir_name 当前资源目录名称
    @params name 资源命名前缀
    @params suffix 资源文件后缀
    '''
    files=glob.glob(joinPath(assets_inc_dir, filter))
    result = ''
    for f in files:
        basename = GetFileBaseName(copy.copy(f), assets_inc_dir, dir_name,  suffix)
        result += '  assets_manager_add(rm, ' + name + basename + ');\n'
    return result

def genAssetsManagerAddWithsuffix(assets_inc_dir, filter, dir_name, name, suffix):
    '''
    生成"assets_manager_add"C语句,往资源管理器添加数据
    用于.data后缀的资源文件
    @params assets_inc_dir 资源inc目录路径
    @params filter 文件过滤器
    @params dir_name 当前资源目录名称
    @params name 资源命名前缀
    @params suffix 源资源文件后缀
    '''
    files=glob.glob(joinPath(assets_inc_dir, filter))
    result = ''
    for f in files:
        basename = GetFileBaseName(copy.copy(f), assets_inc_dir, dir_name, '.data')
        basename += suffix
        basename = basename.replace('.', '_')
        result += '  assets_manager_add(rm, ' + name + basename + ');\n'
    return result


def gen_assets_header(assets_dir, assets_c_path):
    '''
    生成资源的 C 头文件，以将资源引入AWTK中
    @params assets_dir 工程里的资源存放目录
    @params assets_c_path 生成的C文件存放路径
    '''
    #获取inc文件路径
    assets_inc_dir = joinPath(assets_dir, "inc")
    if not os.path.exists(assets_inc_dir):
        print('assets inc dir not exist')
        exit()

    #包含AWTK头文件
    result = '#include "awtk.h"\n'

    #定义是否使用文件系统的宏
    result += '#ifndef WITH_FS_RES\n'

    #包含图片文件
    result += '//包含图片资源文件\n'
    result += "#ifdef WITH_STB_IMAGE\n"
    result += '//如果定义了WITH_STB_IMAGE宏,使用原始的图片res数据,否则使用经解码后的.data数据\n'
    files=glob.glob(joinPath(assets_inc_dir, 'images/*.res')) 
    result += genIncludes(files, assets_inc_dir)
    result += "#else\n"
    files=glob.glob(joinPath(assets_inc_dir, 'images/*.data')) 
    result += genIncludes(files, assets_inc_dir)
    result += '#endif/*WITH_STB_IMAGE*/\n'

    result += "#else\n"

    #包含data文件夹下的数据
    result += '//包含data文件夹下的二进制数据\n'
    files = glob.glob(joinPath(assets_inc_dir, 'data/*.data')) \
        + glob.glob(joinPath(assets_inc_dir, 'data/*.bin'))
    result += genIncludes(files, assets_inc_dir)

    #包含UI二进制资源文件
    result += '//包含UI二进制资源文件\n'
    files=glob.glob(joinPath(assets_inc_dir, 'ui/*.data')) 
    result += genIncludes(files, assets_inc_dir)

    #包含style二进制资源文件
    result += '//包含style二进制资源文件\n'
    files=glob.glob(joinPath(assets_inc_dir, 'styles/*.data')) 
    result += genIncludes(files, assets_inc_dir)

    #包含string二进制资源文件
    result += '//包含string二进制资源文件\n'
    files=glob.glob(joinPath(assets_inc_dir, 'strings/*.data')) 
    result += genIncludes(files, assets_inc_dir)

    #包含字体文件
    result += '//包含字体资源文件\n'
    result += "#ifdef WITH_STB_FONT\n"
    result += '//如果定义了WITH_STB_FONT宏,将使用原始的ttf字体资源,否则使用解码后的.data数据\n'
    result += "#ifdef WITH_MINI_FONT\n"
    result += '//如果定义了WITH_MINI_FONT宏,将使用MINI字体资源\n'
    files=glob.glob(joinPath(assets_inc_dir, 'fonts/default.mini.res')) 
    result += genIncludes(files, assets_inc_dir)
    result += "#else/*WITH_MINI_FONT*/\n"
    files=glob.glob(joinPath(assets_inc_dir, 'fonts/default.res')) 
    result += genIncludes(files, assets_inc_dir)
    result += '#endif/*WITH_MINI_FONT*/\n'
    result += "#else/*WITH_STB_FONT*/\n"
    files=glob.glob(joinPath(assets_inc_dir, 'fonts/*.data')) 
    result += genIncludes(files, assets_inc_dir)
    result += '#endif/*WITH_STB_FONT*/\n'

    #结束WITH_FS_RES
    result += '#endif/*WITH_FS_RES*/\n'

    result += '\n';
    result += '//定义资源初始化函数\n'
    result += 'ret_t assets_init(void) {\n'
    result += '  assets_manager_t* rm = assets_manager();\n\n'
    result += ''

    result += '//在文件系统中预先加载字体资源\n'
    result += '#ifdef WITH_FS_RES\n'
    result += '  #ifdef WITH_MINI_FONT\n'
    result += '  asset_info_t* info = assets_manager_load(rm, ASSET_TYPE_FONT, "default.mini");\n'
    result += '  if (info) {\n'
    result += '    strcpy(info->name, "default");\n'
    result += '  }\n'
    result += '  #else \n'
    result += '    assets_manager_load(rm, ASSET_TYPE_FONT, "default");\n'
    result += '  #endif\n'
    result += '  //预先加载默认风格\n'
    result += '  assets_manager_load(rm, ASSET_TYPE_STYLE, "default");\n'
    result += '#else /*WITH_FS_RES*/\n'
    result += '  #ifdef WITH_MINI_FONT\n'
    result += '  assets_manager_add(rm, "default.mini");\n'
    result += '  if (info) {\n'
    result += '    strcpy(info->name, "default");\n'
    result += '  }\n'
    result += '  #else \n'
    result += '  assets_manager_add(rm, "default");\n'
    result += '  #endif /*WITH_MINI_FONT*/\n'

    #添加资源常量数组
    result += genAssetsManagerAddWithsuffix(assets_inc_dir, 'data/*.data', 'data', 'data_', '.lrc')
    result += genAssetsManagerAdd(assets_inc_dir, 'fonts/*.data', 'fonts', 'font_', '.data')
    result += genAssetsManagerAdd(assets_inc_dir, 'images/*.data', 'images', 'image_', '.data')
    result += genAssetsManagerAdd(assets_inc_dir, 'styles/*.data', 'styles', 'style_', '.data')
    result += genAssetsManagerAdd(assets_inc_dir, 'ui/*.data', 'ui', 'ui_',  '.data')
    result += genAssetsManagerAdd(assets_inc_dir, 'strings/*.data', 'strings', 'strings_', '.data')
    result += '#endif /*WITH_FS_RES*/\n'

    result += '\n'
    result += '  tk_init_assets();\n'
    result += '  return RET_OK;\n'
    result += '}\n'

    print(result)
    writeToFile(assets_c_path, result);

###########################################################################################################

def init(awtk_root, assets_root, asset_c):
    '''
    初始化资源生成工具环境以及资源输出目录
    '''
    tools_dir = os.path.abspath(joinPath(awtk_root, 'bin'))
    assets_root_dir = os.path.abspath(assets_root)
    assets_c_path = os.path.abspath(asset_c)

    #检查 raw 目录是否存在
    assets_raw_dir = joinPath(assets_root_dir, 'raw')
    if os.path.exists(assets_raw_dir):
        print(assets_raw_dir + " exist.")
    else:
        print("cannot find the " + assets_raw_dir)
        exit()

    #检查 inc 目录是否存在
    assets_inc_dir = joinPath(assets_root_dir, 'inc')
    if os.path.exists(assets_inc_dir):
        print(assets_inc_dir + " exist.")
    else:
        print("make dir " + assets_inc_dir)
        os.makedirs(assets_inc_dir)
    #检查 子资源 目录是否存在
    prepareOutputDir(assets_inc_dir, "data")
    prepareOutputDir(assets_inc_dir, "fonts")
    prepareOutputDir(assets_inc_dir, "images")
    prepareOutputDir(assets_inc_dir, "strings")
    prepareOutputDir(assets_inc_dir, "styles")
    prepareOutputDir(assets_inc_dir, "ui")
    #赋值全局变量
    global TOOLS_DIR
    global ASSETS_INC_DIR
    global ASSETS_RAW_DIR
    global ASSETS_ROOT_DIR

    ASSETS_ROOT_DIR = assets_root
    ASSETS_RAW_DIR = assets_raw_dir
    ASSETS_INC_DIR = assets_inc_dir
    TOOLS_DIR = tools_dir


def updateRes(option):
    '''
    功能选择菜单
    '''
    if option == '-all':
        gen_res_all()
    elif option == '-c':
        cleanRes(ASSETS_ROOT_DIR)
    elif option == '-string':
        gen_res_all_string()
    elif option == "-font":
        gen_res_all_font()
    elif option == '-image':
        gen_res_all_image()
    elif option == '-ui':
        gen_res_all_ui()
    elif option == '-style':
        gen_res_all_style()
    elif option == '-data':
        gen_res_all_data()
    else:
        showUsage()


def showUsage():
  
    args = ' action[clean|web|all|font|image|ui|style|string|script|data|xml] dpi[x1|x2] image_options[rgba|bgra+bgr565]'
    if len(sys.argv) == 1:
        print('=========================================================')
        print('Usage: '+sys.argv[0] + args)
        print('Example:')
        print(sys.argv[0] + ' all')
        print(sys.argv[0] + ' clean')
        print(sys.argv[0] + ' style')
        print(sys.argv[0] + ' all x1 bgra+bgr565')
        print('=========================================================')
        sys.exit(0)
    else:
        ACTION = sys.argv[1]
        if len(sys.argv) > 2:
            DPI = sys.argv[2]
        else:
            DPI = 'x1'

        if len(sys.argv) > 3:
            IMAGEGEN_OPTIONS = sys.argv[3]
        else:
            IMAGEGEN_OPTIONS = 'bgra+bgr565'


#showUsage()
if __name__ == '__main__':
    init("../../awtk", "../res/assets", "./");
    #updateRes("-c");
    updateRes("-all")
    #updateRes("-image")
    #gen_assets_header(ASSETS_ROOT_DIR, "../res/assets/assets.h")