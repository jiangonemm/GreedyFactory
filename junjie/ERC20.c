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

//contract administration
char * ceoAddress = "ZEuzshrCsE1cnvPuuRrDYgnVYNDtyt5d3X";
int paused = 0;

char * Pause(char * address){
    if (strcmp(address, ceoAddress)==0){
        paused = 1;
        ZPT_Storage_Put("paused", Itoa(paused));
        return "The cantract is paused successfully.";
    }
    return "You have no permission to pause the contract.";
}

char * UnPause(char * address){
    if (strcmp(address, ceoAddress)==0){
        paused = 0;
        ZPT_Storage_Put("paused", Itoa(paused));
        return "The cantract is unpaused successfully.";
    }
    return "You have no permission to unpause the contract.";
}

//Check if an account is in the database
int IsStored(char * key){
    if (arrayLen(ZPT_Storage_Get(key)) == 0)
        return 0;
    return 1;
}

//Check if an account is appreved by another one
int IsApproved(char * owner, char * spender){
    char * allowedKey = concat(owner, spender);
    if (arrayLen(ZPT_Storage_Get(allowedKey)) == 0)
        return 0;
    return 1;
}

//Initialize the contract, includes the totalSupply value.
char * Init(char * totalSupply){
    if (arrayLen(ZPT_Storage_Get("totalSupply")) != 0)
        return "Already initialized.";
    if (Atoi(totalSupply) <= 0)
        return "The totalsupply can not be less than or equal to 0.";
    ZPT_Storage_Put("totalSupply", totalSupply);
    return "Init success!";
}

char * TotalSupply(){
    char * totalSupply = ZPT_Storage_Get("totalSupply");
    if (arrayLen(totalSupply) == 0)
        return "Please init first.";
    return totalSupply;
}

char * IncreaseTotal(char * address, char * valueChar){
    if (Atoi(ZPT_Storage_Get("paused")) == 1)
        return "The contract has been paused.";
    if (strcmp(address, ceoAddress) != 0)
        return "You have no permission to increase the totalSupply.";
    char * totalSupply = ZPT_Storage_Get("totalSupply");
    if (arrayLen(totalSupply) == 0)
        return "Please init first.";
    int value = Atoi(valueChar);
    if (value <= 0)
        return "The value increased can not be less than or equal to 0.";
    int totalSupplyNew = Atoi(totalSupply) + value;
    ZPT_Storage_Put("totalSupply", Itoa(totalSupplyNew));
    return "Increase success.";
}

char * DecreaseTotal(char * address, char * valueChar){
    if (Atoi(ZPT_Storage_Get("paused")) == 1)
        return "The contract has been paused.";
    if (strcmp(address, ceoAddress) != 0)
        return "You have no permission to increase the totalSupply.";
    char * totalSupply = ZPT_Storage_Get("totalSupply");
    if (arrayLen(totalSupply) == 0)
        return "Please init first.";
    int value = Atoi(valueChar);
    if (value <= 0)
        return "The value decreased can not be less than or equal to 0.";
    int totalSupplyNew = Atoi(totalSupply);
    if (totalSupplyNew <= value)
        return "The value decreased must be less than the current totalSupply.";
    totalSupplyNew -= value;
    ZPT_Storage_Put("totalSupply", Itoa(totalSupplyNew));
    return "Decrease success.";
}

char * BalanceOf(char * address){
    char * balance = ZPT_Storage_Get(address);
    if (arrayLen(balance) == 0)
        return "Address is not in our database.";
    return balance;
}

