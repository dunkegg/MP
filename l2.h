#include <iostream>  
#include <string>
#include  <sstream>
#include <io.h>  
#include<vector>
#include <QtWidgets/QMainWindow>
#include "ui_l2.h"
#include "massage.h"
#include "m.h"

using namespace std;
using namespace massage;


class L2 : public QMainWindow
{
    Q_OBJECT
public slots:
    void traj_select(const QString& qstring);
    void on_safeplane_clicked();
    void on_run_clicked();
    void on_mstart_clicked();
public:
    L2(QWidget* parent = Q_NULLPTR);
    void update_traj();
    void color_init();
public:
    //Massage mas;
private:
    Ui::L2 ui;
    vector<vector<float>> current_points;
    int _k;
    size_t _t;
    vector<CartesianPose> splanes;
    bool sm_in = false;
};

int getFiles(const char* to_search, vector<string>& files);
string  num2str(double  i);