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

char *name = "Planet";

char *mark = "\"";
char *comma = "\,";
char *firstbracket = "\[";
char *lastbracket = "\]";

char *actionMarshal(char *action, char *str_1, char *str_2, char *str_3)
{
    char *str1 = firstbracket;
    char *str2;

    str2 = strconcat(str1, mark);
    str1 = strconcat(str2, action);
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
    str2 = strconcat(str1, mark);

    str1 = strconcat(str2, lastbracket);

    return str1;
}

char *Init()
{
    if (arrayLen(ZPT_Storage_Get("totalSupply")) != 0)
    {
        return "30001";
    }
    if (arrayLen(ZPT_Storage_Get(admin)) != 0)
        return "30005";
    ZPT_Storage_Put(admin, adminAddress);
    ZPT_Storage_Put("paused", init_type);
    ZPT_Storage_Put("totalSupply", Itoa(init_amount));
    return "init success!";
}

char *UpgradePlanet(char *TokenID, char *Level)
{
    char *l = "l.";
    char *newTokenID_L = strconcat(l, TokenID);
    if (arrayLen(ZPT_Storage_Get(newTokenID_L)) == 0)
    {
        return "30002";
    }
    int level = Atoi(ZPT_Storage_Get(newTokenID_L)) + Atoi(Level);
    ZPT_Storage_Put(newTokenID_L, Itoa(level));
    return "the planet level is update";
}

char *GetType(char *TokenID)
{
    if (arrayLen(ZPT_Storage_Get(TokenID)) == 0)
        return "30002";
    char *T = "T.";
    char *newTokenID_T = strconcat(T, TokenID);
    return ZPT_Storage_Get(newTokenID_T);
}

char *ChangeType(char *TokenID)
{
    if (arrayLen(ZPT_Storage_Get(TokenID)) == 0)
        return "30002";
    char *T = "T.";
    char *newTokenID_T = strconcat(T, TokenID);
    char *type = ZPT_Storage_Get(newTokenID_T);
    if (Atoi(type) == 0)
    {
        int newtype = Atoi(type) + 1;
        ZPT_Storage_Put(newTokenID_T, Itoa(newtype));
        return "change type success";
    }
    else
    {
        int newtype = Atoi(type) - 1;
        ZPT_Storage_Put(newTokenID_T, Itoa(newtype));
        return "change type success";
    }
}

char *GetMetadata(char *TokenID)
{
    if (arrayLen(ZPT_Storage_Get(TokenID)) == 0)
        return "30002";
    char *u = "u.";
    char *l = "l.";
    char *T = "T.";
    char *p = ".";
    char *newTokenID_T = strconcat(T, TokenID);
    char *newTokenID_U = strconcat(u, TokenID);
    char *newTokenID_L = strconcat(l, TokenID);
    char *type = ZPT_Storage_Get(newTokenID_T);
    char *Uniqueness = ZPT_Storage_Get(newTokenID_U);
    char *Level = ZPT_Storage_Get(newTokenID_L);
    char *metadata = strconcat(type, p);
    metadata = strconcat(metadata, Uniqueness);
    metadata = strconcat(metadata, p);
    metadata = strconcat(metadata, Level);
    return metadata;
}

char *TotalSupply()
{
    if (arrayLen(ZPT_Storage_Get("totalSupply")) == 0)
        return "30003";
    return ZPT_Storage_Get("totalSupply");
}

char *OwnerOf(char *TokenID)
{
    if (arrayLen(ZPT_Storage_Get(TokenID)) != 34)
        return "31001";
    return ZPT_Storage_Get(TokenID);
}

char *Owns(char *TokenID, char *address)
{
    if (arrayLen(ZPT_Storage_Get(TokenID)) == 0)
        return "30002";
    if (arrayLen(ZPT_Storage_Get(address)) == 0)
        return "31001";
    return Itoa(strcmp(ZPT_Storage_Get(TokenID), address));
}

