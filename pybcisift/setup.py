from distutils.core import setup, Extension

module1 = Extension('pybcisift',
                    include_dirs = [
                    '/usr/local/include',
                    #'/Users/rramele/work/opencv/include/',
                    #'/Users/rramele/work/kfr/include',
                    #'/Users/rramele/work/labstreaminglayer/build/install/LSL/include/',
                    #'/Users/rramele/work/vlfeat/',
                    '/usr/local/include/opencv4/',
                    '/home/shipu/facu/pf/libs/vlfeat',
                    '../src/include/',
                    '/home/shipu/facu/pf/libs/liblsl-1.14.0/include'
                    ],
                    libraries = [
                    'opencv_core',
                    'opencv_highgui',
                    'opencv_imgproc',
                    'opencv_features2d',
                    'opencv_xfeatures2d',
                    'opencv_calib3d',
                    'opencv_imgcodecs',
                    'opencv_ml',
                    'opencv_flann',
                    'lsl',
                    'vl'],
                    library_dirs = ['/usr/local/lib',
                    #'/Users/rramele/work/labstreaminglayer/build/install/LSL/lib/',
                    #'/Users/rramele/work/vlfeat/bin/maci64/',
                    '/home/shipu/facu/pf/libs/vlfeat/bin/glnxa64',
                    '/home/shipu/facu/pf/libs/liblsl-1.14.0'
                    ],
                    sources = ['pybcisift.cpp',
                    '../src/eegimage.cpp',
                    '../src/scalespace.cpp',
                    '../src/plotprocessing.cpp',
                    '../src/dsp.cpp',
                    '../src/lsl.cpp',
                    '../src/lsltransmitter.cpp',
                    '../src/decoder.cpp',
                    '../src/serializer.cpp',
                    '../src/unp.cpp'])

setup (name = 'pybcisift',
       version = '1.0',
       description = 'BciSift Routine for python',
       author = 'Rodrigo Ramele',
       author_email = 'rramele@gmail.com',
       url = 'https://docs.python.org/extending/building',
       long_description = '''
Get a SIFT (HIST) descriptor from a signal
''',
       ext_modules = [module1])
