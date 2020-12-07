#ifndef __CONVERT_H__
#define __CONVERT_H__

/*
convert_yuv_to_rgb
将YuYv图片转为rgb
参数：yuv已经得到的YuYv图片的首地址
rgb转换后的rgb图片内容的首地址
width是图的宽，
height是图的高，
bps:一般为24
返回值：无
*/
void convert_yuv_to_rgb(void *yuv, void *rgb, unsigned int width, unsigned int height, unsigned int bps);
void convert_rgb_to_jpg_init(void);
/*
convert_rgb_to_jpg_work
将rgb图片转为jpeg
参数：rgb已经得到的rgb图片的首地址
jpeg转换后的jpeg图片内容的首地址
width是图的宽，
height是图的高，
bpp:一般为24
quality一般为80
返回值：图片的大小
*/
int convert_rgb_to_jpg_work(void *rgb, void *jpeg, unsigned int width, unsigned int height, unsigned int bpp, int quality);
void convert_rgb_to_jpg_exit(void);

#endif
