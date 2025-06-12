// paciFIST Studios, 20250611, MIT License.

#include "SDL3/SDL.h"
#include <iostream>

#include "core/common.h"


int main(){
    SDL_Log("%s", "Hey girl!");
    //ASSERT(1==0);
    ASSERT_MESSAGE(1==0, "One equals zero");
    print_debug("Heyyyy!!\n");
    return 0;
}


