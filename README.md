change from https://github.com/lmartinking/qgz
use for byte vector and return k string.
special for okex websocket decompress.

Build

$ make {m32,m64,l32,l64}
$ cp qzlib_{m32,m64,l32,l64}.so /path/to/q/bin
Use

q) gunzip: (`$"qzlib_m64") 2:(`gunzip;1); / change m64 to appropriate platform
q) gunzip  / takes a byte vector of gzip data to decompress
NOTE: You might need to set DYLD_LIBRARY_PATH or LD_LIBRARY_PATH environment variables (Mac and Linux respectively) to the directory where the .so lives before running q.

Licence

LGPLv3. See LICENSE and COPYING.LESSER.

Copyright (c) 2019 secwang 
Copyright (c) 2016 Lucas Martin-King.
