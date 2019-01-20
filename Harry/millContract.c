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
char *init_type = "1";

char *admin = "admin";
char *adminAddress = "ZK4xgvBom4D33F9YAmgg89fJW18iVss3tV";
char *CEOAddress = "ZK4xgvBom4D33F9YAmgg89fJW18iVss3tV";

char *mark = "\"";
char *comma = "\,";

char *actionMarshal(char *action, char *str_1, char *str_2, char *str_3)
{
    char *str1 = action;
    char *str2;

    str2 = strconcat(str1, mark);
    str1 = strconcat(str2, comma);

    str2 = strconcat(str1, mark);
    str1 = strconcat(str2, str_1);
    str2 = strconcat(str1, mark);
    str1 = strconcat(str2, comma);

    str2 = strconcat(str1, mark);
    str1 = strconcat(str2, str_2);
    str2 = strconcat(str1, mark);
    str1 = strconcat(str2, comma);

    str2 = strconcat(str1, mark);
    str1 = strconcat(str2, str_3);

    return str1;
}

char *Init()
{
    if (arrayLen(ZPT_Storage_Get("totalSupply")) != 0)
    {
        return "30001";
    }
    if (arrayLen(ZPT_Storage_Get(admin)) != 0)
        return false;
    ZPT_Storage_Put(admin, adminAddress);
    ZPT_Storage_Put("totalSupply", Itoa(init_amount));
    return "init success!";
}

char *UpgradeMill(char *TokenID, char *Level)
{
    char *l = "l.";
    char *newTokenID_L = strconcat(l, TokenID);
    if (arrayLen(ZPT_Storage_Get(newTokenID_L)) == 0)
    {
        return "The planet is an asteroid!";
    }
    int level = Atoi(ZPT_Storage_Get(newTokenID_L)) + Atoi(Level);
    ZPT_Storage_Put(newTokenID_L, Itoa(level));
    return "the planet level is update";
}

char *GetType(char *TokenID)
{
    char *T = "T.";
    char *newTokenID_T = strconcat(T, TokenID);
    return ZPT_Storage_Get(newTokenID_T);
}

char *GetMillMetadata(char *TokenID)
{
    if (arrayLen(ZPT_Storage_Get(TokenID)) == 0)
        return "30002";
    char *l = "l.";
    char *T = "T.";
    char *s = ".";
    char *newTokenID_T = strconcat(T, TokenID);
    char *newTokenID_L = strconcat(l, TokenID);
    char *type = ZPT_Storage_Get(newTokenID_T);
    char *Level = ZPT_Storage_Get(newTokenID_L);
    char *metadata = strconcat(type, s);
    metadata = strconcat(metadata, Level);
    return metadata;
}

char *TotalSupply()
{
    if (arrayLen(ZPT_Storage_Get("totalSupply")) == 0)
    {
        return "30003";
    }
    return ZPT_Storage_Get("totalSupply");
}

char *OwnerOf(char *TokenID)
{
    return ZPT_Storage_Get(TokenID);
}

