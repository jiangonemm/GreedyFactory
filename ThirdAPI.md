### 扫码登录字段

#### id

由第三方自行生成（接到回调的用户信息后，自行与该临时id绑定）

统一8位长度的随机字符串，数字0-9，字母A-Z，字母a-z，无特殊字符

生成方式根据语言的随机库具体选择，但不可超过以上范围

#### url

回调地址（钱包登陆成功后，将id、地址、公钥发到回调地址）

https或者http回调地址

#### 示例

Content-Disposition: form-data; name="id"
a2pX9T8

Content-Disposition: form-data; name="url"
https://zeepin.io/feedback


--------------------------

### 扫码交易

#### tx

交易内容（币种、金额、转账双方，钱包扫瞄信息后对交易内容进行签名）

由SDK封装的交易信息


#### url

回调地址（交易成功后，将结果发送至回调地址）

https或者http回调地址

#### 示例

Content-Disposition: form-data; name="tx"
e21deb896ae1657c190aa8295d55f625b63ba997102e1376e994749d9bbaeb6f

Content-Disposition: form-data; name="url"
https://zeepin.io/feedback
