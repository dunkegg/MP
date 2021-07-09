#pragma once
#include <chrono>
#include <functional>
#include <ios>
#include <iostream>
#include <stdint.h>
#include <thread>
#include "l2.h"
#include "trajedit.h"
#include "SRP.h"
#include "Magv.h"
#include "massage.h"
#include <QtWidgets/QMainWindow>
#include "ui_MPw.h"
#include "m.h"

using namespace srp;
using namespace std;
using namespace massage;
using namespace admittance;

//void th();

class MPw : public QMainWindow
{
    Q_OBJECT

public slots:
    void on_init_clicked();
    void on_agv_clicked();
    void on_enter_m_clicked();
    void on_TE_clicked();
    void on_down_clicked();
public:
    L2 massageWindow;
    Tedit trajEdit;
public:
    MPw(QWidget *parent = Q_NULLPTR);
    bool initializer();
    bool agv_admit_enable();
    bool agv_admit_disable();
    void agv_admit();
    void color_init();
    //agv
    void onConnectFinised(bool result);
    void onDisconnected();
    void onNewSystemState(const proto::SystemState& s);
    void onLaserScan(srp::PoseVector_t points);
    void onNotification(const proto::Notification& notifiation);
    bool agv_off();
    //

private:
    Ui::MPwClass ui;
    //Massage mas;
    Magv agv;
    SRP srp;

    const uint32_t move_to_station_1_seq = 88888; // 移动到站点的seq
    bool move_to_station_1_finished = false; // 标记是否移动到站点1结束
    bool connected = false;                  // 标记连接是否建立
    uint16_t current_station_no = 0;         // 当前站点编号
    double actTorque[6];
    bool agv_on = false;
};
