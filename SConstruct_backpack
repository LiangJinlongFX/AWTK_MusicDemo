# -*- coding: UTF-8 -*-
'''
AWTK工程模版的scons 构建脚本
适用于windows平台
构建环境依赖AWTK SDK的环境
#使用说明
 修改AWTK源码包路径
'''
import os
import sys
import platform

#注意修改AWTK路径
awtk_path = r"../awtk/"

try:
	sys.path.insert(0, awtk_path)
	import awtk_config as awtk
except Exception:
	print("can not find the awtk_config!!!")

#获取工程子目录路径,非特殊情况不要改变工程目录树
APP_ROOT    = os.path.normpath(os.getcwd())
APP_BIN_DIR = os.path.join(APP_ROOT, 'bin')
APP_LIB_DIR = os.path.join(APP_ROOT, 'lib')

#添加编译路径
os.environ['APP_ROOT'] = APP_ROOT;
os.environ['BIN_DIR'] = APP_BIN_DIR;
os.environ['LIB_DIR'] = APP_LIB_DIR;

#添加自定义编译信息
APP_CCFLAGS = ''
APP_LIBS = ['assets']
APP_LIBPATH = [APP_LIB_DIR]

#从awtk_config中获取编译选项
DefaultEnvironment(
  CPPPATH   = awtk.CPPPATH,
  LINKFLAGS = awtk.LINKFLAGS,
  LIBS      = APP_LIBS + awtk.LIBS,
  LIBPATH   = APP_LIBPATH + awtk.LIBPATH,
  CCFLAGS   = APP_CCFLAGS + awtk.CCFLAGS, 
  OS_SUBSYSTEM_CONSOLE=awtk.OS_SUBSYSTEM_CONSOLE,
  OS_SUBSYSTEM_WINDOWS=awtk.OS_SUBSYSTEM_WINDOWS)

#编译src下的源代码
SConscript(['src/SConscript'])

