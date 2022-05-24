#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "pthread.h"
#include "math.h"
#include "errno.h"
#include "unistd.h"
#include "time.h"

#define XD printf("Tester buddy\n");
#define XDD printf("Something gone wrong!\n");

struct Image{
    int width;
    int height;
    int max_pixel_value;
    int *values;
};

struct Context{
    struct Image* image;
    int threadsNo;
    int n;
};

double calc_time(double* time, double start){
    if(start==0) {
        struct timespec *tmp = calloc(1, sizeof(struct timespec));
        clock_gettime(CLOCK_REALTIME, tmp);
        double ans = (double)tmp->tv_sec+(double)tmp->tv_nsec/1000000000;
        free(tmp);
        return ans;
    }
    else{
        struct timespec *tmp = calloc(1, sizeof(struct timespec));
        clock_gettime(CLOCK_REALTIME, tmp);
        *time = (double)tmp->tv_sec+(double)tmp->tv_nsec/1000000000-start;
        free(tmp);
        return 0;
    }
}

struct Image load_image(char* fileName){
    FILE *fp = fopen(fileName, "r");

    struct Image image;
    char buff[1000];

    fgets(buff, 1000, fp);
    char* saveptr;
    char* token;


    fgets(buff, 1000, fp);
    token = strtok_r(buff, " \t\n", &saveptr); //width
    image.width= strtol(token, NULL, 10);

    token = strtok_r(NULL, " \t\n", &saveptr); //height
    image.height= strtol(token, NULL, 10);

    image.values = malloc(image.width*image.height*sizeof(int));


    fgets(buff, 1000, fp);
    token = strtok_r(buff, " \t\n", &saveptr); //max
    image.max_pixel_value= strtol(token, NULL, 10);

    int idx = 0;
    while(fgets(buff, 1000, fp)!=NULL) {
        token = strtok_r(buff, " \t\n", &saveptr);
        while (token != NULL) {
            image.values[idx] = strtol(token, NULL, 10);
            idx++;

            token = strtok_r(NULL, " \t\n", &saveptr);
        }
    }
    fclose(fp);
    return image;
}

void save_image(char* saveName, struct Image image){
    FILE* fp = fopen(saveName, "w");

    fprintf(fp, "P2\n%d  %d\n%d\n", image.width, image.height, image.max_pixel_value);

    for(int i=0;i<image.height*image.height-1;i++){
        fprintf(fp,"%d  ", image.values[i]);
    }
    fprintf(fp,"%d\n", image.values[image.height*image.height-1]);

    fclose(fp);
}

void* calc_numbers(void* arg){
    struct Context* context = (struct Context*) arg;
    struct Image* image = context->image;
    double start = calc_time(NULL, 0);

    for(int i=0;i<image->width*image->height;i++){
        int tmp = image->values[i];
        if(image->values[i]>image->max_pixel_value/2)
            tmp = image->max_pixel_value - image->values[i];
        if(tmp%context->threadsNo==context->n){
            image->values[i] = image->max_pixel_value - image->values[i];
        }
    }
    double * thread_time = malloc(sizeof(double));
    calc_time(thread_time, start);
    pthread_exit((void*)thread_time);
}

void* calc_blocks(void* arg){
    struct Context* context = (struct Context*) arg;
    struct Image* image = context->image;
    double start = calc_time(NULL, 0);

    int left_bound = context->n*ceil(image->width/context->threadsNo);
    int right_bound = (context->n+1)*ceil(image->width/context->threadsNo);
    for(int i=left_bound;i<right_bound;i++){
        for(int j=0;j<image->height;j++) {
            image->values[j*image->width+i] = image->max_pixel_value - image->values[j*image->width+i];
        }
    }
    double * thread_time = malloc(sizeof(double));
    calc_time(thread_time, start);
    pthread_exit((void*)thread_time);
}

struct Image reverse(struct Image image, int mode, int threadsNo){
    double start = calc_time(NULL, 0);
    pthread_t *threads = calloc(threadsNo, sizeof(pthread_t));
    struct Context *context = calloc(threadsNo, sizeof(struct Context));
    for(int i=0;i<threadsNo;i++){
        context[i].image = &image;
        context[i].threadsNo = threadsNo;
        context[i].n = i;
        if(mode==0) {
            if ((pthread_create(&threads[i], NULL, calc_numbers, (void *) &context[i])) != 0)
                XDD
        }
        else
            if((pthread_create(&threads[i], NULL, calc_blocks, (void*)&context[i]))!=0)
                XDD
//        usleep(1000);
    }

    for(int i=0;i<threadsNo;i++){
        double* thread_time;
        pthread_join(threads[i], (void*)&thread_time);
        printf("    Thread time: %f\n", *thread_time);
        free(thread_time);
    }

    double total_time;
    calc_time(&total_time, start);
    printf("         Total time: %f\n", total_time);

    free(threads);
    free(context);
    return image;
}

int main(int argc, char** argv){
    if(argc!=5){
        printf("Invalid args no!\n");
        return -1;
    }
    int threadsNo = strtol(argv[1],NULL, 10);
    int mode;
    if(!strcmp(argv[2],"numbers"))
        mode = 0;
    else if(!strcmp(argv[2],"blocks"))
        mode = 1;
    else {
        printf("Invalid command!\n");
        return -1;
    }
    char* fileName = argv[3];
    char* saveName = argv[4];

//    printf("from %s to %s with %d threads in mode %d\n", fileName, saveName, threadsNo, mode);

//    struct Image image = load_image(fileName);
//    printf("%dx%d with max %d %d\n", image.width, image.height, image.max_pixel_value, image.values[0]);

    save_image(saveName, reverse(load_image(fileName), mode, threadsNo));

    return 0;
}