char *Owns(char *TokenID, char *address)
{
    return Itoa(strcmp(ZPT_Storage_Get(TokenID), address));
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

char *Create(char *TokenID, char *address, char *type)
{
    if (arrayLen(ZPT_Storage_Get("totalSupply")) == 0)
    {
        return "You need `init!";
    }
    char *Result = ZPT_Storage_Get(TokenID);
    if (arrayLen(Result) != 0)
        return "30004";
    Transfer("", address, TokenID);
    int totalSupply = Atoi(IncreaseTotalSupply());
    IncreaseIndex(Itoa(totalSupply), TokenID);
    char *l = "l.";
    char *T = "T.";
    char *newTokenID_T = strconcat(T, TokenID);
    char *newTokenID_L = strconcat(l, TokenID);
    ZPT_Storage_Put(newTokenID_L, Itoa(init_amount));
    ZPT_Storage_Put(newTokenID_T, init_type);
    return true;
}

char *MillUpChain(char *TokenID, char *address, char *Level,char *type)
{
    if (arrayLen(ZPT_Storage_Get("totalSupply")) == 0)
    {
        return "30003";
    }
    char *Result = ZPT_Storage_Get(TokenID);
    if (arrayLen(Result) != 0)
        return "30004";
    Transfer("", address, TokenID);
    int totalSupply = Atoi(IncreaseTotalSupply());
    IncreaseIndex(Itoa(totalSupply), TokenID);
    char *l = "l.";
    char *T = "T.";
    char *newTokenID_T = strconcat(T, TokenID);
    char *newTokenID_L = strconcat(l, TokenID);
    ZPT_Storage_Put(newTokenID_L, Level);
    ZPT_Storage_Put(newTokenID_T, type);
    return true;
}

void ApproveInternal(char *to, char *TokenID)
{
    char *ap = "ap.";
    char *newTokenID = strconcat(ap, TokenID);
    ZPT_Storage_Put(newTokenID, to);
}

char *ApproverOf(char *TokenID)
{
    char *ap = "ap.";
    char *newTokenID = strconcat(ap, TokenID);
    if (arrayLen(ZPT_Storage_Get(newTokenID)) == 0)
    {
        return "31001";
    }
    return ZPT_Storage_Get(newTokenID);
}

char *ApprovedFor(char *address, char *TokenID)
{
    char *ap = "ap.";
    char *newTokenID = strconcat(ap, TokenID);
    return Itoa(strcmp(ZPT_Storage_Get(newTokenID), address));
}

char *Approve(char *from, char *to, char *TokenID)
{
    if (to == "")
        return "31002";
    if (Atoi(Owns(TokenID, from)) == 1)
        return "31003";
    if (ZPT_Runtime_CheckWitness(from) == 0)
        return "31004";
    ApproveInternal(to, TokenID);
    return true;
}

char *TransferFromOwner(char *owner, char *to, char *TokenID)
{
    if (to == "")
        return "31002";
    if (Atoi(Owns(TokenID, owner)) == 1)
        return "31003";
    if (ZPT_Runtime_CheckWitness(owner) == 0)
        return "31004";
    Transfer(owner, to, TokenID);
    return true;
}

char *TransferFromApproval(char *from, char *to, char *approval, char *TokenID)
{
    if (to == "")
        return "31002";
    if (Atoi(ApprovedFor(approval, TokenID)) == 1)
        return "31005";
    if (Atoi(Owns(TokenID, from)) == 1)
        return "31003";
    if (ZPT_Runtime_CheckWitness(approval) == 0)
        return "31004";
    Transfer(from, to, TokenID);
    return true;
}

char *GetHashList(char *address)
{
    int totalSupply = Atoi(ZPT_Storage_Get("totalSupply"));
    char *ForTrim = {"."};
    char *Hash = {""};
    char *Result = {""};
    for (int i = 1; i <= totalSupply; i = i + 1)
    {
        Hash = ZPT_Storage_Get(Itoa(i));
        if (strcmp(ZPT_Storage_Get(Hash), address) == 0)
        {
            Result = strconcat(Result, Hash);
            Result = strconcat(Result, ForTrim);
        }
    }
    return Result;
}

char *invoke(char *method, char *args)
{
    if (strcmp(method, "init") == 0)
    {
        char *value = Init();
        char *result = ZPT_JsonMashalResult(value, "string", 1);
        ZPT_Runtime_Notify(result);
        return result;
    }
    if (strcmp(method, "totalSupply") == 0)
    {
        char *value = TotalSupply();
        char *result = ZPT_JsonMashalResult(value, "string", 1);
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
        char *result = ZPT_JsonMashalResult(value, "string", 1);
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
        char *result = ZPT_JsonMashalResult(value, "string", 1);
        ZPT_Runtime_Notify(result);
        return result;
    }

    if (strcmp(method, "approverOf") == 0)
    {

        struct Params
        {
            char *TokenID;
        };
        struct Params *p = (struct Params *)malloc(sizeof(struct Params));
        ZPT_JsonUnmashalInput(p, sizeof(struct Params), args);
        char *value = ApproverOf(p->TokenID);
        char *result = ZPT_JsonMashalResult(value, "string", 1);
        ZPT_Runtime_Notify(result);
        return result;
    }

    if (strcmp(method, "approvedFor") == 0)
    {

        struct Params
        {
            char *TokenID;
            char *address;
        };
        struct Params *p = (struct Params *)malloc(sizeof(struct Params));
        ZPT_JsonUnmashalInput(p, sizeof(struct Params), args);
        char *value = ApprovedFor(p->address, p->TokenID);
        char *result = ZPT_JsonMashalResult(value, "string", 1);
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
        char *result = ZPT_JsonMashalResult(value, "string", 1);
        ZPT_Runtime_Notify(result);
        return result;
    }

    if (strcmp(method, "getMillMetadata") == 0)
    {

        struct Params
        {
            char *TokenID;
        };
        struct Params *p = (struct Params *)malloc(sizeof(struct Params));
        ZPT_JsonUnmashalInput(p, sizeof(struct Params), args);
        char *value = GetMillMetadata(p->TokenID);
        char *result = ZPT_JsonMashalResult(value, "string", 1);
        ZPT_Runtime_Notify(result);
        return result;
    }

    if (strcmp(method, "getType") == 0)
    {

        struct Params
        {
            char *TokenID;
        };
        struct Params *p = (struct Params *)malloc(sizeof(struct Params));
        ZPT_JsonUnmashalInput(p, sizeof(struct Params), args);
        char *value = GetType(p->TokenID);
        char *result = ZPT_JsonMashalResult(value, "string", 1);
        ZPT_Runtime_Notify(result);
        return result;
    }

    if (strcmp(method, "getAdmin") == 0)
    {
        return ZPT_Storage_Get(admin);
    }

    if (strcmp(method, "changeAdmin") == 0)
    {
        if (ZPT_Runtime_CheckWitness(ZPT_Storage_Get(admin)) == 0)
            return false;
        struct Params
        {
            char *address;
        };
        struct Params *p = (struct Params *)malloc(sizeof(struct Params));
        ZPT_JsonUnmashalInput(p, sizeof(struct Params), args);
        ZPT_Storage_Put(admin, p->address);
        char *result = ZPT_JsonMashalResult(p->address, "string", 1);
        ZPT_Runtime_Notify(result);
        return result;
    }

    if (strcmp(method, "setAdmin") == 0)
    {
        if (ZPT_Runtime_CheckWitness(CEOAddress) == 0)
            return false;
        struct Params
        {
            char *address;
        };
        struct Params *p = (struct Params *)malloc(sizeof(struct Params));
        ZPT_JsonUnmashalInput(p, sizeof(struct Params), args);
        ZPT_Storage_Put(admin, p->address);
        char *result = ZPT_JsonMashalResult(p->address, "string", 1);
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
        char *result = ZPT_JsonMashalResult(value, "string", 1);
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
        char *json;
        if (strcmp(value, "1") == 0)
        {
            json = actionMarshal("approve", p->from, p->to, p->TokenID);
        }
        else
            json = value;
        ZPT_Runtime_Notify(json);
        return json;
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
        char *json;
        if (strcmp(value, "1") == 0)
        {
            json = actionMarshal("transfer", p->from, p->to, p->TokenID);
        }
        else
            json = value;
        ZPT_Runtime_Notify(json);
        return json;
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
        char *json;
        if (strcmp(value, "1") == 0)
        {
            json = actionMarshal("transferFromApproval", p->approval, p->to, p->TokenID);
        }
        else
            json = value;
        ZPT_Runtime_Notify(json);
        return json;
    }

    if (ZPT_Runtime_CheckWitness(ZPT_Storage_Get(admin)) == 1)
    {

        if (strcmp(method, "create") == 0)
        {

            struct Params
            {
                char *TokenID;
                char *Address;
                char *type;
            };
            struct Params *p = (struct Params *)malloc(sizeof(struct Params));
            ZPT_JsonUnmashalInput(p, sizeof(struct Params), args);

            char *value = Create(p->TokenID, p->Address, p->type);
            char *json;
            if (strcmp(value, "1") == 0)
            {
                json = actionMarshal("create", p->TokenID, p->Address, "");
            }
            else
                json = value;
            ZPT_Runtime_Notify(json);
            return json;
        }

        if (strcmp(method, "upgradeMill") == 0)
        {

            struct Params
            {
                char *TokenID;
                char *Level;
            };
            struct Params *p = (struct Params *)malloc(sizeof(struct Params));
            ZPT_JsonUnmashalInput(p, sizeof(struct Params), args);

            char *value = UpgradeMill(p->TokenID, p->Level);
            char *result = ZPT_JsonMashalResult(value, "string", 1);
            ZPT_Runtime_Notify(result);
            return result;
        }

        if (strcmp(method, "millUpChain") == 0)
        {

            struct Params
            {
                char *TokenID;
                char *Address;
                char *Level;
                char *type;
            };
            struct Params *p = (struct Params *)malloc(sizeof(struct Params));
            ZPT_JsonUnmashalInput(p, sizeof(struct Params), args);
            char *value = MillUpChain(p->TokenID, p->Address, p->Level, p->type);
            char *json;
            if (strcmp(value, "1") == 0)
            {
                json = actionMarshal("create", p->TokenID, p->Address, "");
            }
            else
                json = value;
            ZPT_Runtime_Notify(json);
            return json;
        }
    }
}