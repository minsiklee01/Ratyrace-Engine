#include "rtweekend.h"

#include "camera.h"
#include "hittable.h"
#include "hittable_list.h"
#include "material.h"
#include "sphere.h"
#include "texture.h"

void bouncing_spheres() {
    hittable_list world;

    auto checker = make_shared<checker_texture>(0.32, color(.2, .3, .1), color(.9, .9, .9));
    world.add(make_shared<sphere>(point3(0,-1000,0), 1000, make_shared<lambertian>(checker)));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = random_double();
            point3 center(a + 0.9*random_double(), 0.05, b + 0.9*random_double());

            if ((center - point3(4, 0.2, 0)).length() > 0.9) {
                shared_ptr<material> sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = color::random() * color::random();
                    sphere_material = make_shared<lambertian>(albedo);
                    auto center2 = center + vec3(0, random_double(0,.5), 0);
                    world.add(make_shared<sphere>(center, center2, 0.2, sphere_material));
                } else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = color::random(0.5, 1);
                    auto fuzz = random_double(0, 0.5);
                    sphere_material = make_shared<metal>(albedo, fuzz);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                } else {
                    // glass
                    sphere_material = make_shared<dielectric>(1.5);
                    auto center2 = center + vec3(0, random_double(0,1), 0); 
                    double radius = random_double(0.02, 0.08);
                    world.add(make_shared<sphere>(center, center2, radius, sphere_material));
                }
            }
        }
    }

    // glass
    auto material1 = make_shared<dielectric>(1.5);
    world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

    // diffused
    auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
    world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

    // metal
    auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.3);
    world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));

    camera cam;

    cam.aspect_ratio      = 16.0 / 9.0;
    cam.image_width       = 400;
    cam.samples_per_pixel = 10;
    cam.max_depth         = 50;

    cam.vfov     = 20;
    cam.lookfrom = point3(13,2,3);
    cam.lookat   = point3(0,0,0);
    cam.vup      = vec3(0,1,0);

    cam.defocus_angle = 0.6;
    cam.focus_dist    = 10.0;

    cam.render(world);
}

void checkered_spheres() {
    hittable_list world;

    auto checker = make_shared<checker_texture>(0.32, color(.2, .3, .1), color(.9, .9, .9));

    world.add(make_shared<sphere>(point3(0,-10, 0), 10, make_shared<lambertian>(checker)));
    world.add(make_shared<sphere>(point3(0, 10, 0), 10, make_shared<lambertian>(checker)));

    camera cam;

    cam.aspect_ratio      = 16.0 / 9.0;
    cam.image_width       = 400;
    cam.samples_per_pixel = 10;
    cam.max_depth         = 50;

    cam.vfov     = 20;
    cam.lookfrom = point3(13,2,3);
    cam.lookat   = point3(0,0,0);
    cam.vup      = vec3(0,1,0);

    cam.defocus_angle = 0;

    cam.render(world);
}

void simple_light() {
    hittable_list world;

    auto material = make_shared<lambertian>(color(0.4, 0.2, 0.1));
    world.add(make_shared<sphere>(point3(0,-1000,0), 1000, material));
    world.add(make_shared<sphere>(point3(0,2,0), 2, material));

    auto difflight = make_shared<diffuse_light>(color(7,7,7));
    world.add(make_shared<sphere>(point3(2,4,-2), 1, difflight));

    camera cam;

    cam.aspect_ratio      = 16.0 / 9.0;
    cam.image_width       = 800;
    cam.samples_per_pixel = 300;
    cam.max_depth         = 50;
    cam.background        = color(0.01,0.01,0.01);

    cam.vfov     = 20;
    cam.lookfrom = point3(26,3,6);
    cam.lookat   = point3(0,2,0);
    cam.vup      = vec3(0,1,0);

    cam.defocus_angle = 0;

    cam.render(world);
}

