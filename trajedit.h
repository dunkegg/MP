#pragma once
#include <iostream>  
#include <string>  
#include <io.h>  
#include<vector>
#include <QtWidgets/QMainWindow>
#include "ui_trajedit.h"
#include "massage.h"
#include "m.h"

using namespace std;
using namespace massage;

class Tedit : public QMainWindow
{
    Q_OBJECT
public slots:
    void on_merge_clicked();
    void on_trans_clicked();
    void on_scale_clicked();
    void on_vedit_clicked();
    void on_exe_clicked();
    void on_record_clicked();
    void on_delete_clicked();
    void on_dyes_clicked();
    void on_dno_clicked();
public:
    Tedit(QWidget* parent = Q_NULLPTR);
    void update_traj(int s);
    bool judge_filename(const QString& qs);
    void ui_init();
    void Tedit::color_init();
    int getFiles(const char* to_search, vector<string>& files);
public:
    //Massage mas;
private:
    Ui::Tedit ui;
    
    int state=0;
    bool in_record;
    bool d_check;
};

