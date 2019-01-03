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

char *type_1 ="higher";
char *type_2= "lower";
char *type_3= "draw";

char *startTime = "1546424400"; 
char *fixedTime = "3600"; //24hour

char * symbol = "TEST";

//contract administration
char * ceoAddress = "ZK4xgvBom4D33F9YAmgg89fJW18iVss3tV";
char * adminAddress = "ZZeme18pBEbnLyDo9RfVvT1xe3nmp6WMQW";
char * rewardAddress = "ZZryCs8o9Z5qLuEpAPx5aY8SHJ2ggRz9c8";

// Pause the contract to stop increaseTotal, decreaseTotal, transfer, transferFrom and approve methods
char * pause(char *address){
    if (Atoi(ZPT_Storage_Get("paused")) == 1)
        return "The contract has already been paused.";
    if (ZPT_Runtime_CheckWitness(ceoAddress) == 0)
        return "You have no permission to pause the contract.";
    ZPT_Storage_Put("paused", "1");
    return "The cantract is paused successfully.";
}

//Unpause the contract which is paused.
char * unPause(char *address){
    if (Atoi(ZPT_Storage_Get("paused")) == 0)
        return "No need to unpause because the contract is not under pause situation.";
    if (ZPT_Runtime_CheckWitness(ceoAddress) == 0)
        return "You have no permission to unpause the contract.";
    ZPT_Storage_Put("paused", "0");
    return "The cantract is unpaused successfully.";
}

//Check if an account is in the database
int isStored(char * key){
    if (arrayLen(ZPT_Storage_Get(key)) == 0)
        return false;
    return true;
}

//Initialize the contract, includes the totalSupply value.
char * init(char * totalSupply, char * adminBalance){
    if (arrayLen(ZPT_Storage_Get("totalSupply")) != 0)
        return "Already initialized.";
    if (ZPT_Runtime_CheckWitness(ceoAddress) == 0)
        return "You have no permission to init.";
    if (Atoi64(totalSupply) <= 0)
        return "The totalsupply can not be less than or equal to 0.";
    if (Atoi(adminBalance) != Atoi(totalSupply))
        return "The balance alloted to admin must be equal to the totalSupply.";
    ZPT_Storage_Put("totalSupply", totalSupply);
    ZPT_Storage_Put(adminAddress, adminBalance);
    ZPT_Storage_Put("paused", "0");
    return "Init success!";
}


