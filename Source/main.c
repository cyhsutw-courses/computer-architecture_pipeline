#include <stdio.h>
#include <stdlib.h>

#include "util.h"
#include "memoryAccess.c"
#include "execution.h"


int main() {
    FILE *snapshot, *errorDump;
     
    
    int cpuRegister[32] = {0}, cycle=1, programStat = 0;
    unsigned int pc;
    instruction ins;
    
    snapshot = fopen("./snapshot.rpt", "w");
    errorDump = fopen("./Error_dump.rpt", "w");
    
    // load memory and init pc
    pc = initInstMemory();
    //Init $sp
    cpuRegister[29] = initDataMemory();
    
    snapshotGen(0, cpuRegister, pc, snapshot);
    
    while(1) {
        
        if(pc>1020){
        	errorGen(cycle-1, 3, errorDump);
        	break;
        }else;
        
        // instruction fetch
        ins = instructionFetch(pc);
        
        // instruction execution
        programStat = insExecution(ins, cpuRegister, cycle, &pc, errorDump);
        if(programStat!=0){
        	break;
        }else;
        snapshotGen(cycle, cpuRegister, pc, snapshot);
        cycle++;
    }
    //showMemory();
    fclose(snapshot);
    fclose(errorDump);
    return 0;
}
