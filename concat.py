import os
import sys

class Stack:
    def __init__(self):
        self.__stack = []

    def push(self, ele):
        self.__stack.append(ele)

    def pop(self):
        return self.__stack.pop()

    def get(self):
        return self.__stack


class PathFixer:
    def __init__(self, cwd):
        self.__stk = Stack()
        lis = cwd.split('/')
        for e in lis:
            self.__stk.push(e)

    def move(self, path):
        lis = path.split('/')
        for e in lis:
            if e == '.':
                continue
            elif e == '..':
                self.__stk.pop()
            else:
                self.__stk.push(e)
        return self

    def get(self):
        return '/'.join(self.__stk.get())


class ListUpFiles:
    def __init__(self, src):
        self.src = src
        self.__file_list = []
        self.__appended = set()
        self.__include = "#include"
        self.__debug_file = "debug.cpp"

    def __should_listup(self, path):
        if path.find(self.__include) == -1:
            return False
        if path.find(self.__debug_file) != -1:
            return False
        if path.find('<') != -1:
            return False
        return True

    def __get_cwd(self, cur_file):
        return '/'.join(cur_file.split('/')[:-1])

    def __extract_included_file(self, include):
        start = include.find('\"')
        result = include[(start + 1):]
        finish = result.find('\"')
        return result[:finish]

    def __listup_rec(self, cur_file):
        cwd = self.__get_cwd(cur_file)
        with open(cur_file) as f:
            for line in f.readlines():
                if not self.__should_listup(line):
                    continue
                included_file = self.__extract_included_file(line)
                path = PathFixer(cwd).move(included_file).get()
                if path in self.__appended:
                    continue
                self.__appended.add(path)
                self.__listup_rec(path)
        self.__file_list.append(cur_file)

    def listup(self):
        self.__listup_rec(self.src)
        return self.__file_list


def write_to_file(fr, fw):
    lis = filter(lambda l: l.find('#include \"') == -1, fr.readlines())
    lis = filter(lambda l: l.find('#pragma once') == -1, lis)
    fw.writelines(lis)


def concat_files(flie_list, fw):
    for f in flie_list:
        with open(f) as fr:
            write_to_file(fr, fw)


def rewrite_src(src, dst):
    file_list = ListUpFiles(src).listup()
    print(file_list)
    output_filename = './tmp.txt'
    with open(output_filename, mode='w') as fw:
        concat_files(file_list, fw)
    with open(dst, mode='w') as fw:
        with open(output_filename) as fr:
            fw.writelines(fr.readlines())


def main(src):
    src = PathFixer(os.getcwd()).move(src).get()
    ext = '.cpp'
    remove_ext = src[:-len(ext)]
    dst = remove_ext + '_submit' + ext
    rewrite_src(src, dst)


if __name__ == '__main__':
    main(sys.argv[1])
