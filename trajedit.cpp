#include "trajedit.h"

#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif

using namespace std;
using namespace massage;


Tedit::Tedit(QWidget* parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
	connect(ui.merge_2, SIGNAL(clicked()), this, SLOT(on_merge_clicked()));
	connect(ui.trans, SIGNAL(clicked()), this, SLOT(on_trans_clicked()));
	connect(ui.scale, SIGNAL(clicked()), this, SLOT(on_scale_clicked()));
	connect(ui.vedit, SIGNAL(clicked()), this, SLOT(on_vedit_clicked()));
	connect(ui.execute, SIGNAL(clicked()), this, SLOT(on_exe_clicked()));
	connect(ui.trecord, SIGNAL(clicked()), this, SLOT(on_record_clicked()));
	connect(ui.tdelete, SIGNAL(clicked()), this, SLOT(on_delete_clicked()));
	connect(ui.dyes, SIGNAL(clicked()), this, SLOT(on_dyes_clicked()));
	connect(ui.dno, SIGNAL(clicked()), this, SLOT(on_dno_clicked()));
	color_init();
    update_traj(0);
	update_traj(1);
	update_traj(2);
	ui_init();
	in_record = false;
	d_check = false;
    //mas.demo_initialize();
    //mas.ac_config_off();
	

}
void Tedit::on_merge_clicked()
{
	ui.merge_2->setStyleSheet("background-color: rgb(175,238,238)");
	ui.trans->setStyleSheet("background-color: rgb(238,238,238)");
	ui.scale->setStyleSheet("background-color: rgb(238,238,238)");
	ui.vedit->setStyleSheet("background-color: rgb(238,238,238)");
	ui_init();
	ui.execute->setText("执行");

	ui.t_label->show();
	ui.t_label_2->show();

	ui.traj1->show();

	ui.traj2->show();
	state = 1;
}
void Tedit::on_trans_clicked()
{
	ui.trans->setStyleSheet("background-color: rgb(175,238,238)");
	ui.merge_2->setStyleSheet("background-color: rgb(238,238,238)");
	ui.scale->setStyleSheet("background-color: rgb(238,238,238)");
	ui.vedit->setStyleSheet("background-color: rgb(238,238,238)");
	ui_init();
	ui.execute->setText("执行");

	ui.t_label->show();

	ui.traj1->show();

	//ui.t_label_3->show();
	//ui.base_label->show();
	//ui.basex->show(); ui.basey->show(); ui.basez->show();

	ui.ts->setText("输入偏移量");
	ui.ts->show();
	ui.base_label_2->show();
	ui.basex_2->show(); ui.basey_2->show(); ui.basez_2->show();

	state = 2;
}
void Tedit::on_scale_clicked()
{
	ui.scale->setStyleSheet("background-color: rgb(175,238,238)");
	ui.trans->setStyleSheet("background-color: rgb(238,238,238)");
	ui.merge_2->setStyleSheet("background-color: rgb(238,238,238)");
	ui.vedit->setStyleSheet("background-color: rgb(238,238,238)");
	ui_init();
	ui.execute->setText("执行");
	//ui.basey_2->clear();

	ui.t_label->show();

	ui.traj1->show();

	ui.t_label_3->show();
	ui.base_label->show();
	ui.basex->show(); ui.basey->show(); ui.basez->show();

	ui.ts->setText("输入缩放倍数");
	ui.ts->show();
	ui.basey_2->show();

	state = 3;
}
void Tedit::on_vedit_clicked()
{
	ui.vedit->setStyleSheet("background-color: rgb(175,238,238)");
	ui.trans->setStyleSheet("background-color: rgb(238,238,238)");
	ui.scale->setStyleSheet("background-color: rgb(238,238,238)");
	ui.merge_2->setStyleSheet("background-color: rgb(238,238,238)");
	ui_init();
	ui.execute->setText("执行");

	ui.t_label->show();

	ui.traj1->show();

	ui.vlabel->show();
	ui.base_label_3->show();
	ui.multi->show(); ui.const_2->show();

	state = 4;
}
void Tedit::on_exe_clicked()
{
	switch (state)
	{
	case 0:
		ui.execute->setText("未选择动作");
		break;
	case 1: {
		QString inputfn = ui.trajname->text();
		if (judge_filename(inputfn))
		{
			ui.execute->setText("轨迹名重复");
			break;
		}
		ui.execute->setText("执行中...");
		QString qs=ui.traj1->currentText();
		string filename = qs.toStdString();
		//string path = "C:\\workspace\\sros-sdk-cpp\\trajs\\";
		string path = "./trajs/";
		string path1 = path + filename;
		const char* ch = path1.c_str();
		vector<vector<float>> points1 = mas.load_points_io(ch);
		qs = ui.traj2->currentText();
		filename = qs.toStdString();
		string path2 = path + filename;
		ch = path2.c_str();
		vector<vector<float>> points2 = mas.load_points_io(ch);
		vector<vector<float>> pointsm = mas.merge_trajs(points1, points2);
		
		qs = ui.trajname->text();
		filename = qs.toStdString();
		string pathm = path + filename + ".txt";
		ch = pathm.c_str();
		mas.save_points(pointsm, ch);
		ui.execute->setText("执行完成");
		break;
	}
	case 2: {
		QString inputfn = ui.trajname->text();
		if (judge_filename(inputfn))
		{
			ui.execute->setText("轨迹名重复");
			break;
		}
		ui.execute->setText("执行中...");
		QString qs = ui.traj1->currentText();
		string filename = qs.toStdString();
		string path = "./trajs/";
		string path1 = path + filename;
		const char* ch = path1.c_str();
		vector<vector<float>> points1 = mas.load_points_io(ch);
		float transx = ui.basex_2->text().toFloat();
		float transy = ui.basey_2->text().toFloat();
		float transz = ui.basez_2->text().toFloat();
		vector<vector<float>> pointsm=mas.trans_trajs(points1, transx, transy, transz);

		qs = ui.trajname->text();
		filename = qs.toStdString();
		string pathm = path + filename + ".txt";
		ch = pathm.c_str();
		mas.save_points(pointsm, ch);
		ui.execute->setText("执行完成");
		break;
	}
	case 3: {
		QString inputfn = ui.trajname->text();
		if (judge_filename(inputfn))
		{
			ui.execute->setText("轨迹名重复");
			break;
		}
		ui.execute->setText("执行中...");
		QString qs = ui.traj1->currentText();
		string filename = qs.toStdString();
		string path = "./trajs/";
		string path1 = path + filename;
		const char* ch = path1.c_str();
		vector<vector<float>> points1 = mas.load_points_io(ch);
		float basex = ui.basex->text().toFloat();
		float basey = ui.basey->text().toFloat();
		float basez = ui.basez->text().toFloat();
		float scale = ui.basey_2->text().toFloat();
		vector<vector<float>> pointsm = mas.scale_trajs(points1, scale, basex, basey, basez);

		qs = ui.trajname->text();
		filename = qs.toStdString();
		string pathm = path + filename + ".txt";
		ch = pathm.c_str();
		mas.save_points(pointsm, ch);
		ui.execute->setText("执行完成");
		break;
	}
	case 4: {
		QString inputfn = ui.trajname->text();
		if (judge_filename(inputfn))
		{
			ui.execute->setText("轨迹名重复");
			break;
		}
		ui.execute->setText("执行中...");
		QString qs = ui.traj1->currentText();
		string filename = qs.toStdString();
		string path = "./trajs/";
		string path1 = path + filename;
		const char* ch = path1.c_str();
		vector<vector<float>> points1 = mas.load_points_io(ch);
		double multi = ui.multi->text().toDouble();
		double speed = ui.const_2->text().toDouble();
		vector<vector<float>> pointsm = mas.vec_edit(points1,multi,speed,multi,speed);

		qs = ui.trajname->text();
		filename = qs.toStdString();
		string pathm = path + filename + ".txt";
		ch = pathm.c_str();
		mas.save_points(pointsm, ch);
		ui.execute->setText("执行完成");
		break;
	}
	default:
		break;
	}
	update_traj(0);
	update_traj(1);
	update_traj(2);

}
void Tedit::on_record_clicked()
{

	QString qs = ui.trajname->text();
	while (judge_filename(qs))
	{
		ui.label_2->setText("轨迹名重复");
	}
	if (in_record == false)
	{
		ui.trecord->setStyleSheet("background-color: rgb(175,238,238)");
		ui.trecord->setText("录制中");
		string filename = qs.toStdString();
		string path = "./trajs/";
		string path1 = path + filename+".txt";
		const char* ch = path1.c_str();
		mas.record = true;
		std::thread th1(&Massage::thread_record, &mas, ch);
		th1.detach();
		cout << "thread on" << endl;
		//mas.record_traj(ch);
		in_record = true;
	}
	else if (in_record == true)
	{
		mas.record = false;
		ui.trecord->setStyleSheet("background-color: rgb(238,238,238)");
		ui.trecord->setText("录制新轨迹");
		in_record = false;
		update_traj(0);
		update_traj(1);
		update_traj(2);
	}

}
void Tedit::on_delete_clicked() 
{
	if (d_check == false)
	{
		ui.dyes->show();
		ui.dno->show();
		ui.tdelete->setStyleSheet("background-color: rgb(175,238,238)");
		ui.tdelete->setText("确定删除？");
		d_check = true;
	}
}
void Tedit::on_dyes_clicked() 
{
	if(ui.list->currentItem()==nullptr)//没有这个判断程序会崩溃 ？？？
	{
		ui.tdelete->setText("未选中轨迹");
		Sleep(1500);
	}
	else
	{
		QString qs=ui.list->currentItem()->text();
		string fn = qs.toStdString();
		string path = "./trajs/";
		remove(path.c_str());
	}
	ui.dyes->hide();
	ui.dno->hide();
	ui.tdelete->setStyleSheet("background-color: rgb(238,238,238)");
	ui.tdelete->setText("删除轨迹");
	d_check = false;
	update_traj(0);
	update_traj(1);
	update_traj(2);
}
void Tedit::on_dno_clicked()
{
	ui.dyes->hide();
	ui.dno->hide();
	ui.tdelete->setStyleSheet("background-color: rgb(238,238,238)");
	ui.tdelete->setText("删除轨迹");
	d_check = false;
}
void Tedit::update_traj(int s)
{

	vector<string> filenames;
	const char* to_search = "./trajs/*.txt";//获取某类型的文件，本例程中是获取txt文件
	getFiles(to_search, filenames);
	//disconnect(ui.trajBox, SIGNAL(currentTextChanged(QString)), this, SLOT(traj_select(QString)));
	if (s == 0)
	{
		ui.list->clear();
	}
	else if (s == 1)
	{
		ui.traj1->clear();
	}
	else
	{
		ui.traj2->clear();
	}
	for (int i = 0; i < filenames.size(); i++)
	{
		QString qstr;
		string fn = filenames[i];
		if (s == 0)
		{
			string fns=fn.substr(0, fn.rfind("."));
			//cout << "update traj: " << fn.size() << endl;
			qstr = QString::fromStdString(fns);
			ui.list->addItem(qstr);
		}
		else if (s == 1)
		{
			qstr = QString::fromStdString(fn);
			ui.traj1->addItem(qstr);
		}
		else
		{
			qstr = QString::fromStdString(fn);
			ui.traj2->addItem(qstr);
		}
	}
	//connect(ui.trajBox, SIGNAL(currentTextChanged(QString)), this, SLOT(traj_select(QString)));
}
bool Tedit::judge_filename(const QString& qs)
{
	//vector<string> filenames;
	//const char* to_search = "./trajs/";
	//getFiles(to_search, filenames);
	//bool result = false;
	//for (int i = 0; i < filenames.size(); i++)
	//{
	//	string fn = filenames[i];
	//	string fns = fn.substr(0, fn.rfind("."));
	//	QString qsfn = QString::fromStdString(fns);
	//	if (qsfn == qs)
	//	{
	//		result == true;
	//	}
	//}
	//return result;
	int k = ui.list->count();
	bool result = false;
	for (int i = 0; i < k; i++)
	{
		QString qsfn = ui.list->item(i)->text();
		if (qsfn == qs)
		{
			result = true;
		}
	}
	return result;
}
void Tedit::ui_init()
{
	ui.dyes->hide();
	ui.dno->hide();
	
	//merge
	ui.t_label->hide();
	ui.t_label_2->hide();
	ui.traj1->hide();
	ui.traj2->hide();
	//trans
	ui.t_label_3->hide();
	ui.base_label->hide();
	ui.basex->hide(); ui.basey->hide(); ui.basez->hide();
	//scale
	ui.ts->hide();
	ui.base_label_2->hide();
	ui.basex_2->hide(); ui.basey_2->hide(); ui.basez_2->hide();

	//vedit
	ui.vlabel->hide(); 
	ui.base_label_3->hide();
	ui.multi->hide(); ui.const_2->hide();
	if (ui.trajname->text().isEmpty())
	{
		ui.trajname->setText("newtraj");
	}
}
void Tedit::color_init()
{
	ui.vedit->setStyleSheet("background-color: rgb(238,238,238)");
	ui.trans->setStyleSheet("background-color: rgb(238,238,238)");
	ui.scale->setStyleSheet("background-color: rgb(238,238,238)");
	ui.merge_2->setStyleSheet("background-color: rgb(238,238,238)");
	ui.trecord->setStyleSheet("background-color: rgb(238,238,238)");
	ui.tdelete->setStyleSheet("background-color: rgb(238,238,238)");
	ui.dyes->setStyleSheet("background-color: rgb(238,238,238)");
	ui.dno->setStyleSheet("background-color: rgb(238,238,238)");
	ui.execute->setStyleSheet("background-color: rgb(238,238,238)");
}
int Tedit::getFiles(const char* to_search, vector<string>& files)
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