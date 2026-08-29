emcmake cmake -B build -S .     # configure
emmake make -Cbuild -j$(nproc)  # build
emrun build/vis-viva.html       # run
