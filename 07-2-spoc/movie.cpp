#include <stdio.h>
#include <proc.h>
#include <sem.h>
#include <monitor.h>
#include <assert.h>
int cinema=-1;
int people=0;
semaphore_t mov[num]; /* 每个电影一个信号量 */
int wait[3];
void semaphore_test(i) /* i：影片编号 */
{
    if(cinema==-1 || (cinema==i && people>0))
    {
    cinema=i;
    up(&mov[i]);
    }
}
void semaphore_movie_play(int i)
{
    down(&mutex);
    semaphore_test(i);
    int ifwait=0;
    if (i!=cinema) ifwait=1;
    wait[i]+=ifwait;
    //cprintf(""testing %d %d %d\n"",cinema,i,mov[i].value);
    up(&mutex);
    down(&mov[i]);
    down(&mutex);
    wait[i]-=ifwait;
    people++;
    cinema=i;
    cprintf(""No.%d movie_sema is playing,remain people num:%d \n"",i,people); /*电影放映*/
    //cprintf(""testING %d %d %d %d\n"",cinema,i,mov[i].value,wait[i]);
    if (wait[i]!=0) up(&mov[i]);
    up(&mutex);
    //if (bf==people) down(&mov[i]);
}

void semaphore_cinema_end(int i) /* i：影片编号从0到N-1 */
{
    down(&mutex); /* 进入临界区 */
    people--;
    cprintf(""No.%d movie_sema quit,remain people num: %d \n"",i,people);
    if(people==0)  cinema=-1;
    semaphore_test(left);
    semaphore_test(right); /* 看一下其他影片可否播放 */
    up(&mutex); /* 离开临界区 */
}

int semaphore_movie(void * arg) /* i：电影编号，从0到N-1 */
{
    int i, iter=0;
    i=(int)arg;
    cprintf(""I am No.%d movie_sema\n"",i);

    cprintf(""Iter %d, No.%d movie_sema is ready\n"",iter,i);
    do_sleep(SLEEP_TIME);
    semaphore_movie_play(i);
    /* 开始电影放映 */

    do_sleep(SLEEP_TIME);
    semaphore_cinema_end(i);
    /* 结束放映 */

    cprintf(""No.%d movie_sema quit\n"",i);
    return 0;
}

int main() {
    semaphore_movie(0); //播放电影0
    up(&mov[0]);
    people++;
    if (wait[0] == 1) {
        up(&mov[0]);
    }
    down(&move[0]);
}
