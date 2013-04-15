/***
 * Demonstrike Core
 */

#pragma once

/**
This is the access point to the VMapManager.
*/

namespace VMAP
{
    //===========================================================

    class VMapFactory
    {
        public:
            static VMapManager2* createOrGetVMapManager(std::string vmapDir);
            static void clear();
    };

}
