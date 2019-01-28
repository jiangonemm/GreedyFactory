//system apis
void *calloc(int count, int length);
void *malloc(int size);
int arrayLen(void *a);
int memcpy(void *dest, void *src, int length);
int memset(void *dest, char c, int length);

//utility apis
int strcmp(char *a, char *b);
char *strconcat(char *a, char *b);
int Atoi(char *s);
long long Atoi64(char *s);
char *Itoa(int a);
char *I64toa(long long amount, int radix);
char *SHA1(char *s);
char *SHA256(char *s);

//parameter apis
int ZPT_ReadInt32Param(char *args);
long long ZPT_ReadInt64Param(char *args);
char *ZPT_ReadStringParam(char *args);
void ZPT_JsonUnmashalInput(void *addr, int size, char *arg);
char *ZPT_JsonMashalResult(void *val, char *types, int succeed);
char *ZPT_JsonMashalParams(void *s);
char *ZPT_RawMashalParams(void *s);
char *ZPT_GetCallerAddress();
char *ZPT_GetSelfAddress();
char *ZPT_CallContract(char *address, char *contractCode, char *method, char *args);
char *ZPT_MarshalNativeParams(void *s);
char *ZPT_MarshalNeoParams(void *s);

//Runtime apis
int ZPT_Runtime_CheckWitness(char *address);
void ZPT_Runtime_Notify(char *address);
int ZPT_Runtime_CheckSig(char *pubkey, char *data, char *sig);
int ZPT_Runtime_GetTime();
void ZPT_Runtime_Log(char *message);

//Attribute apis
int ZPT_Attribute_GetUsage(char *data);
char *ZPT_Attribute_GetData(char *data);

//Block apis
char *ZPT_Block_GetCurrentHeaderHash();
int ZPT_Block_GetCurrentHeaderHeight();
char *ZPT_Block_GetCurrentBlockHash();
int ZPT_Block_GetCurrentBlockHeight();
char *ZPT_Block_GetTransactionByHash(char *hash);
int *ZPT_Block_GetTransactionCountByBlkHash(char *hash);
int *ZPT_Block_GetTransactionCountByBlkHeight(int height);
char **ZPT_Block_GetTransactionsByBlkHash(char *hash);
char **ZPT_Block_GetTransactionsByBlkHeight(int height);

//Blockchain apis
int ZPT_BlockChain_GetHeight();
char *ZPT_BlockChain_GetHeaderByHeight(int height);
char *ZPT_BlockChain_GetHeaderByHash(char *hash);
char *ZPT_BlockChain_GetBlockByHeight(int height);
char *ZPT_BlockChain_GetBlockByHash(char *hash);
char *ZPT_BlockChain_GetContract(char *address);

//header apis
char *ZPT_Header_GetHash(char *data);
int ZPT_Header_GetVersion(char *data);
char *ZPT_Header_GetPrevHash(char *data);
char *ZPT_Header_GetMerkleRoot(char *data);
int ZPT_Header_GetIndex(char *data);
int ZPT_Header_GetTimestamp(char *data);
long long ZPT_Header_GetConsensusData(char *data);
char *ZPT_Header_GetNextConsensus(char *data);

//storage apis
void ZPT_Storage_Put(char *key, char *value);
char *ZPT_Storage_Get(char *key);
void ZPT_Storage_Delete(char *key);

//transaction apis
char *ZPT_Transaction_GetHash(char *data);
int ZPT_Transaction_GetType(char *data);
char *ZPT_Transaction_GetAttributes(char *data);

#define true "1";
#define false "0";

int init_amount = 0;

char *Init()
{
    if (arrayLen(ZPT_Storage_Get("totalSupply")) != 0)
    {
        return "init has~~~~~ finished!";
    }
    ZPT_Storage_Put("totalSupply", Itoa(init_amount));
    return "init success!";
}

char *TotalSupply()
{
    if (arrayLen(ZPT_Storage_Get("totalSupply")) == 0)
    {
        return "you need init!";
    }
    return ZPT_Storage_Get("totalSupply");
}

char *OwnerOf(char *TokenID)
{
    return ZPT_Storage_Get(TokenID);
}

char *Owns(char *TokenID, char *address)
{
    return  Itoa(strcmp(ZPT_Storage_Get(TokenID), address));
}

char *BalanceOf(char *address)
{
    if (Atoi(ZPT_Storage_Get(address)) == 0)
        return Itoa(0);
    else
        return ZPT_Storage_Get(address);
}

void IncreaseIndex(char *totalSupply, char *TokenID)
{
    ZPT_Storage_Put(totalSupply, TokenID);
}

