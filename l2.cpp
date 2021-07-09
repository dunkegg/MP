#include "l2.h"

#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif

using namespace std;
using namespace massage;


L2::L2(QWidget* parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    connect(ui.trajBox, SIGNAL(currentTextChanged(QString)), this, SLOT(traj_select(QString)));
	connect(ui.safe_plane, SIGNAL(clicked()), this, SLOT(on_safeplane_clicked()));
    connect(ui.run, SIGNAL(clicked()), this, SLOT(on_run_clicked()));
	connect(ui.mstart, SIGNAL(clicked()), this, SLOT(on_mstart_clicked()));
    update_traj();
	color_init();
	ui.text->setText("安全平面未设置");
	ui.mselect->addItem("指压法");
	ui.mselect->addItem("指推法");
	ui.mselect->addItem("一指禅法");
	//mas.demo_initialize();
	//mas.ac_config_off();

}
//slots
void L2::traj_select(const QString& qstring)
{
	string filename = qstring.toStdString();
	string path = "./trajs/";
	path = path + filename;
	const char* ch = path.c_str();
	current_points = mas.load_points_io(ch);
	QString temp;
	temp="最小平面min_z: "+ QString::number(mas.min_z) +"mm";
	ui.text->setText(temp);
}
void L2::on_safeplane_clicked() 
{
	CartesianPose plane;
	QString temp;
	if (splanes.empty())
	{
		mas.demo.get_tcp_position(&plane);
		splanes.push_back(plane);
		if (ui.sp_nums->value() > 1)
		{
			ui.label2->setText("继续设置点2");

		}
		else
		{
			mas.set_limitation(splanes);
			ui.label2->setText("已设置安全平面");
			temp= "最小平面min_z: " + QString::number(mas.min_z) + "mm";
			ui.text->setText(temp);
			ui.safe_plane->setText("再次点击清除平面");
		}
		return;
	}
	if (splanes.size() < ui.sp_nums->value())
	{

		mas.demo.get_tcp_position(&plane);
		splanes.push_back(plane);

		if (splanes.size() == ui.sp_nums->value())
		{
			mas.set_limitation(splanes);
			ui.label2->setText("已设置安全平面");
			temp="最小平面min_z: " + QString::number(mas.min_z) + "mm";
			ui.text->setText(temp);
			ui.safe_plane->setText("再次点击清除平面");
		}
		else
		{
			temp=  "继续设置点" +  QString::number(splanes.size() + 1);
			ui.label2->setText(temp);
		}
		return;
	}
	ui.safe_plane->setText("设置安全平面");
	ui.label2->setText("安全平面未设置，点击设置点1");
	mas.min_z = 0;
	splanes.clear();
	ui.text->setText("安全平面未设置");
}
void L2::on_run_clicked()
{
	QString temp;
	if (mas.min_z == 0)
	{
		temp="还未设置安全平面";
		ui.text->setText(temp);
		return;
	}
	if (current_points.empty())
	{
		temp="还未选择轨迹";
		ui.text->setText(temp);
		return;
	}
	vector<CartesianPose> poses;
	mas.points_to_poses(&current_points,poses);
	_k = ui.spinForce->value();
	_t = ui.spinTimes->value();
	RobotAdmitCtrl rac = {0};
	rac.admit_ctrl[2].opt = 3;
	rac.admit_ctrl[2].ft_user = 20*_k;
	rac.admit_ctrl[2].ft_constant = 3 * _k;
	//textbrowser
	temp= "最小平面min_z: " + QString::number(mas.min_z) + "mm\n" + "力度等级：" +QString::number(_k) +"\n运行次数：" + QString::number(_t) +"\n正在运行";
	ui.text->setText(temp);
	//
	mas.run_points(&poses, _t, rac, TRUE);
	ui.text->setText("运行完成");
}
void L2::on_mstart_clicked()
{
	if (sm_in == false)
	{
		ui.mstart->setStyleSheet("background-color: rgb(175,238,238)");
		cout << "push" << endl;
		int qs = ui.mselect->currentIndex();
		mas.ms_in = true;
		sm_in = true;
		double k = ui.spinForce_2->value();
		QString temp;
		cout << qs << endl;
		if (qs == 0)
		{
			std::thread th1(&Massage::finger_press,&mas,k);
			th1.detach();
			cout << "fpr on" << endl;
			//mas.finger_press(k);
			temp = "指压法正在运行\n力度等级: " + QString::number(k);
		}
		else if (qs == 1)
		{
			std::thread th1(&Massage::finger_push, &mas, k);
			th1.detach();
			cout << "fpu on" << endl;
			//mas.finger_push(k);
			temp="指推法正在运行\n力度等级: " + QString::number(k);
		}
		else
		{
			std::thread th1(&Massage::finger_spin, &mas, k);
			th1.detach();
			cout << "fsp on" << endl;
			//mas.finger_spin(k);
			temp="一指禅法正在运行\n力度等级: "+ QString::number(k);
		}
		ui.text->setText(temp);
		cout << "onono" << endl;
		return;
	}
	else
	{
		ui.mstart->setStyleSheet("background-color: rgb(238,238,238)");
		mas.ms_in = false;
		sm_in = false;
		ui.text->clear();
	}
}
//slots
void L2::update_traj()
{

	vector<string> filenames;
	const char* to_search = "./trajs/*.txt";//获取某类型的文件，本例程中是获取txt文件string path = "./trajs/";
	getFiles(to_search, filenames);
	disconnect(ui.trajBox, SIGNAL(currentTextChanged(QString)), this, SLOT(traj_select(QString)));
	ui.trajBox->clear();
	for (int i = 0; i < filenames.size(); i++)
	{
		QString qstr;
		qstr = QString::fromStdString(filenames[i]);
		ui.trajBox->addItem(qstr);
	}
	connect(ui.trajBox, SIGNAL(currentTextChanged(QString)), this, SLOT(traj_select(QString)));
}
void L2::color_init()
{
	ui.safe_plane->setStyleSheet("background-color: rgb(238,238,238)");
	ui.run->setStyleSheet("background-color: rgb(238,238,238)");
	ui.mstart->setStyleSheet("background-color: rgb(238,238,238)");
}


int getFiles(const char* to_search, vector<string>& files)
{
	intptr_t handle = -1;                                                //用于查找的句柄
	struct _finddata_t fileinfo;  //文件信息的结构体
	//const char *to_search = "E:\\Pictures\\*.jpg";
	handle = _findfirst(to_search, &fileinfo);         //第一次查找
	if (-1 == handle)
		return -1;
	files.push_back(fileinfo.name);
	while (!_findnext(handle, &fileinfo))               //循环查找其他符合的文件，知道找不到其他的为止
	{
		files.push_back(fileinfo.name);
	}
	_findclose(handle);
	return 1;
}
string  num2str(double  i)
{
	stringstream ss;
	ss << i;
	return ss.str();
}