# Project -> Configuration Properties
###### C/C++ -> General -> Additional Include Directories
$(ProjectDir)\External\SDL\include;$(ProjectDir)\External\pugixml\src;
###### Linker -> General -> Additional Library Directories
$(ProjectDir)\External\SDL\lib\x86;
###### Linker -> Input -> Additional Dependencies
SDL2.lib;SDL2main.lib;SDL2_image.lib;SDL2_mixer.lib;SDL2_ttf.lib;
###### Debugging -> Working Directory
$(SolutionDir)\Game\x86