emcmake cmake -B build -S .     # configure
emmake make -Cbuild -j$(nproc)  # build
emrun build/orbit-sim.html      # run
