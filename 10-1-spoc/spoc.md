
问题：缺少choosing语句会出现的问题：（以两进程为例）两个进程同时进入临界区。

假设有两个进程T1,T2 初始number[T1] = number[T2] = 0
赋值，两者都为1，但是去max时，赋值有时间差，T2首先进入临界区， 所以number[T1] = max+1 = 1；
由于number[T1]=number[T2]=1,pid[T1]<pid[T2]则T1也会进入临界区
choosing的存在就是为了让两个进程都赋值了，再判断。


## 小组思考题

1. （spoc）阅读[简化x86计算机模拟器的使用说明](https://github.com/chyyuu/ucore_lab/blob/master/related_info/lab7/lab7-spoc-exercise.md)，理解基于简化x86计算机的汇编代码。

2. （spoc)了解race condition. 进入[race-condition代码目录](https://github.com/chyyuu/ucore_lab/tree/master/related_info/lab7/race-condition)。

 - 执行 `./x86.py -p loop.s -t 1 -i 100 -R dx`， 请问`dx`的值是什么？
 
  答：-1
 
 - 执行 `./x86.py -p loop.s -t 2 -i 100 -a dx=3,dx=3 -R dx` ， 请问`dx`的值是什么？
 
  答：-1
  
 - 执行 `./x86.py -p loop.s -t 2 -i 3 -r -a dx=3,dx=3 -R dx`， 请问`dx`的值是什么？
 
  答：-1
  
 - 变量x的内存地址为2000, `./x86.py -p looping-race-nolock.s -t 1 -M 2000`, 请问变量x的值是什么？
 
  答：-1
  
 - 变量x的内存地址为2000, `./x86.py -p looping-race-nolock.s -t 2 -a bx=3 -M 2000`, 请问变量x的值是什么？为何每个线程要循环3次？
 
  答：6，因为初始bx=3, 每个循环减一，到0时停止。
  
 - 变量x的内存地址为2000, `./x86.py -p looping-race-nolock.s -t 2 -M 2000 -i 4 -r -s 0`， 请问变量x的值是什么？
 
  答：1
  
 - 变量x的内存地址为2000, `./x86.py -p looping-race-nolock.s -t 2 -M 2000 -i 4 -r -s 1`， 请问变量x的值是什么？
 
  答：1
  
 - 变量x的内存地址为2000, `./x86.py -p looping-race-nolock.s -t 2 -M 2000 -i 4 -r -s 2`， 请问变量x的值是什么？ 
 
  答：1
  
 - 变量x的内存地址为2000, `./x86.py -p looping-race-nolock.s -a bx=1 -t 2 -M 2000 -i 1`， 请问变量x的值是什么？ 
 
  答：1
  

3. （spoc） 了解software-based lock, hardware-based lock, [software-hardware-lock代码目录](https://github.com/chyyuu/ucore_lab/tree/master/related_info/lab7/software-hardware-locks)

  - 理解flag.s,peterson.s,test-and-set.s,ticket.s,test-and-test-and-set.s 请通过x86.py分析这些代码是否实现了锁机制？请给出你的实验过程和结论说明。能否设计新的硬件原子操作指令Compare-And-Swap,Fetch-And-Add？
```
Compare-And-Swap

int CompareAndSwap(int *ptr, int expected, int new) {
  int actual = *ptr;
  if (actual == expected)
    *ptr = new;
  return actual;
}
```

```
Fetch-And-Add

int FetchAndAdd(int *ptr) {
  int old = *ptr;
  *ptr = old + 1;
  return old;
}
```
答：均实现了锁机制,x86.py中如下代码可测试
```
     #
     # SUPPORT FOR LOCKS
     #
     def atomic_exchange(self, src, value, reg1, reg2):
         tmp                 = value + self.registers[reg1] + self.registers[reg2]
         old                 = self.memory[tmp]
         self.memory[tmp]    = self.registers[src]
         self.registers[src] = old
         return 0
   
     def fetchadd(self, src, value, reg1, reg2):
         tmp                 = value + self.registers[reg1] + self.registers[reg2]
         old                 = self.memory[tmp]
         self.memory[tmp]    = self.memory[tmp] + self.registers[src] 
         self.registers[src] = old
```
