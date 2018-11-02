package demo;

import java.io.FileInputStream;
import java.io.InputStream;

import com.github.zeepin.ZPTSdk;
import com.github.zeepin.account.Account;
import com.github.zeepin.common.Address;
import com.github.zeepin.common.Helper;
import com.github.zeepin.core.transaction.Transaction;

public class ZeeRight {
    public static long DEFAULT_DEPLOY_CONTRACT_LIMIT = 22000000;

	public static void main(String[] args) throws Exception {
		// TODO Auto-generated method stub
        ZPTSdk zptSdk = getZptSdk(); 
		InputStream is = new FileInputStream("zeeright.avm");
		byte[] bys = new byte[is.available()];
		is.read(bys);
		is.close();
		String code = Helper.toHexString(bys);
//		ZPTSdk.setCodeAddress(Address.AddressFromVmCode(code).toHexString());
		zptSdk.vm().setCodeAddress(Address.AddressFromVmCode(code).toHexString());
		System.out.println(code);
		
		//部署合约
		String pk = "79fad30cc680bafc206c765c2dc6d5eb2397de1540e62a4b04019ffda6df2ca5";
		Account mine = new Account(Helper.hexToBytes(pk), zptSdk.defaultSignScheme);
        
       Transaction tx = zptSdk.vm().makeDeployCodeTransaction(code, true, "ZeeRight",
		                    "v0.1", "Zeepin", "zeepin@gmail.com", "ZeeRight v0.1", mine.getAddressU160().toBase58(),ZeeRight.DEFAULT_DEPLOY_CONTRACT_LIMIT,500);
		System.out.println(tx.verify());
		zptSdk.signTx(tx, new Account[][]{{mine}});
		String txHex = Helper.toHexString(tx.toArray());
		System.out.println(txHex);
		zptSdk.getConnect().sendRawTransaction(txHex);
//		//等待出块
		Thread.sleep(6000);
        
		Object t =  zptSdk.getConnect().getTransaction(txHex);
		System.out.println(t);
	}
    public static ZPTSdk getZptSdk() throws Exception {
//      String ip = "http://139.219.108.204";
  		String ip = "http://127.0.0.1";
      String restUrl = ip + ":" + "20334";
      String rpcUrl = ip + ":" + "20336";
      String wsUrl = ip + ":" + "20335";

      ZPTSdk wm = ZPTSdk.getInstance();
      wm.setRpc(rpcUrl);
      wm.setRestful(restUrl);
      
      wm.setDefaultConnect(wm.getRpc());
      System.out.println(restUrl);
   //   wm.openWalletFile("Demo3.json");

      return wm;
  }
}
