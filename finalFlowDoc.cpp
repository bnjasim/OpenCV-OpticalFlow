#include<stdio.h>
#include<cv.h>
#include<highgui.h>

unsigned short *vw;

void updatevw(CvMat* velx, CvMat* vely);
void savevw(FILE *fp);
void setZerovw();
char* concat(char* a, char* b, char* c);

int main()
{
     char  *location = "D:/Study/ME/Major/Dataset/mv2_0";
     char num[20][3] = {"01","02","03","04","05","06","07","08","09","10","11","12","13","14","15","16","17","18","19","20"};
     FILE *fp = fopen("BagOfWords.txt", "w");
     if(fp == NULL) {
        printf("File couldn't be opened\n");
        return 0;
     }

     int totalFrames; // #frames
     int curr_frame = 1;
     int nextVidToLoad = 1;

     char *toLoad = concat(location, (char*)num[0],".avi");
     CvCapture *capture = cvCaptureFromFile(toLoad);
     if(capture==NULL)  {
     printf("first VIDEO was not  READ properly :(\n");
        return 1; }
     else
        printf("first VIDEO READ successfully\n");
     totalFrames  =   ( int )cvGetCaptureProperty( capture, CV_CAP_PROP_FRAME_COUNT ) - 1;
     int W =  (int)cvGetCaptureProperty( capture, CV_CAP_PROP_FRAME_WIDTH);
     int H  =  (int)cvGetCaptureProperty( capture, CV_CAP_PROP_FRAME_HEIGHT);
     IplImage* frame = cvQueryFrame( capture );
     IplImage *im = cvCreateImage(cvSize(W,H),8,1);
     IplImage *im_p = cvCreateImage(cvSize(W,H),8,1);
     cvConvertImage(frame,im_p); //GrayScale

     CvMat* velx = cvCreateMat(H,W,CV_32FC1);
     CvMat* vely = cvCreateMat(H,W,CV_32FC1);

     vw = (unsigned short*)calloc(13824, sizeof(short));
     if(vw == NULL) {
        printf("Memory couldn't be allocated for vw\nQuitting\n");
        return 1;
     }
     else printf("Memory allocated for vw\n");


     for(int docNo = 0; docNo < 550; docNo++)
     {

            if(curr_frame+300 < totalFrames) // :) we can make one more document from the same video
            {
                for(int i = 0; i < 300; i++)
                {
                    IplImage* frame = cvQueryFrame( capture );
                    cvConvertImage(frame,im); //GrayScale
                    //Code for finding the bag of words for this document
                    cvCalcOpticalFlowLK(im_p,im,cvSize(3,3),velx,vely);
                    updatevw(velx,vely);
                    cvCopy(im, im_p);
                }

                curr_frame += 300;
                printf("document %d is read, last frame = %d\n",docNo+1, curr_frame);
                //One document is over
                savevw(fp);
                setZerovw();
            }
            else
            {
                // Read from the current video till the last frame
                // and then load the next video, and read the remaining
                //printf("\nNot enough frames in the current video, but still reading till the last frame !\n");
                int rem = curr_frame+300 - totalFrames;
                for(;curr_frame<totalFrames; curr_frame++)
                {
                    //Same as if part except vw is not setzero
                    IplImage* frame = cvQueryFrame( capture );
                    cvConvertImage(frame,im); //GrayScale
                    //Code for finding the bag of words for this document
                    cvCalcOpticalFlowLK(im_p,im,cvSize(3,3),velx,vely);
                    updatevw(velx,vely);
                    cvCopy(im, im_p);
                }
                // Load the next video

                char *toLoad = concat(location, (char*)num[nextVidToLoad++],".avi");
                cvReleaseCapture(&capture);
                capture = cvCaptureFromFile(toLoad);
                if(capture==NULL)  {
                  printf("\nVIDEO %d was not  READ properly :(\n",nextVidToLoad);
                  continue; }
                else
                    printf("\nVIDEO %d READ successfully\nlast frame read was %d\nRemaining to read = %d\n",nextVidToLoad, curr_frame,rem);
                totalFrames  =   ( int )cvGetCaptureProperty( capture, CV_CAP_PROP_FRAME_COUNT ) - 1;

                for(curr_frame = 0; curr_frame < rem; curr_frame++)
                {
                    //printf("\nStarted Reading from the new Video . curr_frame = %d !\n",curr_frame+1);
                    IplImage* frame = cvQueryFrame( capture );
                    cvConvertImage(frame,im); //GrayScale
                    //Code for finding the bag of words for this document
                    //if(im == NULL) printf("im is NULL\n");
                    //if(im_p == NULL) printf("im_p is NULL\n");
                    //if(curr_frame == 107) continue;
                    cvCalcOpticalFlowLK(im_p,im,cvSize(3,3),velx,vely);
                    updatevw(velx,vely);
                    cvCopyImage(im, im_p);
                }
                printf("document %d is read, last frame = %d\n",docNo+1, curr_frame);
                savevw(fp);
                setZerovw();

            }

    }


    return 0;
}


void updatevw(CvMat* velx, CvMat* vely)
{
    CvMat* temp = cvCreateMatHeader(10,10,CV_32FC1);
    float u, v;
    for(int i = 0;i<48; i++)
    {
        for(int j = 0; j<72; j++)
        {
            CvRect r = cvRect(j*10, i*10, 10, 10);
            cvGetSubRect(velx, temp, r);
            u = cvAvg(temp).val[0];
            cvGetSubRect(vely, temp, r);
            v = cvAvg(temp).val[0];

            int pos;

            if(sqrt(u*u + v*v) > 0.4) // Threshold for optical flow
            {
                if( fabs(u) > fabs(v) ) // either right or left
                {
                    if(u > 0)
                        pos = 0;
                    else
                        pos = 1;
                }
                else
                {
                    if(v > 0)
                        pos = 2;
                    else
                        pos = 3;
                }
                int offset = (i*72+j) * 4 + pos;
                *(vw + offset) = *(vw + offset) + 1;
            }

        }
    }
}


void setZerovw()
{
    for(int i = 0; i < 13824; i++)
        *(vw+i) = 0;
}



void savevw(FILE *fp)
{
    for(int i=0; i<13824; i++)
    {
        //if(*(vw+i) > 0)
            fprintf(fp, "%d ",*(vw+i));

    }
    fprintf(fp,"\n");
}




char* concat(char* a, char* b, char* c)
{
    int la = strlen(a);
    int lb = strlen(b);
    int lc = strlen(c);
    int l = la + lb + lc + 1;
 //   printf("length = %d\n",la);
    char* temp = (char*)malloc(l);
    char* temp1 = temp;

    for(int i = 0; i < la; i++)
        *(temp1++) = *(a++);

    for(int i = 0; i < lb; i++)
        *(temp1++) = *(b++);

    for(int i = 0; i < lc; i++)
        *(temp1++) = *(c++);

    *(temp1) = '\0';

    return temp;
}

