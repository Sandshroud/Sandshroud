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
            static VMapManager2* createOrGetVMapManager();
            static void clear();
    };

}
