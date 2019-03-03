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

//Joint two arrays
char * concat(char * a, char * b){
    int lena = arrayLen(a);
    int lenb = arrayLen(b);
    char * res = (char *)malloc((lena + lenb)*sizeof(char));
    for (int i = 0 ;i < lena ;i++){
        res[i] = a[i];
    }
    for (int j = 0; j < lenb ;j++){
        res[lena + j] = b[j];
    }
    return res;
}

char * symbol = "Glore";
char * ceoAddress = "ZZfWjs62biEyjmNphFUuuHUZxTZfsa8vFA";
char * adminAddress = "ZUcUohchJYTDfzNvnSQdrjFpVSZHVPhMXm";
char * poolAddress = "ZRfeV4mZCHxWN5uezqxchJw5yhaxcWpQh7"; //Cashier


char * init(char * totalSupply){
    if (arrayLen(ZPT_Storage_Get("totalSupply")) != 0)
        return "40003";
    if (ZPT_Runtime_CheckWitness(adminAddress) == 0)
        return "40002";
    if (Atoi64(totalSupply) <= 0)
        return "40001";
    ZPT_Storage_Put("totalSupply", totalSupply);
    ZPT_Storage_Put(adminAddress, totalSupply);
    ZPT_Storage_Put("paused", "0");
    return actionMarshal("init","-",adminAddress,totalSupply);
}

char * pause(){
    if (ZPT_Runtime_CheckWitness(ceoAddress) == 0)
        return "40002";
    if (Atoi(ZPT_Storage_Get("paused")) == 1)
        return "40004";
    ZPT_Storage_Put("paused", "1");
    return "Pause successfully.";
}

char * unPause(){
    if (ZPT_Runtime_CheckWitness(ceoAddress) == 0)
        return "40002";
    if (Atoi(ZPT_Storage_Get("paused")) == 0)
        return "40005";
    ZPT_Storage_Put("paused", "0");
    return "Unpause successfully.";
}

char * increasePool(char * amountChar){
    if (ZPT_Runtime_CheckWitness(adminAddress) == 0)
        return "40002";
    long long amount = Atoi64(amountChar);
    if (amount <= 0)
        return "40001";
    long long adminBalance = Atoi64(ZPT_Storage_Get(adminAddress));
    if (adminBalance < amount)
        return "40010";
    adminBalance -= amount;
    long long poolBalance = Atoi64(ZPT_Storage_Get(poolAddress)) + amount;
    ZPT_Storage_Put(adminAddress,I64toa(adminBalance,10));
    ZPT_Storage_Put(poolAddress,I64toa(poolBalance,10));
    return actionMarshal("increasePool",adminAddress,poolAddress,amountChar);
}

char * fromPoolToUser(char * toAddr, char * amountChar){
    if (ZPT_Runtime_CheckWitness(poolAddress) == 0)
        return "40002";
    char * toStatus = concat(toAddr, "status");
    if (Atoi(ZPT_Storage_Get(toStatus)) == 1)
        return "40013";
    long long amount = Atoi64(amountChar);
    if (amount <= 0)
        return "40001";
    long long poolBalance = Atoi64(ZPT_Storage_Get(poolAddress));
    if (poolBalance < amount)
        return "40010";
    poolBalance -= amount;
    long long toTotal = Atoi64(ZPT_Storage_Get(toAddr)) + amount;
    ZPT_Storage_Put(poolAddress,I64toa(poolBalance,10));
    ZPT_Storage_Put(toAddr,I64toa(toTotal,10));
    return actionMarshal("fromPoolToUser",poolAddress,toAddr,amountChar);
}

char * setNormal(char * address){
    if (ZPT_Runtime_CheckWitness(ceoAddress) == 0)
        return "40002";
    char * addrStatus = concat(address, "status");
    ZPT_Storage_Delete(addrStatus);
    return "Set successfully.";
}

char * setBlack(char * address){
    if (ZPT_Runtime_CheckWitness(ceoAddress) == 0)
        return "40002";
    char * addrStatus = concat(address, "status");
    ZPT_Storage_Put(addrStatus, "1");
    return "Set successfully.";
}