char *IncreaseTotalSupply()
{
    int totalSupply = Atoi(ZPT_Storage_Get("totalSupply"));
    totalSupply = totalSupply + 1;
    ZPT_Storage_Put("totalSupply", Itoa(totalSupply));
    return Itoa(totalSupply);
}

void Transfer(char *from, char *to, char *TokenID)
{

    ZPT_Storage_Put(TokenID, to);
    int amount = Atoi(ZPT_Storage_Get(to));
    if (amount == 0)
    {
        amount = 0;
    }
    amount = amount + 1;
    ZPT_Storage_Put(to, Itoa(amount));
    if (from != "")
    {
        int fromAmount = Atoi(ZPT_Storage_Get(from));
        fromAmount = fromAmount - 1;
        if (fromAmount == 0)
        {
            ZPT_Storage_Delete(from);
        }
        else
            ZPT_Storage_Put(from, Itoa(fromAmount));

        char *ap = "ap.";
        char *newTokenID = strconcat(ap, TokenID);
        ZPT_Storage_Delete(newTokenID);
    }
}

void ApproveInternal(char *to, char *TokenID)
{
    char *ap = "ap.";
    char *newTokenID = strconcat(ap, TokenID);
    ZPT_Storage_Put(newTokenID, to);
}

char *ApprovedFor(char *address, char *TokenID)
{
    char *ap = "ap.";
    char *newTokenID = strconcat(ap, TokenID);
    return  Itoa(strcmp(ZPT_Storage_Get(newTokenID), address));
}

char *Approve(char *from, char *to, char *TokenID)
{
    if (to == "")
        return false;
    if (Atoi(Owns(TokenID, from)) == 0)
        return false;
    if (ZPT_Runtime_CheckWitness(from) == 0)
        return false;
    ApproveInternal(to, TokenID);
    return true;
}

char *TransferFromOwner(char *owner, char *to, char *TokenID)
{
    if (to == "")
        return false;
    if (Atoi(Owns(TokenID, owner)) == 0)
        return false;
    if (ZPT_Runtime_CheckWitness(owner) == 0)
        return false;
    Transfer(owner, to, TokenID);
    return true;
}

char *TransferFromApproval(char *from, char *to, char *approval, char *TokenID)
{
    if (to == "")
        return false;
    if (Atoi(ApprovedFor(approval, TokenID)) == 0)
        return false;
    if (Atoi(Owns(TokenID, from)) == 0)
        return false;
    if (ZPT_Runtime_CheckWitness(approval) == 0)
        return false;
    Transfer(from, to, TokenID);
    return true;
}

char *GetHashList(char *address)
{
    int totalSupply = Atoi(ZPT_Storage_Get("totalSupply"));
    char *ForTrim = {"...."};
    char *Hash = {""};
    char *Result = {""};
    for (int i = 1; i <= totalSupply; i = i + 1)
    {
        Hash = ZPT_Storage_Get(Itoa(i));
        if (strcmp(ZPT_Storage_Get(Hash), address) == 1)
        {
            Result = strconcat(Result, Hash);
            Result = strconcat(Result, ForTrim);
        }
    }
    return Result;
}

char *PutRental(char *TokenID, char *address)
{
    char *rt = "rt.";
    char *newHash = strconcat(rt, TokenID);
    ZPT_Storage_Put(newHash, address);
    return true;
}

char *GetRental(char *TokenID)
{
    char *rt = "rt.";
    char *newHash = strconcat(rt, TokenID);
    return ZPT_Storage_Get(newHash);
}



