//
// Created by tekatod on 4/2/18.
//

#pragma once


#include <QtWidgets/QWidget>
#include <QTimer>
#include <QLabel>
#include <opencv2/videoio.hpp>
#include <QSlider>
#include <QPushButton>
#include <QCameraInfo>
#include <QComboBox>

class cv_camera: public QWidget {
    Q_OBJECT
public:
    explicit cv_camera(QWidget* parent = 0);

public slots:
    void update_picture();
    void change_zoom(int);
    void capture();
    void record();
    void set_up_camera(const QString& camera_name);

private:

    cv::Mat resize_keep_ar(const cv::Mat &input, const cv::Size &dstSize);


private:
    QTimer* m_timer;
    QLabel* m_label;
    cv::VideoCapture m_cap;
    cv::Mat m_image;
    QSlider* m_zoom_slider;
    int m_v4l_device;
    QPushButton* m_cap_button;
    QPushButton* m_record_button;
    bool m_captured{false};
    bool m_record{false};
    QList<QCameraInfo> m_cameras;
    QComboBox* m_combo_box;
    size_t m_index{0};
};