char * Transfer(char * fromAddr, char * toAddr, char * amountChar){
    if (Atoi(ZPT_Storage_Get("paused")) == 1)
        return "The contract has been paused.";
    if (ZPT_Runtime_CheckWitness(fromAddr) == 0)
        return "Inconsistent address.";
    if (IsStored(fromAddr) == 0)
        return "Sender address is not in our database.";
    int amount = Atoi(amountChar);
    if (amount <= 0)
        return "Transfer amount cannot be less than or equal to 0.";
    int balance_from = Atoi(BalanceOf(fromAddr));
    if (balance_from < amount)
        return "No sufficient balance.";
    
    balance_from -= amount;
    if (balance_from == 0)
        ZPT_Storage_Delete(fromAddr);
    else
        ZPT_Storage_Put(fromAddr,Itoa(balance_from));
    
    if (IsStored(toAddr) == 0)
        ZPT_Storage_Put(toAddr, amountChar);
    else {
        int balance_to = Atoi(BalanceOf(toAddr));
        balance_to += amount;
        ZPT_Storage_Put(toAddr,Itoa(balance_to));
    }
    return "Transfer success!";
}


char * TransferFrom(char *fromAddr, char *spenderAddr, char *toAddr, char *amountChar){
    if (Atoi(ZPT_Storage_Get("paused")) == 1)
        return "The contract has been paused.";
    if (ZPT_Runtime_CheckWitness(spenderAddr) == 0)
        return "Inconsistent address.";
    if (IsStored(fromAddr) == 0)
        return "Sender address is not in our database.";
    if (IsApproved(fromAddr, spenderAddr) == 0)
        return "Not approved.";
    int amount = Atoi(amountChar);
    if (amount <= 0)
        return "TransferFrom amount cannot be less than or equal to 0.";
    int balance_from = Atoi(BalanceOf(fromAddr));
    if (balance_from < amount)
        return "No sufficient balance.";
    char * allowedKey = concat(fromAddr, spenderAddr);
    int allowed = Atoi(ZPT_Storage_Get(allowedKey));
    if (allowed < amount)
        return "No sufficient allowance.";
    
    balance_from -= amount;
    if (balance_from == 0) {
        ZPT_Storage_Delete(fromAddr);
        ZPT_Storage_Delete(allowedKey);
    }
    else
        ZPT_Storage_Put(fromAddr,Itoa(balance_from));
    
    if (IsStored(toAddr) == 0)
        ZPT_Storage_Put(toAddr, amountChar);
    else {
        int balance_to = Atoi(BalanceOf(toAddr));
        balance_to += amount;
        ZPT_Storage_Put(toAddr,Itoa(balance_to));
    }
    
    allowed -= amount;
    if (allowed == 0)
        ZPT_Storage_Delete(allowedKey);
    else
        ZPT_Storage_Put(allowedKey, Itoa(allowed));
    return "TransferFrom success!";
}


char * Approve(char * ownerAddr, char * spenderAddr, char * allowedChar){
    if (Atoi(ZPT_Storage_Get("paused")) == 1)
        return "The contract has been paused.";
    if (ZPT_Runtime_CheckWitness(ownerAddr) == 0)
        return "Inconsistent address.";
    if (IsStored(ownerAddr) == 0)
        return "Owner address is not in our database.";
    int allowed = Atoi(allowedChar);
    if (allowed <= 0)
        return "The allowance can not be less than or equal to 0.";
    int balance_owner = Atoi(BalanceOf(ownerAddr));
    if (balance_owner < allowed)
        return "The allowance can not be larger than the balance of owner.";
    char * allowedKey = concat(ownerAddr, spenderAddr);
    ZPT_Storage_Put(allowedKey, Itoa(allowed));
    return "Approve success!";
}

char * Allowance(char * ownerAddr, char * spenderAddr){
    if (IsStored(ownerAddr) == 0)
        return "Owner address is not in our database.";
    if (IsApproved(ownerAddr, spenderAddr) == 0)
        return "Not approved yet.";
    char * allowedKey = concat(ownerAddr, spenderAddr);
    return ZPT_Storage_Get(allowedKey);
}