char * status(char * address){
    char * addrStatus = concat(address, "status");
    if (Atoi(ZPT_Storage_Get(addrStatus)) == 1)
        return "black";
    return "normal";
}

char * transfer(char * fromAddr, char * toAddr, char * amountChar){
    if (Atoi(ZPT_Storage_Get("paused")) == 1)
        return "40004.";
    if (ZPT_Runtime_CheckWitness(fromAddr) == 0)
        return "40011";
    char * fromStatus = concat(fromAddr, "status");
    char * toStatus = concat(toAddr, "status");
    if (Atoi(ZPT_Storage_Get(fromStatus)) == 1)
        return "40012";
    if (Atoi(ZPT_Storage_Get(toStatus)) == 1)
        return "40013";
    long long amount = Atoi64(amountChar);
    if (amount <= 0)
        return "40001";
    long long fromTotal = Atoi64(ZPT_Storage_Get(fromAddr));
    char * frozenKey = concat(fromAddr, "frozen");
    long long fromFrozen = Atoi64(ZPT_Storage_Get(frozenKey));
    long long fromAvailable = fromTotal - fromFrozen;
    if (fromAvailable < amount)
        return "40015";
    fromTotal -= amount;
    if (fromTotal == 0)
        ZPT_Storage_Delete(fromAddr);
    else
        ZPT_Storage_Put(fromAddr,I64toa(fromTotal,10));
    long long toTotal = Atoi64(ZPT_Storage_Get(toAddr)) + amount;
    ZPT_Storage_Put(toAddr,I64toa(toTotal,10));
    return actionMarshal("transfer",fromAddr,toAddr,amountChar);
}

char * approve(char * ownerAddr, char * allowedChar){
    if (Atoi(ZPT_Storage_Get("paused")) == 1)
        return "40004";
    if (ZPT_Runtime_CheckWitness(ownerAddr) == 0)
        return "40011";
    char * ownerStatus = concat(ownerAddr, "status");
    if (Atoi(ZPT_Storage_Get(ownerStatus)) == 1)
        return "40014";
    long long allowed = Atoi64(allowedChar);
    if (allowed <= 0)
        return "40001";
    long long ownerTotal = Atoi64(ZPT_Storage_Get(ownerAddr));
    char * frozenKey = concat(ownerAddr, "frozen");
    long long ownerFrozen = Atoi64(ZPT_Storage_Get(frozenKey));
    long long ownerAvailable = ownerTotal - ownerFrozen;
    if (ownerAvailable < allowed)
        return "40015";
    ownerFrozen += allowed;
    ZPT_Storage_Put(frozenKey,I64toa(ownerFrozen,10));
    return actionMarshal("approve",ownerAddr,"approve",allowedChar);
}

char * unApprove(char * ownerAddr, char * allowedChar){
    if (ZPT_Runtime_CheckWitness(poolAddress) == 0)
        return "40002";
    long long allowed = Atoi64(allowedChar);
    if (allowed <= 0)
        return "40001";
    char * frozenKey = concat(ownerAddr, "frozen");
    long long ownerFrozen = Atoi64(ZPT_Storage_Get(frozenKey));
    if (ownerFrozen < allowed)
        return "40016";
    ownerFrozen -= allowed;
    ZPT_Storage_Put(frozenKey,I64toa(ownerFrozen,10));
    return actionMarshal("unApprove",ownerAddr,"unapprove",allowedChar);
}

