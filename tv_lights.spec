# -*- mode: python -*-

block_cipher = None

added_files = [
         ( r'C:\Users\micha\stuffs\tv_lights\tv_lights_data', 'tv_lights_data' ),
         ]
      
a = Analysis(['tv_lights.py'],
             pathex=['C:\\Users\\micha\\stuffs\\tv_lights'],
             binaries=[],
             datas=added_files,
             hiddenimports=[],
             hookspath=[],
             runtime_hooks=[],
             excludes=[],
             win_no_prefer_redirects=False,
             win_private_assemblies=False,
             cipher=block_cipher)
pyz = PYZ(a.pure, a.zipped_data,
             cipher=block_cipher)

   
exe = EXE(pyz,
          a.scripts,
          a.binaries,
          a.zipfiles,
          a.datas,
          name='tv_lights',
          debug=False,
          strip=False,
          upx=True,
          runtime_tmpdir=None,
          console=False )
