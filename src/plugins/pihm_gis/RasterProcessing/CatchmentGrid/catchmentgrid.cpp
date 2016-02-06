#include <QtGui>
#include "catchmentgrid.h"
#include "../../pihmRasterLIBS/lsm.h"
#include "../../pihmRasterLIBS/flood.h"
#include "../../pihmRasterLIBS/setdir.h"
#include "../../pihmRasterLIBS/aread8.h"
#include "../../pihmRasterLIBS/catProcessing.h"

#include "../../pihmLIBS/helpDialog/helpdialog.h"

#include <fstream>
using namespace std;

CatchmentGridDlg::CatchmentGridDlg(QWidget *parent)
{
	setupUi(this);
	connect(inputLNKBrowseButton, SIGNAL(clicked()), this, SLOT(inputLNKBrowse()));
	connect(inputFDRBrowseButton, SIGNAL(clicked()), this, SLOT(inputFDRBrowse()));
	connect(outputBrowseButton, SIGNAL(clicked()), this, SLOT(outputBrowse()));
	connect(runButton, SIGNAL(clicked()), this, SLOT(run()));
	connect(helpButton, SIGNAL(clicked()), this, SLOT(help()));
	connect(closeButton, SIGNAL(clicked()), this, SLOT(close()));
}

void CatchmentGridDlg::inputLNKBrowse()
{
	QString str = QFileDialog::getOpenFileName(this, "Choose File", "~/","Link Grid File(*.adf *.asc)");
	inputLNKFileLineEdit->setText(str);
}

void CatchmentGridDlg::inputFDRBrowse()
{
        QString str = QFileDialog::getOpenFileName(this, "Choose File", "~/","Flow Dir Grid File(*.adf *.asc)");
        inputFDRFileLineEdit->setText(str);
}

void CatchmentGridDlg::outputBrowse()
{
	QString temp = QFileDialog::getSaveFileName(this, "Choose File", "~/","Catchment Grid File(*.adf *.asc)");
	QString tmp = temp;
	if(!(tmp.toLower()).endsWith(".asc")){
        tmp.append(".asc");
        temp = tmp;
        }
	
        outputFileLineEdit->setText(temp);
}


void CatchmentGridDlg::run()
{

	QString logFileName("/tmp/log.html");
	ofstream log;
	log.open(logFileName.ascii());
	log<<"<html><body><font size=3 color=black><p> Verifying Files...</p></font></body></html>";
        log.close();
        messageLog->setSource(logFileName);
        messageLog->setFocus();
        messageLog->setModified(TRUE);

	QString inputLNKFileName((inputLNKFileLineEdit->text()));
	QString inputFDRFileName((inputFDRFileLineEdit->text()));
	QString outputFileName((outputFileLineEdit->text()));

	ifstream LNKinFile;      LNKinFile.open((inputLNKFileLineEdit->text()).ascii());
	ifstream FDRinFile;      FDRinFile.open((inputFDRFileLineEdit->text()).ascii());
	ofstream outFile;    outFile.open((outputFileLineEdit->text()).ascii());
	int runFlag = 1;

	log.open(logFileName.ascii(), ios::app);
	if(inputLNKFileName.length()==0){
		log<<"<p><font size=3 color=red> Error! Please input Link Grid Input File</p>";
		runFlag = 0;
	}
	else{
		log<<"<p>Checking... "<<inputLNKFileName.ascii()<<"... ";
		if(LNKinFile == NULL){
			log<<"<font size=3 color=red> Error!</p>";
			qWarning("\n%s doesn't exist!", (inputLNKFileLineEdit->text()).ascii());
			runFlag = 0;
		}
		else
			log<<"Done!</p>";
	}
	log.close();
	messageLog->reload();
	QApplication::processEvents();

	log.open(logFileName.ascii(), ios::app);
	if(inputFDRFileName.length()==0){
		log<<"<p><font size=3 color=red> Error! Please input Flow Dir. Grid Input File</p>";
		runFlag = 0;
	}
	else{
		log<<"<p>Checking... "<<inputFDRFileName.ascii()<<"... ";
		if(FDRinFile == NULL){
			log<<"<font size=3 color=red> Error!</p>";
			qWarning("\n%s doesn't exist!", (inputFDRFileLineEdit->text()).ascii());
			runFlag = 0;
		}
		else
			log<<"Done!</p>";
	}
	log.close();
	messageLog->reload();
	QApplication::processEvents();


	log.open(logFileName.ascii(), ios::app);
	if(outputFileName.length()==0){
		log<<"<p><font size=3 color=red> Error! Please input Catchment Grid Output File</p>";
		runFlag = 0;
	}
	else{
		log<<"<p>Checking... "<<outputFileName.ascii()<<"... ";
		if(outFile == NULL){
			log<<"<font size=3 color=red> Error!</p>";
			qWarning("\nCan not open output file name");
			runFlag = 0;
		}
		else
			log<<"Done!</p>";
	}
	log.close();
	messageLog->reload();
	QApplication::processEvents();


	if(runFlag == 1){
		log.open(logFileName.ascii(), ios::app);
		log<<"<p>Running...";
		log.close();
		messageLog->reload();
		QApplication::processEvents();	

		int err = catchmentGrid((char *)inputLNKFileName.ascii(), (char *)inputFDRFileName.ascii(), (char *)outputFileName.ascii() );

		log.open(logFileName.ascii(), ios::app);
		log<<" Done!</p>";
		log.close();
		messageLog->reload();
		QApplication::processEvents();		

		if(showSG_DFrame->isChecked() == 1){
       			 //QgsRasterLayer *tempLayer = new QgsRasterLayer("/backup/pihm/RasterProcessing/FillPits", "morgedem.asc");
		//??	applicationPointer->addRasterLayer(QStringList(outputFileName));
		}
	}
}

void CatchmentGridDlg::help()
{
	helpDialog* hlpDlg = new helpDialog(this, "Catchment Grid", 1, "helpFiles/catchmentgrid.html", "Help :: Catchment Grid");
	hlpDlg->show();	

}
/* ??
void CatchmentGridDlg::setApplicationPointer(QgisApp* appPtr){
    applicationPointer = appPtr;
}
*/