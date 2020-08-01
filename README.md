# games

## Snake
Get source    
    
    git clone https://github.com/tarun27sh/games.git .

Build

    cd games/
    mkdir build
    cd build
    
    # generate Makefile
    cmake ../ --debug-output
    
    # generate executable
    make
    OR
    make CC=clang++-9

To run:

    ./snake 2>/dev/null
