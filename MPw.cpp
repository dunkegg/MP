#include "MPw.h"

#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif

using namespace srp;
using namespace std;
using namespace massage;
using namespace admittance;

//void th()
//{
//    cout << "in" << endl;
//}
MPw::MPw(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    //std::thread(th);


    connect(ui.init, SIGNAL(clicked()), this, SLOT(on_init_clicked()));
    connect(ui.agv, SIGNAL(clicked()), this, SLOT(on_agv_clicked()));
    connect(ui.enterm, SIGNAL(clicked()), this, SLOT(on_enter_m_clicked()));
    connect(ui.TE, SIGNAL(clicked()), this, SLOT(on_TE_clicked()));
    connect(ui.down, SIGNAL(clicked()), this, SLOT(on_down_clicked()));
    color_init();
    //cout << "over" << endl;

}

bool MPw::initializer()
{
    srp.setConnectFinishedCallback(
        std::bind(&MPw::onConnectFinised, this,placeholders::_1));
    srp.setDisconnectCallback(std::bind(&MPw::onDisconnected,this));
    srp.setSystemStateCallback(std::bind(&MPw::onNewSystemState, this, placeholders::_1));
    srp.setLaserScanCallback(std::bind(&MPw::onLaserScan, this, placeholders::_1));
    srp.setNotificationCallback(std::bind(&MPw::onNotification, this,placeholders::_1));
    cout << "111" << endl;
    bool ret = srp.connect("192.168.71.50", 5001, 3);
    if (ret) {
        cout << "connect send succeed!" << endl;
    }
    else {
        cout << "connect send failed!" << endl;
        return -1;
    }

    if (!connected) {
        return -1;
    }

    auto reply = srp.login("admin", "admin");
    if (reply.replyCode_ == Reply_Ok) {
        cout << "login succeed!" << endl;
    }
    else {
        cout << "login failed!" << endl;
        srp.disconnect();
        return -1;
    }

    this_thread::sleep_for(
        chrono::milliseconds(1000)); // 延时等待系统状态、雷达等状态的回调函数
    cout << "1000h" << endl;
    std::vector<uint16_t> values;
    reply = srp.readInputRegesters(30151, 1, values); // 读取输入寄存器的值
    if (reply.replyCode_ == Reply_Ok) {
        cout << "read input regesters ok! --- values ---" << endl;
        for (auto value : values) {
            cout << value << endl;
        }
        std::cout << "--- end ---" << endl << endl;
    }
    else {
        cout << "read input regesters error!" << endl;
        //srp.disconnect();
        //return -1;
    }

    std::string map_name;
    reply = srp.getCurrentMap(map_name);
    if (reply.replyCode_ == Reply_Ok) {
        cout << "current map is " << map_name << endl;
    }
    else {
        cout << "get current map name error!" << endl;
        //srp.disconnect();
        //return -1;
    }


    //简单动作
    reply = srp.cancelEmergencyState(); // 解除急停
    if (reply.replyCode_ == Reply_Ok) {
        cout << "clean emergency succeed" << endl;
    }
    else {
        cout << "clean emergency failed" << endl;
        //srp.disconnect();
        //return -1;
    }
    //this_thread::sleep_for(chrono::milliseconds(3000));
    srp.enableManualControl(false); // 关闭手动控制模式
    this_thread::sleep_for(chrono::milliseconds(100));
    ////移动到站点，移动到站点前先用chip试一下是否具备移动到站点的条件
    //if (current_station_no != 1) {
    //    reply = srp.moveToStation(1,
    //        srp::ObstacleAvoidPolicy::OBSTACLE_AVOID_WAIT,
    //        move_to_station_1_seq); if (reply.replyCode_ == Reply_Ok) {
    //        cout << "send move to station 1 task succeed" << endl;
    //        // 阻塞等待移动到站点1结束
    //        while (!move_to_station_1_finished) {
    //            this_thread::yield();
    //        }
    //    }
    //        else {
    //        cout << "send move to station 1 task failed" << endl;
    //        //srp.disconnect();
    //        //return -1;
    //    }
    //}
    //this_thread::sleep_for(chrono::milliseconds(100));
    mas.demo_initialize();
    mas.ac_config_off();
    mas.payload_identify();
    mas.demo.servo_move_use_carte_NLF(200, 200, 2000, 60, 60, 100);
    mas.demo.servo_move_enable(1);
    mas.demo.set_torsenosr_brand(2);
    mas.demo.set_torque_sensor_mode(1);
    mas.demo.set_admit_ctrl_config(0, 1, 40, 0, 0, 0);
    mas.demo.set_admit_ctrl_config(1, 0, 40, 0, 0, 0);
    mas.demo.set_admit_ctrl_config(2, 3, 40, 0, 0, 0);
    mas.demo.set_admit_ctrl_config(3, 0, 2, 0, 0, 0);
    mas.demo.set_admit_ctrl_config(4, 0, 0, 0, 0, 0);
    mas.demo.set_admit_ctrl_config(5, 6, 3, 0, 0, 0);
    cout<<demo.set_compliant_type(1, 1)<<endl;
    cout << demo.enable_admittance_ctrl(1) << endl;
    cout << mas.demo.set_compliant_type(0, 0) << endl;
    Sleep(50);

    ui.label1->setText("平台初始化完成");

    return true;
}
bool MPw::agv_admit_enable()
{
    cout << "agv enabling" << endl;
    auto reply = srp.enableManualControl(true); // 启动手动控制模式
    if (reply.replyCode_ == Reply_Ok) {
        cout << "enable manual control succeed" << endl;
        this_thread::sleep_for(chrono::milliseconds(100));
    }
    else {
        cout << "enable manual control failed" << endl;
        //srp.disconnect();
        //return -1;
    }
    cout << "ManualControl enabled" << endl;
    //
    agv_on = true;
    cout << "agv turned on" << endl;
    //
    std::thread th1(&MPw::agv_admit,this);
    th1.detach();

    return true;
}
bool MPw::agv_admit_disable()
{
    srp.enableManualControl(false);
    //srp.logout(false);
    //srp.disconnect();
    this_thread::sleep_for(chrono::milliseconds(100)); // 延时等待onDisconnected()回调函数
    mas.ac_config_off();
    return true;
}
void MPw::on_init_clicked()
{
    initializer();
    ui.label1->setText("initialized");
}
void MPw::on_agv_clicked()
{
    switch (agv_on)
    {
    case true:
        ui.agv->setStyleSheet("background-color: rgb(238,238,238)");
        agv_off();
        agv_admit_disable();
        ui.label1->setText("agv off");
        break;
    case false:
        ui.agv->setStyleSheet("background-color: rgb(175,238,238)");
        agv_admit_enable();
        ui.label1->setText("agv on");
        break;
    default:
        break;
    }
}
void MPw::agv_admit()
{
    cout << "in admit" << endl;
    agv.set_admit_param(1, 3, 0);
    agv.show_admit_param();
    AgvStatus init_status, current_status;
    init_status.pos = { 0,0,0 };
    init_status.vec = { 0,0,0 };
    init_status.acc = { 0,0,0 };
    current_status = init_status;
    agv.set_base(0, 0, 0);
    agv.set_force(0, 0, 0);
    RobotStatus rs;
    while (agv_on == true)
    {
        mas.demo.get_robot_status(&rs);
        actTorque[0] = rs.torq_sensor_monitor_data.actTorque[0];
        actTorque[1] = rs.torq_sensor_monitor_data.actTorque[1];
        actTorque[2] = rs.torq_sensor_monitor_data.actTorque[2];
        actTorque[3] = rs.torq_sensor_monitor_data.actTorque[3];
        actTorque[4] = rs.torq_sensor_monitor_data.actTorque[4];
        actTorque[5] = rs.torq_sensor_monitor_data.actTorque[5];

        double fx = 2 * (-actTorque[1]);
        double tz = -actTorque[5];
        double w = 0;
        if (abs(fx) > 5)
        {
            cout << "fx: " << fx << endl;
            agv.set_force(fx, 0, 0);
        }
        else
        {
            cout << "fx: 0" << endl;
            agv.set_force(0, 0, 0);
        }
        if (abs(tz) > 1.5)
        {
            w = tz * 100;
        }

        agv.admittance_control(current_status);
        current_status = agv.get_status();
        double v = current_status.vec[0];
        srp.setManualControlSpeed(10 * v, w);
        // 
        //cout << "v: " << v << endl;
        //cout << "----------------" << endl;
        this_thread::sleep_for(chrono::milliseconds(80));
        if (abs(actTorque[2]) > 30)
        {
            current_status = init_status;

        }
    }
}
void MPw::on_enter_m_clicked()
{
    massageWindow.show();
    mas.demo_initialize();
    mas.ac_config_off();
}
void MPw::on_TE_clicked()
{
    trajEdit.show();
    mas.demo_initialize();
    mas.ac_config_off();
}
void MPw::on_down_clicked()
{
    srp.logout(false);
    srp.disconnect();
    mas.demo.disable_robot();
    mas.demo.login_out();
    //mas.demo.shut_down();
}
void MPw::color_init()
{
    ui.init->setStyleSheet("background-color: rgb(238,238,238)");
    ui.TE->setStyleSheet("background-color: rgb(238,238,238)");
    ui.enterm->setStyleSheet("background-color: rgb(238,238,238)");
    ui.agv->setStyleSheet("background-color: rgb(238,238,238)");
    ui.down->setStyleSheet("background-color: rgb(238,238,238)");
}
//agv inti
void MPw::onConnectFinised(bool result) {
    cout << "onConnectFinised() result: " << std::boolalpha << result << endl;
    connected = result;
}
void MPw::onDisconnected() {
    cout << "onDisconnected()" << endl;
    connected = false;
}
void MPw::onNewSystemState(const proto::SystemState& s) {
    //    cout << "onNewSystemState! map_name: " << s.map_name() << " station_no:
    //    " << s.station_no() << endl;
    current_station_no = s.station_no();
}
void MPw::onLaserScan(srp::PoseVector_t points) {
    //    cout << "onLaserScan! size:" << points.size() << endl;
}
void MPw::onNotification(const proto::Notification& notifiation) {
    proto::Notification_NotifyType notifyType = notifiation.notify_type();
    cout << __FUNCTION__ << "notify Type: " << notifiation.notify_type() << endl;

    switch (notifyType) {
    case proto::Notification_NotifyType_NOTIFY_MOVE_TASK_FINISHED: {
        if (!notifiation.has_movement_task()) {
            return;
        }
        proto::MovementTask task = notifiation.movement_task();
        cout << "onMovementTask:" << task.no() << task.state();
        if (move_to_station_1_seq == task.no()) {
            move_to_station_1_finished = true;
        }
        break;
    }
    case proto::Notification_NotifyType_NOTIFY_ACTION_TASK_FINISHED: {
        if (!notifiation.has_action_task()) {
            return;
        }
        break;
    }
    case proto::Notification_NotifyType_NOTIFY_MOVE_PATH_SENT: {
        break;
    }
    case proto::Notification_NotifyType_NOTIFY_CALIBRATION_FINISHED: {
        if (!notifiation.has_cal_result()) {
            return;
        }
        break;
    }
    case proto::Notification_NotifyType_NOTIFY_UPDATE_FINISHED: {
        if (!notifiation.has_update_result()) {
            return;
        }
        break;
    }
    default:
        break;
    }
}
bool MPw::agv_off()
{
    agv_on = false;
    return true;
}
//