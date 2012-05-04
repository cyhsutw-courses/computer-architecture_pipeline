void snapshotGen(int cycle, int *regArr, int pc, FILE *snapshot)
{
	int i;
	printf("cycle %d\n", cycle);
	fprintf(snapshot, "cycle %d\n", cycle);
	for(i=0; i<32; i++){
		printf("$%02d: 0x%08X\n", i, regArr[i]);
		fprintf(snapshot, "$%02d: 0x%08X\n", i, regArr[i]);
	}
	printf("PC: 0x%08X\n\n", pc);
	fprintf(snapshot, "PC: 0x%08X\n\n\n", pc);
}

void errorGen(int pc, int stat, FILE *errorDump)
{
	if(stat == 1){ //write to $0 err
     	
     	fprintf(errorDump, "PC: 0x%08X Write $0 error\n", pc);  
     	 		
    }else if(stat == 2){ //number overflow
       
        fprintf(errorDump, "PC: 0x%08X Number overflow\n", pc);  	
        	
    }else if(stat == 3){ //address overflow
        
        fprintf(errorDump, "PC: 0x%08X Address overflow\n", pc);  
        	
    }else if(stat == 4){ //data misaligned
       
        fprintf(errorDump, "PC: 0x%08X Miss align error\n", pc);  
        
    }else; //stat = 5 means halt 
}
