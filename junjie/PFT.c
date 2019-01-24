//system apis
void * calloc(int count,int length);
void * malloc(int size);
int arrayLen(void *a);
int memcpy(void * dest,void * src,int length);
int memset(void * dest,char c,int length);

//utility apis
int strcmp(char *a,char *b);
char * strconcat(char *a,char *b);
int Atoi(char * s);
long long Atoi64(char *s);
char * Itoa(int a);
char * I64toa(long long amount,int radix);
char * SHA1(char *s);
char * SHA256(char *s);

//parameter apis
int ZPT_ReadInt32Param(char *args);
long long ZPT_ReadInt64Param(char * args);
char * ZPT_ReadStringParam(char * args);
void ZPT_JsonUnmashalInput(void * addr,int size,char * arg);
char * ZPT_JsonMashalResult(void * val,char * types,int succeed);
char * ZPT_JsonMashalParams(void * s);
char * ZPT_RawMashalParams(void *s);
char * ZPT_GetCallerAddress();
char * ZPT_GetSelfAddress();
char * ZPT_CallContract(char * address,char * contractCode,char * method,char * args);
char * ZPT_MarshalNativeParams(void * s);
char * ZPT_MarshalNeoParams(void * s);

//Runtime apis
int ZPT_Runtime_CheckWitness(char * address);
void ZPT_Runtime_Notify(char * address);
int ZPT_Runtime_CheckSig(char * pubkey,char * data,char * sig);
int ZPT_Runtime_GetTime();
void ZPT_Runtime_Log(char * message);

//Attribute apis
int ZPT_Attribute_GetUsage(char * data);
char * ZPT_Attribute_GetData(char * data);

//Block apis
char * ZPT_Block_GetCurrentHeaderHash();
int ZPT_Block_GetCurrentHeaderHeight();
char * ZPT_Block_GetCurrentBlockHash();
int ZPT_Block_GetCurrentBlockHeight();
char * ZPT_Block_GetTransactionByHash(char * hash);
int * ZPT_Block_GetTransactionCountByBlkHash(char * hash);
int * ZPT_Block_GetTransactionCountByBlkHeight(int height);
char ** ZPT_Block_GetTransactionsByBlkHash(char * hash);
char ** ZPT_Block_GetTransactionsByBlkHeight(int height);


//Blockchain apis
int ZPT_BlockChain_GetHeight();
char * ZPT_BlockChain_GetHeaderByHeight(int height);
char * ZPT_BlockChain_GetHeaderByHash(char * hash);
char * ZPT_BlockChain_GetBlockByHeight(int height);
char * ZPT_BlockChain_GetBlockByHash(char * hash);
char * ZPT_BlockChain_GetContract(char * address);

//header apis
char * ZPT_Header_GetHash(char * data);
int ZPT_Header_GetVersion(char * data);
char * ZPT_Header_GetPrevHash(char * data);
char * ZPT_Header_GetMerkleRoot(char  * data);
int ZPT_Header_GetIndex(char * data);
int ZPT_Header_GetTimestamp(char * data);
long long ZPT_Header_GetConsensusData(char * data);
char * ZPT_Header_GetNextConsensus(char * data);

//storage apis
void ZPT_Storage_Put(char * key,char * value);
char * ZPT_Storage_Get(char * key);
void ZPT_Storage_Delete(char * key);

//transaction apis
char * ZPT_Transaction_GetHash(char * data);
int ZPT_Transaction_GetType(char * data);
char * ZPT_Transaction_GetAttributes(char * data);


char * mark = "\"";
char * comma = "\,";
char * firstbracket = "\[";
char * lastbracket = "\]";

char * actionMarshal(char * action, char * str_1, char * str_2, char * str_3){
    char * str1 = firstbracket;
    char * str2;
    
    str2 = strconcat(str1,mark);
    str1 = strconcat(str2,action);
    str2 = strconcat(str1,mark);
    str1 = strconcat(str2,comma);
    
    str2 = strconcat(str1,mark);
    str1 = strconcat(str2,str_1);
    str2 = strconcat(str1,mark);
    str1 = strconcat(str2,comma);
    
    str2 = strconcat(str1,mark);
    str1 = strconcat(str2,str_2);
    str2 = strconcat(str1,mark);
    str1 = strconcat(str2,comma);
    
    str2 = strconcat(str1,mark);
    str1 = strconcat(str2,str_3);
    str2 = strconcat(str1,mark);
    
    str1 = strconcat(str2,lastbracket);
    
    return str1;
}

char * symbol = "PFT";

char * recharge(char * addr, char * amountChar) {
    long long amount = Atoi64(amountChar);
    if (amount <= 0)
        return "30001";
    long long balance = Atoi64(ZPT_Storage_Get(addr)) + amount;
    ZPT_Storage_Put(addr,I64toa(balance,10));
    return actionMarshal("recharge","-",addr,amountChar);
}

