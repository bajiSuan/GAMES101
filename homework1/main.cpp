#include "Triangle.hpp"
#include "rasterizer.hpp"
#include <eigen3/Eigen/Eigen>
#include <iostream>
#include <opencv2/opencv.hpp>

constexpr double MY_PI = 3.1415926;

Eigen::Matrix4f get_view_matrix(Eigen::Vector3f eye_pos)
{
    Eigen::Matrix4f view = Eigen::Matrix4f::Identity();

    Eigen::Matrix4f translate;
    translate << 1, 0, 0, -eye_pos[0], 
                0, 1, 0, -eye_pos[1], 
                0, 0, 1, -eye_pos[2], 
                0, 0, 0, 1;

    view = translate * view;

    return view;
}

Eigen::Matrix4f get_model_matrix(float rotation_angle)
{
    Eigen::Matrix4f model = Eigen::Matrix4f::Identity();

    // TODO: Implement this function
    // Create the model matrix for rotating the triangle around the Z axis.
    // Then return it.

    float redian = rotation_angle / 180.0f * MY_PI;

    model << std::cos(redian), -std::sin(redian), 0, 0, 
            std::sin(redian), std::cos(redian), 0, 0, 
            0, 0, 1, 0,
            0, 0, 0, 1;

    return model;
}


Eigen::Matrix4f get_projection_matrix(float eye_fov, float aspect_ratio,
                                      float zNear, float zFar)
{
    // Students will implement this function

    Eigen::Matrix4f projection = Eigen::Matrix4f::Identity();

    // TODO: Implement this function
    // Create the projection matrix for the given parameters.
    // Then return it.

    float t = std::tan(eye_fov/2) * std::abs(zNear);
    float b = -t;
    float r = t * aspect_ratio;
    float l = -r;
    float n = zNear;
    float f = zFar;


    projection << 2*n/(r-l), 0, (l+r)/(l-r), 0,
                0, 2*n/(t-b), (b+t)/(b-t), 0,
                0, 0, (n+f)/(n-f), 2*n*f/(f-n),
                0, 0, 1, 0;

    return projection;
}

Eigen::Matrix4f get_rotation(Vector3f axis, float angle) 
{
    Eigen::Matrix3f model3f;
    Eigen::Matrix4f model4f;
    Eigen::Vector3f n;
    Eigen::Matrix3f N;

    float redian = angle / 180.0f * MY_PI;

    // n << axis.x(), axis.y(), axis.z(), 0;
    N << 0, -axis.z(), axis.y(),
        axis.z(), 0, -axis.x(),
        -axis.y(), axis.x(), 0;

    model3f = Eigen::Matrix3f::Identity() * std::cos(redian)
        + (1 - std::cos(redian)) * axis * axis.transpose()
        + std::sin(redian) * N;

    model4f << model3f, MatrixXf::Zero(3,1),
                0, 0, 0, 1;

    // std::cout << model3f << std::endl;
    std::cout << model4f << std::endl;

    return model4f;
}

int main(int argc, const char** argv)
{
    float angle = 0;
    bool command_line = false;
    std::string filename = "output.png";

    if (argc >= 3) {
        command_line = true;
        angle = std::stof(argv[2]); // -r by default
        if (argc == 4) {
            filename = std::string(argv[3]);
        }
        else
            return 0;
    }

    rst::rasterizer r(700, 700);

    Eigen::Vector3f eye_pos = {0, 0, 5};

    std::vector<Eigen::Vector3f> pos{{2, 0, -2}, {0, 2, -2}, {-2, 0, -2}};

    std::vector<Eigen::Vector3i> ind{{0, 1, 2}};

    auto pos_id = r.load_positions(pos);
    auto ind_id = r.load_indices(ind);

    int key = 0;
    int frame_count = 0;

    if (command_line) {
        r.clear(rst::Buffers::Color | rst::Buffers::Depth);

        r.set_model(get_model_matrix(angle));
        r.set_view(get_view_matrix(eye_pos));
        r.set_projection(get_projection_matrix(45, 1, 0.1, 50));

        r.draw(pos_id, ind_id, rst::Primitive::Triangle);
        cv::Mat image(700, 700, CV_32FC3, r.frame_buffer().data());
        image.convertTo(image, CV_8UC3, 1.0f);

        cv::imwrite(filename, image);

        return 0;
    }


    Eigen::Vector3f axis = {0, 0, 1};

    while (key != 27) {
        r.clear(rst::Buffers::Color | rst::Buffers::Depth);

        angle = std::fmod(360.0f + angle, 360.0f);
        // r.set_model(get_rotation(axis, angle));
        r.set_model(get_model_matrix(angle));
        r.set_view(get_view_matrix(eye_pos));
        r.set_projection(get_projection_matrix(45, 1, 0.1, 50));

        r.draw(pos_id, ind_id, rst::Primitive::Triangle);

        cv::Mat image(700, 700, CV_32FC3, r.frame_buffer().data());
        image.convertTo(image, CV_8UC3, 1.0f);
        cv::imshow("image", image);

        key = cv::waitKey(10);  

        std::cout << "frame count: " << frame_count++;
        std::cout << "    ";
        std::cout << "angle: " << angle;
        std::cout << "    ";
        std::cout << "key: " << key;
        std::cout << std::endl;

        if (key == 'a') {
            angle += 10;
        } else if (key == 'd') {
            angle -= 10;
        }
    }

    return 0;
}
