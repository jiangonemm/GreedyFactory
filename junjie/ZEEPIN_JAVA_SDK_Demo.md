# ZEEPIN_JAVA_SDK 交互
## 账户创建、管理、查询，转账相关Demo
##### -编译运行前请确保钱包文件中存在相应账户-

##### 旧地址转换为新地址
```java
String oldAddress = "AVBokCnLS9NxUjeXTdL8BMsgN41bwtK89J";//A开头的旧地址
String newAddress = ConvertAddress.convertAddress(oldAddress);//Z开头的新地址
System.out.print(newAddress);
```
##### 随机创建一个账户
```java
com.github.zeepin.account.Account acct = new com.github.zeepin.account.Account(zptSdk.defaultSignScheme);
System.out.println(acct.serializePrivateKey());//私钥
System.out.println(acct.serializePublicKey());//公钥
System.out.println(acct.getAddressU160().toBase58());//base58地址
zptSdk.getWalletMgr().writeWallet();
```
##### 输入密码在钱包中批量创建账户
```java
zptSdk.getWalletMgr().createAccounts(10, "Wjj123456789");
zptSdk.getWalletMgr().writeWallet();
```
##### 通过index设置默认账户
```java
zptSdk.getWalletMgr().getWallet().setDefaultAccount(0);//index
zptSdk.getWalletMgr().writeWallet();
```
##### 通过地址设置默认账户
```java
zptSdk.getWalletMgr().getWallet().setDefaultAccount("ZEuzshrCsE1cnvPuuRrDYgnVYNDtyt5d3X");//address
zptSdk.getWalletMgr().writeWallet();
```
##### 获取账户
```java
com.github.zeepin.account.Account acct0 = zptSdk.getWalletMgr().getAccount("ZEuzshrCsE1cnvPuuRrDYgnVYNDtyt5d3X","Wjj123456789");
```
##### 查询账户的zpt和gala余额
```java
zptSdk.getConnect().getBalance("ZK4hRD28ucvxKU1zJqavUJbfU2ene5uJ9n");
```
##### 从钱包文件中删除指定的账户
```java
zptSdk.getWalletMgr().getWallet().removeAccount("ZEKXwQwUXbsLmMMyc31vQoCuNzhLYwN27h");
zptSdk.getWalletMgr().writeWallet();
```
##### 查询zpt信息
```java
zptSdk.nativevm().zpt().queryName();
zptSdk.nativevm().zpt().querySymbol();
zptSdk.nativevm().zpt().queryDecimals();
zptSdk.nativevm().zpt().queryTotalSupply();
```
##### 查询gala信息
```java
zptSdk.nativevm().gala().queryName();
zptSdk.nativevm().gala().querySymbol();
zptSdk.nativevm().gala().queryDecimals();
zptSdk.nativevm().gala().queryTotalSupply();
```
##### 查询交易是否在交易池中，存在则打印相关信息
```java
zptSdk.getConnect().getMemPoolTxState("d61aeeec9adcb7190e1d28604fcfa8ceb0cddf785eceec269313e967a16a9e38");
```
##### 根据交易Hash查询智能合约推送内容
```java
zptSdk.getConnect().getSmartCodeEvent("d61aeeec9adcb7190e1d28604fcfa8ceb0cddf785eceec269313e967a16a9e38");
```
##### 根据块高查询智能合约事件，返回有事件的交易
```java
zptSdk.getConnect().getSmartCodeEvent(230869);
```
##### 查询节点数量
```java
zptSdk.getConnect().getNodeCount();
```
##### 根据块高查询块
```java
zptSdk.getConnect().getBlock(231089);	
```
##### 根据块的Hash查询块
```java
zptSdk.getConnect().getBlock("dfac114454fe13793ecc4f644dc5915abc55e58bcf4684dfddf718dc681ddf98");
```
##### 根据块高查询块的Json信息
```java
zptSdk.getConnect().getBlockJson(231089);	
```
##### 根据块的Hash查询块的Json信息
```java
zptSdk.getConnect().getBlockJson("dfac114454fe13793ecc4f644dc5915abc55e58bcf4684dfddf718dc681ddf98");
```
##### 查询当前块高
```java
zptSdk.getConnect().getBlockHeight();
```
##### 根据交易Hash查询交易信息
```java
zptSdk.getConnect().getTransaction("d61aeeec9adcb7190e1d28604fcfa8ceb0cddf785eceec269313e967a16a9e38");
```
##### 查询交易所在块的高度
```java
zptSdk.getConnect().getBlockHeightByTxHash("d61aeeec9adcb7190e1d28604fcfa8ceb0cddf785eceec269313e967a16a9e38");
```
##### 根据交易Hash获取merkle证明
```java
zptSdk.getConnect().getMerkleProof("d61aeeec9adcb7190e1d28604fcfa8ceb0cddf785eceec269313e967a16a9e38");
```
##### 查询交易池中交易总量
```java
zptSdk.getConnect().getMemPoolTxCount();
```
##### 查询交易池中交易状态
```java
zptSdk.getConnect().getMemPoolTxState("d61aeeec9adcb7190e1d28604fcfa8ceb0cddf785eceec269313e967a16a9e38");
```
##### 单签地址，一转一交易
```java
com.github.zeepin.account.Account acct0 = zptSdk.getWalletMgr().getAccount("ZEuzshrCsE1cnvPuuRrDYgnVYNDtyt5d3X","Wjj123456789");
com.github.zeepin.account.Account acct1 = zptSdk.getWalletMgr().getAccount("ZNEo7CMRpQXGDgSwvhm2iDGPTXhVRJcMfc","Wjj123456789");

String sendAddr = "ZEuzshrCsE1cnvPuuRrDYgnVYNDtyt5d3X";
String recvAddr = "ZNEo7CMRpQXGDgSwvhm2iDGPTXhVRJcMfc";
long amount = 10000000;

//构造转账交易
Transaction tx = zptSdk.nativevm().zpt().makeTransfer(sendAddr, recvAddr, amount, sendAddr, 20000, 1); //转zpt

//Transaction tx = zptSdk.nativevm().gala().makeTransfer(sendAddr, recvAddr, amount, sendAddr, 20000, 1); //转gala

//单签签名
zptSdk.signTx(tx, new com.github.zeepin.account.Account[][]{{acct0}});

//发送交易
zptSdk.getConnect().sendRawTransaction(tx.toHexString());
```
##### 单签地址&多签地址，多转一交易
```java
com.github.zeepin.account.Account acct0 = zptSdk.getWalletMgr().getAccount("ZEuzshrCsE1cnvPuuRrDYgnVYNDtyt5d3X","Wjj123456789");					
com.github.zeepin.account.Account acct1 = zptSdk.getWalletMgr().getAccount("ZNEo7CMRpQXGDgSwvhm2iDGPTXhVRJcMfc","Wjj123456789");
com.github.zeepin.account.Account acct2 = zptSdk.getWalletMgr().getAccount("ZFPE3GjRRW958TAizn8iLdjYZevbVu7isT","Wjj123456789");
com.github.zeepin.account.Account acct3 = zptSdk.getWalletMgr().getAccount("ZZTf9Pbnkr4F1eRe19AUDKhjBUyxjsE6ZR","Wjj123456789");			

Address sender1 = acct0.getAddressU160();
Address sender2 = Address.addressFromMultiPubKeys(2, acct1.serializePublicKey(), acct2.serializePublicKey());
Address recvAddr = acct3.getAddressU160();
int amount1 = 20000000;
int amount2 = 100000;
State state1 = new State(sender1, recvAddr, amount1);
State state2 = new State(sender2, recvAddr, amount2);	

//构造转账交易
Transaction tx = zptSdk.nativevm().zpt().makeTransfer(new State[]{state1,state2},sender1.toBase58(),20000,1); //转zpt

//Transaction tx = zptSdk.nativevm().gala().makeTransfer(new State[]{state1,state2},sender1.toBase58(),20000,1); //转gala

//单签签名
zptSdk.signTx(tx, new com.github.zeepin.account.Account[][]{{acct0}});
//添加多签签名
zptSdk.addMultiSign(tx,2,new byte[][]{acct1.serializePublicKey(),acct2.serializePublicKey()},acct1);
zptSdk.addMultiSign(tx,2,new byte[][]{acct1.serializePublicKey(),acct2.serializePublicKey()},acct2);	

//发送交易
zptSdk.getConnect().sendRawTransaction(tx.toHexString());
```
##### 查询未提取的Gala
```java
String gala = zptSdk.nativevm().gala().unboundGala("ZEuzshrCsE1cnvPuuRrDYgnVYNDtyt5d3X");
System.out.println(gala);
```
##### 提取Gala到自己账户中
```java
com.github.zeepin.account.Account acct0 = zptSdk.getWalletMgr().getAccount("ZEuzshrCsE1cnvPuuRrDYgnVYNDtyt5d3X","Wjj123456789");
String hash = zptSdk.nativevm().gala().withdrawGala(acct0, "ZEuzshrCsE1cnvPuuRrDYgnVYNDtyt5d3X", 100000, acct0, 20000, 1);
```

