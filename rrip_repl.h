

#ifndef RRIP_REPL_H_
#define RRIP_REPL_H_

#include "repl_policies.h"

// Static RRIP
class SRRIPReplPolicy : public ReplPolicy {
    protected:
        //uint64_t timestamp; // incremented on each access
        uint32_t rpvMax;
        uint64_t* RRPVarray;
        uint32_t numLines;
        // add class member variables here

    public:
        SRRIPReplPolicy(uint32_t _numLines,uint32_t rpvMax) : rpvMax(rpvMax), numLines(_numLines) {
            RRPVarray = gm_calloc<uint64_t>(numLines);  // holds the RPVV 

            // Initializing elements to RPVmax
            if (RRPVarray != NULL) {
             for (uint32_t i = 0; i < numLines; i++) 
             {
             RRPVarray[i] = rpvMax;
            }
            }
        
        }
        ~SRRIPReplPolicy() {
            gm_free(RRPVarray);
            
        }

        // set RRPV to 0
        void update(uint32_t id, const MemReq* req)
        {
            //RPVV[id]=0;
            RRPVarray[id]=0;
        }
        void replaced(uint32_t id)
        {
             RRPVarray[id] = rpvMax-1;
        }

        template <typename C> inline uint32_t rank(const MemReq* req, C cands) {
          
            while(true)
            {
            for (auto ci = cands.begin(); ci != cands.end(); ci.inc())  
            {
                uint32_t id = *ci;
                if(RRPVarray[id]==rpvMax)
             {
                return id;
             } 
            }
            for (auto ci = cands.begin(); ci != cands.end(); ci.inc())
            {
                uint32_t id = *ci;
               if (RRPVarray[id] < rpvMax) {
                    RRPVarray[id]+=1;
                }
            }
            }
        }
         DECL_RANK_BINDINGS;
};
#endif // RRIP_REPL_H_





