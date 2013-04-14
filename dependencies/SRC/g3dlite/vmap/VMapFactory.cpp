/***
 * Demonstrike Core
 */

#include "../G3DAll.h"

namespace VMAP
{
    VMapManager2 *gVMapManager = NULL;

    //===============================================
    // just return the instance
    VMapManager2* VMapFactory::createOrGetVMapManager()
    {
        if(gVMapManager == 0)
            gVMapManager = new VMapManager2();                // should be taken from config ... Please change if you like :-)
        return gVMapManager;
    }

    //===============================================
    // delete all internal data structures
    void VMapFactory::clear()
    {
        if(gVMapManager)
        {
            delete gVMapManager;
            gVMapManager = NULL;
        }
    }
}
