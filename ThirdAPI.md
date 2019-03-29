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

00d142f1e9f00100000000000000204e000000000000a80c86b55edd9c92ed1741df571a5902d18c609a7700c66b14a80c86b55edd9c92ed1741df571a5902d18c609a6a7cc814f1c16885a4a801cfc5ce6fb100787e188ae2a9886a7cc803a086016a7cc86c51c1087472616e736665721400000000000000000000000000000000000000010068195a656570696e436861696e2e4e61746976652e496e766f6b650001424101b9dcf8bba8c63a3cd142bfa0c956b65b9b987d51289e52b7ed036a714fea117cb84a9bfef8066d186129a780832c8fe3e31efc4622e9018c1db4e1d6cca635a4232103d43a98680a311d5f283a92c499ac02b0f09d4601ee4267872be7c7965621c8d4ac

Content-Disposition: form-data; name="url"
https://zeepin.io/feedback
