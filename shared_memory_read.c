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
    const char *sem_name = "sem";

    int shm_fd;
    void *ptr;
    sem_t *sem;

    /* 打开共享内存对象 */
    shm_fd = shm_open(name, O_RDONLY, 0666);
    if (shm_fd == -1) {
        perror("In shm_open");
        exit(1);
    }

    /* 将共享内存对象映射到内存 */
    ptr = mmap(0, SIZE, PROT_READ, MAP_SHARED, shm_fd, 0);
    if (ptr == MAP_FAILED) {
        perror("In mmap");
        exit(1);
    }

    /* 打开信号量 */
    sem = sem_open(sem_name, 0);
    if (sem == SEM_FAILED) {
        perror("In sem_open");
        exit(1);
    }

    /* 等待信号量 */
    if (sem_wait(sem) == -1) {
        perror("In sem_wait");
        exit(1);
    }

    /* 从共享内存中读取数据 */
    printf("%s\n", (char *)ptr);

    /* 删除共享内存对象 */
    if (shm_unlink(name) == -1) {
        perror("In shm_unlink");
        exit(1);
    }

    /* 删除信号量 */
    if (sem_unlink(sem_name) == -1) {
        perror("In sem_unlink");
        exit(1);
    }

    return 0;
}