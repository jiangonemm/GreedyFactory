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
int add(int a, int b)
{
    return a + b;
}

char *concat(char *a, char *b)
{
    int lena = arrayLen(a);
    int lenb = arrayLen(b);
    char *res = (char *)malloc((lena + lenb) * sizeof(char));
    for (int i = 0; i < lena; i++)
    {
        res[i] = a[i];
    }

    for (int j = 0; j < lenb; j++)
    {
        res[lena + j] = b[j];
    }
    return res;
}

int sumArray(int *a, int *b)
{

    int res = 0;
    int lena = arrayLen(a);
    int lenb = arrayLen(b);

    for (int i = 0; i < lena; i++)
    {
        res += a[i];
    }
    for (int j = 0; j < lenb; j++)
    {
        res += b[j];
    }
    return res;
}

#define true 1;
#define false 0;

long long init_amount = 0;

void Transference(char *from, char *to, char *TokenID);
void Approval(char *from, char *to, char *TokenID);

void PutRecord(char *key, char *value);
void GetRecord(char *key);

_Bool Init()
{
    ZPT_Storage_Put("totalSupply", I64toa(init_amount, 10));
    return true;
}

long long TotalSupply()
{
    return ZPT_Storage_Get("totalSupply");
}

char *OwnerOf(char *TokenID)
{
    return ZPT_Storage_Get(TokenID);
}

_Bool Owns(char *address, char *TokenID)
{
    return ByteArrayCompare(ZPT_Storage_Get(TokenID), address);
}

long long BalanceOf(char *address)
{
    if (ZPT_Storage_Get(address) == 0)
        return 0;
    else
        return ZPT_Storage_Get(address);
}

void IncreaseIndex(char *totalSupply, char *TokenID)
{
    ZPT_Storage_Put(totalSupply, TokenID);
}

long long IncreaseTotalSupply()
{
    long long totalSupply = Atoi64(ZPT_Storage_Get("totalSupply"));
    totalSupply = totalSupply + 1;
    ZPT_Storage_Put("totalSupply", I64toa(totalSupply, 10));
    return I64toa(totalSupply, 10);
}

void Transfer(char *from, char *to, char *TokenID)
{
    ZPT_Storage_Put(TokenID, to);
    long long amount = Atoi64(ZPT_Storage_Get(to));
    if (amount == 0)
    {
        amount = 0;
    }
    amount = amount + 1;
    ZPT_Storage_Put(to, I64toa(amount, 10));
    if (from != 0)
    {
        long long fromAmount = Atoi64(ZPT_Storage_Get(from));
        fromAmount = fromAmount - 1;
        if (fromAmount == 0)
        {
            ZPT_Storage_Delete(from);
        }
        else
            ZPT_Storage_Put(from, I64toa(fromAmount, 10));

        char *ap = "ap";
        char *newTokenID = concat(ap, TokenID);
        ZPT_Storage_Delete(newTokenID);
    }
}

void ApproveInternal(char *to, char *TokenID)
{
    char *ap = "ap";
    char *newTokenID = concat(ap, TokenID);
    ZPT_Storage_Put(newTokenID, to);
}

_Bool ApprovedFor(char *address, char *TokenID)
{
    char *ap = "ap";
    char *newTokenID = concat(ap, TokenID);
    return ByteArrayCompare(ZPT_Storage_Get(newTokenID), address);
}

_Bool Approve(char *from, char *to, char *TokenID)
{
    if (to == "")
        return false;
    if (!Owns(TokenID, from))
        return false;
    if (ZPT_Runtime_CheckWitness(from))
        return false;
    ApproveInternal(to, TokenID);
    ApprovalEvent("Approval", from, to, TokenID);
    ZPT_Runtime_Notify("Approval");
    return true;
}

_Bool TransferFromOwner(char *owner, char *to, char *TokenID)
{
    if (to == "")
        return false;
    if (!Owns(owner, TokenID))
        return false;
    if (ZPT_Runtime_CheckWitness(owner))
        return false;
    Transfer(owner, to, TokenID);
    TransferEvent("Transfer", owner, to, TokenID);
    ZPT_Runtime_Notify("Transfer");
    return true;
}

_Bool TransferFromApproval(char *from, char *to, char *approval, char *TokenID)
{
    if (to == "")
        return false;
    if (!ApprovedFor(approval, TokenID))
        return false;
    if (!Owns(from, TokenID))
        return false;
    if (ZPT_Runtime_CheckWitness(approval))
        return false;
    Transfer(from, to, TokenID);
    TransferEvent("Transfer", from, to, TokenID);
    ZPT_Runtime_Notify("Transfer");
    return true;
}

char *GetHashLish(char *address)
{
    long long totalSupply = Atoi64(ZPT_Storage_Get("totalSupply"));
    char *ForTrim = {"////"};
    char *Result;
    for (long long i = 1; i <= totalSupply; i = i + 1)
    {
        char *Hash = ZPT_Storage_Get(I64toa(i, 10));
        if (ByteArrayCompare(ZPT_Storage_Get(Hash), address))
        {
            Result = concat(Result, Hash);
            Result = concat(Result, ForTrim);
        }
    }
    return Result;
}

