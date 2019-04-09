### 扫码登录字段

#### id

由第三方自行生成（接到回调的用户信息后，自行与该临时id绑定）

统一8位长度的随机字符串，数字0-9，字母A-Z，字母a-z，无特殊字符

生成方式根据语言的随机库具体选择，但不可违背以上规则

#### url

回调地址（钱包登陆成功后，将id、地址、公钥发到回调地址）

https或者http回调地址

#### 示例

Content-Disposition: form-data; name="id"
a2pX9T8

Content-Disposition: form-data; name="url"
https://zeescan.io/feedback

实例：http://zeescan.io/api/v1/scanlogin?id=a2pX9T8&url=https://zeescan.io/feedback


![login](https://github.com/jiangonemm/GreedyFactory/blob/master/img/scanlogin.png)

--------------------------

### 扫码交易

#### order

由第三方自行生成（接到回调的用户信息后，自行与该临时id绑定）

统一8位长度的随机字符串，数字0-9，字母A-Z，字母a-z，无特殊字符

生成方式根据语言的随机库具体选择，但不可违背以上规则 

#### contract

合约地址（token的合约地址）

0001为ZPT，0002为Gala，其余合约按照正常处理

#### to

收款地址（Zeepin地址，收款方默认为扫码地址）


#### amount

转账金额（非同质资产也用该字段）

#### url

回调地址（交易成功后，将结果发送至回调地址）

https或者http回调地址

#### 示例


Content-Disposition: form-data; name="order"
T1t2PX8e

Content-Disposition: form-data; name="contract"
0001


Content-Disposition: form-data; name="to"
ZTMpJFXdmgosonQn5KVy3fi8YmBkztAs4Q


Content-Disposition: form-data; name="amount"
10

Content-Disposition: form-data; name="url"
https://zeescan.io/feedback


实例：http://zeescan.io/api/v1/buildtx?order=T1t2PX8e&contract=0001&to=ZTMpJFXdmgosonQn5KVy3fi8YmBkztAs4Q&amount=10&url=https://zeepin.io/feedback
