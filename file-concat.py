import os
import sys
import re
from itertools import chain


class ListUpFiles:
    std_pattern = re.compile(r'#include\s?<(.*)>.*$', flags=re.DOTALL)
    lib_pattern = re.compile(r'#include\s?"(.*)".*$', flags=re.DOTALL)

    def __init__(self, src):
        self._src = src
        self._files = []
        self._appended = set()
        self._std = []
        self._lib = []

    def _should_append(self, path):
        if path in self._appended:
            return False
        if debug_file in path:
            return False
        if '<' in path:
            return False
        return True

    def _fix_abs_path(self, dirname, path):
        for d in [ '/home/taroy/kyopuro/lib/', dirname ]:
            p = os.path.join(d, path)
            if os.path.exists(p):
                return p
        return None

    def _get_included(self, path):
        lis1 = []
        lis2 = []
        dirname = os.path.dirname(path)
        
        def extract(s, lis, pattern, fix=False):
            m = pattern.match(s)
            if m is None:
                return

            rs = m.group(1)
            if s != rs:
                if not fix:
                    lis.append(rs)
                    return
                fixed = self._fix_abs_path(dirname, rs)
                if fixed is None:
                    exit(1)
                lis.append(fixed)

        with open(path) as fp:
            for line in fp.readlines():
                extract(line, lis1, ListUpFiles.std_pattern)
                extract(line, lis2, ListUpFiles.lib_pattern, fix=True)

        return lis1, lis2

    def _listup_rec(self, cur):
        cur = os.path.normpath(cur)
        lis1, lis2 = self._get_included(cur)

        for f in lis1:
            if not (f in self._appended):
                self._appended.add(f)
                self._std.append(f)

        for f in lis2:
            if 'debug' in f:
                continue
            if not (f in self._appended):
                self._appended.add(f)
                self._listup_rec(f)
                self._lib.append(f)

    def listup(self):
        self._listup_rec(self._src)
        self._lib.append(self._src)
        return self._std, self._lib


class ContentsFixer:
    def __init__(self, lib):
        self._lib = lib

    def _open(self):
        def readlines(f):
            with open(f) as fp:
                return fp.readlines()

        return chain(*[ readlines(f) for f in self._lib ])

    def _remove_include(self, gen):
        return filter(lambda line: not '#include' in line, gen)

    def _remove_debug(self, gen):
        return filter(lambda line: not 'DEBUG' in line, gen)

    def _remove_pragma_once(self, gen):
        return filter(lambda line: not '#pragma once' in line, gen)

    def fix(self):
        gen = self._open()
        gen = self._remove_include(gen)
        gen = self._remove_debug(gen)
        gen = self._remove_pragma_once(gen)
        return gen


class Printer:
    def __init__(self, src):
        self._src = os.path.abspath(src)

    def _get_dst(self):
        dirname, filename = os.path.split(self._src)
        problem, ext = filename.split('.')
        submit_filename = problem + '_submit.' + ext
        return os.path.join(dirname, submit_filename)

    def print(self):
        std, lib = ListUpFiles(self._src).listup()
        std = map(lambda s: '#include <{0}>\n'.format(s), std)
        lib = ContentsFixer(lib).fix()
        dst = self._get_dst()
        with open(dst, 'w+') as fp:
            fp.writelines(chain(std, lib))


if __name__ == '__main__':
    Printer(sys.argv[1]).print()
