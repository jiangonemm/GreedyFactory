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

char * Transfer(char * fromAddr, char * toAddr, char * amountChar){
    int amount = Atoi(amountChar);
    if (amount <= 0)
        return "Transfer amount cannot be less than or equal to 0.";
    int balance_from = Atoi(ZPT_Storage_Get(fromAddr));
    if (balance_from < amount)
        return "No sufficient balance.";
    balance_from = balance_from - amount;
    int balance_to = Atoi(ZPT_Storage_Get(toAddr)) + amount;
    ZPT_Storage_Put(fromAddr,Itoa(balance_from));
    ZPT_Storage_Put(toAddr,Itoa(balance_to));
    return "Transfer success! You can check new balance by the method \"balanceOf\".";
}

char * TransferFrom(char *fromAddr, char *spenderAddr, char *toAddr, char *amountChar){
    int amount = Atoi(amountChar);
    if (amount <= 0)
        return "Transfer amount cannot be less than or equal to 0.";
    int balance_from = Atoi(ZPT_Storage_Get(fromAddr));
    if (balance_from < amount)
        return "No sufficient balance.";
    char * allowedKey = concat(fromAddr, spenderAddr);
    int allowed = Atoi(ZPT_Storage_Get(allowedKey));
    if (allowed < amount)
        return "No sufficient allowance.";
    balance_from = balance_from - amount;
    int balance_to = Atoi(ZPT_Storage_Get(toAddr)) + amount;
    ZPT_Storage_Put(fromAddr,Itoa(balance_from));
    ZPT_Storage_Put(toAddr,Itoa(balance_to));
    allowed = allowed - amount;
    ZPT_Storage_Put(allowedKey, Itoa(allowed));
    return "TransferFrom success! You can check new balance by the method \"balanceOf\".";
}

char * invoke(char * method,char * args){
    
    if (strcmp(method ,"init")==0 ){
        if (arrayLen(ZPT_Storage_Get("totalSupply")) != 0);
        return "Already initialized.";
        struct Params{
            char * _totalSupply;
        };
        struct Params *p = (struct Params *)malloc(sizeof(struct Params));
        ZPT_JsonUnmashalInput(p,sizeof(struct Params),args);
        ZPT_Storage_Put("totalSupply",p->_totalSupply);
        return "Init success!";
    }
    
    if (strcmp(method, "totalSupply")==0){
        char * totalSupply = ZPT_Storage_Get("totalSupply");
        if (arrayLen(totalSupply) == 0)
            return "Please init first.";
        return totalSupply;
    }
    
    if (strcmp(method, "balanceOf")==0){
        struct Params{
            char * _address;
        };
        struct Params *p = (struct Params *)malloc(sizeof(struct Params));
        ZPT_JsonUnmashalInput(p,sizeof(struct Params),args);
        char * balance = ZPT_Storage_Get(p->_address);
        if (arrayLen(balance) == 0)
            return "Address is not in our database!";
        return balance;
    }
    
    if (strcmp(method, "transfer")==0){
        struct Params{
            char * _from;
            char * _to;
            char * _amount;
        };
        struct Params *p = (struct Params *)malloc(sizeof(struct Params));
        ZPT_JsonUnmashalInput(p,sizeof(struct Params),args);
        if (ZPT_Runtime_CheckWitness(p->_from) == 0)
            return "Inconsistent address.";
        return Transfer(p->_from, p->_to, p->_amount);
    }
    
    if (strcmp(method, "transferFrom")==0){
        struct Params{
            char * _from;
            char * _spender;
            char * _to;
            char * _amount;
        };
        struct Params *p = (struct Params *)malloc(sizeof(struct Params));
        ZPT_JsonUnmashalInput(p,sizeof(struct Params),args);
        return TransferFrom(p->_from, p->_spender, p->_to, p->_amount);
    }
    
    if (strcmp(method, "approve")==0){
        struct Params{
            char * _owner;
            char * _spender;
            char * _allowed;
        };
        struct Params *p = (struct Params *)malloc(sizeof(struct Params));
        ZPT_JsonUnmashalInput(p,sizeof(struct Params),args);
        if ((Atoi(p->_allowed)) <= 0)
            return "The allowance can not be less than or equal to 0.";
        char * allowedKey = concat(p->_owner, p->_spender);
        ZPT_Storage_Put(allowedKey,p->_allowed);
        return "Approve success.";
    }
    
    if (strcmp(method, "allowance")==0){
        struct Params{
            char * _owner;
            char * _spender;
        };
        struct Params *p = (struct Params *)malloc(sizeof(struct Params));
        ZPT_JsonUnmashalInput(p,sizeof(struct Params),args);
        char * allowedKey = concat(p->_owner, p->_spender);
        if (arrayLen(allowedKey) == 0)
            return "Not approved yet.";
        return ZPT_Storage_Get(allowedKey);
    }
    
    if(strcmp(method,"addStorage")==0){
        
        struct Params{
            char * a;
            char * b;
        };
        struct Params *p = (struct Params *)malloc(sizeof(struct Params));
        ZPT_JsonUnmashalInput(p,sizeof(struct Params),args);
        ZPT_Storage_Put(p->a,p->b);
        char * result = ZPT_JsonMashalResult("Done","string",1);
        ZPT_Runtime_Notify(result);
        return result;
    }
    
    if(strcmp(method,"getStorage")==0){
        struct Params{
            char * a;
        };
        struct Params *p = (struct Params *)malloc(sizeof(struct Params));
        ZPT_JsonUnmashalInput(p,sizeof(struct Params),args);
        char * value = ZPT_Storage_Get(p->a);
        char * result = ZPT_JsonMashalResult(value,"string",1);
        ZPT_Runtime_Notify(result);
        return result;
    }
    
    if(strcmp(method,"deleteStorage")==0){
        
        struct Params{
            char * a;
        };
        struct Params *p = (struct Params *)malloc(sizeof(struct Params));
        ZPT_JsonUnmashalInput(p,sizeof(struct Params),args);
        ZPT_Storage_Delete(p->a);
        char * result = ZPT_JsonMashalResult("Done","string",1);
        ZPT_Runtime_Notify(result);
        return result;
    }
}
