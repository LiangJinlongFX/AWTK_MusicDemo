@rem ---------------------------------------------------------------------------------------------------
@rem һ��˫���������⡢�ַ��������塢ͼƬ��UI�ȶ������ļ�(���е���assets_gen.py)                                   
@rem �����޸� AWTK_BIN_DIR=E:/zlgopen/awtk/bin Ϊ����awtk��Ӧ��binĿ¼
@rem ����assets_gen.bat˵��
@rem	  1 Ϊ��(ʹ��800*480��Ļ)��
@rem		��assets_gen.bat
@rem	  2 -480_272(ʹ��480*272��Ļ):
@rem		��assets_gen.bat -480_272	
@rem �ġ�%PY_VERSION% assets_gen.py %AWTK_BIN_DIR%  ./src/assets/raw ./src/assets/inc˵����
@rem    1 Ϊ�գ�
@rem      ��assets_gen.py AWTK_BIN_DIR ./src/assets/raw ./src/assets/inc���������⡢�ַ��������塢ͼƬ��UI�ȶ������ļ�
@rem    2 -style 
@rem      ��assets_gen.py AWTK_BIN_DIR ./src/assets/raw ./src/assets/inc -style��  ��������
@rem    3 -string
@rem      ��assets_gen.py AWTK_BIN_DIR ./src/assets/raw ./src/assets/inc -string�� �����ַ���
@rem    4 -font
@rem      ��assets_gen.py AWTK_BIN_DIR ./src/assets/raw ./src/assets/inc -font��   ��������
@rem    5 -image
@rem      ��assets_gen.py AWTK_BIN_DIR ./src/assets/raw ./src/assets/inc -image��  ����ͼƬ
@rem    6 -ui
@rem      ��assets_gen.py AWTK_BIN_DIR ./src/assets/raw ./src/assets/inc -ui��     ����ui�ļ�
	
@rem ----------------------------------------------------------------------------------------------------

@set AWTK_BIN_DIR=../../awtk/bin
@set PY_VERSION=python
@if exist %SystemRoot%\py.exe (
	@set PY_VERSION=py -2
)

%PY_VERSION% ../assets_gen.py %AWTK_BIN_DIR% ./assets/raw ./assets/inc
%PY_VERSION% ../assets_c_gen.py ./assets