char * increaseTotal(char * valueChar){
    if (Atoi(ZPT_Storage_Get("paused")) == 1)
        return "The contract has been paused.";
    if ((ZPT_Runtime_CheckWitness(ceoAddress) == 0) && (ZPT_Runtime_CheckWitness(adminAddress) == 0))
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

char * decreaseTotal(char * valueChar){
    if (Atoi(ZPT_Storage_Get("paused")) == 1)
        return "The contract has been paused.";
    if ((ZPT_Runtime_CheckWitness(ceoAddress) == 0) && (ZPT_Runtime_CheckWitness(adminAddress) == 0))
        return "You have no permission to decrease the totalSupply.";
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


char * balanceOf(char * address){
    char * balance = ZPT_Storage_Get(address);
    if (arrayLen(balance) == 0)
        return "Address is not in our database.";
    return balance;
}

char * transfer(char * fromAddr, char * toAddr, char * amountChar){
    if (Atoi(ZPT_Storage_Get("paused")) == 1)
        return "The contract has been paused.";
    if (arrayLen(ZPT_Storage_Get("totalSupply")) == 0)
        return "Please init first.";
    if (ZPT_Runtime_CheckWitness(fromAddr) == 0)
        return "Inconsistent address.";
    if (isStored(fromAddr) == 0)
        return "Sender address is not in our database.";    
    long long  amount = Atoi64(amountChar);
    if (amount <= 0)
        return "Transfer amount cannot be less than or equal to 0.";
    long long  balance_from = Atoi64(ZPT_Storage_Get(fromAddr));
    if (balance_from < amount)
        return "No sufficient balance.";
    
    balance_from -= amount;
    if (balance_from == 0)
        ZPT_Storage_Delete(fromAddr);
    else
        ZPT_Storage_Put(fromAddr,I64toa(balance_from, 10));
    
    if (isStored(toAddr) == 0)
        ZPT_Storage_Put(toAddr, amountChar);
    else {
        long long  balance_to = Atoi64(ZPT_Storage_Get(toAddr));
        balance_to += amount;
        ZPT_Storage_Put(toAddr,I64toa(balance_to, 10));
    }
    return "Transfer success.";
}


char * _transfer(char * fromAddr, char * toAddr, char * amountChar){
    if (Atoi(ZPT_Storage_Get("paused")) == 1)
        return "The contract has been paused.";
    if (arrayLen(ZPT_Storage_Get("totalSupply")) == 0)
        return "Please init first."; 
    long long  amount = Atoi64(amountChar);
    if (amount <= 0)
        return "Transfer amount cannot be less than or equal to 0.";
    long long  balance_from = Atoi64(ZPT_Storage_Get(fromAddr));
    if (balance_from < amount)
        return "No sufficient balance.";
    
    balance_from -= amount;
    if (balance_from == 0)
        ZPT_Storage_Delete(fromAddr);
    else
        ZPT_Storage_Put(fromAddr,I64toa(balance_from, 10));
    
    if (isStored(toAddr) == 0)
        ZPT_Storage_Put(toAddr, amountChar);
    else {
        long long  balance_to = Atoi64(ZPT_Storage_Get(toAddr));
        balance_to += amount;
        ZPT_Storage_Put(toAddr,I64toa(balance_to, 10));
    }
}


//--------------------------------------------------------------------------------------------

char *Reward() {
    int _amount = Atoi(ZPT_Storage_Get("serial_number"));

    if(strcmp(ZPT_Storage_Get("FinalMatch"), type_1)){
        for (int i = 1; i <= _amount; i = i + 1){
            char *address = ZPT_Storage_Get(Itoa(i));
            char *address_type = strconcat(address, type_1);
            long long  amount = Atoi64( ZPT_Storage_Get(address_type));
            if(amount > 0){
                char *reward = I64toa(amount * Atoi64(ZPT_Storage_Get("HigherOdds")) /1000 , 10);
                _transfer(rewardAddress, address, reward);
            }
        }
        return true;
           
    }else if(strcmp(ZPT_Storage_Get("FinalMatch"), type_2)){
        for (int i = 1; i <= _amount; i = i + 1){
            char *address = ZPT_Storage_Get(Itoa(i));
            char *address_type = strconcat(address, type_2);
            long long  amount = Atoi64( ZPT_Storage_Get(address_type));
            if(amount > 0){
                char *reward = I64toa(amount * Atoi64(ZPT_Storage_Get("LowerOdds")) /1000 , 10);
                _transfer(rewardAddress, address, reward);
                
            }
        }
        return true;
    }else {
        for(int i = 1; i <= _amount; i=i+1){
            char *address = ZPT_Storage_Get(Itoa(i));
            char *address_type = strconcat(address, type_2);
            char * amount =  ZPT_Storage_Get(address_type);              
            _transfer(rewardAddress, address, amount);    
    }  
    return true;
}
}

//清除之前赛况数据
char *DeletePreData(){
  int _amount = Atoi(ZPT_Storage_Get("serial_number"));
  for (int i = 1; i <= _amount; i = i + 1){
      char *address = ZPT_Storage_Get(Itoa(i));
      char *address_type_1 = strconcat(address, type_1);
      char *address_type_2 = strconcat(address, type_2);
      ZPT_Storage_Delete(address_type_1);
      ZPT_Storage_Delete(address_type_2);          
  }
 
  ZPT_Storage_Delete(type_1);
  ZPT_Storage_Delete(type_2);
  return true;
}

//每隔24小时开始设置一次值
char *SetReferenceValue(char *value)
{
    if (arrayLen(ZPT_Storage_Get("FixedTime")) == 0){
        ZPT_Storage_Put("FixedTime", startTime);    //存截止时间
        return "init time";
    }
        if (arrayLen(ZPT_Storage_Get("ReferenceValue")) == 0)
        {
            int time = ZPT_Runtime_GetTime() - Atoi(ZPT_Storage_Get("FixedTime")) ;
            if (30 > time && time > 0)
            {
                ZPT_Storage_Put("ReferenceValue", value);
                ZPT_Storage_Put("serial_number", Itoa(init_amount)); //设置序号
                return "sucess";
            }
            else
                return false;
        }
        else{
            int time = ZPT_Runtime_GetTime() - Atoi(ZPT_Storage_Get("FixedTime")) - Atoi(fixedTime);
                    
            if (30 > time  && time > 0) //add 24 hours compare
            {
                char *referenceValue = ZPT_Storage_Get("ReferenceValue");
                if (Atoi(referenceValue) - Atoi(value) > 0)
                {
                    ZPT_Storage_Put("FinalMatch", type_1);
                }
                else if (Atoi(referenceValue) - Atoi(value) < 0)
                {
                    ZPT_Storage_Put("FinalMatch", type_2);
                }else {
                    ZPT_Storage_Put("FinalMatch", type_3);
                }

                ZPT_Storage_Put("ReferenceValue", value);
                char *time = Itoa(Atoi(ZPT_Storage_Get("FixedTime")) + Atoi(fixedTime));
                ZPT_Storage_Put("FixedTime", time);
                  
                Reward();
                DeletePreData();
                ZPT_Storage_Put("serial_number", Itoa(init_amount)); //放在奖励后执行 

                return "reference  success";
            }  
        } 
}


//Odds
char *BalanceAmount() {
    long long higherAmount = Atoi64(ZPT_Storage_Get(type_1));
    long long lowerAmount = Atoi64(ZPT_Storage_Get(type_2));
    long long sumAmount = higherAmount+lowerAmount;

    if (higherAmount >= lowerAmount){
        if (lowerAmount * 0.97 > higherAmount* 0.03){
            char *higherOdds = I64toa(sumAmount*0.97/higherAmount *1000, 10);           
            char *lowerOdds = I64toa(sumAmount*0.97/lowerAmount *1000, 10);
            ZPT_Storage_Put("HigherOdds", higherOdds);
            ZPT_Storage_Put("LowerOdds", lowerOdds);
        }else {
            char *higherOdds = I64toa((5 * lowerAmount + higherAmount)/higherAmount *1000, 10);
            long long criticalValue = higherAmount *0.03/0.97 ;
            char *lowerOdds = I64toa((37 - lowerAmount/criticalValue *20) *1000, 10);
            ZPT_Storage_Put("HigherOdds", higherOdds);
            ZPT_Storage_Put("LowerOdds", lowerOdds);
        }
    }else {
        if (higherAmount * 0.97 > lowerAmount * 0.03){
            char *higherOdds = I64toa(sumAmount*0.97/higherAmount *1000, 10);          
            char *lowerOdds = I64toa(sumAmount*0.97/lowerAmount *1000, 10);
            ZPT_Storage_Put("HigherOdds", higherOdds);
            ZPT_Storage_Put("LowerOdds", lowerOdds);
        }else {
            char *lowerOdds = I64toa((5 * higherAmount+lowerAmount)/lowerAmount *1000, 10);
            long long criticalValue = lowerAmount *0.03/0.97 ;
            char *higherOdds = I64toa((37 - higherAmount/criticalValue *20) *1000, 10);
            ZPT_Storage_Put("HigherOdds", higherOdds);
            ZPT_Storage_Put("LowerOdds", lowerOdds);
        }
    } 
}

char *HigherAmount(char *address){
  char *address_type = strconcat(address, type_1);  
  char *higherAmount = ZPT_Storage_Get(address_type);
  if (arrayLen(higherAmount) == 0) {
    return "0";
  }else return higherAmount;
}

char *LowerAmount(char *address){
  char *address_type = strconcat(address, type_2);  
  char *lowerAmount = ZPT_Storage_Get(address_type);
  if (arrayLen(lowerAmount) == 0) {
    return "0";
  }else return lowerAmount;
}

char *addTypeAmount(char *type, char *amount){
   if(arrayLen(ZPT_Storage_Get(type)) == 0){
      ZPT_Storage_Put(type, amount);    //type -> amount
   }else{
   long long _amount = Atoi64(ZPT_Storage_Get(type));
   _amount = _amount + Atoi64(amount);
   ZPT_Storage_Put(type, I64toa(_amount,10));
   }
}

char *addSerial_number(char *address){
  char *_amount = Itoa(Atoi(ZPT_Storage_Get("serial_number"))+1);
  ZPT_Storage_Put("serial_number", _amount);
  ZPT_Storage_Put(_amount,address);
}
//betting
char *BuyType(char *address, char *type, char *amount){
    int balance_address = Atoi64(ZPT_Storage_Get(address));
    if ( balance_address < Atoi64(amount)){
        return "balance not enough";
    }
    char *address_type = strconcat(address, type);  
    if (arrayLen(ZPT_Storage_Get(address_type)) == 0){
        if (strcmp(type, type_1))
        {            
            ZPT_Storage_Put(address_type, amount);     //address_type  -> amount
           addTypeAmount(type, amount);
           addSerial_number(address);
            _transfer(address, rewardAddress, amount);        //不加限制
            
        }else {   
            ZPT_Storage_Put(address_type, amount);     //address_type  -> amount
            addTypeAmount(type, amount);
            addSerial_number(address);
            _transfer(address, rewardAddress, amount);
        }
    }else {
        if (strcmp(type, type_1)){
            char *address_type = strconcat(address, type);
            char *sumAmount = I64toa(Atoi64(ZPT_Storage_Get(address_type)) + Atoi64(amount), 10);
            ZPT_Storage_Put(address_type, sumAmount);
            addTypeAmount(type, amount);
            _transfer(address, rewardAddress, amount);
            
        }else {
            char *address_type = strconcat(address, type);
            char *sumAmount = I64toa(Atoi64(ZPT_Storage_Get(address_type)) + Atoi64(amount), 10);
            ZPT_Storage_Put(address_type, sumAmount);
            addTypeAmount(type, amount);
            _transfer(address, rewardAddress, amount);  
        }
    }
    BalanceAmount();
    return true;
}

char *HigherOdds(){
    return ZPT_Storage_Get("HigherOdds");
}

char *LowerOdds(){
    return ZPT_Storage_Get("LowerOdds");
}


char *invoke(char *method, char *args)
{
   
    if (strcmp(method, "getSymbol")==0){
        ZPT_Runtime_Notify(symbol);
        return symbol;
    }
    
    if (strcmp(method, "getCeoAddress")==0){
        ZPT_Runtime_Notify(ceoAddress);
        return ceoAddress;
    }
    
    if (strcmp(method, "getAdminAddress")==0){
        ZPT_Runtime_Notify(adminAddress);
        return adminAddress;
    }
    
    if (strcmp(method ,"pause")==0){
        
        struct Params{
            char * address;
        };
        struct Params *p = (struct Params *)malloc(sizeof(struct Params));
        ZPT_JsonUnmashalInput(p,sizeof(struct Params),args);
        char *result = pause(p->address);
        ZPT_Runtime_Notify(result);
        return result;
    }
    
    if (strcmp(method ,"unpause")==0){
    
        struct Params{
            char * address;
        };
        struct Params *p = (struct Params *)malloc(sizeof(struct Params));
        ZPT_JsonUnmashalInput(p,sizeof(struct Params),args);
        char *result = unPause(p->address);
        ZPT_Runtime_Notify(result);
        return result;
    }
    
    if (strcmp(method ,"init")==0){
        
        struct Params{
            char * totalSupply;
            char * adminAddress;
        };
        struct Params *p = (struct Params *)malloc(sizeof(struct Params));
        ZPT_JsonUnmashalInput(p,sizeof(struct Params),args);
        char *result = init(p->totalSupply, p->adminAddress);
        ZPT_Runtime_Notify(result);
        return result;
    }
    
    if (strcmp(method, "totalSupply")==0){        //?????????
        char * total_supply = ZPT_Storage_Get("totalSupply");
        char *result = ZPT_JsonMashalResult(total_supply, "string", 1);
        ZPT_Runtime_Notify(result);
        return result;
    }
    
    
    if (strcmp(method, "increaseTotal")==0){
        
        struct Params{
            char * value;
        };
        struct Params *p = (struct Params *)malloc(sizeof(struct Params));
        ZPT_JsonUnmashalInput(p,sizeof(struct Params),args);
        char *result = increaseTotal(p->value);
        ZPT_Runtime_Notify(result);
        return result;
    }
    
    if (strcmp(method, "decreaseTotal")==0){
        
        struct Params{
            char * value;
        };
        struct Params *p = (struct Params *)malloc(sizeof(struct Params));
        ZPT_JsonUnmashalInput(p,sizeof(struct Params),args);
        char *result = decreaseTotal(p->value);
        ZPT_Runtime_Notify(result);
        return result;
    }
    
    if (strcmp(method, "balanceOf")==0){
       
        struct Params{
            char * address;
        };
        struct Params *p = (struct Params *)malloc(sizeof(struct Params));
        ZPT_JsonUnmashalInput(p,sizeof(struct Params),args);
        char *result = balanceOf(p->address);
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
        char *result = transfer(p->from, p->to, p->amount);
        ZPT_Runtime_Notify(result);
        return result;
    }
    
      
    //////////////////////////////////////////////////////////////////////
      if (strcmp(method, "getReferenceValue") == 0)
    {
        char *Value = ZPT_Storage_Get("ReferenceValue");
        char *result = ZPT_JsonMashalResult(Value, "string", 1);
        ZPT_Runtime_Notify(result);
        return result;
    }
    
    if (strcmp(method, "setReferenceValue") == 0)
    {
        struct Params
        {
            char *value;
        };
        struct Params *p = (struct Params *)malloc(sizeof(struct Params));
        ZPT_JsonUnmashalInput(p, sizeof(struct Params), args);
        char *Value = SetReferenceValue(p->value);
        char *result = ZPT_JsonMashalResult(Value, "string", 1);
        ZPT_Runtime_Notify(result);
        return result;
    }

    if (strcmp(method, "buyType") == 0)
    {
        struct Params
        {
            char *address;
            char *type;
            char *amount;
        };
        struct Params *p = (struct Params *)malloc(sizeof(struct Params));
        ZPT_JsonUnmashalInput(p, sizeof(struct Params), args);
        char *value = BuyType(p->address, p->type, p->amount);
        char *result = ZPT_JsonMashalResult(value, "string", 1);
        ZPT_Runtime_Notify(result);
        return result;
    }

    if (strcmp(method, "higherOdds") == 0)
    {
        char *value = HigherOdds();
        char *result = ZPT_JsonMashalResult(value, "string", 1);
        ZPT_Runtime_Notify(result);
        return result;
    }

    if (strcmp(method, "lowerOdds") == 0)
    {
        char *value = LowerOdds();
        char *result = ZPT_JsonMashalResult(value, "string", 1);
        ZPT_Runtime_Notify(result);
        return result;
    }
    
    if (strcmp(method, "higherAmount") == 0)
    {
      struct Params
        {
            char *address;
        };
        struct Params *p = (struct Params *)malloc(sizeof(struct Params));
        ZPT_JsonUnmashalInput(p, sizeof(struct Params), args);
        char *value = HigherAmount(p->address);
        char *result = ZPT_JsonMashalResult(value, "string", 1);
        ZPT_Runtime_Notify(result);
        return result;
    }
    
    if (strcmp(method, "lowerAmount") == 0)
    {
      struct Params
        {
            char *address;
        };
        struct Params *p = (struct Params *)malloc(sizeof(struct Params));
        ZPT_JsonUnmashalInput(p, sizeof(struct Params), args);
        char *value = LowerAmount(p->address);
        char *result = ZPT_JsonMashalResult(value, "string", 1);
        ZPT_Runtime_Notify(result);
        return result;
    }
    
     if (strcmp(method, "type_1Amount") == 0)
    {
      
        char *value = ZPT_Storage_Get(type_1);
        char *result = ZPT_JsonMashalResult(value, "string", 1);
        ZPT_Runtime_Notify(result);
        return result;
    }
    
    if (strcmp(method, "type_2Amount") == 0)
    {
      
        char *value = ZPT_Storage_Get(type_2);
        char *result = ZPT_JsonMashalResult(value, "string", 1);
        ZPT_Runtime_Notify(result);
        return result; 
    }
    
}