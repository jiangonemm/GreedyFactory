# runtimeNotify

Location: imZhuFei/zeepin/smartcontract/service/wasmvm/runtime.go

#### Before
```go
func (this *WasmVmService) runtimeNotify(engine *exec.ExecutionEngine) (bool, error) {
	vm := engine.GetVM()
	envCall := vm.GetEnvCall()
	params := envCall.GetParams()
	if len(params) != 1 {  //与接口ZPT_Runtime_Notify(char * a)一致
		return false, errors.NewErr("[RuntimeNotify]parameter count error ")
	}
	item, err := vm.GetPointerMemory(params[0])  //获得接口传进来的值
	if err != nil {
		return false, err
	}
	context := this.ContextRef.CurrentContext()

	this.Notifications = append(this.Notifications, &event.NotifyEventInfo{ContractAddress: context.ContractAddress, States: []string{string(item)}})  //装载Notify，其中states为字符串
	vm.RestoreCtx()
	return true, nil
}
```
合约接口：`ZPT_Runtime_Notify(char * a)`
#### Modified
```go
func (this *WasmVmService) runtimeNotify(engine *exec.ExecutionEngine) (bool, error) {
	vm := engine.GetVM()
	envCall := vm.GetEnvCall()
	params := envCall.GetParams()
	if len(params) != 2 {  //这里只有值为1时没问题，其余的全部报下面这个错
		return false, errors.NewErr("[RuntimeNotify]parameter count error")
	}
	item1, err := vm.GetPointerMemory(params[0]) //想获取第一个值
	if err != nil {
		return false, err
	}
	item2, err := vm.GetPointerMemory(params[1]) //第二个值
	if err != nil {
		return false, err
	}

	context := this.ContextRef.CurrentContext()
	this.Notifications = append(this.Notifications, &event.NotifyEventInfo{ContractAddress: context.ContractAddress, States:[]string{string(item1),string(item2)}}) //states将两个值都装载了进去，但是并不能运行到这一步
	vm.RestoreCtx()
	return true, nil
}
```
合约接口改为：`ZPT_Runtime_Notify(char * a, char * b)`