from os.path import join
Import("env")

env.Execute("python ./scripts/html2cpp.py")