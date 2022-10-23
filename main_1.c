#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#define STB_IMAGE_IMPLEMENTATION
#include "./headers/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "./headers/stb_image_write.h"
unsigned char *uc_arrayNew_1d(int _size)
{
    return (unsigned char *)calloc(_size, sizeof(unsigned char));
}
/*bg :Background matrix
  fg: Foreground matrix
  h:Height
  w:width
  c:channel
  abs_sb_pth :absolute substracted path
  wf:weather_forecast
*/
void  mix_fg_bg(unsigned char *abs_sb_arr, unsigned char *wf ,unsigned char *fg,int w,int h,int c)
{
    for(int i=0;i<h;i++)
    {
        for(int j=0;j<w;j++)
        {
            if( abs_sb_arr[i*w +j]== 255)
            {           
            for(int k=0;k<c;k++)
                {   
                    {
                    wf[i*w*c + j*c + k]=fg[i*w*c + j*c + k ];    
                    }
                }
            }
        }
    }
}
 unsigned char *matrix_substraction(unsigned char *bg,unsigned char *fg,int w,int h,int c)
 {
    //creating a temporary array
    unsigned char *temp_arr = uc_arrayNew_1d(h*w);
    for(int i =0 ;i<h;i++ )
    {
        for(int j=0;j<w;j++)
        {
            int sum=0;
            for(int k=0;k<c;k++)
            {
                sum += abs(bg[i*w*c + j*c +k]-fg[i*w*c+j*c+k]) ;
            }
            // temp_arr[ i*w +j]= sum/3;
            //set threshold (45) to compare binary (0100)
            if (sum/3 <45)
            { 
                temp_arr[i*w+j] = 0;
            }
            else{
                temp_arr[i*w+j]= 255;
            }
        }
    }
    return temp_arr;
    //

 }

int main()
{
    int width,height,channel;
    char path_background[]="./images/background.jpg"; 
    char path_foreground[]="./images/foreground.jpg";
    char path_weather_forecast[]="./images/weather_forecast-resized.jpg";
    char save_abs_substracted_path[]="./images/save_abs_substracted_img2.jpg";
    char final_result[]="./images/weather_forecast_result.jpg";
      // read image data
    unsigned char *background = stbi_load(path_background, &width, &height, &channel, 0);
    unsigned char *foreground = stbi_load(path_foreground, &width, &height, &channel, 0);   
    printf("Width = %d\nHeight = %d\nChannel = %d\n", width, height, channel);


    unsigned char *weather_forecast = stbi_load(path_weather_forecast, &width, &height, &channel, 0);
    if (background == NULL || foreground == NULL || weather_forecast == NULL)
    {
        printf("\nError in loading the image\n");
        exit(1);
    }
    printf("Width = %d\nHeight = %d\nChannel = %d\n", width, height, channel);
    unsigned char *abs_substracted_arr = matrix_substraction(background,foreground,width,height,channel);
    //substracted path ,channel =1
    stbi_write_png(save_abs_substracted_path ,width, height, 1, abs_substracted_arr, width);
    printf("New image saved to %s\n",save_abs_substracted_path);
    //mix bg_fg ,channel =3
    mix_fg_bg(abs_substracted_arr,weather_forecast,foreground,width,height,channel);
    stbi_write_png(final_result,width, height, channel,weather_forecast , width*channel);
    printf("Final result is saved to %s\n",final_result);
}