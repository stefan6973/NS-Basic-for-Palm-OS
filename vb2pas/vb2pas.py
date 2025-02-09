import os
from x2pas import VBConverter
from bas2pas import VBModule
from frm2pas import VBForm
from cls2pas import VBClass

src_folder = 'z:\\10 Projects\\Retro\\Palm\\NS-Basic-for-Palm-OS\\src'
dst_folder = 'z:\\10 Projects\\Retro\\Palm\\NS-Basic-for-Palm-OS\\pas2'

#------------------------------------------------------------
# returns a list of files with the given extension
#------------------------------------------------------------
def getFilesbyExtension(path: str, ext: str):
    result = []
    for root, dirs, fs in os.walk(path):
        for f in fs:
            if f.endswith(ext):
                result.append(os.path.join(root, f))
    return result

#------------------------------------------------------------
# convert all the files in the source folder
#------------------------------------------------------------
def main():
    #------------------------------------------------------------
    # Visual Basic Modules
    basFiles = getFilesbyExtension(src_folder, '.bas')
    for b in basFiles:
        try:
            VBModule(b).convert(dst_folder)
        except Exception as e:
            print(e)
    pass

    #------------------------------------------------------------
    # Visual Basic Classes
    clsFiles = getFilesbyExtension(src_folder, '.cls')
    for c in clsFiles:
        try:
            VBClass(c).convert(dst_folder)
        except Exception as e:
            print(e)    

    #------------------------------------------------------------
    # Visual Basic Forms
    frmFiles = getFilesbyExtension(src_folder, '.frm')
    for f in frmFiles:
        try:
            VBForm(f).convert(dst_folder)
        except Exception as e:
            print(e)

main()