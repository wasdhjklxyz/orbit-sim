emcmake cmake -B build -S .     # configure
emmake make -Cbuild -j$(nproc)  # build
emrun build/orbit-sim.html      # run

Archived to focus on things I think are more interesting. Graphics/physics is
not my forte.
