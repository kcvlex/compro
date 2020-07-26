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
        ite = filter(lambda s: s != '', cwd.split('/'))
        for e in ite:
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
        ret = '/'.join(self.__stk.get())
        if self.__stk.get()[0] == 'home':
            ret = '/' + ret
        return ret

class ListUpFiles:
    def __init__(self, src):
        self.src = src
        self.__file_list = []
        self.__appended = set()
        self.__include = "#include"
        self.__debug_file = "debug.hpp"
        self.__default_dir = "/home/taroy/kyopuro/lib/"

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
        import os.path
        cwd = self.__get_cwd(cur_file)
        with open(cur_file) as f:
            for line in f.readlines():
                if not self.__should_listup(line):
                    continue
                included_file = self.__extract_included_file(line)
                path1 = PathFixer(cwd).move(included_file).get()
                path2 = PathFixer(self.__default_dir + included_file).get()
                paths = [ path1, path2 ]
                for path in paths:
                    if path in self.__appended: continue;
                    if not os.path.isfile(path): continue;
                    self.__appended.add(path)
                    self.__listup_rec(path)
        self.__file_list.append(cur_file)

    def listup(self):
        self.__listup_rec(self.src)
        return self.__file_list


class Printer:
    def __init__(self):
        self._is_defined_cpp17 = False
        self._define_cpp17 = '#define CPP17'
        self._contents = []

    def _read_file(self, fr):
        lis = filter(lambda l: l.find('#include \"') == -1, fr.readlines())
        lis = filter(lambda l: l.find('#pragma once') == -1, lis)
        lis = list(lis)
        if len(list(filter(lambda l: l.find(self._define_cpp17) != -1, lis))) != 0:
            self._is_defined_cpp17 = True
        for i, e in enumerate(lis):
            if e.find('#define DEBUGGING') != -1:
                lis = lis[0:i]
                break
        lis = list(filter(lambda l: l.find('DEBUG') == -1, lis))
        self._contents += lis

    def _concat_files(self, flie_list):
        for f in flie_list:
            with open(f) as fr:
                self._read_file(fr)

    def _rewrite_src(self, src, dst):
        file_list = ListUpFiles(src).listup()
        # print(file_list)
        self._concat_files(file_list)
        if self._is_defined_cpp17:
            self._contents = [ self._define_cpp17 + '\n' ] + self._contents
        with open(dst, mode='w') as fw:
            fw.writelines(self._contents)

    def print_submit_file(self, src):
        src = PathFixer(os.getcwd()).move(src).get()
        ext = '.cpp'
        remove_ext = src[:-len(ext)]
        dst = remove_ext + '_submit' + ext
        self._rewrite_src(src, dst)


def main(src):
    printer = Printer()
    printer.print_submit_file(src)

if __name__ == '__main__':
    main(sys.argv[1])