char *BalanceOf(char *address)
{
    if (arrayLen(address) != 34)
        return false;
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

char *GetPlanetURL()
{
    if (arrayLen(ZPT_Storage_Get("PlanetURL")) == 0)
    {
        return "31001";
    }
    char *URL = ZPT_Storage_Get("PlanetURL");
    return URL;
}

char *SetPlanetURL(char *URL)
{
    if (arrayLen(URL) == 0)
    {
        return "31001";
    }
    ZPT_Storage_Put("PlanetURL", URL);
    return "set URL success";
}

char *Create(char *TokenID, char *address, char *Uniqueness)
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
    char *u = "u.";
    char *l = "l.";
    char *T = "T.";
    char *newTokenID_T = strconcat(T, TokenID);
    char *newTokenID_U = strconcat(u, TokenID);
    char *newTokenID_L = strconcat(l, TokenID);
    ZPT_Storage_Put(newTokenID_U, Uniqueness);
    ZPT_Storage_Put(newTokenID_L, Itoa(init_amount));
    ZPT_Storage_Put(newTokenID_T, init_type);
    return true;
}

char *PutChain(char *TokenID, char *address, char *Uniqueness, char *Level, char *type)
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
    char *u = "u.";
    char *l = "l.";
    char *T = "T.";
    char *newTokenID_T = strconcat(T, TokenID);
    char *newTokenID_U = strconcat(u, TokenID);
    char *newTokenID_L = strconcat(l, TokenID);
    ZPT_Storage_Put(newTokenID_U, Uniqueness);
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
    if (arrayLen(ZPT_Storage_Get(TokenID)) == 0)
        return "30002";
    if (arrayLen(address) != 34)
        return "31001";
    char *ap = "ap.";
    char *newTokenID = strconcat(ap, TokenID);
    return Itoa(strcmp(ZPT_Storage_Get(newTokenID), address));
}

char *Paused(char *address)
{
    if (ZPT_Runtime_CheckWitness(address) == 0)
        return "31004";
    if (strcmp(address, CEOAddress) != 0)
        return "31001";
    ZPT_Storage_Put("paused", Itoa(init_amount));
    return "Set paused contract success!";
}

char *Unpaused(char *address)
{
    if (ZPT_Runtime_CheckWitness(address) == 0)
        return "31004";
    if (strcmp(address, CEOAddress) != 0)
        return "31001";
    ZPT_Storage_Put("paused", init_type);
    return "Set Unpaused contract success!";
}

char *Freeze(char *address, char *TokenID)
{
    char *ma = "ma.";
    char *newTokenID = strconcat(ma, TokenID);
    ZPT_Storage_Put(newTokenID, address);
}

char *UnFreeze(char *address, char *TokenID)
{
    if (ZPT_Runtime_CheckWitness(address) == 0)
        return "31004";
    char *ma = "ma.";
    char *newTokenID = strconcat(ma, TokenID);
    if (arrayLen(ZPT_Storage_Get(newTokenID)) == 0){
        return "31001";
    }
    char *ap = "ap.";
    char *newTokenID_a = strconcat(ap, TokenID);
    ZPT_Storage_Delete(newTokenID);
    ZPT_Storage_Delete(newTokenID_a);
    return "Your asset UnFreeze success!";
}

char *Approve(char *from, char *to, char *TokenID)
{
    char *ma = "ma.";
    char *newTokenID = strconcat(ma, TokenID);
    if (arrayLen(ZPT_Storage_Get(newTokenID)) != 0)
    {
        return "31006";
    }
    if (to == "")
        return "31002";
    if (Atoi(Owns(TokenID, from)) != 0)
        return "31003";
    if (ZPT_Runtime_CheckWitness(from) == 0)
        return "31004";
    if (strcmp(to, adminAddress) == 0)
    {
        Freeze(to, TokenID);
        ApproveInternal(to, TokenID);
        return true;
    }
    ApproveInternal(to, TokenID);
    return true;
}