char * invoke(char * method,char * args){
    
    char * result;
    
    if (strcmp(method ,"pause")==0){
        struct Params{
            char * address;
        };
        struct Params *p = (struct Params *)malloc(sizeof(struct Params));
        ZPT_JsonUnmashalInput(p,sizeof(struct Params),args);
        result = Pause(p->address);
        ZPT_Runtime_Notify(result);
        return result;
    }
    
    if (strcmp(method ,"unpause")==0){
        struct Params{
            char * address;
        };
        struct Params *p = (struct Params *)malloc(sizeof(struct Params));
        ZPT_JsonUnmashalInput(p,sizeof(struct Params),args);
        result = UnPause(p->address);
        ZPT_Runtime_Notify(result);
        return result;
    }
    
    if (strcmp(method ,"init")==0){
        struct Params{
            char * totalSupply;
        };
        struct Params *p = (struct Params *)malloc(sizeof(struct Params));
        ZPT_JsonUnmashalInput(p,sizeof(struct Params),args);
        result = Init(p->totalSupply);
        ZPT_Runtime_Notify(result);
        return result;
    }
    
    if (strcmp(method, "totalSupply")==0){
        result = TotalSupply();
        ZPT_Runtime_Notify(result);
        return result;
    }
    
    if (strcmp(method, "increaseTotal")==0){
        struct Params{
            char * address;
            char * value;
        };
        struct Params *p = (struct Params *)malloc(sizeof(struct Params));
        ZPT_JsonUnmashalInput(p,sizeof(struct Params),args);
        result = IncreaseTotal(p->address, p->value);
        ZPT_Runtime_Notify(result);
        return result;
    }
    
    if (strcmp(method, "decreaseTotal")==0){
        struct Params{
            char * address;
            char * value;
        };
        struct Params *p = (struct Params *)malloc(sizeof(struct Params));
        ZPT_JsonUnmashalInput(p,sizeof(struct Params),args);
        result = DecreaseTotal(p->address, p->value);
        ZPT_Runtime_Notify(result);
        return result;
    }
    
    if (strcmp(method, "balanceOf")==0){
        struct Params{
            char * address;
        };
        struct Params *p = (struct Params *)malloc(sizeof(struct Params));
        ZPT_JsonUnmashalInput(p,sizeof(struct Params),args);
        result = BalanceOf(p->address);
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
        result = Transfer(p->from, p->to, p->amount);
        ZPT_Runtime_Notify(result);
        return result;
    }
    
    if (strcmp(method, "transferFrom")==0){
        struct Params{
            char * from;
            char * spender;
            char * to;
            char * amount;
        };
        struct Params *p = (struct Params *)malloc(sizeof(struct Params));
        ZPT_JsonUnmashalInput(p,sizeof(struct Params),args);
        result = TransferFrom(p->from, p->spender, p->to, p->amount);
        ZPT_Runtime_Notify(result);
        return result;
    }
    
    if (strcmp(method, "approve")==0){
        struct Params{
            char * owner;
            char * spender;
            char * allowed;
        };
        struct Params *p = (struct Params *)malloc(sizeof(struct Params));
        ZPT_JsonUnmashalInput(p,sizeof(struct Params),args);
        result = Approve(p->owner, p->spender, p->allowed);
        ZPT_Runtime_Notify(result);
        return result;
    }
    
    if (strcmp(method, "allowance")==0){
        struct Params{
            char * owner;
            char * spender;
        };
        struct Params *p = (struct Params *)malloc(sizeof(struct Params));
        ZPT_JsonUnmashalInput(p,sizeof(struct Params),args);
        result = Allowance(p->owner, p->spender);
        ZPT_Runtime_Notify(result);
        return result;
    }
    
    if(strcmp(method,"addStorage")==0){
        struct Params{
            char * key;
            char * value;
        };
        struct Params *p = (struct Params *)malloc(sizeof(struct Params));
        ZPT_JsonUnmashalInput(p,sizeof(struct Params),args);
        ZPT_Storage_Put(p->key,p->value);
        result = "Add success.";
        ZPT_Runtime_Notify(result);
        return result;
    }
    
    result = "Invalid invoke method.";
    ZPT_Runtime_Notify(result);
    return result;
}
