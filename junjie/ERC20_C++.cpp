//
//  main.cpp
//  ERC20_C++
//
//  Created by Cindice on 2018/11/28.
//  Copyright © 2018年 junjie. All rights reserved.
//

#include <iostream>
#include <map>
#include <string>
#include <assert.h>
using namespace std;

typedef char* address; //定义别名
address msgSender; //声明全局变量

class ERC20 {
private:
    map<address, uint> _balances;
    map<address, map<address, uint>> _allowed;
    uint _totalSupply;
    
public:
    uint totalSupply() {
        return _totalSupply;
    }
    
    uint balanceOf(address owner) {
        return _balances[owner];
    }
    
    uint allowance(address owner, address spender) {
        return _allowed[owner][spender];
    }
    
    bool transfer(address to, uint value) {
        _transfer(msgSender, to, value);
        return true;
    }
    
    bool approve(address spender, uint value) {
        assert(spender != 0); //solidity中的require，不知是否用assert来替换？
        _allowed[msgSender][spender] = value;
//        emit Approval(msgsender, spender, value);
        return true;
    }
    
    bool transferFrom(address from, address to, uint value) {
        _allowed[from][msgSender] = _allowed[from][msgSender]-value; //未解决SafeMath问题
        _transfer(from, to, value);
        return true;
    }
    
    bool increaseAllowance(address spender, uint addedValue) {
        assert(spender != 0);
        _allowed[msgSender][spender] = _allowed[msgSender][spender] + addedValue; //未解决SafeMath问题
//        emit Approval(msgSender, spender, _allowed[msgSender][spender]);
        return true;
    }
    
    bool decreaseAllowance(address spender, uint subtractedValue) {
        assert(spender != 0);
        _allowed[msgSender][spender] = _allowed[msgSender][spender] - subtractedValue; //未解决SafeMath问题
//        emit Approval(msgSender, spender, _allowed[msgSender][spender]);
        return true;
    }
    
    void _transfer(address from, address to, uint value) {
        assert(to != 0);
        _balances[from] = _balances[from] - value; //未解决SafeMath问题
        _balances[to] = _balances[to] + value; //未解决SafeMath问题
//        emit Transfer(from, to, value);
    }
    
    void _mint(address account, uint value) {
        assert(account != 0);
        _totalSupply = _totalSupply + value; //未解决SafeMath问题
        _balances[account] = _balances[account] + value; //未解决SafeMath问题
//        emit Transfer(0, account, value);
    }
    
    void _burn(address account, uint value) {
        assert(account != 0);
        _totalSupply = _totalSupply - value; //未解决SafeMath问题
        _balances[account] = _balances[account] - value; //未解决SafeMath问题
//        emit Transfer(account, 0, value);
    }
    
    void _burnFrom(address account, uint value) {
        _allowed[account][msgSender] = _allowed[account][msgSender] - value; //未解决SafeMath问题
        _burn(account, value);
    }
    
};

int main(int argc, const char * argv[]) {
    cin>>msgSender;
    //Do something here.（做一些调用）
    return 0;
}
