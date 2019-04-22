#!/usr/bin/python
#encoding:utf-8

import os
import sys 
import update_res_common as common

#获取AWTK源码包目录
AWTK_ROOT="E:/awtk/"

#获取工程根目录
APP_ROOT=os.getcwd()
print(APP_ROOT)
if APP_ROOT.endswith('scripts'):
  APP_ROOT=os.path.dirname(APP_ROOT)

#改变工作目录
os.chdir(APP_ROOT);
#生成资源文件路径
ASSETS_ROOT=common.joinPath(APP_ROOT, 'res/assets')
ASSET_C=common.joinPath(APP_ROOT, 'res/assets.c')

def buildTools():
  os.system('scons '+common.toExe('resgen'))
  os.system('scons '+common.toExe('themegen'))
  os.system('scons '+common.toExe('imagegen'))
  os.system('scons '+common.toExe('fontgen'))
  os.system('scons '+common.toExe('xml_to_ui'))

def run():
  common.updateRes()

print(sys.getdefaultencoding())
print(sys.stdout.encoding)

#检测参数输入并判断指令功能
#common.showUsage()
common.init(AWTK_ROOT, ASSETS_ROOT, ASSET_C)
common.ACTION = "clean"
run()



#run()