char * transferFrom(char *fromAddr, char *toAddr, char *amountChar){
    if (Atoi(ZPT_Storage_Get("paused")) == 1)
        return "40004";
    if (ZPT_Runtime_CheckWitness(poolAddress) == 0)
        return "40002";
    char * fromStatus = concat(fromAddr, "status");
    char * toStatus = concat(toAddr, "status");
    if (Atoi(ZPT_Storage_Get(fromStatus)) == 1)
        return "40012";
    if (Atoi(ZPT_Storage_Get(toStatus)) == 1)
        return "40013";
    long long amount = Atoi64(amountChar);
    if (amount <= 0)
        return "40001";
    char * frozenKey = concat(fromAddr, "frozen");
    long long fromFrozen = Atoi64(ZPT_Storage_Get(frozenKey));
    if (fromFrozen < amount)
        return "40016";
    fromFrozen -= amount;
    if (fromFrozen == 0)
        ZPT_Storage_Delete(frozenKey);
    else
        ZPT_Storage_Put(frozenKey,I64toa(fromFrozen,10));
    long long fromTotal = Atoi64(ZPT_Storage_Get(fromAddr)) - amount;
    if (fromTotal == 0)
        ZPT_Storage_Delete(fromAddr);
    else
        ZPT_Storage_Put(fromAddr,I64toa(fromTotal,10));
    long long toTotal = Atoi64(ZPT_Storage_Get(toAddr)) + amount;
    ZPT_Storage_Put(toAddr,I64toa(toTotal,10));
    return actionMarshal("transferFrom",fromAddr,toAddr,amountChar);
}

