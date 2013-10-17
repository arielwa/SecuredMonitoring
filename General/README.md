General Scripts & Stuff
=======================

This directory contains general scripts that were not specifically made for any
specific part of the project (along with any other files deemed general enough
to be placed here).

Present Scripts
---------------

 *  **printCompilerDefines**:
   
    Prints a list of all the external #define's given to the compiler in a set of
    given compiler commands.
    
    *Example usage*:
     *  Create a text file with all the build commands of the goldfish kernel (for example):
            
            make goldfish_armv7_bgu_defconfig -n > buildOutput.txt
            make -j32 -n >> buildOutput.txt
            
     *  Show the #define's declared using -D flags to the compiler:
            
            ~/Git/SecuredMonitoring/printCompilerDefines buildOutput.txt
            
        or:
            
            cat buildOutput.txt | ~/Git/SecuredMonitoring/printCompilerDefines
            
    
 *  **generateDefinesFromConfig**:
    
    Generates an .h file with #define's from a given configuration file such as
    the "goldfish/arch/arm/configs/goldfish_armv7_defconfig" file.
    
    *Example usage*:
            
        ~/Git/SecuredMonitoring/generateDefinesFromConfig arch/arm/configs/goldfish_armv7_defconfig > ConfigDefines.h
            
    or:
            
        cat arch/arm/configs/goldfish_armv7_defconfig | ~/Git/SecuredMonitoring/generateDefinesFromConfig > ConfigDefines.h
            
