from os import listdir,remove
from os.path import isfile, join

path_to_cpp_source = "./src/pages.h"

def make_cpp_header():
    with open(path_to_cpp_source,"w+", encoding="utf8") as f:
        f.write("#include \"string.h\"\n\n")

def add_to_cpp(name,lines):
    with open(path_to_cpp_source, "a", encoding="utf8") as f:
        f.write(f"char* {name}_page = ")
        f.writelines(lines_to_write)
        f.write("\n\n")

if __name__ == "__main__":

    pages = [f for f in listdir("./pages") if isfile(join("./pages", f))]
    print(f"\nIn ./pages folder founded {len(pages)} docs")
    print(*pages)
    print("\n")
    make_cpp_header()

    for page in pages:
        with open ("./pages/" + page, encoding="utf8") as p:
            lines_to_write = ["\""]
            for line in p.readlines():
                line = line.replace("\"","\\\"")
                line = line.replace("\t","   ")
                line = line[:-1] + "\\n\\\n"
                lines_to_write.append(line)
            lines_to_write.append("\";")

            add_to_cpp(page[:page.find(".")], lines_to_write)


            
