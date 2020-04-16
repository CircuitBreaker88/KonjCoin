// Copyright (c) 2009-2012 The Bitcoin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <boost/assign/list_of.hpp> // for 'map_list_of()'
#include <boost/foreach.hpp>

#include "checkpoints.h"

#include "txdb.h"
#include "main.h"
#include "uint256.h"


static const int nCheckpointSpan = 5000;

namespace Checkpoints
{
    typedef std::map<int, uint256> MapCheckpoints;

    //
    // What makes a good checkpoint block?
    // + Is surrounded by blocks with reasonable timestamps
    //   (no blocks before with a timestamp after, none after with
    //    timestamp before)
    // + Contains no strange transactions
    //
    static MapCheckpoints mapCheckpoints = boost::assign::map_list_of
            (0, uint256("0x00002b696e1b86375c688cbcc6c7fd8cf62c6e47c0b4a9a107b3174c1879674d"))
            (35000, uint256("0xb085e70152e4e095631c6a772322d35e0f792f5d263c29c3d69447292eed2a6d"))
            (50000, uint256("0xd3e3e4362b607caa055ae1d6dd98588e586f7c01b137ee7a18271fa5833c2aff"))
            (75000, uint256("0xb09c831df5fdf89f6723da6eef5bcd89c2849d77f715336b779554b8b3446bac"))
            (150000, uint256("0x3815238984b1e23173e2ab6d8469821675dd058b3e455de9ca50f1c573731fa3"))
            (175000, uint256("0xe8d65188a138906ea276c994bab790d69c649f01e6d79b291659091607f365e7"))
            (200000, uint256("0xd2adf0eaea56d0ec9befecec83d1d0ca2249643a8eb1faf9e2add842484d1f90"))
            (225000, uint256("0x7a1a6fb06c0c5958e21b8ee8ce3b9c73615c249ed04892d3b893162ff11eaf8d"))
            (250000, uint256("0xdd55f4b2a66448c71757635e6ea19ae93d166a89383b3aa7739b29242286a63a"))
            (275000, uint256("0x48d1aa29d1f614bdeb069eb53f720b89825b521ece7786a175ac16bf6a909f22"))
            (300000, uint256("0xd67f56dd5737d37421f4583f18bd8d0d5da6056c8b26625e53b42a3bebe42547"))
    ;

    // TestNet has no checkpoints
    static MapCheckpoints mapCheckpointsTestnet;

    bool CheckHardened(int nHeight, const uint256& hash)
    {
        MapCheckpoints& checkpoints = (TestNet() ? mapCheckpointsTestnet : mapCheckpoints);

        MapCheckpoints::const_iterator i = checkpoints.find(nHeight);
        if (i == checkpoints.end()) return true;
        return hash == i->second;
    }

    int GetTotalBlocksEstimate()
    {
        MapCheckpoints& checkpoints = (TestNet() ? mapCheckpointsTestnet : mapCheckpoints);

        if (checkpoints.empty())
            return 0;
        return checkpoints.rbegin()->first;
    }

    CBlockIndex* GetLastCheckpoint(const std::map<uint256, CBlockIndex*>& mapBlockIndex)
    {
        MapCheckpoints& checkpoints = (TestNet() ? mapCheckpointsTestnet : mapCheckpoints);

        BOOST_REVERSE_FOREACH(const MapCheckpoints::value_type& i, checkpoints)
        {
            const uint256& hash = i.second;
            std::map<uint256, CBlockIndex*>::const_iterator t = mapBlockIndex.find(hash);
            if (t != mapBlockIndex.end())
                return t->second;
        }
        return NULL;
    }

    // Automatically select a suitable sync-checkpoint
    const CBlockIndex* AutoSelectSyncCheckpoint()
    {
        const CBlockIndex *pindex = pindexBest;
        // Search backward for a block within max span and maturity window
        while (pindex->pprev && pindex->nHeight + nCheckpointSpan > pindexBest->nHeight)
            pindex = pindex->pprev;
        return pindex;
    }

    // Check against synchronized checkpoint
    bool CheckSync(int nHeight)
    {
        const CBlockIndex* pindexSync = AutoSelectSyncCheckpoint();
        if (nHeight <= pindexSync->nHeight){
            return false;
        }
        return true;
    }
}
