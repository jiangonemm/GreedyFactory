
## 1 公私钥和地址
账户是基于公私钥创建的，地址是公钥转换而来。
#### 旧地址转换为新地址：
```javascript
String oldAddress = "AVBokCnLS9NxUjeXTdL8BMsgN41bwtK89J";     //A开头的旧地址
String newAddress = ConvertAddress.convertAddress(oldAddress);//Z开头的新地址
```
### 1.1 公私钥存储
公私钥存储可以存储在数据库中，也可以根据钱包规范存储在文件中。

#### 1.1.1 自己存储： 

自己存储，是指账户信息保存在用户数据库或其他地方，而不存储在遵循钱包规范的文件中。
####随机创建账号：
```javascript
com.github.zeepin.account.Account acct = new com.github.zeepin.account.Account(zptSdk.defaultSignScheme);
acct.serializePrivateKey(); //私钥
acct.serializePublicKey(); //公钥
acct.getAddressU160().toBase58();  //base58地址
```
####根据私钥创建账号：
```javascript
com.github.zeepin.account.Account acct0 = new com.github.zeepin.account.Account(Helper.hexToBytes(privatekey0), zptSdk.defaultSignScheme);
com.github.zeepin.account.Account acct1 = new com.github.zeepin.account.Account(Helper.hexToBytes(privatekey1), zptSdk.defaultSignScheme);
com.github.zeepin.account.Account acct2 = new com.github.zeepin.account.Account(Helper.hexToBytes(privatekey2), zptSdk.defaultSignScheme);
```
###1.1.2 按钱包规范存储：
####在钱包中批量创建账号：
```javascript
//钱包中批量创建账号
zptSdk.getWalletMgr().createAccounts(10, "passwordtest");
zptSdk.getWalletMgr().writeWallet();
//随机创建一个密码为“passwordtest”的账号：
AccountInfo info0 = zptSdk.getWalletMgr().createAccountInfo("passwordtest");
//通过私钥创建一个密码为“passwordtest”的账号
AccountInfo info = zptSdk.getWalletMgr().createAccountInfoFromPriKey("passwordtest", "6aea72296201fbf18eaaa095ad8d21d13e9ed3b7cf2db8070f5b38973e7bf07c");
//获取上面info的账号
com.github.zeepin.account.Account acct0 = zptSdk.getWalletMgr().getAccount(info.addressBase58, "passwordtest");
```
###1.2 地址：
包括单签地址和多签地址,签地址是由一个公钥转换而来，多签地址是由多个公钥转换而来。
```javascript
//单签地址生成：
String privatekey0 = "c19f16785b8f3543bbaf5e1dbb5d398dfa6c85aaad54fc9d71203ce83e505c07";
String privatekey1 = "49855b16636e70f100cc5f4f42bc20a6535d7414fb8845e7310f8dd065a97221";
String privatekey2 = "1094e90dd7c4fdfd849c14798d725ac351ae0d924b29a279a9ffa77d5737bd96";

//生成账号，获取地址
com.github.zeepin.account.Account acct0 = new com.github.zeepin.account.Account(Helper.hexToBytes(privatekey0), zptSdk.defaultSignScheme);
Address sender = acct0.getAddressU160();

//base58地址解码
sender = Address.decodeBase58("ZC3Fmgr3oS56Rg9vxZeVo2mwMMcUiYGcPp")；

//多签地址生成：
Address recvAddr = Address.addressFromMultiPubKeys(2, acct1.serializePublicKey(), acct2.serializePublicKey());
```
##2 原生数字资产转账：
#####对于在主网转账，请将gaslimit 设为20000，gasprice设为1。
###2.1 初始化：
```javascript
String ip = "http://test1.zeepin.net";
String rpcUrl = ip + ":" + "20336";
ZPTSdk zptSdk = ZPTSdk.getInstance();
zeepinSdk.setRpc(rpcUrl);
zeepinSdk.setDefaultConnect(zptSdk.getRpc());
//设置钱包
wm.openWalletFile("wallet.dat"); //如果不存在钱包文件，会自动创建钱包文件。
```
###2.2 查询：
当发完交易之后可能需要查询交易是否已经落账，还可能需要查询账户余额。

