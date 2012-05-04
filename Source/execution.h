
int insExecution(instruction ins, int *cpuRegister, int cycle, unsigned int *pc, FILE *errorDump)
{
	char *word;
	int temp, temp2, i;
	*pc+=4;
	switch(ins.type) {
        case 'R':
            switch(ins.func) {

                case 0x00:
                    //printf("%5s", "sll");
                    cpuRegister[ins.rd]=cpuRegister[ins.rt] << ins.c;
                    break;

                case 0x02:
                    //printf("%5s", "srl");
                    cpuRegister[ins.rd] = (unsigned int)cpuRegister[ins.rt] >> ins.c;
                    break;

                case 0x03:
                    //printf("%5s", "sra");
                    cpuRegister[ins.rd] = cpuRegister[ins.rt] >> ins.c;
                    break;

                case 0x08:
                    //printf("%5s", "jr");
                    *pc = cpuRegister[ins.rs];
                    break;

                case 0x20:
                    //printf("%5s", "add");
                    temp = cpuRegister[ins.rs];
                    temp2= cpuRegister[ins.rt];
                    //printf("%d %d %d\n", temp, temp2, cpuRegister[ins.rs] + cpuRegister[ins.rt]);
                    cpuRegister[ins.rd]=cpuRegister[ins.rs] + cpuRegister[ins.rt];
                    if((temp>0 && temp2>0 && cpuRegister[ins.rd]<=0)||(temp<0 && temp2<0 && cpuRegister[ins.rd]>=0)){
                    	//number overflow
						errorGen(cycle-1, 2, errorDump);
                    }else;
                    break;

                case 0x22:
                    //printf("%5s", "sub");
                    temp = cpuRegister[ins.rs];
                    temp2= cpuRegister[ins.rt];
                    cpuRegister[ins.rd]=cpuRegister[ins.rs] - cpuRegister[ins.rt];
                    if((temp>0 && temp2<0 && cpuRegister[ins.rd]<=0)||(temp<0 && temp2>0 && cpuRegister[ins.rd]>=0)){
						//number overflow
						errorGen(cycle-1, 2, errorDump);
                    }else;
                    break;

                case 0x24:
                    //printf("%5s", "and");
                    cpuRegister[ins.rd]=cpuRegister[ins.rs] & cpuRegister[ins.rt];
                    break;

                case 0x25:
                    //printf("%5s", "or");
                    cpuRegister[ins.rd]=cpuRegister[ins.rs] | cpuRegister[ins.rt];
                    break;

                case 0x26:
                    //printf("%5s", "xor");
                    cpuRegister[ins.rd]=cpuRegister[ins.rs] ^ cpuRegister[ins.rt];
                    break;

                case 0x27:
                    //printf("%5s", "nor");
                    cpuRegister[ins.rd] = ~(cpuRegister[ins.rs] | cpuRegister[ins.rt]);
                    break;

                case 0x28:
                    //printf("%5s", "nand");
                    cpuRegister[ins.rd] = ~(cpuRegister[ins.rs] & cpuRegister[ins.rt]);
                    break;

                case 0x2A:
                    //printf("%5s", "slt");
                    cpuRegister[ins.rd] = (cpuRegister[ins.rs] < cpuRegister[ins.rt])?1:0;
                    break;

                default:
                    //printf("%5s", "null");
                    break;
            }
            //printf(" 0x%2X", ins.func);
            if(ins.rd==0 && ins.func!=0x08){
            	cpuRegister[0]=0;
            	errorGen(cycle-1, 1, errorDump);
            }
            break;
        case 'I':
            switch(ins.op) {
                case 0x04:
                    //printf("%5s", "beq");
                    if(cpuRegister[ins.rs] == cpuRegister[ins.rt]){
                    	*pc += 4*(short int)ins.c;
                    	if((short int)ins.c > 0 && (int)*pc < 0){
                    		//number overflow
                    		errorGen(cycle-1, 2, errorDump);
                    	}else;
                    }else;
                    break;
                case 0x05:
                    //printf("%5s", "bne");
                    if(cpuRegister[ins.rs] != cpuRegister[ins.rt]){
                    	*pc += 4*(short int)ins.c;
                    	if((short int)ins.c > 0 && (int)*pc < 0){
                    		//number overflow
                    		errorGen(cycle-1, 2, errorDump);
                    	}else;
                    }else;
                    break;
                case 0x08:
                    //printf("%5s", "addi");
                    temp = cpuRegister[ins.rs];
                    cpuRegister[ins.rt] = cpuRegister[ins.rs] + (short int)ins.c;
                    if((temp>0 && (short int)ins.c>0 && cpuRegister[ins.rt]<=0)||(temp<0 && (short int)ins.c<0 && cpuRegister[ins.rt]>=0)){
                    	//number overflow
                    	errorGen(cycle-1, 2, errorDump);
                    }else;
                    break;
                case 0x0A:
                    //printf("%5s", "slti");
                    cpuRegister[ins.rt] = (cpuRegister[ins.rs] < (short int)ins.c) ? 1:0;
                    break;
                case 0x0C:
                    //printf("%5s", "andi");
                    cpuRegister[ins.rt] = cpuRegister[ins.rs] & ins.c;
                    break;
                case 0x0D:
                    //printf("%5s", "ori");
                    cpuRegister[ins.rt] = cpuRegister[ins.rs] | ins.c;
                    break;
                case 0x0E:
                    //printf("%5s", "nori");
                    cpuRegister[ins.rt] = ~(cpuRegister[ins.rs] | ins.c);
                    break;
                case 0x0F:
                    //printf("%5s", "lui");
                    temp = (short int) ins.c;
                    temp = temp << 16;
                    cpuRegister[ins.rt] = temp;
                    break;
                case 0x20:
                    //printf("%5s", "lb");
                    temp = cpuRegister[ins.rs];
                    word = loadDataMemory((unsigned int)(cpuRegister[ins.rs]+(short int)ins.c));
                    cpuRegister[ins.rt] = (int) word[3];
                    if((temp>0 && (short int)ins.c>0 && temp+(short int)ins.c<=0)||(temp<0 && (short int)ins.c<0 && temp+(short int)ins.c>=0)){
                    	//number overflow
                    	errorGen(cycle-1, 2, errorDump);
                    }
                    if((unsigned int)(temp+(short int)ins.c) > 1023 || (unsigned int)(temp+(short int)ins.c) < 0){
                    	//address overflow
                    	errorGen(cycle-1, 3, errorDump);
                    	return 1;
                    }else;

                    break;
                case 0x21:
                    //printf("%5s %d %d", "lh", ins.rs, cpuRegister[ins.rs]);
                    temp2 = cpuRegister[ins.rs];
                    word = loadDataMemory(cpuRegister[ins.rs]+(short int)ins.c);
                    temp = (short int)((((unsigned char) word[2]) << 8) | ((unsigned char) word[3]));
                    cpuRegister[ins.rt] = temp;
                    if((temp2>0 && (short int)ins.c>0 && temp2+(short int)ins.c<=0)||(temp2<0 && (short int)ins.c<0 && temp2+(short int)ins.c>=0)){
                    	//number overflow
                    	errorGen(cycle-1, 2, errorDump);
                    }
                    if((unsigned int)(temp2+(short int)ins.c) > 1022 || (unsigned int)(temp+(short int)ins.c) < 0){
                    	//address overflow
                    	errorGen(cycle-1, 3, errorDump);
                    	return 1;
                    }
                    if(((unsigned int)(temp2+(short int)ins.c))%2!=0){
                    	//misaligned
                    	errorGen(cycle-1, 4, errorDump);
                    	return 1;
                    }else;
                    
                    break;
                case 0x23:
                    //printf("%5s %d %d addr=%d", "lw", ins.rs, cpuRegister[ins.rs], (unsigned int)(cpuRegister[ins.rs]+(short int)ins.c)>1020);
                    
                    temp2 = cpuRegister[ins.rs];
                    word = loadDataMemory(cpuRegister[ins.rs]+(short int)ins.c);
                    temp = ((((unsigned char) word[0]) << 24) | (((unsigned char) word[1]) << 16) | (((unsigned char) word[2]) << 8) | ((unsigned char) word[3]));
                    cpuRegister[ins.rt] = temp;
                    if((temp2>0 && (short int)ins.c>0 && temp2+(short int)ins.c<=0)||(temp2<0 && (short int)ins.c<0 && temp2+(short int)ins.c>=0)){
                    	//number overflow
                    	errorGen(cycle-1, 2, errorDump);
                    }
                    if((unsigned int)(temp2+(short int)ins.c) > 1020 || (unsigned int)(temp+(short int)ins.c) < 0){
                    	errorGen(cycle-1, 3, errorDump);
                    	return 1;
                    }
                    if(((unsigned int)(temp2+(short int)ins.c))%4!=0){
                    	//misaligned
                    	errorGen(cycle-1, 4, errorDump);
                    	return 1;
                    }else;

                    break;
                case 0x24:
                    //printf("%5s", "lbu");
                    temp = cpuRegister[ins.rs];
                    word = loadDataMemory(cpuRegister[ins.rs]+(short int)ins.c);
                    cpuRegister[ins.rt] = (unsigned char) word[3];

                    if((temp>0 && (short int)ins.c>0 && temp+(short int)ins.c<=0)||(temp<0 && (short int)ins.c<0 && temp+(short int)ins.c>=0)){
                    	//number overflow
                    	errorGen(cycle-1, 2, errorDump);
                    }
                    if((unsigned int)(temp+(short int)ins.c) > 1023 || (unsigned int)(temp+(short int)ins.c) < 0){
                    	//address overflow
                    	errorGen(cycle-1, 3, errorDump);
                    	return 1;
                    }else;
                    break;
                case 0x25:
                    //printf("%5s", "lhu");
                    temp2= cpuRegister[ins.rs];
                    word = loadDataMemory(cpuRegister[ins.rs]+(short int)ins.c);
                    temp = ((((unsigned char) word[2]) << 8) | ((unsigned char) word[3]));
                    cpuRegister[ins.rt] = temp;
                    if((temp2>0 && (short int)ins.c>0 && temp2+(short int)ins.c<=0)||(temp2<0 && (short int)ins.c<0 && temp2+(short int)ins.c>=0)){
                    	//number overflow
                    	errorGen(cycle-1, 2, errorDump);
                    }
                    if((unsigned int)(temp2+(short int)ins.c) > 1022 || (unsigned int)(temp+(short int)ins.c) < 0){
                    	//address overflow
                    	errorGen(cycle-1, 3, errorDump);
                    	return 1;
                    }
                    if(((unsigned int)(temp2+(short int)ins.c))%2!=0){
                    	//misaligned
                    	errorGen(cycle-1, 4, errorDump);
                    	return 1;
                    }else;
                    break;
                case 0x28:
                    //printf("%5s", "sb");
                    temp2= cpuRegister[ins.rs];
                    word = (char *)malloc(4*sizeof(char));
                   	word[0] = 0;
                   	word[1] = 0;
                   	word[2] = 0;
                   	word[3] = cpuRegister[ins.rt];
                   	saveDataMemory(cpuRegister[ins.rs]+(short int)ins.c, word, 1);
                   	if((temp2>0 && (short int)ins.c>0 && temp2+(short int)ins.c<=0)||(temp2<0 && (short int)ins.c<0 && temp2+(short int)ins.c>=0)){
                    	//number overflow
                    	errorGen(cycle-1, 2, errorDump);
                    }
                    if((unsigned int)(temp2+(short int)ins.c) > 1023 || (unsigned int)(temp+(short int)ins.c) < 0){
                    	//address overflow
                    	errorGen(cycle-1, 3, errorDump);
                    	return 1;
                    }else;
                    break;
                case 0x29:
                    //printf("%5s", "sh");
                    temp2= cpuRegister[ins.rs];
                    word = (char *)malloc(4*sizeof(char));
                   	word[0] = 0;
                   	word[1] = 0;
                   	word[2] = cpuRegister[ins.rt] >> 8;
                   	word[3] = cpuRegister[ins.rt];
                   	saveDataMemory(cpuRegister[ins.rs]+(short int)ins.c, word, 2);
                   	if((temp2>0 && (short int)ins.c>0 && temp2+(short int)ins.c<=0)||(temp2<0 && (short int)ins.c<0 && temp2+(short int)ins.c>=0)){
                    	//number overflow
                    	return 2;
                    }else if((unsigned int)(temp2+(short int)ins.c) > 1022 || (unsigned int)(temp+(short int)ins.c) < 0){
                    	//address overflow
                    	errorGen(cycle-1, 3, errorDump);
                    	return 1;
                    }else if(((unsigned int)(temp2+(short int)ins.c))%2!=0){
                    	//misaligned
                    	errorGen(cycle-1, 4, errorDump);
                    	return 1;
                    }else;
                    break;
                case 0x2B:
                    //printf("%5s", "sw");
                    temp2= cpuRegister[ins.rs];
                    word = (char *)malloc(4*sizeof(char));
                   	word[0] = cpuRegister[ins.rt] >> 24;
                   	word[1] = cpuRegister[ins.rt] >> 16;
                   	word[2] = cpuRegister[ins.rt] >> 8;
                   	word[3] = cpuRegister[ins.rt];
                   	saveDataMemory(cpuRegister[ins.rs]+(short int)ins.c, word, 4);
                   	if((temp2>0 && (short int)ins.c>0 && temp2+(short int)ins.c<=0)||(temp2<0 && (short int)ins.c<0 && temp2+(short int)ins.c>=0)){
                    	//number overflow
                    	errorGen(cycle-1, 2, errorDump);
                    }
                    if((unsigned int)(temp2+(short int)ins.c) > 1020 || (unsigned int)(temp+(short int)ins.c) < 0){
                    	//address overflow
                    	errorGen(cycle-1, 3, errorDump);
                    	return 1;
                    }
                    if(((unsigned int)(temp2+(short int)ins.c))%4!=0){
                    	//misaligned
                    	errorGen(cycle-1, 4, errorDump);
                    	return 1;
                    }else;
                    break;
            }
            if(ins.rt==0 && (ins.op!=0x04 && ins.op!=0x05 && ins.op!=0x28 && ins.op!=0x29 && ins.op!=0x2B)){
            	cpuRegister[0]=0;
            	errorGen(cycle-1, 1, errorDump);
            }
            break;
        case 'J':
            switch(ins.op) {
                case 0x02:
                    //printf("%5s", "j");
                    *pc = ((*pc)&(0xF0000000))|(ins.c << 2);
                    break;
                case 0x03:
                    //printf("%5s", "jal");
                    cpuRegister[31] = *pc;
                    *pc = ((*pc+4)&(0xF0000000))|(ins.c << 2);
                    break;
            }
            break;
        case 'S':
            //printf("%5s", "halt");
            return 1;
            break;
        default:
            break;
	}
	return 0;
}
