#include <iostream>
#include <QtGui>
#include "runtriangle.h"
#include <fstream>
#include "../../pihmLIBS/helpDialog/helpdialog.h"
#include "triangle.h"

#include <fstream>
using namespace std;

using namespace std;

runTriangleDlg::runTriangleDlg(QWidget *parent)
{
	setupUi(this);
	connect(polyFileBrowseButton, SIGNAL(clicked()), this, SLOT(inputBrowse()));
	connect(showOptionsButton, SIGNAL(clicked()), this, SLOT(options()));
	connect(runButton, SIGNAL(clicked()), this, SLOT(run()));
	connect(helpButton, SIGNAL(clicked()), this, SLOT(help()));
	connect(cancelButton, SIGNAL(clicked()), this, SLOT(close()));
}


void runTriangleDlg::inputBrowse()
{
	QString temp = QFileDialog::getOpenFileName(this, "Choose File", "~/","Poly File(*.poly)");
	
        polyFileLineEdit->setText(temp);
}

void runTriangleDlg::options()
{
	helpDialog* hlpDlg = new helpDialog(this, "TRIANGLE Options", 1, "helpFiles/triangleoptions.html", "Help :: TRIANGLE Options");
        hlpDlg->show();
}

void runTriangleDlg::run()
{

	QString logFileName("/tmp/log.html");
	ofstream log;
	log.open(logFileName.ascii());
	log<<"<html><body><font size=3 color=black><p> Verifying Files...</p></font></body></html>";
        log.close();
        MessageLog->setSource(logFileName);
        MessageLog->setFocus();
        MessageLog->setModified(TRUE);

/*
	ifstream temp;
        temp.open((polyFileLineEdit->text()).ascii());
        if((polyFileLineEdit->text()).length()==0){
        	qWarning("\n Enter Poly File");
	}
        else if(temp==NULL){
        	qWarning("\n %s doesn't exist!", (polyFileLineEdit->text()).ascii());
	}
*/

	ifstream inFile;      inFile.open((polyFileLineEdit->text()).ascii());
	int runFlag = 1;

	log.open(logFileName.ascii(), ios::app);
	if((polyFileLineEdit->text()).length()==0){
		log<<"<p><font size=3 color=red> Error! Please input poly File</p>";
		runFlag = 0;
	}
	else{
		log<<"<p>Checking... "<<(polyFileLineEdit->text()).ascii()<<"... ";
		if(inFile == NULL){
			log<<"<font size=3 color=red> Error!</p>";
			//qWarning("\n%s doesn't exist!", (inputFileLineEdit->text()).ascii());
			runFlag = 0;
		}
		else
			log<<"Done!</p>";
	}
	log.close();
	MessageLog->reload();
	QApplication::processEvents();
	
        if(runFlag == 1){
		QString temp(polyFileLineEdit->text());
		int slashPos = temp.findRev('/');
        	temp.truncate(slashPos);
        	QString presentDir(temp);
		
		QString* exeString = new QString("triangle");
		QString* optString = new QString("-");
                if(nOptionCheckBox->isChecked()){optString->append("n");}
                if(VOptionsCheckBox->isChecked()){optString->append("V");}
                if(qOptionsCheckBox->isChecked()){optString->append("q"); optString->append(qOptionLineEdit->text());}
                if(aOptionsCheckBox->isChecked()){optString->append("a"); optString->append(aOptionLineEdit->text());}
                if(otherOptionCheckBox->isChecked()) {optString->append(otherOptionLineEdit->text());}
		//cmdString->append(aOptionLineEdit->text());}

                //qWarning("cmdstring = %s\n", cmdString->ascii());

                //cmdString->append(" ");
                QString fileString(polyFileLineEdit->text());
		//cmdString->append(polyFileLineEdit->text());
		int arg1=0;
		char **arg2;
		arg2 = (char **)malloc(3 * sizeof(char *));
		for (int ii=0; ii<3; ii++)
			arg2[ii]=(char *)malloc(1000*sizeof(char));
		arg2[0]=(char *)exeString->ascii();
                arg1++;

		if(optString->length() != 0){
			arg2[1]=(char *)optString->ascii();
			arg1++;
			arg2[2]=(char *)fileString.ascii();
			arg1++;
		}
		else{
			arg2[1]=(char *)fileString.ascii();
			arg1++;
		}

		log.open(logFileName.ascii(), ios::app);
		log<<"<p>Running...";
		log.close();
		MessageLog->reload();
		QApplication::processEvents();

		ShewchukTRIANGLE(arg1, arg2);
		
		log.open(logFileName.ascii(), ios::app);
		log<<" Done!</p>";
		log.close();
		MessageLog->reload();
		QApplication::processEvents();

		// std::cout<<str;
                //cmdString->append(" 1>");
                //cmdString->append(presentDir);
                //cmdString->append("/statistics.txt");
		
		//const char* str = cmdString->ascii();
		//std::system(str);
/*
		QFile statFile( presentDir+"/statistics.txt" );

                if ( !statFile.open( IO_ReadOnly ) )
                       return;
                QTextStream textStream( &statFile );

                int lines2print;
                if(VOptionsCheckBox->isChecked()){
                       if(!qOptionsCheckBox->isChecked())
                               lines2print = 23;
                       else
                               lines2print = 29;
                }
                else{
                       if(!qOptionsCheckBox->isChecked())
                               lines2print = 15;
                       else
                               lines2print = 17;
                }

                for(int i=1; i<=lines2print; i++){
                       qWarning("\n %s",(textStream.readLine()).ascii());
                }
*/
	}
}
			
void runTriangleDlg::help()
{
	helpDialog* hlpDlg = new helpDialog(this, "Run TRIANGLE", 1, "helpFiles/runtriangle.html", "Help :: Run TRIANGLE");
	hlpDlg->show();	
}