void earth() {
    hittable_list world;


    auto earth_texture = make_shared<image_texture>("earthmap.jpg");
    auto earth_surface = make_shared<lambertian>(earth_texture);
    auto globe = make_shared<sphere>(point3(0,-1000,0), 1000, earth_surface);
    world.add(globe);

    // snowman
    // z, y, x
    auto snow_texture = make_shared<image_texture>("snow.jpg");
    auto snow_surface = make_shared<lambertian>(snow_texture);
    auto snow_body = make_shared<sphere>(point3(0,0.7,0), 0.7, snow_surface);
    world.add(snow_body);
    auto snow_top = make_shared<sphere>(point3(0,1.6,0.1), 0.5, snow_surface);
    world.add(snow_top);
    auto snow_eye = make_shared<lambertian>(color(0.0, 0.0, 0.0));
    world.add(make_shared<sphere>(point3(0.4, 1.8, -0.03), 0.07, snow_eye));
    auto snow_eye2 = make_shared<lambertian>(color(0.0, 0.0, 0.0));
    world.add(make_shared<sphere>(point3(0.4, 1.8, 0.4), 0.06, snow_eye));

    auto snow_button = make_shared<lambertian>(color(0.1, 0.2, 0.1));
    world.add(make_shared<sphere>(point3(0.8, 1, 0.1), 0.08, snow_eye));
    auto snow_button2 = make_shared<lambertian>(color(0.1, 0.2, 0.1));
    world.add(make_shared<sphere>(point3(0.8, 0.6, 0.1), 0.1, snow_eye));


    // balls
    auto redl = make_shared<diffuse_light>(color(2,1,1));
    auto greenl = make_shared<diffuse_light>(color(1,2,1));
    auto redMetal = make_shared<metal>(color(0.4, 0.2, 0.2), 0.2);
    world.add(make_shared<sphere>(point3(-0.4,0.4,-1.2), 0.4, redMetal));

    auto redlight = make_shared<lambertian>(color(0.8,0.3,0.3));
    world.add(make_shared<sphere>(point3(1,0.3,-0.6), 0.3, redl));

    auto greenlight = make_shared<lambertian>(color(0.3,0.8,0.3));
    world.add(make_shared<sphere>(point3(-1.2,0.4,1.4), 0.4, greenl));

    auto greenMetal = make_shared<metal>(color(0.2, 0.4, 0.2), 0.2);
    world.add(make_shared<sphere>(point3(0.1,0.2,1), 0.16, greenMetal));

    world.add(make_shared<sphere>(point3(0.8,0.36,2.4), 0.33, redMetal));
    world.add(make_shared<sphere>(point3(0.8,0.2,-2.2), 0.24, greenMetal));


    // snowfall
    for (int a = -4; a < 4; a++) {
        for (int b = -4; b < 4; b++) {
            point3 center(a + 0.9*random_double(), random_double(0, 2), b + 0.9*random_double());

            if ((center - point3(4, 0.2, 0)).length() > 0.9) {
                shared_ptr<material> sphere_material;

                auto center2 = center + vec3(0, random_double(0,0.3), 0);
                world.add(make_shared<sphere>(center, center2, random_double(0.03,0.1), snow_surface));  
            }
        }
    }

    // glass globe
    auto glass = make_shared<dielectric>(1.33);
    world.add(make_shared<sphere>(point3(0, 0.1, 0), 6, glass));

    camera cam;

    cam.aspect_ratio      = 16.0 / 9.0;
    cam.image_width       = 800;
    cam.samples_per_pixel = 500;
    cam.max_depth         = 50;

    cam.vfov              = 22;
    cam.lookfrom          = point3(20,3,2);
    cam.lookat            = point3(0,2.6,-0.1);
    cam.vup               = vec3(0,1,0);
    cam.tilt_angle = 0;
    cam.background        = color(0.40, 0.50, 0.70);

    cam.focus_dist        = 20.0;
    cam.defocus_angle     = 3.3; 

    cam.render(world);
}

int main() {
    switch (4) {
        case 1:  bouncing_spheres();  break;
        case 2:  checkered_spheres(); break;
        case 3:  earth();             break;
        case 4:  simple_light();      break;
    }
}