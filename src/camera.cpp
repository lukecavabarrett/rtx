#include "camera.h"
#include <cstdlib>
#include <cmath>
namespace rtx{
    camera::camera(const vector3& p,const vector3& d,const vector3& v, int w,int h,dtype f,int b)
    {
        position = p;
        depth = d.normalised();
        vertical = v.normalised();
        width_px = w;
        height_px = h;
        fov = f;
        bounces = b;
        aspect_ratio = ((dtype)width_px)/((dtype)height_px);
        width_m = std::tan(fov*M_PIl/360)*2;
        height_m = width_m / aspect_ratio;
        x_step_m = width_m / width_px;
        y_step_m = height_m / height_px;
    }
    ray3 camera::cast_ray(int x, int y)
    {
        dtype x_pos = (x_step_m - width_m) / 2 + x * x_step_m;
        dtype y_pos = (y_step_m + height_m) / 2 - y * y_step_m;
        return ray3(position, (depth+vertical*y_pos+depth.cross(vertical)*x_pos-position).normalised());
    }
    ray3 camera::cast_ray(dtype x, dtype y)
    {
        dtype x_pos = (x_step_m - width_m) / 2 + x * x_step_m;
        dtype y_pos = (y_step_m + height_m) / 2 - y * y_step_m;
        return ray3(position, (depth+vertical*y_pos+depth.cross(vertical)*x_pos-position).normalised());
    }
    color_rgb camera::get_pixel(int x, int y, const scene& s)
    {
        return s.trace(cast_ray(x,y),bounces);
    }
    void camera::render_screen(screen& scr, const scene& s) {
        //#pragma omp parallel for
        for(int y=0;y<height_px;y++)
        {
            for(int x=0;x<width_px;x++)
            {

                scr.put_pixel(y,x,get_pixel(x,y,s).tonemap().to_rgb_le());
                //*(int *)i = ((x>200)?(color_rgb<D>::from_rgb(0x333333)):(color_rgb<D>(0))).tonemap().to_rgb_le();
            }
            if(y%10==0 || y==height_px-1)fprintf(stderr,"\b\b\b\b\b\b\b\b\b\b\b\b%2.2f%%",((dtype)y+1)*100/height_px);
        }
        fprintf(stderr,"\n");
    }
    /*void render(std::string filename, const scene<D>& s)
    {
        unsigned char *tmp_mem = new unsigned char [height_px*width_px*3];
        unsigned char *i = tmp_mem;
        #pragma omp parallel for
        for(int y=0;y<height_px;y++)
        {
            unsigned char *i = tmp_mem + y*width_px*3;
        for(int x=0;x<width_px;x++,i+=3)
        {

            *(int *)i = get_pixel(x,y,s).tonemap().to_rgb_le();
            if(x==0 && y%10==0)fprintf(stderr,"%f %%\n",((D)y+1)*100/height_px);
            //*(int *)i = ((x>200)?(color_rgb<D>::from_rgb(0x333333)):(color_rgb<D>(0))).tonemap().to_rgb_le();
        }
        }
        lodepng_encode24_file(filename.c_str(),tmp_mem,width_px,height_px);
        delete[] tmp_mem;
    }*/
};
