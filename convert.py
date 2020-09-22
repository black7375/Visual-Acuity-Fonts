from os import listdir
from os.path import isfile, join, islink, splitext
from fontTools.ttLib import TTFont
import fontforge

# target => otf ttf
# converted => otf ttf woff woff2
origin = (".otf", ".ttf")
target = (".otf", ".ttf", ".woff", ".woff2")

# Symbolic converted => pass
PATH = "./dist/"
files = [ f for f in listdir( PATH )
          if  isfile( join(PATH, f) )
          and islink( join(PATH, f) )]

def get_origin():
    fileNames  = [ splitext(f)[0] for f in files ]

    originIdxs  = list(set([ fileNames.index(fn) for fn in fileNames ]))
    originFiles = [ files[i] for i in originIdxs ]
    return originFiles

def get_target():
    originNames = [ splitext(originFile)[0] for originFile in originFiles ]
    targetNames = [ list(map(lambda extension: originName + extension, target))
                    for originName in originNames ]
    targetFiles = [[ targetFile for targetFile in targetName
                     if targetFile not in files ]
                   for targetName in targetNames ]
    return targetFiles

# Convert
originFiles = get_origin()
targetFiles = get_target()

def web_convert(originP, targetP, flavor):
    font = TTFont(originP)
    font.flavor = flavor
    font.save(targetP)

def general_convert(originP, targetP):
    font = fontforge.open(originP)
    font.generate(targetP)

def convert_font(originFile, targetFile):
    originP = join(PATH, originFile)
    targetP = join(PATH, targetFile)
    flavor  = splitext(targetFile)[1][1:]

    if flavor in ["woff", "woff2"]:
        web_convert(originP, targetP, flavor)
    else:
        general_convert(originP, targetP)

    print(originP + " -> " + targetP)

def convert_fonts():
    for i in range(len(originFiles)):
        originFile = originFiles[i]
        for targetFile in targetFiles[i]:
            convert_font(originFile, targetFile)

if __name__ == "__main__":
    convert_fonts()
