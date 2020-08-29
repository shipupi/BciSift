from distutils.core import setup, Extension

module1 = Extension('pybcisift',
                    sources = ['pybcisift.cpp'])

setup (name = 'PackageName',
       version = '1.0',
       description = 'BciSift Routine for python',
       ext_modules = [module1])
