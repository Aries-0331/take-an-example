#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <semaphore.h>

int main()
{
    const int SIZE = 4096;
    const char *name = "OS";
    const char *message_0 = "Hello";
    const char *message_1 = "World!";
    const char *sem_name = "sem";

    int shm_fd;
    void *ptr;
    sem_t *sem;

    /* 创建共享内存对象 */
    shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);
    if (shm_fd == -1) {
        perror("In shm_open");
        exit(1);
    }

    /* 配置共享内存对象的大小 */
    if (ftruncate(shm_fd, SIZE) == -1) {
        perror("In ftruncate");
        exit(1);
    }

    /* 将共享内存对象映射到内存 */
    ptr = mmap(0, SIZE, PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (ptr == MAP_FAILED) {
        perror("In mmap");
        exit(1);
    }

    /* 创建信号量 */
    sem = sem_open(sem_name, O_CREAT, 0666, 0);
    if (sem == SEM_FAILED) {
        perror("In sem_open");
        exit(1);
    }

    /* 将数据写入共享内存对象 */
    sprintf(ptr, "%s", message_0);
    ptr += strlen(message_0);
    sprintf(ptr, "%s", message_1);
    ptr += strlen(message_1);

    /* 通过信号量通知其他进程可以读取数据 */
    if (sem_post(sem) == -1) {
        perror("In sem_post");
        exit(1);
    }

    return 0;
}