####查询zeepin\Gala余额
```javascript
zptSdk.getConnect().getBalance("ZFkd49K9Q4DwPUbNdGzfz9cJbfiEGbqJPy");         //查询zpt，gaga两个
zptSdk.nativevm().zpt().queryBalanceOf("ZFkd49K9Q4DwPUbNdGzfz9cJbfiEGbqJPy"); //查询zpt余额
zptSdk.nativevm().gala().queryBalanceOf("ZFkd49K9Q4DwPUbNdGzfz9cJbfiEGbqJPy");//查询gala余额

//查ZPT信息
zptSdk.nativevm().zpt().queryName();        //功能说明： 查询资产名信息----返回值：资产名称
zptSdk.nativevm().zpt().querySymbol();      //功能说明： 查询资产Symbol信息-----返回值：Symbol信息
zptSdk.nativevm().zpt().queryDecimals();     //功能说明： 查询资产的精确度----返回值：精确度
zptSdk.nativevm().zpt().queryTotalSupply();   //功能说明： 查询资产的总供应量----返回值：总供应量

//查Gala信息
zptSdk.nativevm().gala().queryName();
zptSdk.nativevm().gala().querySymbol();
zptSdk.nativevm().gala().queryDecimals();
zptSdk.nativevm().gala().queryTotalSupply();
```
#### 查询交易是否在交易池中
```javascript
zptSdk.getConnect().getMemPoolTxState("00d9336a5e83754815fdd609f7ecce31135428d4fcc40469082658cfdb8b62c4")
```
```
response 交易池存在此交易:

{
    "Action": "getmempooltxstate",
    "Desc": "SUCCESS",
    "Error": 0,
    "Result": {
        "State":[
            {
              "Type":1,
              "Height":451,
              "ErrCode":0
            },
            {
              "Type":0,
              "Height":0,
              "ErrCode":0
            }
       ]
    },
    "Version": "0.1.0"
}

或 交易池不存在此交易

{
    "Action": "getmempooltxstate",
    "Desc": "UNKNOWN TRANSACTION",
    "Error": 44001,
    "Result": "",
    "Version": "0.1.0"
}
```
#### 查询交易是否调用成功
查询智能合约推送内容，代表交易执行成功，如果没有成功States中不会有transfer的事件。
```javascript
zptSdk.getConnect().getSmartCodeEvent("00d9336a5e83754815fdd609f7ecce31135428d4fcc40469082658cfdb8b62c4")
```
```
response:
{
    "Action": "getsmartcodeeventbyhash",
    "Desc": "SUCCESS",
    "Error": 0,
    "Result": {
        "TxHash": "00d9336a5e83754815fdd609f7ecce31135428d4fcc40469082658cfdb8b62c4",
        "State": 1,
        "GasConsumed": 0,
        "Notify": [
            {
                "CzeepinractAddress": "0100000000000000000000000000000000000000",
                "States": [
                    "transfer",
                    "ZSviKhEgka2fZhhoUjv2trnSMtjUhm3fyz",
                    "ZC3Fmgr3oS56Rg9vxZeVo2mwMMcUiYGcPp",
                    300000
                ]
            }
        ]
    },
    "Version": "0.1.0"
}
```
#### 根据块高查询智能合约事件，返回有事件的交易hash
```javascript
zptSdk.getConnect().getSmartCodeEvent(100)
```
```
response:
{
    "Action": "getsmartcodeeventbyhash",
    "Desc": "SUCCESS",
    "Error": 0,
    "Result": {
        "TxHash": "00d9336a5e83754815fdd609f7ecce31135428d4fcc40469082658cfdb8b62c4",
        "State": 1,
        "GasConsumed": 0,
        "Notify": [
            {
                "CzeepinractAddress": "0100000000000000000000000000000000000000",
                "States": [
                    "transfer",
                    "ZSviKhEgka2fZhhoUjv2trnSMtjUhm3fyz",
                    "ZC3Fmgr3oS56Rg9vxZeVo2mwMMcUiYGcPp",
                    300000
                ]
            }
        ]
    },
    "Version": "0.1.0"
}
```
#####通过查看“State” 判断：
* 1 代表交易成功
* 0 代表交易失败