char * deduct(char * addr, char * amountChar) {
    long long amount = Atoi64(amountChar);
    if (amount <= 0)
        return "30002";
    long long balance = Atoi64(ZPT_Storage_Get(addr));
    if (balance < amount)
        return "30003";
    balance -= amount;
    ZPT_Storage_Put(addr,I64toa(balance,10));
    return actionMarshal("deduct",addr,"-",amountChar);
}

char * balanceOf(char * address){
    char * balance = ZPT_Storage_Get(address);
    if (arrayLen(balance) == 0)
        return "0";
    return balance;
}

char * startGame(char *userAddr, char *potAddr){
    long long userBalance = Atoi64(ZPT_Storage_Get(userAddr));
    if (userBalance < 1000000)
        return "30003";
    userBalance -= 1000000;
    ZPT_Storage_Put(userAddr,I64toa(userBalance,10));
    long long potBalance = Atoi64(ZPT_Storage_Get(potAddr)) + 500000;
    ZPT_Storage_Put(potAddr,I64toa(potBalance,10));
    return actionMarshal("half to pot",userAddr,potAddr,1000000);
}

char * credits(char *userAddr, char *rank, char *potAddr){
    int rankInt = Atoi(rank);
    long long reward = 0;
    long long potBalance = Atoi64(ZPT_Storage_Get(potAddr));
    switch(rankInt){
        case 1:
            reward = potBalance*30/100;
            break;
        case 2:
            reward = potBalance*20/100;
            break;
        case 3:
            reward = potBalance*15/100;
            break;
        default:
            reward = potBalance*5/100;
            break;
    }
    long long userBalance = Atoi64(ZPT_Storage_Get(userAddr)) + reward;
    ZPT_Storage_Put(userAddr,I64toa(userBalance,10));
    return actionMarshal("reward",potAddr,userAddr,I64toa(reward,10));
}

char * reset(char * addr){
    long long balance = Atoi64(ZPT_Storage_Get(addr));
    ZPT_Storage_Put(addr,"0");
    return actionMarshal("reset",addr,"-",I64toa(balance,10));
}


char * invoke(char * method,char * args){
    
    char * result;
    
    if (strcmp(method, "getSymbol")==0){
        ZPT_Runtime_Notify(symbol);
        return symbol;
    }
    
    if (strcmp(method, "recharge")==0){
        struct Params{
            char * address;
            char * amount;
        };
        struct Params *p = (struct Params *)malloc(sizeof(struct Params));
        ZPT_JsonUnmashalInput(p,sizeof(struct Params),args);
        result = recharge(p->address, p->amount);
        ZPT_Runtime_Notify(result);
        return result;
    }
    
    if (strcmp(method, "deduct")==0){
        struct Params{
            char * address;
            char * amount;
        };
        struct Params *p = (struct Params *)malloc(sizeof(struct Params));
        ZPT_JsonUnmashalInput(p,sizeof(struct Params),args);
        result = deduct(p->address, p->amount);
        ZPT_Runtime_Notify(result);
        return result;
    }
    
    if (strcmp(method, "balanceOf")==0){
        struct Params{
            char * address;
        };
        struct Params *p = (struct Params *)malloc(sizeof(struct Params));
        ZPT_JsonUnmashalInput(p,sizeof(struct Params),args);
        result = balanceOf(p->address);
        ZPT_Runtime_Notify(result);
        return result;
    }
    
    if (strcmp(method, "startGame")==0){
        struct Params{
            char * userAddr;
            char * potAddr;
        };
        struct Params *p = (struct Params *)malloc(sizeof(struct Params));
        ZPT_JsonUnmashalInput(p,sizeof(struct Params),args);
        result = startGame(p->userAddr, p->potAddr);
        ZPT_Runtime_Notify(result);
        return result;
    }
    
    if (strcmp(method, "credits")==0){
        struct Params{
            char * userAddr;
            char * rank;
            char * potAddr;
        };
        struct Params *p = (struct Params *)malloc(sizeof(struct Params));
        ZPT_JsonUnmashalInput(p,sizeof(struct Params),args);
        result = credits(p->userAddr, p->rank, p->potAddr);
        ZPT_Runtime_Notify(result);
        return result;
    }
    
    if (strcmp(method, "reset")==0){
        struct Params{
            char * addr;
        };
        struct Params *p = (struct Params *)malloc(sizeof(struct Params));
        ZPT_JsonUnmashalInput(p,sizeof(struct Params),args);
        result = reset(p->addr);
        ZPT_Runtime_Notify(result);
        return result;
    }
    
}
