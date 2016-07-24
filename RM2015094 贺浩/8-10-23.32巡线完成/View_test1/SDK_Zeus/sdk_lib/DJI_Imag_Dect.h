#ifndef DJI_IMAG_DECT_H
#define DJI_IMAG_DECT_H
extern int Start_ImagDect_Thread(void);
//extern void * Imag_Dect_Function(void * arg);


typedef struct
{
    int besttheta;
    int bestrho;
}image_data;
extern image_data   drone_image_data;
#endif // DJI_IMAG_DECT_H