char *TransferFromOwner(char *owner, char *to, char *TokenID)
{
    if (to == "")
        return "31002";
    if (Atoi(Owns(TokenID, owner)) != 0)
        return "31003";
    if (Atoi(ApprovedFor(to, TokenID)) == 1)
        return "31005";
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
    if (Atoi(Owns(TokenID, from)) != 0)
        return "31003";
    if (ZPT_Runtime_CheckWitness(approval) == 0)
        return "31004";
    Transfer(from, to, TokenID);
    return true;
}

char *ApproveByAdmin(char *to, char *TokenID)
{
    if (to == "")
        return "31002";
    if (Atoi(Owns(TokenID, adminAddress)) != 0)
        return "31003";
    ApproveInternal(to, TokenID);
    return true;
}

char *TransferFromAdmin(char *to, char *TokenID)
{
    if (to == "")
        return "31002";
    if (Atoi(Owns(TokenID, adminAddress)) != 0)
        return "31003";
    if (Atoi(ApprovedFor(to, TokenID)) == 1)
        return "31005";
    Transfer(adminAddress, to, TokenID);
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
    char *unpaused = ZPT_Storage_Get("paused");

    if (strcmp(method, "init") == 0)
    {
        char *value = Init();
        char *result = ZPT_JsonMashalResult(value, "string", 1);
        ZPT_Runtime_Notify(result);
        return result;
    }

    if (strcmp(method, "getName") == 0)
    {
        char *result = ZPT_JsonMashalResult(name, "string", 1);
        ZPT_Runtime_Notify(result);
        return name;
    }

    if (strcmp(method, "paused") == 0)
    {

        struct Params
        {
            char *address;
        };
        struct Params *p = (struct Params *)malloc(sizeof(struct Params));
        ZPT_JsonUnmashalInput(p, sizeof(struct Params), args);
        char *value = Paused(p->address);
        char *result = ZPT_JsonMashalResult(value, "string", 1);
        ZPT_Runtime_Notify(result);
        return value;
    }

    if (strcmp(method, "unpaused") == 0)
    {

        struct Params
        {
            char *address;
        };
        struct Params *p = (struct Params *)malloc(sizeof(struct Params));
        ZPT_JsonUnmashalInput(p, sizeof(struct Params), args);
        char *value = Unpaused(p->address);
        char *result = ZPT_JsonMashalResult(value, "string", 1);
        ZPT_Runtime_Notify(result);
        return value;
    }

    if (strcmp(method, "totalSupply") == 0)
    {
        char *value = TotalSupply();
        char *result = ZPT_JsonMashalResult(value, "string", 1);
        ZPT_Runtime_Notify(result);
        return value;
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
        return value;
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
        return value;
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
        return value;
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
        return value;
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
        return value;
    }

    if (strcmp(method, "getMetadata") == 0)
    {

        struct Params
        {
            char *TokenID;
        };
        struct Params *p = (struct Params *)malloc(sizeof(struct Params));
        ZPT_JsonUnmashalInput(p, sizeof(struct Params), args);
        char *value = GetMetadata(p->TokenID);
        char *result = ZPT_JsonMashalResult(value, "string", 1);
        ZPT_Runtime_Notify(result);
        return value;
    }

    if (strcmp(method, "getPlanetURL") == 0)
    {
        char *value = GetPlanetURL();
        char *result = ZPT_JsonMashalResult(value, "string", 1);
        ZPT_Runtime_Notify(result);
        return value;
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
        return value;
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
        return value;
    }

    if (strcmp(method, "getAdmin") == 0)
    {
        char *value = ZPT_Storage_Get(admin);
        char *result = ZPT_JsonMashalResult(value, "string", 1);
        ZPT_Runtime_Notify(result);
        return value;
    }

    if (strcmp(unpaused, init_type) == 0)
    {
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
                json = actionMarshal("transferFromApproval", p->from, p->to, p->TokenID);
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
                    char *Uniqueness;
                };
                struct Params *p = (struct Params *)malloc(sizeof(struct Params));
                ZPT_JsonUnmashalInput(p, sizeof(struct Params), args);

                char *value = Create(p->TokenID, p->Address, p->Uniqueness);
                char *json;
                if (strcmp(value, "1") == 0)
                {
                    json = actionMarshal("create", "", p->Address, p->TokenID);
                }
                else
                    json = value;
                ZPT_Runtime_Notify(json);
                return json;
            }

            if (strcmp(method, "upgradePlanet") == 0)
            {

                struct Params
                {
                    char *TokenID;
                    char *Level;
                };
                struct Params *p = (struct Params *)malloc(sizeof(struct Params));
                ZPT_JsonUnmashalInput(p, sizeof(struct Params), args);

                char *value = UpgradePlanet(p->TokenID, p->Level);
                char *result = ZPT_JsonMashalResult(value, "string", 1);
                ZPT_Runtime_Notify(result);
                return result;
            }

            if (strcmp(method, "unFreeze") == 0)
            {

                struct Params
                {
                    char *address;
                    char *TokenID;
                };
                struct Params *p = (struct Params *)malloc(sizeof(struct Params));
                ZPT_JsonUnmashalInput(p, sizeof(struct Params), args);
                char *value = UnFreeze(p->address, p->TokenID);
                char *result = ZPT_JsonMashalResult(value, "string", 1);
                ZPT_Runtime_Notify(value);
                return result;
            }

            if (strcmp(method, "approveByAdmin") == 0)
            {

                struct Params
                {
                    char *to;
                    char *TokenID;
                };
                struct Params *p = (struct Params *)malloc(sizeof(struct Params));
                ZPT_JsonUnmashalInput(p, sizeof(struct Params), args);
                char *value = ApproveByAdmin(p->to, p->TokenID);
                char *json;
                if (strcmp(value, "1") == 0)
                {
                    json = actionMarshal("approveByAdmin", adminAddress, p->to, p->TokenID);
                }
                else
                    json = value;
                ZPT_Runtime_Notify(json);
                return json;
            }

            if (strcmp(method, "transferFromAdmin") == 0)
            {

                struct Params
                {
                    char *to;
                    char *TokenID;
                };
                struct Params *p = (struct Params *)malloc(sizeof(struct Params));
                ZPT_JsonUnmashalInput(p, sizeof(struct Params), args);
                char *value = TransferFromAdmin(p->to, p->TokenID);
                char *json;
                if (strcmp(value, "1") == 0)
                {
                    json = actionMarshal("transferFromAdmin", adminAddress, p->to, p->TokenID);
                }
                else
                    json = value;
                ZPT_Runtime_Notify(json);
                return json;
            }

            if (strcmp(method, "putChain") == 0)
            {

                struct Params
                {
                    char *TokenID;
                    char *Address;
                    char *Uniqueness;
                    char *Level;
                    char *type;
                };
                struct Params *p = (struct Params *)malloc(sizeof(struct Params));
                ZPT_JsonUnmashalInput(p, sizeof(struct Params), args);
                char *value = PutChain(p->TokenID, p->Address, p->Uniqueness, p->Level, p->type);
                char *json;
                if (strcmp(value, "1") == 0)
                {
                    json = actionMarshal("PutChain", "", p->Address, p->TokenID);
                }
                else
                    json = value;
                ZPT_Runtime_Notify(json);
                return json;
            }

            if (strcmp(method, "setPlanetURL") == 0)
            {

                struct Params
                {
                    char *URL;
                };
                struct Params *p = (struct Params *)malloc(sizeof(struct Params));
                ZPT_JsonUnmashalInput(p, sizeof(struct Params), args);
                char *value = SetPlanetURL(p->URL);
                char *result = ZPT_JsonMashalResult(value, "string", 1);
                ZPT_Runtime_Notify(result);
                return result;
            }

            if (strcmp(method, "changeType") == 0)
            {

                struct Params
                {
                    char *TokenID;
                };
                struct Params *p = (struct Params *)malloc(sizeof(struct Params));
                ZPT_JsonUnmashalInput(p, sizeof(struct Params), args);
                char *value = ChangeType(p->TokenID);
                char *result = ZPT_JsonMashalResult(value, "string", 1);
                ZPT_Runtime_Notify(result);
                return result;
            }
        }
    }
}