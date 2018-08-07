#pragma once

#include <vector>

namespace creativecoin{ namespace plugins { namespace p2p {

#ifdef IS_TEST_NET
const std::vector< std::string > default_seeds;
#else
const std::vector< std::string > default_seeds = {
   "seed-east.creativecoin.com:2001",          // creativecoin
   "seed-central.creativecoin.com:2001",       // creativecoin
   "seed-west.creativecoin.com:2001",          // creativecoin
   "creativecoin-seed1.abit-more.com:2001",      // abit
   "52.74.152.79:2001",                   // smooth.witness
   "seed.creativecoind.com:34191",               // roadscape
   "anyx.co:2001",                        // anyx
   "seed.xeldal.com:12150",               // xeldal
   "seed.creativecoinnodes.com:2001",            // wackou
   "seed.liondani.com:2016",              // liondani
   "gtg.creativecoin.house:2001",                // gtg
   "seed.jesta.us:2001",                  // jesta
   "creativecoind.pharesim.me:2001",             // pharesim
   "5.9.18.213:2001",                     // pfunk
   "lafonacreativecoin.com:2001",                // lafona
   "seed.rossco99.com:2001",              // rossco99
   "creativecoin-seed.altcap.io:40696",          // ihashfury
   "seed.roelandp.nl:2001",               // roelandp
   "creativecoin.global:2001",                   // klye
   "seed.ecreativecoin.ws:2001",                 // good-karma
   "94.23.33.61:2001",                    // timcliff
   "104.199.118.92:2001",                 // clayop
   "192.99.4.226:2001",                   // dele-puppy
   "seed.bhuz.info:2001",                 // bhuz
   "seed.creativecoinviz.com:2001",              // ausbitbank
   "creativecoin-seed.lukestokes.info:2001",     // lukestokes
   "seed.blackrift.net:2001",             // drakos
   "seed.followbtcnews.com:2001",         // followbtcnews
   "node.mahdiyari.info:2001",            // mahdi.yari
   "seed.jerrybanfield.com:2001",         // jerrybanfield
   "seed.windforce.farm:2001",            // windforce
   "seed.curiecreativecoin.com:2001",            // curie
   "seed.rivercreativecoin.com:2001",            // riverhead
   "creativecoin-seed.furion.me:2001",           // furion
   "148.251.237.104:2001",                // creativecoin-bounty
   "seed1.blockbrothers.io:2001"          // blockbrothers
};
#endif

} } } // creativecoin::plugins::p2p