_Bool PutRental(char *hash, char *address)
{
    char *rt = "rt";
    char *newHash = concat(rt, hash);
    ZPT_Storage_Put(newHash, address);
    PutEvent("Put", hash, address);
    ZPT_Runtime_Notify("Put");
    return true;
}

char *GetRental(char *hash)
{
    char *rt = "rt";
    char *newHash = concat(rt, hash);
    GetEvent("Get", hash);
    ZPT_Runtime_Notify("Get");
    return ZPT_Storage_Get(newHash);
}

int ByteArrayCompare(char *a1, char *a2)
{
    if (arrayLen(a1) != arrayLen(a2))
    {
        return false;
    }
    for (int i = 0; i < arrayLen(a1); i++)
    {
        if (a1[i] != a2[i])
        {
            return false;
        }
    }
    return true;
}

char *invoke(char *method, char *args)
{

    if (strcmp(method, "init") == 0)
    {
        Init();
        return "init success!";
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
        char *result = ZPT_Storage_Get(p->TokenID);
        if (arrayLen(result) != 0)
            return false;
        Transfer("", p->address, p->TokenID);
        long long totalSupply = Atoi64(IncreaseTotalSupply());
        IncreaseIndex(I64toa(totalSupply, 10), p->TokenID);
        return true;

        // if(arrayLen(args) != 2) return false;
        // char *TokenID = args[0];
        // char *address = args[1];
        // char *result = ZPT_Storage_Get("TokenID");
        // if(arrayLen(result) != 0) return false;
        // Tranfer( "", address, TokenID);
        // long long totalSupply = IncreaseTotalSupply();
        // IncreaseIndex(totalSupply, TokenID);
        // return true;
    }

    if (strcmp(method, "totalSupply") == 0)
    {
        return TotalSupply();
    }

    if (strcmp(method, "ownerOf") == 0)
    {

        // if(arrayLen(args) != 1) return false;
        // char *TokenID = args[0];
        // return OwnerOf(TokenID);
        struct Params
        {
            char *TokenID;
        };
        struct Params *p = (struct Params *)malloc(sizeof(struct Params));
        ZPT_JsonUnmashalInput(p, sizeof(struct Params), args);
        return OwnerOf(p->TokenID);
    }

    if (strcmp(method, "owns") == 0)
    {

        // if(arrayLen(args) != 2) return false;
        // char *TokenID = args[0];
        // char *address = args[1];
        // return Owns(address, TokenID);
        struct Params
        {
            char *TokenID;
            char *address;
        };
        struct Params *p = (struct Params *)malloc(sizeof(struct Params));
        ZPT_JsonUnmashalInput(p, sizeof(struct Params), args);
        char *result = Owns(p->address, p->TokenID);
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
        return BalanceOf(p->address);
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
        return Approve(p->from, p->to, p->TokenID);
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
        return TransferFromOwner(p->from, p->to, p->TokenID);
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
        return TransferFromApproval(p->from, p->to, p->approval, p->TokenID);
    }

    if (strcmp(method, "getHashList") == 0)
    {

        struct Params
        {
            char *address;
        };
        struct Params *p = (struct Params *)malloc(sizeof(struct Params));
        ZPT_JsonUnmashalInput(p, sizeof(struct Params), args);
        return GetHashList(p->address);
    }

    if (strcmp(method, "putRental") == 0)
    {

        struct Params
        {
            char *hash;
            char *address;
        };
        struct Params *p = (struct Params *)malloc(sizeof(struct Params));
        ZPT_JsonUnmashalInput(p, sizeof(struct Params), args);
        return PutRental(p->hash, p->address);
    }

    if (strcmp(method, "getRental") == 0)
    {

        struct Params
        {
            char *hash;
        };
        struct Params *p = (struct Params *)malloc(sizeof(struct Params));
        ZPT_JsonUnmashalInput(p, sizeof(struct Params), args);
        return GetRental(p->hash);
    }

    if (strcmp(method, "addStorage") == 0)
    {

        struct Params
        {
            char *a;
            char *b;
        };
        struct Params *p = (struct Params *)malloc(sizeof(struct Params));
        ZPT_JsonUnmashalInput(p, sizeof(struct Params), args);
        ZPT_Storage_Put(p->a, p->b);
        char *result = ZPT_JsonMashalResult("Done", "string", 1);
        ZPT_Runtime_Notify(result);
        return result;
    }

    if (strcmp(method, "getStorage") == 0)
    {
        struct Params
        {
            char *a;
        };
        struct Params *p = (struct Params *)malloc(sizeof(struct Params));
        ZPT_JsonUnmashalInput(p, sizeof(struct Params), args);
        char *value = ZPT_Storage_Get(p->a);
        char *result = ZPT_JsonMashalResult(value, "string", 1);
        ZPT_Runtime_Notify(result);
        return result;
    }

    if (strcmp(method, "deleteStorage") == 0)
    {

        struct Params
        {
            char *a;
        };
        struct Params *p = (struct Params *)malloc(sizeof(struct Params));
        ZPT_JsonUnmashalInput(p, sizeof(struct Params), args);
        ZPT_Storage_Delete(p->a);
        char *result = ZPT_JsonMashalResult("Done", "string", 1);
        ZPT_Runtime_Notify(result);
        return result;
    }
}