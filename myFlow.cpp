#include<stdio.h>
#include<cv.h>
#include<highgui.h>
#include<cxcore.h>
#include<cvaux.h>

void average_flow(CvMat*, CvMat*);
void saveMat(CvMat*, char*);

int main()
{
    const IplImage* im1 = cvLoadImage("302.png",0);
    const IplImage* im2 = cvLoadImage("303.png",0);
    //int w_s = 10;
    int w = im1->width;
    int h = im1->height;
    //printf("Width = %d\nHeight = %d\n",w,h);
    CvMat* vel = cvCreateMat(h,w,CV_32FC2);
    CvMat* velx = cvCreateMat(h,w,CV_32FC1);
    CvMat* vely = cvCreateMat(h,w,CV_32FC1);
    CvMat* u = cvCreateMat(h/10, w/10, CV_32FC1); // Averaged Optical flows
    CvMat* v = cvCreateMat(h/10, w/10, CV_32FC1);

   //printf("matDimU = %d %d\nMatDimVel = %d %d\n ",cvGetMatSize(u),cvGetMatSize(velx));
   //printf("Ptr = %d %d \n",im1->data.ptr,velx->data.ptr);
    //cvCalcOpticalFlowLK(im1,im2,cvSize(4,4),velx,vely);
    //cvCalcOpticalFlowFarneback(const CvArr* prev, const CvArr* next, CvArr* flow,
    //          double pyr_scale, int levels, int winsize, int iterations, int poly_n, double poly_sigma, int flags) flag means to use Gaussian smoothing
    cvCalcOpticalFlowFarneback(im1, im2, vel,0.5, 1, 2, 2, 2, 0.17, 0);//, iterations, poly_n, poly_sigma
    cvSplit(vel, velx, vely, NULL, NULL);
    average_flow(velx, u);
    average_flow(vely, v);

    /*//cvSave("u.xml", u);
    //cvSave("v.xml", v);*/
    saveMat(u,"ux.m");
    saveMat(v,"vy.m");

/*    CvMat* Big = cvCreateMat(50,50,CV_32FC1);
    cvSetIdentity(Big);
    CvMat* small = cvCreateMat(5,5,CV_32FC1);
    average_flow(Big,small);
    printMat(small);*/
    return 0;
}

void average_flow(CvMat* vel, CvMat* u)
{
    int nh = u->rows;
    int nw = u->cols;
    float avg = -1; // average of a 10x10 window

    for(int y=0; y<nh; y++)
    {
        float* ptrU = (float*)(u->data.ptr + y*u->step);

        for(int x=0; x<nw; x++)
        {
            avg = 0;
            for(int i=0; i<10;i++)
            {
                const float* ptrBig = (const float*)(vel->data.ptr + (y*10+i)*vel->step + x*10*4);

                for(int j=0; j<10; j++)
                    avg += *(ptrBig++);
            }
             avg /= 100;
            *(ptrU++) = avg;
        }
    }
}

void saveMat(CvMat* m, char* fname)
{
    FILE* fp = fopen(fname, "w");

    int r = m->rows;
    int c = m->cols;

    for(int i=0; i<r; i++)
    {
        const float* ptr = (const float*)(m->data.ptr + i*m->step);
        for(int j=0; j<c; j++)
            fprintf(fp, "%f ",*(ptr++));

        fprintf(fp, "\n");
    }

}