#####“Notify"解析数组如下：
######ContractAddress：合约地址
* 0100000000000000000000000000000000000000 为ZPT
* 0200000000000000000000000000000000000000 为Gala
#######States：数组
```
transfer    代表转账操作
from		转出地址
to		目标地址
第四行为转账数量（ZPT和Gala的精度为4，所以这里ZPT和Gala的实际数量因除以10000）
```
```
No     Main Function                                           	Description
2 	zptSdk.getConnect().getNodeCount() 	                       查询节点数量
3 	zptSdk.getConnect().getBlock(15) 	                         查询块
4 	zptSdk.getConnect().getBlockJson(15)                   	   查询块
5 	zptSdk.getConnect().getBlockJson("txhash") 	               查询块
6 	zptSdk.getConnect().getBlock("txhash") 	                   查询块
7 	zptSdk.getConnect().getBlockHeight() 	                     查询当前块高
8 	zptSdk.getConnect().getTransaction("txhash") 	             查询交易
9 	zptSdk.getConnect().getStorage("czeepinractaddress", key)     查询智能合约存储
10 	zptSdk.getConnect().getBalance("address") 	               查询余额
11 	zptSdk.getConnect().getCzeepinractJson("czeepinractaddress") 查询智能合约
12 	zptSdk.getConnect().getSmartCodeEvent(59) 	               查询智能合约事件
13 	zptSdk.getConnect().getSmartCodeEvent("txhash") 	         查询智能合约事件
14 	zptSdk.getConnect().getBlockHeightByTxHash("txhash") 	    查询交易所在高度
15 	zptSdk.getConnect().getMerkleProof("txhash") 	            获取merkle证明
16 	zptSdk.getConnect().sendRawTransaction("txhexString") 	   发送交易
17 	zptSdk.getConnect().sendRawTransaction(Transaction) 	     发送交易
18 	zptSdk.getConnect().sendRawTransactionPreExec() 	         发送预执行交易
19 	zptSdk.getConnect().getAllowance("zeepin","from","to") 	  查询允许使用值
20 	zptSdk.getConnect().getMemPoolTxCount() 	                 查询交易池中交易总量
21 	zptSdk.getConnect().getMemPoolTxState() 	                 查询交易池中交易状态
```

###2.3 zeepin转账
ZPT和GALA转账可以一对一，也可以一对多，多对多，多对一。
#####构造转账交易并发送
```javascript
//构造转账交易并发送 单转
//转出方与收款方地址
Address sender = acct0.getAddressU160();
Address recvAddr = acct1.getAddressU160();   
//多签地址生成
//Address recvAddr = Address.addressFromMultiPubKeys(2, acct1.serializePublicKey(), acct2.serializePublicKey());

//构造转帐交易
long amount = 20000;
Transaction tx = zptSdk.nativevm().zpt().makeTransfer(sender.toBase58(), recvAddr.toBase58(), amount, sender.toBase58(), 30000, 1);
String hash = tx.hash().toString();

zptSdk.addSign(txRx, acct0);//签名      
//多签签名分多次签    -- 添加多签签名 ---如果转出方与网络费付款人不是同一个地址，需要添加网络费付款人的签名
zptSdk.addMultiSign(tx, 2,new byte[][]{acct.serializePublicKey(),acct2.serializePublicKey()},acct1);   //acct1签名
zptSdk.addMultiSign(tx, 2,new byte[][]{acct.serializePublicKey(),acct2.serializePublicKey()},acct2);  //acct2签名

//发送预执行（余额不足返回异常，余额充足则成功返回） (可选)
Object obj = zptSdk.getConnect().sendRawTransactionPreExec(tx.toHexString());

//发送交易
zptSdk.getConnect().sendRawTransaction(tx.toHexString());

//同步发送交易
Object obj = zptSdk.getConnect().syncSendRawTransaction(tx.toHexString());
```

#####一转多或多转多
######  1.构造多个state的交易
######  2.签名
######  3.一笔交易上限为1024笔转账
```javascript
Address sender1 = acct0.getAddressU160();  //转出方
Address sender2 =  Address.addressFromMultiPubKeys(2, acct1.serializePublicKey(), acct2.serializePublicKey());    //转出方的多签地址         
int amount1 = 10000;
int amount2 = 20000;
Address recvAddr = acct1.getAddressU160(); //接收方

//交易
State state1 = new State(sender1, recvAddr, amount1);  
State state2 = new State(sender2, recvAddr, amount2);
Transaction tx = zptSdk.nativevm().zpt().makeTransfer(new State[]{state1, state2}, sender1.toBase58(), 30000,1);   //2转1交易

//签名
zptSdk.signTx(tx, new com.github.zeepin.account.Account[][] {{acct0}});   //转出方的单签地址
zptSdk.addMultiSign(tx,2,new byte[][]{acct1.serializePublicKey(),acct2.serializePublicKey()},acct1);//转出方多签地址 acct1签名
zptSdk.addMultiSign(tx,2,new byte[][]{acct1.serializePublicKey(),acct2.serializePublicKey()},acct2);//acct2签名        

//同步发送交易
Object obj = zptSdk.getConnect().syncSendRawTransaction(tx.toHexString());

```
#####使用签名机签名
######构造交易并签名
* 构造交易，序列化交易，发送交易给签名机
* 签名机接收到交易，反序列化，检查交易，添加签名
* 发送交易

