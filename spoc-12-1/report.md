- cvp->count含义是什么？cvp->count是否可能<0, 是否可能>1？请举例或说明原因。

	cvp->count的含义是等待该条件变量的进程数量 当认为++，--都是原子操作的时候，count不可能<0，因为++和 --都是成对存在的.而这条假设按理说应该由管程进入和退出时的lock完成。
可能>1，有多个进程等待的时候cvp->count>1

- cvp->owner->next_count含义是什么？cvp->owner->next_count是否可能<0, 是否可能>1？请举例或说明原因。

	next_count的含义是，由于signal+Hoare而等待的线程数量。

	不可能小于0，原因同上

	不可能>1

- 目前的lab7-answer中管程的实现是Hansen管程类型还是Hoare管程类型？请在lab7-answer中实现另外一种类型的管程。
Hoare管程类型
- 现在的管程（条件变量）实现是否有bug?
