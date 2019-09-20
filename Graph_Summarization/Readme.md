
XCode Configuration:
# Add Boost:
    -With the help of brew install boost
        
        
    a.  HEADER_SEARCH_PATHS = /usr/local/include/  /usr/local/Cellar/boost/1.70.0/include
    b.  LIBRARY_SEARCH_PATHS = /usr/local/lib/  /usr/local/Cellar/boost/1.70.0/lib/
    c.  OTHER_LDFLAGS = -lboost_system -lboost_filesystem
    d.  Drag required(filesystem, system, serialization etc) to the project, that will automatically link the lib to build phase
    
# Add SNAP Lib(optional):
    a. Place this whole project inside SNAP directory
    b. Add snap-env, snap-core, glib-core reference to xcode
    c. build phase-> compile source -> add Snap.cpp


//----------------------//

## Tested Environment
- C++ 11
- GCC 4.8
- Boost
- cmake


## Compile:
- create build directory.
- Use below commands from inside of the build directory:
```sh
$ cmake ../
$ make
```

## Parameters

- Action:(For future extension) Currently use "summ" only
    - summ:  build exact supernodes S, superEdges P and Corrections C
    - summA: Droppoing some edges while maintaining the error bound
 
 - Options:
    - --prefix <prefix>
    - --epsilon <epsilon> 
    - --dataset <dataset>
    - --result_dir <directory to place results>
    - --iteration < Number of Iteration Count T>
    

    

# Sample Argument:

    ./sweg summ --prefix <data-folder> --dataset <graph-name> --result_dir <output-folder> --epsilon <relative error>

Examples:
    
    ./sweg summ   --prefix ./data/ --dataset test --result_dir Result --epsilon 0.8 
    
    ./sweg summ  --dataset test3 --result_dir Result --epsilon 0.8 --iteration 5
