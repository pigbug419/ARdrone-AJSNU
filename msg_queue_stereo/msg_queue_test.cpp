#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define KEY_ID          8800

#define CAMERA_WIDTH    320
#define CAMERA_HEIGHT   240

using namespace cv;
using namespace std;

typedef struct {
    long msgtype;
    int width, height;
    unsigned char mData[CAMERA_WIDTH*CAMERA_HEIGHT];
} MSG_BUF;


int main()
{
    key_t key_id = KEY_ID;
    Mat disp8;
    MSG_BUF mbuf;

    mbuf.msgtype = KEY_ID;
    key_id = msgget(key_id, IPC_CREAT | 0666);
    if(key_id == -1) {
        printf("Don't create msg\n");
        return -1;
    }
    while(1)
    {
        if (msgrcv( key_id, (void *)&mbuf, sizeof(MSG_BUF), KEY_ID, 0) == -1)
        {
            printf("Don't receive msg queue\n");
            break;
        }
        else
        {
            printf("msg width %d   msg height %d\n", mbuf.width, mbuf.height);
            
            disp8 = Mat(Size(mbuf.width, mbuf.height), CV_8UC1, mbuf.mData);

            imshow("receive data", disp8);
            char ch = (char)waitKey(10);
            if(ch == 'q')
                break;
        }

    }
    return 1;
}
