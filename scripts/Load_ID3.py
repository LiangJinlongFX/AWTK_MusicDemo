#coding: utf-8
import mutagen
import glob
import os


AUDIO_DIR = "D:\\CloudMusic\\"

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

def joinPath(root, subdir):
  return os.path.normpath(os.path.join(root, subdir))

def Get_IMAGE(file):
	try:
		info = mutagen.File(file)
	except Exception as e:
		print("cannot open the file: %s" % file)
	else:
		album_name = info.tags['TALB'].text[0]
		print(album_name)
		artwork = info.tags['APIC:'].data
		file_name = to_var_name(album_name)
		with open(file_name + '.jpg', 'wb') as img:
			img.write(artwork)

if __name__ == '__main__':
	audiofiles = glob.glob(joinPath(AUDIO_DIR, "*.mp3"))
	for file in audiofiles:
		print(file)
		Get_IMAGE(file)
	