sysinfo

一开始一直编译错误，查半天发现是因为没改`usys.pl`

以此为例，理一下syscall的步骤：
1. 用户程序调用（和调普通函数形式一样）
2. 通过`usys.S`加载syscall号码并执行
3. 转到`syscall`函数
4. 通过映射找到`sys_sysinfo`函数


trace

根目录下的README被用来做测试了，所以不能改
否则会出现奇怪的错误：`MISSING '^\d+: syscall read -> 1023'`

trace children不通过，如果是超时：`MISSING '^ALL TESTS PASSED'`
可修改`gradelib.py`里`run_qemu_kw`函数的`timeout`参数