```javascript
Transaction tx = zptSdk.nativevm().zpt().makeTransfer(sender.toBase58(), recvAddr.toBase58(), amount, sender.toBase58(), 30000, 1);

String txHex = tx.toHexString();//序列化交易发送给签名机

Transaction txRx = Transaction.deserializeFrom(Helper.hexToBytes(txHex));//接收方反序列化交易并签名

zptSdk.addSign(txRx, acct0);//签名   
```

###2.4 Gala转账
##### Gala转账接口与ZPT类似：

```javascript
//构建一个交易
Transaction tx =  zptSdk.nativevm().gala().makeTransfer(sender.toBase58(), recvAddr.toBase58(), amount, sender.toBase58(), 30000, 1);   
zptSdk.signTx(tx, new com.github.zeepin.account.Account[][] {{acct0}}); //签名
zptSdk.getConnect().syncSendRawTransaction(tx.toHexString());//发送gala交易
```

##### 提取Gala
* 查询是否有Gala可以提取
* 构造交易和签名
* 发送提取Gala交易

```javascript
String addr = acct0.getAddressU160().toBase58();
String gala = zptSdk.nativevm().gala().unboundGala(addr);//查询未提取的Gala

//提取Gala
com.github.zeepin.account.Account account = new com.github.zeepin.account.Account(Helper.hexToBytes(privatekey0),zptSdk.defaultSignScheme);
String hash = zptSdk.nativevm().gala().withdrawGala(account,recvAddr.toBase58(),100000,account,20000,1); //claim提取者，提给谁，金额，网络付费人地址，gaslimit，gasprice

```
###2.5 原生数字资产接口  -
######原生数字资产包括zpt和Gala。封装了构造交易、交易签名、发送交易。
```javascript
//转账
zptSdk.nativevm().zpt().sendTransfer(acct0,acct2.getAddressU160().toBase58(),10000,acct0,zptSdk.DEFAULT_GAS_LIMIT,1); //从发送方转移一定数量的zpt到接收方账户
ptSdk.nativevm().gala().sendTransfer(acct0,acct2.getAddressU160().toBase58()10000,acct0,zptSdk.DEFAULT_GAS_LIMIT,1);//从发送方转移一定数量的Gala到接收方账户

//授权转移资产
zptSdk.nativevm().zpt().sendApprove(acct0, acct2.getAddressU160().toBase58(),10000, acct0, zptSdk.DEFAULT_GAS_LIMIT,1);//sendAddr账户允许recvAddr转移amount数量的资产

zptSdk.nativevm().zpt().sendTransferFrom(acct0, acct0.getAddressU160().toBase58(),acct1.getAddressU160().toBase58(), 10000, acct0, zptSdk.DEFAULT_GAS_LIMIT,1);// sendAcct账户从fromAddr账户转移amount数量的资产到toAddr账户

```

## 3 数字身份管理
###钱包文件及规范
钱包文件是一个Json格式的数据存储文件，可同时存储多个数字身份和多个数字资产账户。
为了创建数字身份，您首先需要创建/打开一个钱包文件。
```javascript
//如果不存在钱包文件，会自动创建钱包文件。
ZptSdk zptSdk = ZptSdk.getInstance();
zptSdk.openWalletFile("Demo.dat");

```
* 注：目前仅支持文件形式钱包文件，也可以扩展支持数据库或其他存储方式。


###数字身份接口
####1. 注册身份
```javascript
Identity identity = zptSdk.getWalletMgr().createIdentity("passwordtest");
//创建的账号或身份只在内存中，如果要写入钱包文件，需调用写入接口
zptSdk.getWalletMgr().writeWallet();
```
#####向链上注册身份
只有向区块链链成功注册身份之后，该身份才可以真正使用。
```javascript
Identity identity = zptSdk.getWalletMgr().createIdentity("passwordtest");
zptSdk.nativevm().GId().sendRegister(identity, "passwordtest", acct0, 20000, 1);
```
