

# Python instructions after compiling:

- Primer error: 
    ```
    ImportError: libvl.so: cannot open shared object file: No such file or directory
    ```

    Lo solucione  importando las librerias de vlfeat:
    ```sh
    export LD_LIBRARY_PATH=/home/shipu/facu/pf/libs/vlfeat/bin/glnxa64:$LD_LIBRARY_PATH
    export LD_LIBRARY_PATH=/home/shipu/facu/pf/libs/vlfeat/bin/glnxa64/objs:$LD_LIBRARY_PATH
    ```
- Segundo error:

    export LD_LIBRARY_PATH=/home/shipu/facu/pf/libs/opencv2_contrib/build/lib:$LD_LIBRARY_PATH
    export LD_LIBRARY_PATH=/home/shipu/facu/pf/libs/opencv2_contrib/build:$LD_LIBRARY_PATH
    export LD_LIBRARY_PATH=/home/shipu/facu/pf/libs/kfr/build:$LD_LIBRARY_PATH
    export LD_LIBRARY_PATH=/usr/local/lib:$LD_LIBRARY_PATH
    