//Methods invoke entry
char * invoke(char * method,char * args){
    
    char * result;
    
    if (strcmp(method, "getSymbol")==0){
        ZPT_Runtime_Notify(symbol);
        return symbol;
    }
    
    if (strcmp(method, "getAdminAddress")==0){
        ZPT_Runtime_Notify(adminAddress);
        return adminAddress;
    }
    
    if (strcmp(method, "getPoolAddress")==0){
        ZPT_Runtime_Notify(poolAddress);
        return poolAddress;
    }
    
    if (strcmp(method ,"init")==0){
        struct Params{
            char * totalSupply;
        };
        struct Params *p = (struct Params *)malloc(sizeof(struct Params));
        ZPT_JsonUnmashalInput(p,sizeof(struct Params),args);
        result = init(p->totalSupply);
        ZPT_Runtime_Notify(result);
        return result;
    }
    
    if (strcmp(method ,"pause")==0){
        result = pause();
        ZPT_Runtime_Notify(result);
        return result;
    }
    
    if (strcmp(method ,"unPause")==0){
        result = unPause();
        ZPT_Runtime_Notify(result);
        return result;
    }
    
    if (strcmp(method, "totalSupply")==0){
        result = ZPT_Storage_Get("totalSupply");
        ZPT_Runtime_Notify(result);
        return result;
    }
    
    if (strcmp(method, "currentSupply")==0){
        long long totalSupply = Atoi64(ZPT_Storage_Get("totalSupply"));
        long long currentSupply = totalSupply - Atoi64(ZPT_Storage_Get(adminAddress));
        result = I64toa(currentSupply,10);
        ZPT_Runtime_Notify(result);
        return result;
    }
    
    if (strcmp(method, "currentCirculation")==0){
        long long totalSupply = Atoi64(ZPT_Storage_Get("totalSupply"));
        long long currentSupply = totalSupply - Atoi64(ZPT_Storage_Get(adminAddress));
        long long currentCirculation = currentSupply - Atoi64(ZPT_Storage_Get(poolAddress));
        result = I64toa(currentCirculation,10);
        ZPT_Runtime_Notify(result);
        return result;
    }
    
    if (strcmp(method, "increasePool")==0){
        struct Params{
            char * amount;
        };
        struct Params *p = (struct Params *)malloc(sizeof(struct Params));
        ZPT_JsonUnmashalInput(p,sizeof(struct Params),args);
        result = increasePool(p->amount);
        ZPT_Runtime_Notify(result);
        return result;
    }
    
    if (strcmp(method, "fromPoolToUser")==0){
        struct Params{
            char * to;
            char * amount;
        };
        struct Params *p = (struct Params *)malloc(sizeof(struct Params));
        ZPT_JsonUnmashalInput(p,sizeof(struct Params),args);
        result = fromPoolToUser(p->to,p->amount);
        ZPT_Runtime_Notify(result);
        return result;
    }
    
    if (strcmp(method, "balanceOf")==0){
        struct Params{
            char * address;
        };
        struct Params *p = (struct Params *)malloc(sizeof(struct Params));
        ZPT_JsonUnmashalInput(p,sizeof(struct Params),args);
        result = ZPT_Storage_Get(p->address);
        ZPT_Runtime_Notify(result);
        return result;
    }
    
    if (strcmp(method, "availableAmount")==0){
        struct Params{
            char * address;
        };
        struct Params *p = (struct Params *)malloc(sizeof(struct Params));
        ZPT_JsonUnmashalInput(p,sizeof(struct Params),args);
        long long totalAmount = Atoi64(ZPT_Storage_Get(p->address));
        char * frozenKey = concat(p->address,"frozen");
        long long frozenAmount = Atoi64(ZPT_Storage_Get(frozenKey));
        long long availableAmount = totalAmount - frozenAmount;
        result = I64toa(availableAmount,10);
        ZPT_Runtime_Notify(result);
        return result;
    }
    
    if (strcmp(method, "frozenAmount")==0){
        struct Params{
            char * address;
        };
        struct Params *p = (struct Params *)malloc(sizeof(struct Params));
        ZPT_JsonUnmashalInput(p,sizeof(struct Params),args);
        char * frozenKey = concat(p->address,"frozen");
        long long frozenAmount = Atoi64(ZPT_Storage_Get(frozenKey));
        result = I64toa(frozenAmount,10);
        ZPT_Runtime_Notify(result);
        return result;
    }
    
    if (strcmp(method, "transfer")==0){
        struct Params{
            char * from;
            char * to;
            char * amount;
        };
        struct Params *p = (struct Params *)malloc(sizeof(struct Params));
        ZPT_JsonUnmashalInput(p,sizeof(struct Params),args);
        result = transfer(p->from, p->to, p->amount);
        ZPT_Runtime_Notify(result);
        return result;
    }
    
    if (strcmp(method, "transferFrom")==0){
        struct Params{
            char * from;
            char * to;
            char * amount;
        };
        struct Params *p = (struct Params *)malloc(sizeof(struct Params));
        ZPT_JsonUnmashalInput(p,sizeof(struct Params),args);
        result = transferFrom(p->from, p->to, p->amount);
        ZPT_Runtime_Notify(result);
        return result;
    }
    
    if (strcmp(method, "approve")==0){
        struct Params{
            char * owner;
            char * allowed;
        };
        struct Params *p = (struct Params *)malloc(sizeof(struct Params));
        ZPT_JsonUnmashalInput(p,sizeof(struct Params),args);
        result = approve(p->owner, p->allowed);
        ZPT_Runtime_Notify(result);
        return result;
    }
    
    if (strcmp(method, "unApprove")==0){
        struct Params{
            char * owner;
            char * allowed;
        };
        struct Params *p = (struct Params *)malloc(sizeof(struct Params));
        ZPT_JsonUnmashalInput(p,sizeof(struct Params),args);
        result = unApprove(p->owner, p->allowed);
        ZPT_Runtime_Notify(result);
        return result;
    }
    
    if (strcmp(method, "status")==0){
        struct Params{
            char * address;
        };
        struct Params *p = (struct Params *)malloc(sizeof(struct Params));
        ZPT_JsonUnmashalInput(p,sizeof(struct Params),args);
        result = status(p->address);
        ZPT_Runtime_Notify(result);
        return result;
    }
    
    if (strcmp(method, "setNormal")==0){
        struct Params{
            char * address;
        };
        struct Params *p = (struct Params *)malloc(sizeof(struct Params));
        ZPT_JsonUnmashalInput(p,sizeof(struct Params),args);
        result = setNormal(p->address);
        ZPT_Runtime_Notify(result);
        return result;
    }
    
    if (strcmp(method, "setBlack")==0){
        struct Params{
            char * address;
        };
        struct Params *p = (struct Params *)malloc(sizeof(struct Params));
        ZPT_JsonUnmashalInput(p,sizeof(struct Params),args);
        result = setBlack(p->address);
        ZPT_Runtime_Notify(result);
        return result;
    }
    
    result = "Invalid invoke method.";
    ZPT_Runtime_Notify(result);
    return result;
}
