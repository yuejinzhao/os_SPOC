###描述ucore操作系统中“键盘输入”从请求到完成的整个执行过程，并分析I/O过程的时间开销。
键盘输入会触发中断机制，在trap_dispatch()中对键盘处理的流程如下
```
case IRQ_OFFSET + IRQ_KBD:
    c = cons_getc();
    {
        extern void dev_stdin_write(char c);
        dev_stdin_write(c);

    }
    break;
```
通过cons_getc() 从控制台获取输入的字符c, 然后通过dev_stdin_write(c)函数将字符c存储到缓冲区，唤醒等待键盘输入的进程

dev_stdin_write的处理过程如下：
```
bool intr_flag;
    if (c != '\0') {
        local_intr_save(intr_flag);
        {
            stdin_buffer[p_wpos % STDIN_BUFSIZE] = c;
            if (p_wpos - p_rpos < STDIN_BUFSIZE) {
                p_wpos ++;
            }
            if (!wait_queue_empty(wait_queue)) {
                wakeup_queue(wait_queue, WT_KBD, 1);
            }
        }
        local_intr_restore(intr_flag);
    }
```
