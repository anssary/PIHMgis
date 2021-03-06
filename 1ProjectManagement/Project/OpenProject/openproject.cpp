#include "openproject.h"
#include "ui_openproject.h"
#include <QtGui>

#include "../../pihmLIBS/helpDialog/helpdialog.h"

OpenProject::OpenProject(QWidget *parent)
    : QDialog(parent), ui(new Ui::OpenProject)
{
    ui->setupUi(this);
}

OpenProject::~OpenProject()
{
    delete ui;
}

void OpenProject::on_pushButtonFile_clicked()
{
    QString projDir, projFile;
    QFile tFile(QDir::homePath()+"/project.txt");
    tFile.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream tin(&tFile);
	projDir  = tin.readLine();
	projFile = tin.readLine();
    tFile.close();
    //cout << qPrintable(projDir);

    QString str = QFileDialog::getOpenFileName(this, "Open Existing Project", projDir, "PIHMgis project (*.pihmgis)");
    ui->lineEditFile->setText(str);
    ui->lineEditFolder->setText(str.left(str.lastIndexOf("/",-1)));
    QString tempStr = str.left(str.lastIndexOf("/",-1));
    ui->lineEditFolder2->setText(tempStr.left(tempStr.lastIndexOf("/",-1)));
}

void OpenProject::on_pushButtonClose_clicked()
{
    close();
}

void OpenProject::on_pushButtonOpen_clicked()
{
    if(ui->lineEditFile->text().length()>1){
        QFile t2File(QDir::homePath()+"/project.txt");
        if( !t2File.open(QIODevice::WriteOnly | QIODevice::Text) ){
		return;
	}
        QTextStream tout(&t2File);
        tout<<ui->lineEditFolder->text()<<"\n";
        tout<<ui->lineEditFile->text()<<"\n";
        t2File.close();
	QMessageBox::information(this,tr("Open Project"),tr("Project Successfully. Opened\nClick OK to close the dialog!"),QMessageBox::Ok);
        close();
    }
}

void OpenProject::on_pushButtonHelp_clicked()
{
    helpDialog* hlpDlg = new helpDialog(this, "Open Project", 1, "helpFiles/openproject.html", "Help :: Open Project");
            hlpDlg->show();
}
