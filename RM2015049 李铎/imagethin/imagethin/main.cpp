#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <limits>

using namespace cv;
using namespace std;

/**
 * @brief ������ͼ�����ϸ��
 * @param[in] srcΪ����ͼ��,��cvThreshold�����������8λ�Ҷ�ͼ���ʽ��Ԫ����ֻ��0��1,1������Ԫ�أ�0����Ϊ�հ�
 * @param[out] dstΪ��srcϸ��������ͼ��,��ʽ��src��ʽ��ͬ������ǰ��Ҫ����ռ䣬Ԫ����ֻ��0��1,1������Ԫ�أ�0����Ϊ�հ�
 * @param[in] maxIterations���Ƶ���������������������ƣ�Ĭ��Ϊ-1���������Ƶ���������ֱ��������ս��
 */
Mat thinImage(Mat srcImage,int maxIterations = -1 )
{
	Mat dstImage;
	IplImage* src = &IplImage(srcImage);
	IplImage* dst = cvCreateImage(cvGetSize(src), src->depth, src->nChannels);

	CvSize size = cvGetSize(src);
	cvCopy(src,dst);//��src�е����ݿ�����dst��
	int count = 0;	//��¼��������
	while (true)
	{
		count++;
		if(maxIterations!=-1 && count > maxIterations) //���ƴ������ҵ�����������
			break;
		//std::cout << count << ' ';�����������
		vector<pair<int,int> > mFlag; //���ڱ����Ҫɾ���ĵ�
		//�Ե���
		for (int i=0; i<size.height; ++i)
		{
			for (int j=0; j<size.width; ++j)
			{
				//��������ĸ����������б��
				//  p9 p2 p3
				//  p8 p1 p4
				//  p7 p6 p5
				int p1 = CV_IMAGE_ELEM(dst,uchar,i,j);
				int p2 = (i==0)?0:CV_IMAGE_ELEM(dst,uchar,i-1,j);
				int p3 = (i==0 || j==size.width-1)?0:CV_IMAGE_ELEM(dst,uchar,i-1,j+1);
				int p4 = (j==size.width-1)?0:CV_IMAGE_ELEM(dst,uchar,i,j+1);
				int p5 = (i==size.height-1 || j==size.width-1)?0:CV_IMAGE_ELEM(dst,uchar,i+1,j+1);
				int p6 = (i==size.height-1)?0:CV_IMAGE_ELEM(dst,uchar,i+1,j);
				int p7 = (i==size.height-1 || j==0)?0:CV_IMAGE_ELEM(dst,uchar,i+1,j-1);
				int p8 = (j==0)?0:CV_IMAGE_ELEM(dst,uchar,i,j-1);
				int p9 = (i==0 || j==0)?0:CV_IMAGE_ELEM(dst,uchar,i-1,j-1);

				if ((p2+p3+p4+p5+p6+p7+p8+p9)>=2 && (p2+p3+p4+p5+p6+p7+p8+p9)<=6)
				{
					int ap=0;
					if (p2==0 && p3==1) ++ap;
					if (p3==0 && p4==1) ++ap;
					if (p4==0 && p5==1) ++ap;
					if (p5==0 && p6==1) ++ap;
					if (p6==0 && p7==1) ++ap;
					if (p7==0 && p8==1) ++ap;
					if (p8==0 && p9==1) ++ap;
					if (p9==0 && p2==1) ++ap;
					
					if (ap==1)
					{
						if (p2*p4*p6==0)
						{
							if (p4*p6*p8==0)
							{
								//���
								mFlag.push_back(make_pair(i,j));
							}
						}
					}
				}
			}
		}

		//����ǵĵ�ɾ��
		for (vector<pair<int,int> >::iterator i=mFlag.begin(); i!=mFlag.end(); ++i)
		{
			CV_IMAGE_ELEM(dst,uchar,i->first,i->second) = 0;
		}

		//ֱ��û�е����㣬�㷨����
		if (mFlag.size()==0)
		{
			break;
		}
		else
		{
			mFlag.clear();//��mFlag���
		}

		//�Ե���
		for (int i=0; i<size.height; ++i)
		{
			for (int j=0; j<size.width; ++j)
			{
				//��������ĸ����������б��
				//  p9 p2 p3
				//  p8 p1 p4
				//  p7 p6 p5
				int p1 = CV_IMAGE_ELEM(dst,uchar,i,j);
				if(p1!=1) continue;
				int p2 = (i==0)?0:CV_IMAGE_ELEM(dst,uchar,i-1,j);
				int p3 = (i==0 || j==size.width-1)?0:CV_IMAGE_ELEM(dst,uchar,i-1,j+1);
				int p4 = (j==size.width-1)?0:CV_IMAGE_ELEM(dst,uchar,i,j+1);
				int p5 = (i==size.height-1 || j==size.width-1)?0:CV_IMAGE_ELEM(dst,uchar,i+1,j+1);
				int p6 = (i==size.height-1)?0:CV_IMAGE_ELEM(dst,uchar,i+1,j);
				int p7 = (i==size.height-1 || j==0)?0:CV_IMAGE_ELEM(dst,uchar,i+1,j-1);
				int p8 = (j==0)?0:CV_IMAGE_ELEM(dst,uchar,i,j-1);
				int p9 = (i==0 || j==0)?0:CV_IMAGE_ELEM(dst,uchar,i-1,j-1);

				if ((p2+p3+p4+p5+p6+p7+p8+p9)>=2 && (p2+p3+p4+p5+p6+p7+p8+p9)<=6)
				{
					int ap=0;
					if (p2==0 && p3==1) ++ap;
					if (p3==0 && p4==1) ++ap;
					if (p4==0 && p5==1) ++ap;
					if (p5==0 && p6==1) ++ap;
					if (p6==0 && p7==1) ++ap;
					if (p7==0 && p8==1) ++ap;
					if (p8==0 && p9==1) ++ap;
					if (p9==0 && p2==1) ++ap;

					if (ap==1)
					{
						if (p2*p4*p8==0)
						{
							if (p2*p6*p8==0)
							{
								//���
								mFlag.push_back(make_pair(i,j));
							}
						}
					}
				}
			}
		}
		//ɾ��
		for (vector<pair<int,int> >::iterator i=mFlag.begin(); i!=mFlag.end(); ++i)
		{
			CV_IMAGE_ELEM(dst,uchar,i->first,i->second) = 0;
		}

		//ֱ��û�е����㣬�㷨����
		if (mFlag.size()==0)
		{
			break;
		}
		else
		{
			mFlag.clear();//��mFlag���
		}
	}

	for (int i = 0; i<dst->height; ++i)
	{
		for (int j = 0; j<dst->width; ++j)
		{
			if (CV_IMAGE_ELEM(dst, uchar, i, j) == 1)
				CV_IMAGE_ELEM(dst, uchar, i, j) = 255;
		}
	}

	dstImage = Mat(dst);
	return dstImage;
}

int main(int argc, char*argv[])
{
	Mat frame = imread("image.jpg",0);
	IplImage *pSrc = &IplImage(frame);
	if (!pSrc)
	{
		cout << "��ȡ�ļ�ʧ�ܣ�" << endl;
		return -1;
	}
	IplImage *pTemp = cvCreateImage(cvGetSize(pSrc),pSrc->depth,pSrc->nChannels);
	IplImage *pDst = cvCreateImage(cvGetSize(pSrc),pSrc->depth,pSrc->nChannels);
	
	//��ԭͼ��ת��Ϊ��ֵͼ��
	cvThreshold(pSrc,pTemp,128,1,CV_THRESH_BINARY);	
	//ͼ��ϸ��
	//thinImage(pTemp,pDst);
	Mat dst=thinImage(Mat(pTemp));



	Mat disp = Mat(pSrc);
	imshow("dsp",disp);
	imshow("dst",dst);


	waitKey(0);
}