char *invoke(char *method, char *args)
{
    if (strcmp(method, "init") == 0)
    {
        char *value = Init();
        char * result = ZPT_JsonMashalResult(value,"string",1);
        ZPT_Runtime_Notify(result);
        return result;
    }

    if (strcmp(method, "create") == 0)
    {

        struct Params
        {
            char *TokenID;
            char *address;
        };
        struct Params *p = (struct Params *)malloc(sizeof(struct Params));
        ZPT_JsonUnmashalInput(p, sizeof(struct Params), args);
        if (arrayLen(ZPT_Storage_Get("totalSupply")) == 0)
        {
            return "You need `init!";
        }
        char *Result = ZPT_Storage_Get(p->TokenID);
        if (arrayLen(Result) != 0)
            return "Your TokenID is existed";
        Transfer("", p->address, p->TokenID);
        int totalSupply = Atoi(IncreaseTotalSupply());
        IncreaseIndex(Itoa(totalSupply), p->TokenID);
        char *value = "create success";
        char * result = ZPT_JsonMashalResult(value,"string",1);
        ZPT_Runtime_Notify(result);
        return result;
    }

    if (strcmp(method, "totalSupply") == 0)
    {
        char *value = TotalSupply();
        char * result = ZPT_JsonMashalResult(value,"string",1);
        ZPT_Runtime_Notify(result);
        return result;
    }

    if (strcmp(method, "ownerOf") == 0)
    {

        struct Params
        {
            char *TokenID;
        };
        struct Params *p = (struct Params *)malloc(sizeof(struct Params));
        ZPT_JsonUnmashalInput(p, sizeof(struct Params), args);
        char *value = OwnerOf(p->TokenID);
        char * result = ZPT_JsonMashalResult(value,"string",1);
        ZPT_Runtime_Notify(result);
        return result;
    }

    if (strcmp(method, "owns") == 0)
    {

        struct Params
        {
            char *TokenID;
            char *address;
        };
        struct Params *p = (struct Params *)malloc(sizeof(struct Params));
        ZPT_JsonUnmashalInput(p, sizeof(struct Params), args);
        char *value = Owns(p->TokenID, p->address);
        char * result = ZPT_JsonMashalResult(value,"string",1);
        ZPT_Runtime_Notify(result);
        return result;
    }

    if (strcmp(method, "balanceOf") == 0)
    {

        struct Params
        {
            char *address;
        };
        struct Params *p = (struct Params *)malloc(sizeof(struct Params));
        ZPT_JsonUnmashalInput(p, sizeof(struct Params), args);
        char *value = BalanceOf(p->address);
        char * result = ZPT_JsonMashalResult(value,"string",1);
        ZPT_Runtime_Notify(result);
        return result;
    }

    if (strcmp(method, "approve") == 0)
    {

        struct Params
        {
            char *from;
            char *to;
            char *TokenID;
        };
        struct Params *p = (struct Params *)malloc(sizeof(struct Params));
        ZPT_JsonUnmashalInput(p, sizeof(struct Params), args);
        char *value = Approve(p->from, p->to, p->TokenID);
        char * result = ZPT_JsonMashalResult(value,"string",1);
        ZPT_Runtime_Notify(result);
        return result;
    }

    if (strcmp(method, "transferFromOwner") == 0)
    {

        struct Params
        {
            char *from;
            char *to;
            char *TokenID;
        };
        struct Params *p = (struct Params *)malloc(sizeof(struct Params));
        ZPT_JsonUnmashalInput(p, sizeof(struct Params), args);
        char *value = TransferFromOwner(p->from, p->to, p->TokenID);
        char * result = ZPT_JsonMashalResult(value,"string",1);
        ZPT_Runtime_Notify(result);
        return result;
    }

    if (strcmp(method, "transferFromApproval") == 0)
    {

        struct Params
        {
            char *from;
            char *to;
            char *approval;
            char *TokenID;
        };
        struct Params *p = (struct Params *)malloc(sizeof(struct Params));
        ZPT_JsonUnmashalInput(p, sizeof(struct Params), args);
        char *value = TransferFromApproval(p->from, p->to, p->approval, p->TokenID);
        char * result = ZPT_JsonMashalResult(value,"string",1);
        ZPT_Runtime_Notify(result);
        return result;
    }

    if (strcmp(method, "getHashList") == 0)
    {

        struct Params
        {
            char *address;
        };
        struct Params *p = (struct Params *)malloc(sizeof(struct Params));
        ZPT_JsonUnmashalInput(p, sizeof(struct Params), args);
        char *value = GetHashList(p->address);
        char * result = ZPT_JsonMashalResult(value,"string",1);
        ZPT_Runtime_Notify(result);
        return result;
    }

    if (strcmp(method, "putRental") == 0)
    {

        struct Params
        {
            char *TokenID;
            char *address;
        };
        struct Params *p = (struct Params *)malloc(sizeof(struct Params));
        ZPT_JsonUnmashalInput(p, sizeof(struct Params), args);
        char *value = PutRental(p->TokenID, p->address);
        char * result = ZPT_JsonMashalResult(value,"string",1);
        ZPT_Runtime_Notify(result);
        return result;
    }

    if (strcmp(method, "getRental") == 0)
    {

        struct Params
        {
            char *TokenID;
        };
        struct Params *p = (struct Params *)malloc(sizeof(struct Params));
        ZPT_JsonUnmashalInput(p, sizeof(struct Params), args);
        char *value = GetRental(p->TokenID);
        char * result = ZPT_JsonMashalResult(value,"string",1);
        ZPT_Runtime_Notify(result);
        return result;
    }
}
