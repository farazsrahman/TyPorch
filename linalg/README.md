NOTE: building this project became a little tricky when using OpenMP
homebrew installed OpenMP on my system as "keg-only" meaning it was
an optional package that the compiler had access too

As a result I had to use these following env variables to allow for
OpenMP to be accessible by my CLang compiler.

export LDFLAGS="-L/usr/local/opt/libomp/lib"
export CPPFLAGS="-I/usr/local/opt/libomp/include"
