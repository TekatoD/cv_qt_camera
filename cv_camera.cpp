//
// Created by tekatod on 4/2/18.
//

#include <opencv2/imgproc.hpp>
#include <cv.hpp>
#include <QtWidgets/QGridLayout>
#include <fcntl.h>
#include <linux/videodev2.h>
#include <sys/ioctl.h>
#include <iostream>
#include "cv_camera.h"


cv_camera::cv_camera(QWidget* parent) : QWidget(parent), m_cap(0) {
    m_cap.set(cv::CAP_PROP_FRAME_WIDTH, 1920);
    m_cap.set(cv::CAP_PROP_FRAME_HEIGHT, 1080);
    m_v4l_device = open("/dev/video0", O_RDWR, 0);
    m_zoom_slider = new QSlider(Qt::Horizontal, this);
    m_zoom_slider->setMinimum(100);
    m_zoom_slider->setMaximum(500);
    m_cap_button = new QPushButton("capture");
    m_record_button = new QPushButton("record");
    m_timer = new QTimer(this);
    m_label = new QLabel(this);
    m_label->setSizePolicy(QSizePolicy::Expanding,
                                    QSizePolicy::Expanding);
    m_label->setAlignment(Qt::AlignCenter);
    this->setMinimumSize(640, 480);
//    m_label->setMinimumSize(1280, 720);
    auto* layout1 = new QGridLayout;
    layout1->addWidget(m_label);
    layout1->addWidget(m_zoom_slider);
    layout1->addWidget(m_cap_button);
    layout1->addWidget(m_record_button);
    this->setLayout(layout1);
    QObject::connect(m_timer, SIGNAL(timeout()), this, SLOT(update_picture()));
    QObject::connect(m_zoom_slider, SIGNAL(valueChanged(int)), this, SLOT(change_zoom(int)));
    QObject::connect(m_cap_button, SIGNAL(clicked()), this, SLOT(capture()));
    QObject::connect(m_record_button, SIGNAL(clicked()), this, SLOT(record()));
    m_timer->start(5);

}

void cv_camera::update_picture() {
    m_captured = false;
    m_cap >> m_image;
    m_captured = true;
    cv::Mat dst;
    auto t_size = this->size();
    dst = this->resize_keep_ar(m_image, cv::Size(t_size.width() - 50, t_size.height() - 50));
    cv::cvtColor(dst, dst, CV_BGR2RGB);
    QImage image1 = QImage((uchar*) dst.data, dst.cols, dst.rows, dst.step, QImage::Format_RGB888);


    //show Qimage using QLabel
    m_label->setPixmap(QPixmap::fromImage(image1));
    m_label->adjustSize();
    if(m_record) {
        cv::imwrite("image_" + std::to_string(m_index) + ".jpg", m_image);
        m_index++;
    }

//    m_label->show();

}

cv::Mat cv_camera::resize_keep_ar(const cv::Mat& input, const cv::Size& dstSize)
{
    cv::Mat output;

    double h1 = dstSize.width * (input.rows/(double)input.cols);
    double w2 = dstSize.height * (input.cols/(double)input.rows);
    if( h1 <= dstSize.height) {
        cv::resize( input, output, cv::Size(dstSize.width, h1));
    } else {
        cv::resize( input, output, cv::Size(w2, dstSize.height));
    }

    int top = (dstSize.height-output.rows) / 2;
    int down = (dstSize.height-output.rows+1) / 2;
    int left = (dstSize.width - output.cols) / 2;
    int right = (dstSize.width - output.cols+1) / 2;

    cv::copyMakeBorder(output, output, top, down, left, right, cv::BORDER_CONSTANT, cv::Scalar(0, 0, 0));

    return output;
}

void cv_camera::change_zoom(int zoom) {
    struct v4l2_control v4l2_ctrl = {V4L2_CID_ZOOM_ABSOLUTE, zoom};
    ioctl(m_v4l_device, VIDIOC_S_CTRL, &v4l2_ctrl);
}

void cv_camera::capture() {
    if(m_captured) {
        cv::imwrite("image_" + std::to_string(m_index) + ".jpg", m_image);
        m_index++;
    }

}

void cv_camera::record() {
    if(!m_record) {
        m_record = true;
        m_record_button->setText("stop");
        m_cap_button->setDisabled(true);
    } else {
        m_record = false;
        m_record_button->setText("record");
        m_cap_button->setDisabled(false);
    }
}

