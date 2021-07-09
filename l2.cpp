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
	ui.text->setText("��ȫƽ��δ����");
	ui.mselect->addItem("ָѹ��");
	ui.mselect->addItem("ָ�Ʒ�");
	ui.mselect->addItem("һָ����");
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
	temp="��Сƽ��min_z: "+ QString::number(mas.min_z) +"mm";
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
			ui.label2->setText("�������õ�2");

		}
		else
		{
			mas.set_limitation(splanes);
			ui.label2->setText("�����ð�ȫƽ��");
			temp= "��Сƽ��min_z: " + QString::number(mas.min_z) + "mm";
			ui.text->setText(temp);
			ui.safe_plane->setText("�ٴε�����ƽ��");
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
			ui.label2->setText("�����ð�ȫƽ��");
			temp="��Сƽ��min_z: " + QString::number(mas.min_z) + "mm";
			ui.text->setText(temp);
			ui.safe_plane->setText("�ٴε�����ƽ��");
		}
		else
		{
			temp=  "�������õ�" +  QString::number(splanes.size() + 1);
			ui.label2->setText(temp);
		}
		return;
	}
	ui.safe_plane->setText("���ð�ȫƽ��");
	ui.label2->setText("��ȫƽ��δ���ã�������õ�1");
	mas.min_z = 0;
	splanes.clear();
	ui.text->setText("��ȫƽ��δ����");
}
void L2::on_run_clicked()
{
	QString temp;
	if (mas.min_z == 0)
	{
		temp="��δ���ð�ȫƽ��";
		ui.text->setText(temp);
		return;
	}
	if (current_points.empty())
	{
		temp="��δѡ��켣";
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
	temp= "��Сƽ��min_z: " + QString::number(mas.min_z) + "mm\n" + "���ȵȼ���" +QString::number(_k) +"\n���д�����" + QString::number(_t) +"\n��������";
	ui.text->setText(temp);
	//
	mas.run_points(&poses, _t, rac, TRUE);
	ui.text->setText("�������");
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
			temp = "ָѹ����������\n���ȵȼ�: " + QString::number(k);
		}
		else if (qs == 1)
		{
			std::thread th1(&Massage::finger_push, &mas, k);
			th1.detach();
			cout << "fpu on" << endl;
			//mas.finger_push(k);
			temp="ָ�Ʒ���������\n���ȵȼ�: " + QString::number(k);
		}
		else
		{
			std::thread th1(&Massage::finger_spin, &mas, k);
			th1.detach();
			cout << "fsp on" << endl;
			//mas.finger_spin(k);
			temp="һָ������������\n���ȵȼ�: "+ QString::number(k);
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
	const char* to_search = "./trajs/*.txt";//��ȡĳ���͵��ļ������������ǻ�ȡtxt�ļ�string path = "./trajs/";
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
	intptr_t handle = -1;                                                //���ڲ��ҵľ��
	struct _finddata_t fileinfo;  //�ļ���Ϣ�Ľṹ��
	//const char *to_search = "E:\\Pictures\\*.jpg";
	handle = _findfirst(to_search, &fileinfo);         //��һ�β���
	if (-1 == handle)
		return -1;
	files.push_back(fileinfo.name);
	while (!_findnext(handle, &fileinfo))               //ѭ�������������ϵ��ļ���֪���Ҳ���������Ϊֹ
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